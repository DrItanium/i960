#ifndef I960_PROCESSOR_CONTROL_BLOCK_H__
#define I960_PROCESSOR_CONTROL_BLOCK_H__
#include "types.h"

namespace i960 {
    /**
     * Also known as the PRCB, it is a data structure in memory which the cpu uses to track
     * various states
     */
    struct ProcessorControlBlock {
        Ordinal reserved0 = 0;
        Ordinal processorControls;
        Ordinal reserved1 = 0;
        Ordinal currentProcessSS;
        Ordinal dispatchPortSS;
        Ordinal interruptTablePhysicalAddress;
        Ordinal interruptStackPointer;
        Ordinal reserved2 = 0;
        Ordinal region3SS;
        Ordinal systemProcedureTableSS;
        Ordinal faultTablePhysicalAddress;
        Ordinal reserved3 = 0;
        Ordinal multiprocessorPreemption[3];
        Ordinal reserved4 = 0;
        Ordinal idleTime[2];
        Ordinal systemErrorFault;
        Ordinal reserved5;
        Ordinal resumptionRecord[12];
        Ordinal systemErrorFaultRecord[11];
    } __attribute__((packed));
}
#endif // end I960_PROCESSOR_CONTROL_BLOCK_H__
