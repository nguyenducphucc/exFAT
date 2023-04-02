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
#include <ctype.h>

#include "verify.h"
#include "routines.h"

void checkSum(Option op){
    if(!op.verify) return;

   int fd = open(op.verify, O_RDWR);
   if (fd == -1)
   {
      perror("file open: ");
      exit(0);
   }

   // Take the pointer returned from mmap() and turn it into

  // Take the pointer returned from mmap() and turn it into
   // a structure that understands the layout of the data
   //This sets the pointer to the main boot sector, at offset 0
   Main_Boot *MB = (Main_Boot *)mmap(NULL,
                                     sizeof(Main_Boot),
                                     PROT_READ,
                                     MAP_PRIVATE,
                                     fd,
                                     0); // note the offset

   //This sets the pointer to backup boot sector, offset by 4096 bytes which is the page size
   void *t    = (Main_Boot *)mmap(NULL,
                                     sizeof(Main_Boot) + 4096, // worse case misallignment is 4096
                                     PROT_READ,
                                     MAP_PRIVATE,
                                     fd,
                                     4096); // note the offset

   t+=(512*12)-4096;
   //Sets pointer to backup boot sector 
   Main_Boot *BB = (Main_Boot*) t;
   if (MB == (Main_Boot *)-1)
   {
      perror("error from mmap:");
      exit(0);
   }

    int bytesPerSector = 2  << (MB->BytesPerSectorShift - 1);

   uint32_t mbrChecksum = BootChecksum((uint8_t*) MB, (short) bytesPerSector);
   uint32_t bbrChecksum = BootChecksum((uint8_t*) BB, (short) bytesPerSector);

   printf("verify complete, verify = %d", (mbrChecksum==bbrChecksum));
   printf("Name of input file: %c", op.verify);
   printf("Checksum  (MB) %x (BBR) %x\n",mbrChecksum,bbrChecksum);
   
   if(mbrChecksum==bbrChecksum){printf("The main and backup boot sectors are the same.")}
   else{printf("The main and backup boot sectors are different.")};

}