// include fileInfo.c and fileManip.c file in main for other functions' usage
#pragma once
#include "listFile.h"
#include <math.h>

#define OKAY 1

int64_t* getOffsetToDataRegion(char* filename) {
    // Open file descriptor of file
    int64_t* res = (int64_t*) malloc(5 * sizeof(int64_t));
    res[0] = -1; 
    res[1] = -1;

    int fd = openFileDescriptor(filename, 1);

    if (fd == -1) {
      perror("error from openFile:");
      return res;
    }

    // Get the size of the file
    long size = getInputFileSize(fd);

    if(size == -1) {
      perror("error from getInputFileSize: ");
      return res;
    }

    // Create mappings in the virtual address space
    Main_Boot* src = mmapToFile(fd, size, 1);

    if (src == (Main_Boot *) -1) {
      perror("error from mmap:");
      return res;
    }

    res[0] = src->ClusterHeapOffset * (1 << src->BytesPerSectorShift) / 32;
    res[1] = src->ClusterCount * (1 << src->SectorsPerClusterShift) / 32;   // not used
    res[2] = src->FirstClusterOfRootDirectory;
    res[3] = (1 << src->BytesPerSectorShift) * (1 << src->SectorsPerClusterShift);
    res[4] = src->ClusterCount;

    // Unmap the file
    if (munmap(src, sizeof(Main_Boot)) == -1) {
      perror("error from unmap:");
      res[0] = -1;
      return res;
    }

    // Close the file
    close(fd);
    return res;
}

int listDirectoryofFiles(Option op) {
    if(!op.dirFlag) return OKAY;

    if(!op.inputFile) {
      printf("Missing required inputFile / outputFile");
      return !OKAY;
    }

    int i;

    // Get offset so that it can skip Main Boot, Backup Boot, and FAT region and then read Data Region
    // [0]: ClusterHeapOffset
    // [1]: ClusterCount * (2 ^ SectorsPerClusterShift)
    // [2]: FirstClusterOfRootDirectory
    // [3]: Bytes per Cluster
    // [4]: ClusterCount

    int64_t* offset = getOffsetToDataRegion(op.inputFile);

    if(offset[0] == -1) {
      printf("Got errors from getOffsetToDataRegion\n");
      return !OKAY;
    }
    
    int fd = openFileDescriptor(op.inputFile, 1);

    // Skipping using above offset[0] and function lseek
    lseek(fd, 32 * offset[0], SEEK_SET);

    // Necessary structure to read and store data
    FileDirectoryEntry fde;
    StreamExtensionDirectoryEntry sede;
    FileNameDirectoryEntry fnde;

    int cluster = 2;   // The start index of cluster is 2 according to the Microsoft document
    int count = 0;
    int limit = offset[3] / 32;

    struct FileInfo** arr = malloc(offset[4] * sizeof(struct FileInfo*));
    arr[offset[2]] = FIinit();
    arr[offset[2]]->name = malloc(5 * sizeof(char));
    strcpy(arr[offset[2]]->name, "ROOT");

    // Read all data and get all files and directory
    PQinit();
    PQpush(offset[2]);
    while(!PQempty()) {
      
      int targetCluster = PQpop();
      lseek(fd, (targetCluster - cluster) * offset[3], SEEK_CUR);
      cluster = targetCluster;

      count = 0;
      while(count < limit) {

        // One cluster = bytesPerSector * sectorsPerCluster / 32
        read(fd, &fde, 32);
        count += 1;

        // The directory entry is 0x85. If not, keep reading data
        if(fde.entryType != 0x85) continue;
      
        read(fd, &sede, 32);
        count += 1;
        fde.secondaryCount -= 1;
        arr[sede.firstCluster] = FIinit();
        arr[sede.firstCluster]->status = 1 + (fde.fileAttributes == 0x0010);
        arr[sede.firstCluster]->name = malloc((sede.nameLength + 1) * sizeof(char));

        int length = sede.nameLength;
        arr[sede.firstCluster]->name[length] = 0;
        // arr[sede.firstCluster]->name[length] = 97;
        int at = 0;

        while(fde.secondaryCount--) {
          read(fd, &fnde, 32);
          count += 1;

          for(i = 0; i < 15 && at < length; i++) arr[sede.firstCluster]->name[at++] = (char)fnde.fileName[i];
        }

        FIpush(arr[sede.firstCluster], arr[cluster]);
        if(fde.fileAttributes == 0x0010) PQpush(sede.firstCluster);
      }

      cluster += 1; 
    }

    // Traverse FileInfo tree and print directory of files
    FItraverse(arr[offset[2]], 0);

    // Free space to avoid leak memory
    FIclear(arr[offset[2]]);
    free(arr);
    free(offset);

    // Close the file descriptor
    close(fd);
    return OKAY; 
}