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
        bool isOverrideFault() const noexcept { return fault.type == 0; }
        bool isParallelFault() const noexcept { return fault.type == 0; }
        bool isTraceFault() const noexcept { return fault.type == 1; }
        bool isOperationFault() const noexcept { return fault.type == 2; }
        bool isArithmeticFault() const noexcept { return fault.type == 3; }
        bool isConstraintFault() const noexcept { return fault.type == 5; }
        bool isProtectionFault() const noexcept { return fault.type == 7; }
        bool isTypeFault() const noexcept { return fault.type == 10; }
        bool isInstructionTraceFault() const noexcept { return isTraceFault() && ((fault.subtype & (1<<1)) != 0); }
        bool isBranchTraceFault() const noexcept { return isTraceFault() && ((fault.subtype & (1<<2)) != 0); }
        bool isCallTraceFault() const noexcept { return isTraceFault() && ((fault.subtype & (1<<3)) != 0); }
        bool isReturnTraceFault() const noexcept { return isTraceFault() && ((fault.subtype & (1<<4)) != 0); }
        bool isPrereturnTraceFault() const noexcept { return isTraceFault() && ((fault.subtype & (1<<5)) != 0); }
        bool isSupervisorTraceFault() const noexcept { return isTraceFault() && ((fault.subtype & (1<<6)) != 0); }
        bool isMarkTraceFault() const noexcept { return isTraceFault() && ((fault.subtype & (1<<7)) != 0); }
        bool isInvalidOpcodeFault() const noexcept { return isOperationFault() && (fault.subtype == 1); }
        bool isUnimplementedFault() const noexcept { return isOperationFault() && (fault.subtype == 2); }
        bool isUnalignedFault() const noexcept { return isOperationFault() && (fault.subtype == 3); }
        bool isInvalidOperandFault() const noexcept { return isOperationFault() && (fault.subtype == 4); }
        bool isIntegerOverflowFault() const noexcept { return isArithmeticFault() && (fault.subtype == 1); }
        bool isZeroDivideFault() const noexcept { return isArithmeticFault() && (fault.subtype == 2); }
        bool isRangeConstraintFault() const noexcept { return isConstraintFault() && (fault.subtype == 1); }
        bool isProtectionLengthFault() const noexcept { return isProtectionFault() && ((fault.subtype & (1<<1)) != 0); }
        bool isTypeMismatchFault() const noexcept { return isTypeFault() && (fault.subtype == 1); }
        bool isPreciseFault() const noexcept {
            return isProtectionFault() ||
                   isTraceFault();
        }
        bool isImpreciseFault() const noexcept {
            return isParallelFault() ||
                   isOperationFault() ||
                   isConstraintFault() ||
                   isArithmeticFault() ||
                   isTypeFault();
        }

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
