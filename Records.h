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
    enum class FaultRecordKind : ByteOrdinal
    {
        Override = 0,
        Parallel = Override,
        Trace = 1,
        Operation,
        Arithmetic,
        FloatingPoint,
        Constraint,
        VirtualMemory,
        Protection,
        Machine,
        Structural,
        Type,
        Reserved,
        Process,
        Descriptor,
        Event,
    };
    constexpr auto isLegalValue(FaultRecordKind kind) noexcept {
        return convert(kind) < 0x10;
    }
    constexpr auto convert(FaultRecordKind kind) noexcept {
        return static_cast<ByteOrdinal>(kind);
    }
    constexpr FaultRecordKind toFaultRecordKind(ByteOrdinal value) noexcept {
        return static_cast<FaultRecordKind>(value);
    }
    constexpr auto isPreciseFault(FaultRecordKind kind) noexcept {
        switch (kind) {
            case FaultRecordKind::Trace:
            case FaultRecordKind::VirtualMemory:
            case FaultRecordKind::Protection:
            case FaultRecordKind::Descriptor:
                return true;
            default:
                return false;
        }
    }
    constexpr auto isImpreciseFault(FaultRecordKind kind) noexcept {
        switch (kind) {
            case FaultRecordKind::Trace:
            case FaultRecordKind::VirtualMemory:
            case FaultRecordKind::Protection:
            case FaultRecordKind::Descriptor:
            case FaultRecordKind::Override:
            case FaultRecordKind::Parallel:
            case FaultRecordKind::Reserved:
                return false;
            default:
                return true;

        }
    }
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
        bool subtypeBitIsSet(int shift) const noexcept { return (fault.subtype & (1 << shift)) != 0; }
        bool subtypeValueIs(ByteOrdinal code) const noexcept { return fault.subtype == code; }
        bool isOfKind(FaultRecordKind kind) const noexcept { return convert(kind) == fault.type; }
#define X(kind) bool is ## kind () const noexcept { return isOfKind(FaultRecordKind:: kind ) ; }
        X(Override);
        X(Parallel);
        X(Trace);
        bool isInstructionTrace() const noexcept { return isTrace() && subtypeBitIsSet(1); }
        bool isBranchTrace() const noexcept { return isTrace() && subtypeBitIsSet(2); }
        bool isCallTrace() const noexcept { return isTrace() && subtypeBitIsSet(3); }
        bool isReturnTrace() const noexcept { return isTrace() && subtypeBitIsSet(4); }
        bool isPrereturnTrace() const noexcept { return isTrace() && subtypeBitIsSet(5); }
        bool isSupervisorTrace() const noexcept { return isTrace() && subtypeBitIsSet(6); }
        bool isMarkTrace() const noexcept { return isTrace() && subtypeBitIsSet(7); }
        X(Operation);
        bool isInvalidOpcode() const noexcept { return isOperation() && subtypeValueIs(1); }
        bool isUnimplemented() const noexcept { return isOperation() && subtypeValueIs(2); }
        bool isUnaligned() const noexcept { return isOperation() && subtypeValueIs(3); }
        bool isInvalidOperand() const noexcept { return isOperation() && subtypeValueIs(4); }
        X(Arithmetic);
        bool isIntegerOverflow() const noexcept { return isArithmetic() && subtypeValueIs(1); }
        bool isZeroDivide() const noexcept { return isArithmetic() && subtypeValueIs(2); }
        X(FloatingPoint);
#define Y(subkind, shift) \
        bool isFloatingPoint ## subkind () const noexcept { \
            return isFloatingPoint() && \
            subtypeBitIsSet( shift ) ; \
        }
        Y(Overflow, 0);
        Y(Underflow, 1);
        Y(InvalidOperation, 2);
        Y(ZeroDivide, 3);
        Y(Inexact, 4);
        Y(ReservedEncoding, 5);
#undef Y
        X(Constraint);
        bool isRangeConstraint() const noexcept { return isConstraint() && subtypeValueIs(1); }
        bool isPrivileged() const noexcept { return isConstraint() && subtypeValueIs(2); }
        X(VirtualMemory);
        bool isInvalidSegmentTableEntry() const noexcept { return isVirtualMemory() && subtypeValueIs(1); }
        bool isInvalidPageTableDirEntry() const noexcept { return isVirtualMemory() && subtypeValueIs(2); }
        bool isInvalidPageTableEntry() const noexcept { return isVirtualMemory() && subtypeValueIs(3); }
        X(Protection);
        bool isProtectionLength() const noexcept { return isProtection() && subtypeBitIsSet(1); }
        bool isProtectionPageRights() const noexcept { return isProtection() && subtypeBitIsSet(2); }
        X(Machine);
        bool isBadAccess() const noexcept { return isMachine() && subtypeValueIs(1); }
        X(Structural);
        bool isControlStructural() const noexcept { return isStructural() && subtypeValueIs(1); } 
        bool isDispatchStructural() const noexcept { return isStructural() && subtypeValueIs(2); } 
        bool isIACStructural() const noexcept { return isStructural() && subtypeValueIs(3); } 
        X(Type);
        bool isTypeMismatch() const noexcept { return isType() && subtypeValueIs(1); }
        bool isTypeContents() const noexcept { return isType() && subtypeValueIs(2); }
        X(Reserved);
        X(Process);
        bool isTimeSlice() const noexcept { return isProcess() && subtypeValueIs(1); }
        X(Descriptor);
        bool isInvalidDescriptor() const noexcept { return isDescriptor() && subtypeValueIs(1); } 
        X(Event);
        bool isNoticeEvent() const noexcept { return isEvent() && subtypeValueIs(1); }
#undef X
        bool isPrecise() const noexcept {
            return isPreciseFault(toFaultRecordKind(fault.type););
        }
        bool isImprecise() const noexcept {
            return isImpreciseFault(toFaultRecordKind(fault.type)); 
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
        FaultTableEntry entries[32];
        template<FaultRecordKind kind>
        FaultTableEntry& getEntry() noexcept {
            static_assert(isLegalValue(kind), "Illegal value passed to getEntry");
            return entries[convert(kind)];
        }
    } __attribute__((packed));

} // end namespace i960
#endif // end I960_RECORDS_H__
