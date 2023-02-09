
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stddef.h>

#include "extfat.h"

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
   Main_Boot *MB = (Main_Boot *)mmap(NULL,
                                     sizeof(Main_Boot),
                                     PROT_READ,
                                     MAP_PRIVATE,
                                     fd,
                                     0); // note the offset

   if (MB == (Main_Boot *)-1)
   {
      perror("error from mmap:");
      exit(0);
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

   int bytesPerSector = 2  << MB->BytesPerSectorShift;
   int sectorsPerCluster = 2 << MB->SectorsPerClusterShift;

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