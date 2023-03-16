#pragma once
#include <stdint.h>

typedef struct {
    uint8_t     entryType;                  // Must be 0x85 to be shown file directory entry
    uint8_t     secondaryCount;             // At least 2, one for 0xC0 entry and the rest for 0xC1 entry
    uint16_t    setCheckSum;                
    uint16_t    fileAttributes;
    uint8_t     reserved1[2];
    uint32_t    createTimestamp;
    uint32_t    lastModifiedTimestamp;
    uint32_t    lastAccessedTimestamp;
    uint8_t     create10msIncrement;
    uint8_t     lastModified10msIncrement;
    uint8_t     createUTCOffset;
    uint8_t     lastModifiedUTCOffset;
    uint8_t     lastAccessedUTCOffset;
    uint8_t     reserved2[7];
} FileDirectoryEntry;

typedef struct {
    uint8_t     entryType;                  // Must be 0xC0 to be shown stream extension directory entry
    uint8_t     generalSecondaryFlags;
    uint8_t     reserved1;
    uint8_t     nameLength;                 // Length of file name
    uint16_t    nameHash;
    uint8_t     reserved2[2];
    uint64_t    validDataLength;
    uint8_t     reserved3[4];
    uint32_t    firstCluster;
    uint64_t    dataLength;
} StreamExtensionDirectoryEntry;

typedef struct {
    uint8_t     entryType;                  // Must be 0xC1
    uint8_t     generalSecondaryFlags;      // Must be 0x00 to allow attributes "AllocationPossible" and "NoFatChain"
    uint16_t    fileName[15];               // Name of the file
} FileNameDirectoryEntry;

int64_t*    getOffsetToDataRegion(char* filename);
int         listDirectoryofFiles(Option op);

struct FileInfo {
    int         status;                     // 0 is undefined, 1 is file, 2 directory
    char*       name;

    int         size;
    int         realSize;
    struct FileInfo** vector;
};

struct FileInfo* FIinit() {
    struct FileInfo* p = malloc(sizeof(struct FileInfo));
    p->status = 0;
    p->size = 1;
    p->realSize = 0;
    p->vector = malloc(sizeof(struct FileInfo*));
    return p;
}

void FIpush(struct FileInfo* from, struct FileInfo* to) {
    if(to->size == to->realSize) {
        to->size *= 2;
        to->vector = realloc(to->vector, to->size * sizeof(struct FileInfo*));
    }
    
    to->vector[to->realSize++] = from;
}

void FItraverse(struct FileInfo* root, int nested) {
    int i;

    for(i = 0; i < nested; i++) printf("     ");
    printf("|-- %s %s\n", root->name, root->status == 1 ? "(FILE)" : "(DIR)");

    for(int i = 0; i < root->realSize; i++) FItraverse(root->vector[i], nested + 1);
}


