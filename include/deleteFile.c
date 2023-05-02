#include "deleteFile.h"

int deleteFile (Option op) {
    if(!op.deleteFlag) return 1;

    if(!op.inputFile || !op.deleteFile) {
      printf("Missing required inputFile / deleteFile");
      return 0;
    }
    
    // Borrows function from extractFile.c
    extractPath(op.deleteFile);

    // Borrows function from listFile.c
    int64_t* offset = getOffsetToDataRegion(op.inputFile);

    if(offset[0] == -1) {
      printf("Got errors from getOffsetToDataRegion\n");
      return !OKAY;
    }

    // Borrows function from fileManip.c
    int fd = openFileDescriptor(op.inputFile, 1);

    // Skipping using above offset[0] and function lseek
    lseek(fd, 32 * offset[0], SEEK_SET);

    // Necessary structure borrowed from listFile.h
    FileDirectoryEntry fde;
    StreamExtensionDirectoryEntry sede;
    FileNameDirectoryEntry fnde;

    int cur = 2;
    int next = offset[2];
    int count = 0;
    int last = 0;
    int limit = offset[3] / 32;
    int j;
    int i = 1;
    int k = 1;

    // Start grabbing content of specific file
    while(i <= stkRealSize && cur != next && k) {
      k = 0;
      lseek(fd, (next - cur) * offset[3], SEEK_CUR);
      cur = next;
      
      last = 0;
      count = 0;
      while(count < limit) {
        read(fd, &fde, 32);
        count += 1;

        if(fde.entryType != 0x85) continue;
        last = count - 1;

        read(fd, &sede, 32);
        count += 1;
        fde.secondaryCount -= 1;
        
        int length = sede.nameLength;
        char* filename = malloc((length + 1) * sizeof(char));
        filename[length] = 0;
        int at = 0;

        while(fde.secondaryCount--) {
          read(fd, &fnde, 32);
          count += 1;

          for(j = 0; j < 15 && at < length; j++) filename[at++] = (char)fnde.fileName[j];
        }

        // Compare to the name tokens, if all file in the folder is not match, ends immediately
        if(!strcmp(filename, stk[i])) {
          next = sede.firstCluster;
          free(filename);
          lseek(fd, (limit - count) * 32, SEEK_CUR);
          k = 1 + (fde.fileAttributes == 0x0010);
          break;
        }

        free(filename);
      }

      i += 1;
      cur += 1;
    }

    if(k == 1) {
      lseek(fd, (last - limit) * 32, SEEK_CUR);

      // Clear InUse property
      int hash = 0x05;
      write(fd, &hash, 1);
    } 
    
    else {
      printf("\n!!! ERROR --- This is probably not-a-file-name, unexisted file or a directory, can not proceed to delete --- EXIT !!!\n\n");
    }

    // Free the memory
    STKdestroy();
    free(offset);

    // Close the file
    close(fd); 

    return 1;
}