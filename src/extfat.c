#include "fileInfo.c"
//#include "verify.c"
#include "fileManip.c"
#include "bootSec.c"

int main(int argc, char *argv[]) {
    Option op = inputProcess(argc, argv);
    if(op.helpFlag || op.errorFlag) return 0;


    copyInputFileToAnotherFile(op);
    bootCheck(op);

    return 0;
}