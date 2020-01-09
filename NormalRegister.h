#ifndef I960_NORMAL_REGISTER_H__
#define I960_NORMAL_REGISTER_H__

#include "types.h"
#include "ProcessControls.h"
namespace i960 {
    class PreviousFramePointer {
        public:
            static constexpr Ordinal ReturnCodeMask = 0x0000'0007;
            static constexpr Ordinal PreReturnTraceMask = 0x0000'0008;
            static constexpr Ordinal AddressMask = 0xFFFF'FFC0; // aligned to 64-byte boundaries
            static constexpr Ordinal ExtractionMask = constructOrdinalMask(ReturnCodeMask, PreReturnTraceMask, AddressMask); 
            static constexpr Ordinal ReservedMask = ~ExtractionMask;
            constexpr PreviousFramePointer(Ordinal raw) noexcept : 
                _returnCode(ReturnCodeMask & raw), 
                _prereturnTrace(PreReturnTraceMask & raw),
                _address(AddressMask & raw) { }
            constexpr PreviousFramePointer() noexcept = default;
            constexpr auto getReturnCode() const noexcept { return _returnCode; }
            void setReturnCode(Ordinal value) noexcept { _returnCode = ReturnCodeMask & value; }
            constexpr auto getPrereturnTrace() const noexcept { return _prereturnTrace; }
            void setPrereturnTrace(bool value) noexcept { _prereturnTrace = value; }
            constexpr auto getAddress() const noexcept { return _address; }
            void setAddress(Ordinal value) noexcept { _address = AddressMask & value; }
            constexpr auto getRawValue() const noexcept {
                return (getReturnCode() | getAddress() | (getPrereturnTrace() ? PreReturnTraceMask : 0)) & ExtractionMask;
            }
            void setRawValue(Ordinal raw) noexcept {
                _returnCode = ReturnCodeMask & raw;
                _prereturnTrace = PreReturnTraceMask & raw;
                _address = AddressMask & raw;
            }
        private:
            Ordinal _returnCode = 0;
            bool _prereturnTrace = false;
            Ordinal _address = 0;
    };
    class ProcedureEntry {
        public:
            static constexpr Ordinal TypeMask = 0b11;
            static constexpr auto AddressMask = ~TypeMask;
            constexpr ProcedureEntry() noexcept = default;
            constexpr ProcedureEntry(Ordinal raw) noexcept : _type(raw & TypeMask), _address(raw & AddressMask) { }
            constexpr bool isLocal() const noexcept { return _type == 0; }
            constexpr bool isSupervisor() const noexcept { return _type == 0b10; }
            constexpr auto getAddress() const noexcept { return _address; }
            constexpr auto getRawValue() const noexcept { return (_type & TypeMask) | (_address & AddressMask); }
            void setType(Ordinal type) noexcept { _type = TypeMask & type; }
            void setAddress(Ordinal addr) noexcept { _address = (addr & _address); }
            constexpr auto getType() const noexcept { return _type; }
        private:
            Ordinal _type = 0;
            Ordinal _address = 0;

    };
    class TraceControls {
        public:
            static constexpr Ordinal InstructionTraceModeMask = 0x0000'0002;
            static constexpr Ordinal BranchTraceModeMask = 0x0000'0004;
            static constexpr Ordinal CallTraceModeMask = 0x0000'0008;
            static constexpr Ordinal ReturnTraceModeMask = 0x0000'0010;
            static constexpr Ordinal PrereturnTraceModeMask = 0x0000'0020; 
            static constexpr Ordinal SupervisorTraceModeMask = 0x0000'0040;
            static constexpr Ordinal MarkTraceModeMask = 0x0000'0080;
            static constexpr Ordinal InstructionAddressBreakpoint0Mask = 0x0100'0000;
            static constexpr Ordinal InstructionAddressBreakpoint1Mask = 0x0200'0000;
            static constexpr Ordinal DataAddressBreakpoint0Mask = 0x0400'0000;
            static constexpr Ordinal DataAddressBreakpoint1Mask = 0x0800'0000;
            static constexpr Ordinal ExtractionMask = constructOrdinalMask(
                    InstructionTraceModeMask, 
                    BranchTraceModeMask, 
                    CallTraceModeMask, 
                    ReturnTraceModeMask, 
                    PrereturnTraceModeMask,
                    SupervisorTraceModeMask,
                    MarkTraceModeMask,
                    InstructionAddressBreakpoint0Mask,
                    InstructionAddressBreakpoint1Mask,
                    DataAddressBreakpoint0Mask,
                    DataAddressBreakpoint1Mask);
#if 0
            struct {
                Ordinal unused0 : 1;
                // trace mode bits
                Ordinal instructionTraceMode : 1;
                Ordinal branchTraceMode : 1;
                Ordinal callTraceMode : 1;
                Ordinal returnTraceMode : 1;
                Ordinal prereturnTraceMode : 1;
                Ordinal supervisorTraceMode : 1;
                Ordinal markTraceMode : 1;
                Ordinal unused1 : 16;
                // hardware breakpoint event flags
                Ordinal instructionAddressBreakpoint0 : 1;
                Ordinal instructionAddressBreakpoint1 : 1;
                Ordinal dataAddressBreakpoint0 : 1;
                Ordinal dataAddressBreakpoint1 : 1;
                Ordinal unused2 : 4;
            };
            Ordinal value;
            constexpr TraceControls(Ordinal raw = 0) : value(raw) { }
            constexpr bool traceMarked() const noexcept { return markTraceMode != 0; }
#endif
        public:
#define Base(v, m) \
            static constexpr bool extract ## v (Ordinal raw) noexcept { \
                return m & raw;  \
            } \
            static constexpr Ordinal encode ## v (bool value) noexcept { \
                return encodeBool< m > (value); \
            }
#define X(v) \
            Base(v ## TraceMode , v ## TraceModeMask )
#define Y(v) \
            Base(v ## AddressBreakpoint0, v ## AddressBreakpoint0Mask ); \
            Base(v ## AddressBreakpoint1, v ## AddressBreakpoint1Mask )
            X(Instruction);
            X(Branch);
            X(Call);
            X(Return);
            X(Prereturn);
            X(Supervisor);
            X(Mark);
            Y(Instruction);
            Y(Data);
#undef Y
#undef X
#undef Base

        public:
            constexpr TraceControls() noexcept = default;
            constexpr TraceControls(Ordinal raw) noexcept : 
                _instructionTraceMode(InstructionTraceModeMask & raw),
                _branchTraceMode(BranchTraceModeMask & raw),
                _callTraceMode(CallTraceModeMask & raw),
                _returnTraceMode(ReturnTraceModeMask & raw),
                _prereturnTraceMode(PrereturnTraceModeMask& raw),
                _supervisorTraceMode(SupervisorTraceModeMask & raw),
                _markTraceMode(MarkTraceModeMask & raw),
                _instructionAddressBreakpoint0(InstructionAddressBreakpoint0Mask & raw),
                _instructionAddressBreakpoint1(InstructionAddressBreakpoint1Mask & raw),
                _dataAddressBreakpoint0(DataAddressBreakpoint0Mask & raw),
                _dataAddressBreakpoint1(DataAddressBreakpoint1Mask & raw) { }
            constexpr bool traceMarked() const noexcept { return _markTraceMode != 0; }
            void clear() noexcept;
            constexpr auto getRawValue() const noexcept {
                return constructOrdinalMask(encodeBool<InstructionTraceModeMask>(_instructionTraceMode),
                        encodeBool<BranchTraceModeMask>(_branchTraceMode),
                        encodeBool<CallTraceModeMask>(_callTraceMode),
                        encodeBool<ReturnTraceModeMask>(_returnTraceMode),
                        encodeBool<PrereturnTraceModeMask>(_prereturnTraceMode),
                        encodeBool<SupervisorTraceModeMask>(_supervisorTraceMode),
                        encodeBool<MarkTraceModeMask>(_markTraceMode),
                        encodeBool<InstructionAddressBreakpoint0Mask>(_instructionAddressBreakpoint0),
                        encodeBool<InstructionAddressBreakpoint1Mask>(_instructionAddressBreakpoint1),
                        encodeBool<DataAddressBreakpoint0Mask>(_dataAddressBreakpoint0),
                        encodeBool<DataAddressBreakpoint1Mask>(_dataAddressBreakpoint1)) & ExtractionMask;
            }
            void setRawValue(Ordinal value) noexcept;
#define Base(v, i) \
            constexpr auto get ## v () const noexcept { return i ; } \
            void set ## v (bool value) noexcept { i = value; }
#define X(v, i) \
            Base(v ## TraceMode , _ ## i ## TraceMode )
#define Y(v, i) \
            Base(v ## AddressBreakpoint0, _ ## i ## AddressBreakpoint0); \
            Base(v ## AddressBreakpoint1, _ ## i ## AddressBreakpoint1)
            X(Instruction, instruction);
            X(Branch, branch);
            X(Call, call);
            X(Return, return);
            X(Prereturn, prereturn);
            X(Supervisor, supervisor);
            X(Mark, mark);
            Y(Instruction, instruction);
            Y(Data, data);
#undef X
#undef Y
#undef Base
        private:
            bool _instructionTraceMode = false;
            bool _branchTraceMode = false;
            bool _callTraceMode = false;
            bool _returnTraceMode = false;
            bool _prereturnTraceMode = false;
            bool _supervisorTraceMode = false;
            bool _markTraceMode = false;
            bool _instructionAddressBreakpoint0 = false;
            bool _instructionAddressBreakpoint1 = false;
            bool _dataAddressBreakpoint0 = false;
            bool _dataAddressBreakpoint1 = false;

    };
    union NormalRegister {
        public:
            constexpr NormalRegister(Ordinal value = 0) : ordinal(value) { }
            ~NormalRegister(); 

            PreviousFramePointer pfp;
            ProcedureEntry pe;
            ProcessControls pc;
            TraceControls te;
            Ordinal ordinal;
            Integer integer;
            ByteOrdinal byteOrd;
            ShortOrdinal shortOrd;
            ByteInteger byteInt;
            ShortInteger shortInt;

            template<typename T>
                constexpr T get() const noexcept {
                    using K = std::decay_t<T>;
                    if constexpr(std::is_same_v<K, Ordinal>) {
                        return ordinal;
                    } else if constexpr(std::is_same_v<K, Integer>) {
                        return integer;
                    } else if constexpr(std::is_same_v<K, ByteOrdinal>) {
                        return byteOrd;
                    } else if constexpr(std::is_same_v<K, ByteInteger>) {
                        return byteInt;
                    } else if constexpr(std::is_same_v<K, ShortOrdinal>) {
                        return shortOrd;
                    } else if constexpr(std::is_same_v<K, ShortInteger>) {
                        return shortInt;
                    } else if constexpr(std::is_same_v<K, PreviousFramePointer>) {
                        return pfp;
                    } else if constexpr(std::is_same_v<K, LongOrdinal>) {
                        return static_cast<LongOrdinal>(ordinal);
                    } else {
                        static_assert(false_v<K>, "Illegal type requested");
                    }
                }
            template<typename T>
                void set(T value) noexcept {
                    using K = std::decay_t<T>;
                    if constexpr(std::is_same_v<K, Ordinal>) {
                        ordinal = value;
                    } else if constexpr(std::is_same_v<K, Integer>) {
                        integer = value;
                    } else if constexpr(std::is_same_v<K, ByteOrdinal>) {
                        byteOrd = value;
                    } else if constexpr(std::is_same_v<K, ShortOrdinal>) {
                        shortOrd = value;
                    } else if constexpr(std::is_same_v<K, ByteInteger>) {
                        byteInt = value;
                    } else if constexpr(std::is_same_v<K, ShortInteger>) {
                        shortInt = value;
                    } else if constexpr(std::is_same_v<K, PreviousFramePointer>) {
                        ordinal = value.value;
                    } else {
                        static_assert(false_v<K>, "Illegal type requested");
                    }
                }
            void move(const NormalRegister& other) noexcept;
            constexpr Ordinal mostSignificantBit() const noexcept { return getMostSignificantBit(ordinal); }
            constexpr bool mostSignificantBitSet() const noexcept     { return i960::mostSignificantBitSet(ordinal); }
            constexpr bool mostSignificantBitClear() const noexcept   { return i960::mostSignificantBitClear(ordinal); }
    };

} // end namespace i960
#endif // end I960_NORMAL_REGISTER_H__
