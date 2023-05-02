#include "fileInfo.c"
#include "fileManip.c"
#include "verify.c"
#include "bootSec.c"
#include "listFile.c"
#include "extractFile.c"
#include "deleteFile.c"

int main(int argc, char *argv[]) {
    Option op = inputProcess(argc, argv);
    if(op.helpFlag || op.errorFlag) return 0;

    copyInputFileToAnotherFile(op);
    listDirectoryofFiles(op);
    checkSum(op);
    extractFile(op);
    deleteFile(op);

    return 0;
}