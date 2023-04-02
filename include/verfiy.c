#pragma once
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

#include "verify.h"
#include "routine.h"

void checkSum(Option op){
    if(!op.verify) return;

    printf("hi");
}