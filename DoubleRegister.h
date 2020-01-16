#ifndef I960_DOUBLE_REGISTER_H__
#define I960_DOUBLE_REGISTER_H__
#include "types.h"
#include "NormalRegister.h"
namespace i960 {
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
#endif // end I960_DOUBLE_REGISTER_H__
