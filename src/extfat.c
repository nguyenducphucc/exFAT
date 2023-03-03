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
#include "../include/fileInfo.h"
#include "../include/fileManip.h"

int main(int argc, char *argv[]) {
    if(copyInputFileToAnotherFile(inputProcess(argc, argv)) != OKAY)
        printf("\nERROR --- Copy paste file process --- FAILED \n\n");

    return 0;
}