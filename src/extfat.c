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
#include <getopt.h>
#include <string.h>
#include "extfat.h"

#include "fileManip.h"

#define OKAY 1


int main()
{

    struct Option op;

    char *hardcodeInputPath;
    char *hardcodeOutputPath;
    op.outputFile = "";
    op.inputFile = "";

    hardcodeInputPath = op.inputFile;
    hardcodeOutputPath = op.outputFile;
    
    if(copyInputFileToAnotherFile(hardcodeInputPath, hardcodeOutputPath) != OKAY) {
      perror("Got problems while copying file to another file.");
    }


    return 0;
}