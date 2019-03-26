#ifndef I960_MEMORY_MAP_H__
#define I960_MEMORY_MAP_H__
#include "types.h"
namespace i960 {
    enum class MemoryMap : Ordinal {
        NMIVector = 0x0000'0000,
        OptionalInterruptVectorsBegin = 0x0000'0004,
        OptionalInterruptVectorsEnd = 0x0000'003F,
        DataCacheUnreservedStart = 0x0000'0040,
        DataCacheUnreservedEnd = 0x0000'03FF,
        // internal data cache end
        // normal memory begin
        ExternalUnusedMemoryBegin = 0x0000'0400,
        ExternalUnusedMemoryEnd = 0xFEFF'FF2F,
        InitializationBootRecordBegin = 0xFEFF'FF30;
        InitializationBootRecordEnd = 0xFEFF'FF30 + 48;
        ReservedMemoryBegin = 0xFEFF'FF60,
        ReservedMemoryEnd = 0xFEFF'FFFF,
        MemoryMappedRegisterSpaceBegin = 0xFF00'0000,
        MemoryMappedRegisterSpaceEnd = 0xFFFF'FFFF,
    };

} // end namespace i960
#endif // end I960_MEMORY_MAP_H__
