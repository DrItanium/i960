#ifndef I960_RECORDS_H__
#define I960_RECORDS_H__
#include "types.h"
#include "ProcessControls.h"
#include "ArithmeticControls.h"
namespace i960 {
    /**
     * Describes the intermediate processor state from a fault or interrupt 
     * occurring during processor execution
     */
    using ResumptionRecord = ByteOrdinal[16];
    struct FaultRecord {
        ProcessControls pc;
        ArithmeticControls ac;
        union {
            Ordinal value;
            struct {
                ByteOrdinal subtype;
                ByteOrdinal reserved;
                ByteOrdinal type;
                ByteOrdinal flags;
            };
        } fault;
        Ordinal faultingInstructionAddr;
    } __attribute__((packed));
    struct FullFaultRecord {
        Ordinal reserved = 0;
        Ordinal overrideFaultData[3];
        Ordinal faultData[3];
        union {
            Ordinal value;
            struct {
                ByteOrdinal subtype;
                ByteOrdinal reserved;
                ByteOrdinal type;
                ByteOrdinal flags;
            };
        } override;
        FaultRecord actual;
    } __attribute__((packed));

    struct InterruptRecord {
        ResumptionRecord record; // optional
        ProcessControls pc;
        ArithmeticControls ac;
        union {
            Ordinal value;
            ByteOrdinal number;
        } vector;
        // I see 
    } __attribute__((packed));

    struct FaultTableEntry {
        struct {
            Ordinal entryType : 2;
            Ordinal faultHandlerAddress : 30;
        } __attribute__((packed));
        Ordinal magicNumber;
        bool isLocalCallEntry() const noexcept { return entryType == 0b00; }
        bool isSystemCallEntry() const noexcept { return (entryType == 0b10) && (magicNumber == 0x0000'027F); }
    } __attribute__((packed));

    struct FaultTable {
        enum class Index {
            Override = 0,
            Parallel = Override,
            Trace = 1,
            Operation = 2, 
            Arithmetic = 3,
            Constraint = 5,
            Protection = 7,
            Type = 10,
        };
        FaultTableEntry entries[32];
        FaultTableEntry& getOverrideFaultEntry() noexcept { return entries[Index::Override]; }
        FaultTableEntry& getParallelFaultEntry() noexcept { return entries[Index::Parallel]; }
        FaultTableEntry& getTraceFaultEntry() noexcept { return entries[Index::Trace]; }
        FaultTableEntry& getOperationFaultEntry() noexcept { return entries[Index::Operation]; }
        FaultTableEntry& getArithmeticFaultEntry() noexcept { return entries[Index::Arithmetic]; }
        FaultTableEntry& getConstraintFaultEntry() noexcept { return entries[Index::Constraint]; }
        FaultTableEntry& getProtectionFaultEntry() noexcept { return entries[Index::Protection]; }
        FaultTableEntry& getTypeFaultEntry() noexcept { return entries[Index::Type]; }
    } __attribute__((packed));

} // end namespace i960
#endif // end I960_RECORDS_H__
