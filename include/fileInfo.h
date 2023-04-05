#pragma once

typedef struct
{
    char *inputFile;
    char *outputFile;
    int outputFlag;
    int helpFlag;
    int copyFlag;
    int verifyFlag;
    char *verify;
    int mmapFlag;
    int freadFlag;
    int errorFlag; // If an error happens, return this flag setting true
} Option;

typedef struct
{
    char JumpBoot[3];
    char FileSystemName[8];
    char MustBeZero[53];
    long int PartitionOffset;
    long int VolumeLength;
    int FatOffset;
    int FatLength;
    int ClusterHeapOffset;
    int ClusterCount;
    int FirstClusterOfRootDirectory;
    int VolumeSerialNumber;
    short int FileSystemRevision;
    short int VolumeFlags;
    unsigned char BytesPerSectorShift;
    unsigned char SectorsPerClusterShift;
    unsigned char NumberOfFats;
    unsigned char DriveSelect;
    unsigned char PercentInUse;
    unsigned char Reserved[7];
    unsigned char BootCode[390];
    short int BootSignature;
    unsigned char ExcessSpace;
} Main_Boot;

void help();

Option inputProcess(int argc, char *argv[]);