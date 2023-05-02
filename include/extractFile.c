#include "extractFile.h"

void extractPath(char *pathname) {
  STKinit();

  char* name = strtok(pathname, "/");
  while(name != NULL) {
    if(!strcmp(name, ".")) {}
    else if(!strcmp(name, "..")) { STKpop(); }
    else { STKpush(name); }

    name = strtok(NULL, "/");
  }
}

int extractFile(Option op) {
    if(!op.extractFlag) return OKAY;

    if(!op.inputFile || !op.outputFile || !op.extractFile) {
      printf("Missing required inputFile / outputFile");
      return !OKAY;
    }

    // Break name path into several name tokens
    extractPath(op.extractFile);

    // Function borrowed from listFile.c
    int64_t* offset = getOffsetToDataRegion(op.inputFile);

    if(offset[0] == -1) {
      printf("Got errors from getOffsetToDataRegion\n");
      return !OKAY;
    }

    // Open file descriptor of input and output file
    int fd = openFileDescriptor(op.inputFile, 1);
    int fdout = openFileDescriptor(op.outputFile, 0);

    // Skipping using above offset[0] and function lseek
    lseek(fd, 32 * offset[0], SEEK_SET);

    // Necessary structure to read and store data
    FileDirectoryEntry fde;
    StreamExtensionDirectoryEntry sede;
    FileNameDirectoryEntry fnde;

    int cur = 2;
    int next = offset[2];
    int count = 0;
    int limit = offset[3] / 32;
    int j;
    int i = 1;
    int k = 1;

    // Start grabbing content of specific file
    while(i <= stkRealSize && cur != next && k) {
      k = 0;
      lseek(fd, (next - cur) * offset[3], SEEK_CUR);
      cur = next;

      count = 0;
      while(count < limit) {
        read(fd, &fde, 32);
        count += 1;

        if(fde.entryType != 0x85) continue;
        
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

    // Only print out content if it is a file
    if(k == 1) {
      lseek(fd, (next - cur) * offset[3], SEEK_CUR);
      lseek(fdout, 0, SEEK_SET);
      int hash = 1;
      while(hash) {
        read(fd, &hash, 1);
        if(hash > 9) write(fdout, (char*) &hash, 1);
      }
    }

    // Free the memory
    STKdestroy();
    free(offset);

    // Close the file
    close(fd); 
    close(fdout);
    return OKAY;
}