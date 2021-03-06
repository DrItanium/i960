#ifndef I960_NORMAL_REGISTER_H__
#define I960_NORMAL_REGISTER_H__

#include "types.h"
#include "ProcessControls.h"
namespace i960 {
    class RegisterView;
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
                } else {
                    static_assert(false_v<K>, "Illegal type requested!");
                }
            }
            template<typename T>
            void set(T value) noexcept {
                using K = std::decay_t<T>;
                Ordinal outcome = 0;
                if constexpr (IsOneOfThese<K, Ordinal, ByteOrdinal, ShortOrdinal, LongOrdinal>) {
                    outcome = static_cast<Ordinal>(value);
                } else if constexpr (IsOneOfThese<K, Integer, ByteInteger, ShortInteger>) {
                    outcome = static_cast<Ordinal>(toOrdinal(value));
                } else {
                    static_assert(false_v<K>, "Illegal type to store into register!");
                }
                setValue(outcome);
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
            template<typename R, ShiftMaskPair<Ordinal> desc>
            constexpr R decodeField() noexcept {
                return i960::decode<decltype(_value), R, desc>(_value);
            }
            template<typename R, Ordinal mask, Ordinal shift = 0>
            constexpr R decodeField() noexcept {
                return i960::decode<decltype(_value), R, mask, shift>(_value);
            }
            template<typename T, Ordinal mask, Ordinal shift = 0>
            void encodeField(T field) noexcept {
                _value = i960::encode<decltype(_value), T, mask, shift>(_value, field);
            }
            template<typename T, ShiftMaskPair<Ordinal> desc>
            void encodeField(T field) noexcept {
                _value = i960::encode<decltype(_value), T, desc>(_value, field);
            }
            void clear() noexcept; 

            template<typename T, typename = std::enable_if_t<std::is_base_of_v<RegisterView, std::decay_t<T>>>>
            T viewAs() noexcept {
                return {*this};
            }
        private:
            Ordinal _value;

    };
    class RegisterView {
        public:
            constexpr RegisterView(NormalRegister& r) noexcept : _reg(r) { }
            constexpr auto getRawValue() const noexcept { return _reg.getValue(); }
            template<typename R, Ordinal mask, Ordinal shift = 0>
            constexpr R decodeField() const noexcept {
                return _reg.decodeField<R, mask, shift>();
            }
            void clear() noexcept { _reg.clear(); }
            void setRawValue(Ordinal value) noexcept { _reg.setValue(value); }
            template<typename T, Ordinal mask, Ordinal shift = 0>
            void encodeField(T value) noexcept {
                _reg.encodeField<T, mask, shift>(value);
            }
        private:
            NormalRegister& _reg;
    };
    class PreviousFramePointer : public RegisterView {
        public:
            static constexpr Ordinal ReturnCodeMask = 0x0000'0007;
            static constexpr Ordinal PreReturnTraceMask = 0x0000'0008;
            static constexpr Ordinal AddressMask = 0xFFFF'FFC0; // aligned to 64-byte boundaries
            using RegisterView::RegisterView;
            constexpr auto getReturnCode() const noexcept { return decodeField<Ordinal, ReturnCodeMask>(); }
            void setReturnCode(Ordinal value) noexcept { encodeField<Ordinal, ReturnCodeMask>(value); }
            constexpr auto getPrereturnTrace() const noexcept { return decodeField<bool, PreReturnTraceMask>(); }
            void setPrereturnTrace(bool value) noexcept { encodeField<bool, PreReturnTraceMask>(value); }
            constexpr auto getAddress() const noexcept { return decodeField<Ordinal, AddressMask>(); }
            void setAddress(Ordinal value) noexcept { encodeField<Ordinal, AddressMask>(value); }
    };
    class ProcedureEntry : public RegisterView {
        public:
            static constexpr Ordinal TypeMask = 0b11;
            static constexpr auto AddressMask = ~TypeMask;
            using RegisterView::RegisterView;
            constexpr auto getType() const noexcept { return decodeField<Ordinal, TypeMask>(); }
            void setType(Ordinal type) noexcept { encodeField<Ordinal, TypeMask>(type); }
            constexpr auto getAddress() const noexcept { return decodeField<Ordinal, AddressMask>(); }
            void setAddress(Ordinal addr) noexcept { encodeField<Ordinal, AddressMask>(addr); }
            constexpr bool isLocal() const noexcept { return getType() == 0; }
            constexpr bool isSupervisor() const noexcept { return getType() == 0b10; }

    };

    class TraceControls : public RegisterView {
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
        public:
            using RegisterView::RegisterView;
#define B(field, mask) \
            void set ## field (bool value) noexcept { \
                encodeField<bool, mask>(value); \
            } \
            constexpr auto get ## field () const noexcept { \
                return decodeField<bool, mask>(); \
            }
#define TM(title) B(title ## TraceMode , title ## TraceModeMask)
#define ABM(title) \
            B(title ## AddressBreakpoint0, title ## AddressBreakpoint0Mask); \
            B(title ## AddressBreakpoint1, title ## AddressBreakpoint1Mask)
            TM(Instruction);
            TM(Branch);
            TM(Call);
            TM(Return);
            TM(Prereturn);
            TM(Supervisor);
            TM(Mark);
            ABM(Instruction);
            ABM(Data);
#undef TM
#undef ABM
#undef B
            constexpr auto traceMarked() const noexcept { return getMarkTraceMode(); }
            void modify(Ordinal mask, Ordinal input) noexcept;
    };

    class DoubleRegister final {
        public:
            constexpr DoubleRegister(NormalRegister& lower, NormalRegister& upper) noexcept : _lower(lower), _upper(upper) { }
            ~DoubleRegister() = default;
            constexpr LongOrdinal get() const noexcept { return _lower.get<LongOrdinal>() | (_upper.get<LongOrdinal>() << 32); }
            void set(LongOrdinal value) noexcept { set(static_cast<Ordinal>(value), static_cast<Ordinal>(value >> 32)); }
            void set(Ordinal lower, Ordinal upper) noexcept;
            constexpr Ordinal getLowerHalf() const noexcept { return _lower.get<Ordinal>(); }
            constexpr Ordinal getUpperHalf() const noexcept { return _upper.get<Ordinal>(); }
        private:
            NormalRegister& _lower;
            NormalRegister& _upper;
    };


} // end namespace i960

namespace std {
template<typename T>
constexpr T get(const i960::NormalRegister& reg) noexcept {
    return reg.get<T>();
}

template<typename T>
constexpr T get(i960::NormalRegister&& reg) noexcept {
    return reg.get<T>();
}
template<size_t I>
constexpr i960::Ordinal get(const i960::DoubleRegister& dr) noexcept {
    if constexpr (I == 0) {
        return dr.getLowerHalf();
    } else if constexpr (I == 1) {
        return dr.getUpperHalf();
    } else {
        static_assert(I >= 2, "Out of range accessor");
    }
}

template<size_t I>
constexpr i960::Ordinal get(i960::DoubleRegister&& dr) noexcept {
    if constexpr (I == 0) {
        return dr.getLowerHalf();
    } else if constexpr (I == 1) {
        return dr.getUpperHalf();
    } else {
        static_assert(I >= 2, "Out of range accessor");
    }
}
} // end namespace std
#endif // end I960_NORMAL_REGISTER_H__
