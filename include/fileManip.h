

int openFileDescriptor(char* path, int isReadOnly);

long getInputFileSize(int fd);

Main_Boot* mmapToFile(int fd, off_t length, int isReadOnly);

int copyInputFileToAnotherFile(char* inputPath, char* outputPath);


