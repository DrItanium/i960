#ifndef I960_RECORDS_H__
#define I960_RECORDS_H__
#include "types.h"
#include "ProcessControls.h"
#include "ArithmeticControls.h"
#include <optional>
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
        bool flagBitIsSet(int shift) const noexcept { return (fault.flags & (1 << shift)) != 0; }
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
        bool isPageRightsFailedRead() const noexcept { return isProtectionPageRights() && !flagBitIsSet(0); }
        bool isPageRightsFailedWrite() const noexcept { return isProtectionPageRights() && flagBitIsSet(1); }
        bool isPrecise() const noexcept {
            return isPreciseFault(toFaultRecordKind(fault.type););
        }
        bool isImprecise() const noexcept {
            return isImpreciseFault(toFaultRecordKind(fault.type)); 
        }
        ByteOrdinal getNumberOfParallelFaults() const noexcept {
            if (isParallel()) {
                return fault.subtype;
            } else {
                return 0;
            }
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


    class FaultTableEntry {
        public:
            Ordinal getAddress() const noexcept { return _rawFaultHandlerAddress & (~0b11); }
            Ordinal getEntryType() const noexcept { return _rawFaultHandlerAddress & 0b11; }
            bool isLocalCallEntry() const noexcept { return getEntryType() == 0b00; }
            bool isSystemCallEntry() const noexcept { return (getEntryType() == 0b10) && (magicNumber == 0x0000'027F); }
            Ordinal getMagicNumber() const noexcept { return _magicNumber; }
            void setMagicNumber(Ordinal value) noexcept { _magicNumber = value; }
            void setFaultHandlerAddress(Ordinal value) noexcept { _rawFaultHandlerAddress = value; }
            Ordinal getFaultHandlerAddress() const noexcept { return _rawFaultHandlerAddress; }
        private:
            Ordinal _rawFaultHandlerAddress;
            Ordinal _magicNumber;
    } __attribute__((packed));

    struct FaultTable {
        FaultTableEntry entries[32];
        template<FaultRecordKind kind>
        FaultTableEntry& getEntry() noexcept {
            static_assert(isLegalValue(kind), "Illegal value passed to getEntry");
            return entries[convert(kind)];
        }
    } __attribute__((packed));
    // begin interrupt table stuff
    struct InterruptRecord {
        ResumptionRecord record; // optional
        ProcessControls pc;
        ArithmeticControls ac;
        union {
            Ordinal value;
            ByteOrdinal number;
        } vector;
    } __attribute__((packed));

    struct InterruptTable {
        class VectorEntry {
            public:
                Ordinal getType() const noexcept { return _raw & 0b11; }
                Ordinal getIP() const noexcept { return _raw & (~0b11); }
                bool isNormalEntryType() const noexcept { return getType() == 0b00; }
                bool isTargetInCacheType() const noexcept { return getType() == 0b10; }
                bool isReservedType() const noexcept { return getType() == 0b01 || getType() == 0b11; }
                Ordinal getRaw() const noexcept { return _raw; }
                void setRaw(Ordinal value) noexcept { _raw = value; }
            private:
                Ordinal _raw;
        } __attribute__((packed));
        class IllegalInterruptVector : std::exception final {
            public:
                explicit IllegalInterruptVector(ByteOrdinal index) : _index(index) { }
                ~IllegalInterruptVector() = default;
                std::string what() {
                    std::stringstream ss;
                    ss << "Illegal interrupt vector index " << _index;
                    auto str = ss.str();
                    return str;
                }
            private:
                ByteOrdinal _index;
        };
        Ordinal pendingPriorities;
        Ordinal pendingInterrupts[8];
        VectorEntry interruptProcedures[248];
        VectorEntry& getVectorEntry(ByteOrdinal index)
        {
            if (index < 8) {
                // vector numbers below 8 are unsupported
                throw IllegalInterruptVector(index);
            } else {
                return interruptProcedures[index - 8];
            }
        }
        VectorEntry& getNMI() noexcept { return getVectorEntry(248); }
        bool vectorIsReserved(ByteOrdinal index) const noexcept {
            switch(index) {
                case 244:
                case 245:
                case 246:
                case 247:
                case 249:
                case 250:
                case 251:
                    return true;
                default:
                    return false;

            }
        }
        std::optional<VectorEntry> getNMI() 
    } __attribute__((packed));

} // end namespace i960
#endif // end I960_RECORDS_H__
