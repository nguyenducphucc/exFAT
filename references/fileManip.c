#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stddef.h>
#include <string.h>
#include "fileManip.h"

#define OKAY 1

int openFileDescriptor(char* path, int isReadOnly) {
    /*
        int open(const char *pathname, int flags);
        int open(const char *pathname, int flags, mode_t mode);

        O_RDONLY:   read only flags
        O_RDWR:     read and write flags
        O_CREAT:    flag that states if pathname does not exist, create it as a regular file.
        O_TRUNC:    flag that states if the file already exists and is a regular file and the access 
                    mode allows writing, it will be truncated to length 0.

        S_IRUSR:    user has read permission --- mode
        S_IWUSR:    user has write permission -- mode
    */

    int flags = isReadOnly ? O_RDWR : (O_RDWR | O_CREAT | O_TRUNC);
    int mode = S_IRUSR | S_IWUSR;
    return isReadOnly ? open(path, flags) : open(path, flags, mode);
}

long getInputFileSize(int fd) {
    //  int fstat(int fd, struct stat *statbuf);

    struct stat st;
    if(fstat(fd, &st) == -1) return -1;
    return st.st_size;
}

Main_Boot* mmapToFile(int fd, off_t length, int isReadOnly) {
    /*
        void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);

        PROT_READ:    File can be read
        PROT_WRITE:   File can be written
        MAP_SHARED:   Updates are visible and carried through underlying file
    */

    int prot = isReadOnly ? PROT_READ : (PROT_READ | PROT_WRITE);
    int flags = MAP_SHARED;
    off_t offset = 0;
    void* addr = 0;

    return (Main_Boot*) mmap(addr, length, prot, flags, fd, offset);
}

int copyInputFileToAnotherFile(Option op) {

    if(!op.inputFile || !op.outputFile) {
      printf("Missing required inputFile / outputFile");
      return !OKAY;
    }
   
    if(!strcmp(op.inputFile, op.outputFile)){
    
      return OKAY;
    } 
    
    // Open file descriptor of input file and output file
    int fdin = openFileDescriptor(op.inputFile, 1);
    int fdout = openFileDescriptor(op.outputFile, 0);

    if (fdin == -1 || fdout == -1) {
      perror("error from openFile:");
      return !OKAY;
    }

    if(!op.copyFlag) {
      close(fdin);
      close(fdout);
      return OKAY;
    }

    // Get the size of the input file
    long size = getInputFileSize(fdin);

    if(size == -1) {
      perror("error from getInputFileSize: ");
      return !OKAY;
    }
    
    lseek (fdout, size, SEEK_SET);
    lseek (fdout, 0, SEEK_SET);

    // Create mappings in the virtual address space
    Main_Boot* src = mmapToFile(fdin, size, 1);
    // Main_Boot* to = mmapToFile(fdout, size, 0);

    if (src == (Main_Boot *) -1 /* || to == (Main_Boot *) -1 */) {
      perror("error from mmap:");
      return !OKAY;
    }

    // write() function that write from the buffer to the file referred to by fdout
    write(fdout, src, size);

    // Unmap the file
    if (munmap(src, sizeof(Main_Boot)) == -1 /* || munmap(to, sizeof(Main_Boot)) == -1 */) {
      perror("error from unmap:");
      return !OKAY;
    }

    // Close the file
    close(fdin); 
    close(fdout);

    return OKAY;
}


