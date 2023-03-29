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
#include <getopt.h>//add this
#include <string.h>//a dd this
#include "bootSec.h"


//function to take in struct pointers and compare each part of the struct
int cmp_struct(Main_Boot* MB, Main_Boot* BB)
{
   int count=0;
   if(MB->JumpBoot[0]==BB->JumpBoot[0])
      count++;
   if(MB->JumpBoot[1]==BB->JumpBoot[1])
      count++;
   if(MB->JumpBoot[2]==BB->JumpBoot[2])
      count++;
   if(strcmp(MB->FileSystemName,BB->FileSystemName)==0)
      count++;
   if(MB->PartitionOffset==BB->PartitionOffset)
      count++;
   if(MB->VolumeLength==BB->VolumeLength)
      count++;
   if(MB->FatOffset==BB->FatOffset)
      count++;
   if(MB->FatLength==BB->FatLength)
      count++;
   if(MB->ClusterHeapOffset==BB->ClusterHeapOffset)
      count++;
   if(MB->ClusterCount == BB->ClusterCount)
      count++;
   if(MB->FirstClusterOfRootDirectory==BB->FirstClusterOfRootDirectory)
      count++;
   if(MB->VolumeSerialNumber == BB->VolumeSerialNumber)
      count++;
   if(MB->PercentInUse == BB->PercentInUse)
      count++;

   if(count==13)
      return 0;
   else
      return -1;
}

void bootCheck(Option op) {
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

   //Add in the functionality to meet the requirement #6, verifies that Main Boot Sector and Backup Boot are identical
   //int flag=memcmp(MB,BB, sizeof(Main_Boot)); //
   //call struct compare function to test if the structs are the same, returns 0 if same
   if(cmp_struct(MB,BB) == 0) printf("\n\n ** MAIN AND BACKUP SECTOR ARE THE SAME **\n\n");
   else printf("\n\n ** MAIN AND BACKUP SECTOR ARE NOT THE SAME **\n\n");
}