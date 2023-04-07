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

    for(i = 0; i < root->realSize; i++) FItraverse(root->vector[i], nested + 1);
}

void FIclear(struct FileInfo* root) {
    int i;
    for(i = 0; i < root->realSize; i++) FIclear(root->vector[i]);
    
    free(root->name);
    free(root->vector);
    free(root);
}

int* pq;
int nul = -1;
int pqSize;
int realSize;

void PQinit() {
    pq = malloc(2 * sizeof(int));
    pqSize = 1;
    realSize = 0;
}

void PQpush(int i) {
    realSize++;

    if(realSize > pqSize) {
        pqSize *= 2;
        pq = realloc(pq, (pqSize + 1) * sizeof(int));
    }

    pq[realSize] = i;
    int index = realSize;
    int parentIndex = index / 2;

    while((pq[index] < pq[parentIndex]) && index > 1) {
        int temp = pq[index];
        pq[index] = pq[parentIndex];
        pq[parentIndex] = temp;
        index = parentIndex;
        parentIndex = index / 2;
    }
}

int PQtop() {
    if(!realSize) {
        printf("There is no item in this heap");
        return nul;
    }

    return pq[1];
}

int PQpop() {
    if(!realSize) {
        printf("There is no item in this heap");
        return nul;
    }

    int res = pq[1];
    pq[1] = pq[realSize];
    realSize--;
    
    int index = 1;
    while(index <= realSize / 2) {
        int left = index * 2;
        int right = index * 2 + 1;

        if((pq[index] > pq[left]) || (pq[index] > pq[right])) {
            if(pq[left] < pq[right]) {
                int temp = pq[left];
                pq[left] = pq[index];
                pq[index] = temp;
                index = left;
            } else {
                int temp = pq[right];
                pq[right] = pq[index];
                pq[index] = temp;
                index = right;
            }
        } else break;
    }

    return res;
}

int PQsize() { return realSize; }
int PQempty() { return !realSize; }
