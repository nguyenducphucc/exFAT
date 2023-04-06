#include "fileInfo.c"
#include "fileManip.c"
#include "verify.c"

int main(int argc, char *argv[]) {
    Option op = inputProcess(argc, argv);
    if(op.helpFlag || op.errorFlag) return 0;

    copyInputFileToAnotherFile(op);
    checkSum(op);

    return 0;
}