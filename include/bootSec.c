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

