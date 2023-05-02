#pragma once
#include <sys/types.h>
#define OKAY 1

int openFileDescriptor(char* path, int isReadOnly);

long getInputFileSize(int fd);

Main_Boot* mmapToFile(int fd, off_t length, int isReadOnly);

int copyInputFileToAnotherFile(Option op);