#include "../include/fileInfo.h"
#include "../include/fileManip.h"

int main(int argc, char *argv[]) {
    if(copyInputFileToAnotherFile(inputProcess(argc, argv)) != OKAY)
        printf("\nERROR --- Copy paste file process --- FAILED \n\n");

    return 0;
}