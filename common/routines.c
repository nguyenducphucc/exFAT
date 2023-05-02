#include "routines.h"

// from https://learn.microsoft.com/en-gb/windows/win32/fileio/exfat-specification


uint32_t BootChecksum
(
    uint8_t  * Sectors,        // points to an in-memory copy of the 11 sectors
    uint16_t   BytesPerSector
)
{
    uint32_t NumberOfBytes = (uint32_t)BytesPerSector * 11;
    uint32_t Checksum = 0;
    uint32_t Index;

    for (Index = 0; Index < NumberOfBytes; Index++)
    {
        if ((Index == 106) || (Index == 107) || (Index == 112))
        {
            continue;
        }
        Checksum = ((Checksum&1) ? 0x80000000 : 0) + (Checksum>>1) + (uint32_t)Sectors[Index];
    }

    return Checksum;
}
