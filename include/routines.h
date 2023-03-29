#include <stdint.h>

// perhaps typedefs would be a better solution?

//#define UInt32 uint32_t
//#define UCHAR  uint8_t
//#define USHORT uint16_t

uint32_t BootChecksum
(
    uint8_t  * Sectors,        // points to an in-memory copy of the 11 sectors
    uint16_t   BytesPerSector
);

