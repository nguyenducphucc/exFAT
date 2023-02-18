/*

   This is an example of using mmap to read an extFAT image file.

   To make the sample file:

    % # create the file system image
    % dd if=/dev/zero of=test.image count=1 bs=1G
    % sudo losetup /dev/loop2 test.image
    % sudo /usr/sbin/mkexfatfs /dev/loop2

    % # put something in the file system image
    % mkdir /tmp/d
    % sudo mount /dev/loop2 /tmp/d
    % cp examples/mmap.c /tmp/d

    % # clean up
    % sudo umount /tmp/d
    % sudo losetup -d /dev/loop2
    % rm -rf /tmp/d
    % rm test.image



   Written by Bud Davis, jimmie.davis@uta.edu
   (c) 2023, All Rights Reserved
   Provided to students of CSE3310, UTA. Any use
   other than this course is prohibited.

*/

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

#include "extfat.h"

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

int main()
{
   // open the file system image file
   int fd = open("test.image", O_RDWR);
   if (fd == -1)
   {
      perror("file open: ");
      exit(0);
   }

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
   //int flag=memcmp(MB,BB, sizeof(Main_Boot));

   //call struct compare function to test if the structs are the same, returns 0 if same
   int flag = cmp_struct(MB,BB);

   //Error message if main and boot are not same
   if(flag!=0)
   {
      printf("\n\n ** MAIN AND BACKUP SECTOR NOT SAME **\n\n");
   }


   // print out some things we care about

   printf("the pointer to MB is %p  \n", MB);

   printf("JumpBoot  %d %d %d \n", MB->JumpBoot[0], MB->JumpBoot[1], MB->JumpBoot[2]);
   printf("FileSystemName %s\n", MB->FileSystemName); // warning, not required to be terminated
   printf("PartitionOffset %ld\n", MB->PartitionOffset);
   printf("VolumeLength %ld\n", MB->VolumeLength);
   printf("FatOffset %d\n", MB->FatOffset);
   printf("FatLength %d\n", MB->FatLength);
   printf("ClusterHeapOffset %d\n", MB->ClusterHeapOffset);
   printf("ClusterCount %d\n", MB->ClusterCount);
   printf("FirstClusterofRootDirectory %d\n", MB->FirstClusterOfRootDirectory);
   printf("VolumeSerialNumber %x\n", MB->VolumeSerialNumber);
   printf("PercentInUse %d\n", MB->PercentInUse);
   

   // check that the offsets match the documentation
   assert(offsetof(Main_Boot, PercentInUse) == 112);
   //printf("the offset of PerCentInUse %ld\n", offsetof(Main_Boot, PercentInUse));

   printf("BytesPerSectorShift %d\n",MB->BytesPerSectorShift);
   printf("SectorsPerClusterShift %d\n",MB->SectorsPerClusterShift);
   printf("NumberOfFats %d\n",MB->NumberOfFats);

   int bytesPerSector = 2  << MB->BytesPerSectorShift-1;
   int sectorsPerCluster = 2 << MB->SectorsPerClusterShift-1;

   printf("-----> the bytesPerSector are %d\n",bytesPerSector);
   printf("-----> the sectorsPerCluster are %d\n",sectorsPerCluster);

   
   // unmap the file
   if (munmap(MB, sizeof(Main_Boot)) == -1)
   {
      perror("error from unmap:");
      exit(0);
   }

   // close the file
   if (close(fd))
   {
      perror("closeStat:");
   }
   fd = 0;

   return 0;
}