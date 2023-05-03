#pragma once
#include <string.h>

int extractFile(Option op);

char** stk;
int stkSize, stkRealSize, stkMaxSize;

void STKinit() {
    stk = malloc(2 * sizeof(char*));
    stkSize = 1;
    stkRealSize = 0;
    stkMaxSize = 0;
}

void STKpush(char* str) {
    stkRealSize++;
    if(stkRealSize <= stkMaxSize) free(stk[stkRealSize]);
    else stkMaxSize = stkRealSize;

    if(stkRealSize > stkSize) {
        stkSize *= 2;
        stk = realloc(stk, (stkSize + 1) * sizeof(char*));
    }

    stk[stkRealSize] = malloc((strlen(str) + 1) * sizeof(char));
    strcpy(stk[stkRealSize], str);
}

void STKpop() {
    stkRealSize--;
}

void STKdestroy() {
    while(stkMaxSize) free(stk[stkMaxSize--]);
    free(stk);
}