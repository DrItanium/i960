#ifndef I960_PROCESSOR_CONTROL_BLOCK_H__
#define I960_PROCESSOR_CONTROL_BLOCK_H__
#include "types.h"

namespace i960 {
    /**
     * Also known as the PRCB, it is a data structure in memory which the cpu uses to track
     * various states
     */
    struct JxHxCxProcessorControlBlock {
        Ordinal faultTableAddress;
        Ordinal controlTableAddress;
        Ordinal initialACRegister;
        Ordinal faultConfiguration;
        Ordinal interruptTableAddress;
        Ordinal systemProcedureTableAddress;
        Ordinal reserved;
        Ordinal interruptStackAddress;
        Ordinal instructionCacheConfiguration;
        Ordinal registerCacheConfiguration;
    } __attribute__((packed));

    struct KxSxProcessorControlBlock {
        Ordinal reserved0;
        Ordinal magicNumber0;
        Ordinal reserved1[3];
        Ordinal interruptTableAddress;
        Ordinal interruptStackAddress;
        Ordinal reserved2;
        Ordinal magicNumber1, magicNumber2;
        Ordinal faultTableAddress;
        Ordinal reserved3[32];
    } __attribute__((packed));
    
    template<ProcessorSeries proc>
    struct ProcessorControlBlockType final {
        ProcessorControlBlockType() = delete;
        ~ProcessorControlBlockType() = delete;
        ProcessorControlBlockType(const ProcessorControlBlockType&) = delete;
        ProcessorControlBlockType(ProcessorControlBlockType&&) = delete;
        ProcessorControlBlockType& operator=(const ProcessorControlBlockType&) = delete;
        ProcessorControlBlockType& operator=(ProcessorControlBlockType&&) = delete;
    };
#define X(series, type) \
    template<> \
    struct ProcessorControlBlockType<ProcessorSeries:: series > final { \
        ProcessorControlBlockType() = delete; \
        ~ProcessorControlBlockType() = delete; \
        ProcessorControlBlockType(const ProcessorControlBlockType&) = delete; \
        ProcessorControlBlockType(ProcessorControlBlockType&&) = delete; \
        ProcessorControlBlockType& operator=(const ProcessorControlBlockType&) = delete; \
        ProcessorControlBlockType& operator=(ProcessorControlBlockType&&) = delete; \
        using ValueType = type ; \
    }
    X(Jx, JxHxCxProcessorControlBlock);
    X(Hx, JxHxCxProcessorControlBlock);
    X(Cx, JxHxCxProcessorControlBlock);
    X(Kx, KxSxProcessorControlBlock);
    X(Sx, KxSxProcessorControlBlock);
#undef X 

    template<ProcessorSeries proc>
    using ProcessorControlBlock = typename ProcessorControlBlockType<proc>::ValueType;
}
#endif // end I960_PROCESSOR_CONTROL_BLOCK_H__
