
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
#include "fileInfo.h"

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

    Option inputProcess(int argc, char *argv[]) 
    {
        Option op;
        
        op.copyFlag = 0;
        op.helpFlag = 0;
        op.errorFlag = 0;
        op.verifyFlag = 0;

        int c = 0;
        //to test on command line: ./extfat -(option) (filename, if applicable)

        while((c = getopt(argc, argv, "i:o:hcmfv"))!= -1){

            switch (c){
                case 'i':
                op.inputFile = optarg;
                op.outputFile = optarg;
                op.verify = optarg;
                //printf("\ninput file: %s\n", op.inputFile);
                break;

                case 'o':
                op.outputFile = optarg;
                op.outputFlag = 1;
                //printf("\noutput file: %s\n", op.outputFile);
                break;

                case 'c':
                op.copyFlag = 1;
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
                op.verifyFlag = 1;
                //printf("\nverify file: %s\n", op.verify);
                break;
                
                case '?':
                op.errorFlag = 1;
                break;
            }
        }

        return op;
    }
