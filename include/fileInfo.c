
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
#include <string.h>//add this

//////////////////////////////////KAYLA'S CODE//////////////////////////////////////
void help(){

   printf("How To Execute:\n\n");
   printf("In the command line, type ./extfat -(option).\n");
   printf("The following options are:\n");
   printf("i: takes the input file name as an argument.\n");
   printf("o: takes the output file name as an argument. If no filename is provided, it will output file name will be the same as the input file name.\n");
   printf("c: the input file is copied to the output file without change. \n");
   printf("m: access the file with mmap().\n");
   printf("f: access the file with fread().\n");
   printf("v: takes an input file name as an argument. The main and backup boot sectors will be read and checked to see if they are the same. if they are not the same, an error message will be written to stdout.\n");
}

int main(int argc, char *argv[])
{
   struct Option op;

   op.outputFile = 0;
   op.outputFile = "";
   op.inputFile = "";

   int c = 0;
   //to test on command line: ./extfat -(option) (filename, if applicable)

   while((c = getopt(argc, argv, "i:o:hcmfv:"))!= -1){

      switch (c){
         case 'i':
         op.inputFile = optarg;
         op.outputFile = optarg;
         printf("input file: %s\n", op.inputFile);
         break;

         case 'o':
         op.outputFile = optarg;
         op.outputFlag = 1;
         break;

         case 'h':
         op.helpFlag = 1;
         help();
         break;

         case 'm':
         op.mmapFlag = 1;
         break;

         case 'f':
         op.freadFlag = 1;
         break;

         case 'v':
         op.verify = optarg;
         printf("verify file: %s\n", op.verify);
         break;
         
         case '?':
         printf("Unknown option or missing argument.\n");
         break;
      }
   }

   printf("output file: %s\n", op.outputFile);

////////////////////////////////////////////////////////////////////////

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

   int bytesPerSector = 2  << (MB->BytesPerSectorShift-1);
   int sectorsPerCluster = 2 << (MB->SectorsPerClusterShift-1);

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