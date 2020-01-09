#ifndef I960_NORMAL_REGISTER_H__
#define I960_NORMAL_REGISTER_H__

#include "types.h"
#include "ProcessControls.h"
namespace i960 {
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
            void clear() noexcept;
            void setRawValue(Ordinal value) noexcept;
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
    class PreviousFramePointer;
    class RegisterWrapper;
    class ProcedureEntry;
    template<typename T, typename ... Args>
    constexpr auto IsOneOfThese = ( ... || std::is_same_v<std::decay_t<T>, Args>);
    class NormalRegister {
        public:
            constexpr NormalRegister(Ordinal value = 0) noexcept : _value(value) { }
            constexpr auto getValue() const noexcept { return _value; }
            void setValue(Ordinal value) noexcept { _value = value; }
            template<typename T>
            constexpr T get() const noexcept {
                using K = std::decay_t<T>;
                if constexpr (std::is_same_v<K, Ordinal>) {
                    return getValue();
                } else if constexpr (std::is_same_v<K, Integer>) {
                    return toInteger(getValue());
                } else if constexpr (std::is_same_v<K, ByteOrdinal>) {
                    return static_cast<ByteOrdinal>(getValue());
                } else if constexpr (std::is_same_v<K, ByteInteger>) {
                    return toInteger(static_cast<ByteOrdinal>(getValue()));
                } else if constexpr (std::is_same_v<K, ShortOrdinal>) {
                    return static_cast<ShortOrdinal>(getValue());
                } else if constexpr (std::is_same_v<K, ShortInteger>) {
                    return toInteger(static_cast<ShortOrdinal>(getValue()));
                } else if constexpr (std::is_same_v<K, LongOrdinal>) {
                    return static_cast<LongOrdinal>(getValue());
                } else if constexpr (std::is_base_of_v<RegisterWrapper, K>) {
                    return {*this};
                } else {
                    static_assert(false_v<K>, "Illegal type requested!");
                }
            }
            template<typename T>
            void set(T value) noexcept {
                using K = std::decay_t<T>;
                if constexpr (std::is_base_of_v<RegisterWrapper, K>) {
                    // copy I guess?
                    setValue(value.getRawValue());
                } else if constexpr (IsOneOfThese<K, Ordinal, ByteOrdinal, ShortOrdinal, LongOrdinal>) {
                    setValue(static_cast<Ordinal>(value));
                } else if constexpr (IsOneOfThese<K, Integer, ByteInteger, ShortInteger>) {
                    setValue(static_cast<Ordinal>(toOrdinal(value)));
                } else {
                    static_assert(false_v<K>, "Illegal type to store into register!");
                }
            }
            void move(const NormalRegister& other) noexcept;
            constexpr auto mostSignificantBit() const noexcept {
                return getMostSignificantBit(_value);
            }
            constexpr auto mostSignificantBitSet() const noexcept {
                return i960::mostSignificantBitSet(_value);
            }
            constexpr auto mostSignificantBitClear() const noexcept {
                return i960::mostSignificantBitClear(_value);
            }
            template<typename R, Ordinal mask, Ordinal shift = 0>
            constexpr R decodeField() noexcept {
                return i960::decode<decltype(_value), R, mask, shift>(_value);
            }
            template<typename T, Ordinal mask, Ordinal shift = 0>
            void encodeField(T field) noexcept {
                _value = i960::encode<decltype(_value), T, mask, shift>(_value, field);
            }
            void clear() noexcept; 
        private:
            Ordinal _value;

    };
    class RegisterWrapper {
        public:
            constexpr RegisterWrapper(NormalRegister& r) noexcept : _reg(r) { }
            constexpr auto getRawValue() const noexcept { return _reg.getValue(); }
            template<typename R, Ordinal mask, Ordinal shift = 0>
            constexpr R decodeField() const noexcept {
                return _reg.decodeField<R, mask, shift>();
            }
            void clear() noexcept { _reg.clear(); }
        protected:
            template<typename T, Ordinal mask, Ordinal shift = 0>
            void encodeField(T value) noexcept {
                _reg.encodeField<T, mask, shift>(value);
            }
        private:
            NormalRegister& _reg;
    };
    class PreviousFramePointer : public RegisterWrapper {
        public:
            static constexpr Ordinal ReturnCodeMask = 0x0000'0007;
            static constexpr Ordinal PreReturnTraceMask = 0x0000'0008;
            static constexpr Ordinal AddressMask = 0xFFFF'FFC0; // aligned to 64-byte boundaries
            static constexpr Ordinal ExtractionMask = constructOrdinalMask(ReturnCodeMask, PreReturnTraceMask, AddressMask); 
            static constexpr Ordinal ReservedMask = ~ExtractionMask;
            using RegisterWrapper::RegisterWrapper;
            constexpr auto getReturnCode() const noexcept { return decodeField<Ordinal, ReturnCodeMask>(); }
            void setReturnCode(Ordinal value) noexcept { encodeField<Ordinal, ReturnCodeMask>(value); }
            constexpr auto getPrereturnTrace() const noexcept { return decodeField<bool, PreReturnTraceMask, 3>(); }
            void setPrereturnTrace(bool value) noexcept { encodeField<bool, PreReturnTraceMask, 3>(value); }
            constexpr auto getAddress() const noexcept { return decodeField<Ordinal, AddressMask>(); }
            void setAddress(Ordinal value) noexcept { encodeField<Ordinal, AddressMask>(value); }
    };
    class ProcedureEntry : public RegisterWrapper {
        public:
            static constexpr Ordinal TypeMask = 0b11;
            static constexpr auto AddressMask = ~TypeMask;
            using RegisterWrapper::RegisterWrapper;
            constexpr auto getType() const noexcept { return decodeField<Ordinal, TypeMask>(); }
            void setType(Ordinal type) noexcept { encodeField<Ordinal, TypeMask>(type); }
            constexpr auto getAddress() const noexcept { return decodeField<Ordinal, AddressMask>(); }
            void setAddress(Ordinal addr) noexcept { encodeField<Ordinal, AddressMask>(addr); }
            constexpr bool isLocal() const noexcept { return getType() == 0; }
            constexpr bool isSupervisor() const noexcept { return getType() == 0b10; }

    };

} // end namespace i960
#endif // end I960_NORMAL_REGISTER_H__
