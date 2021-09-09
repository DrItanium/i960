#ifndef I960_QUAD_REGISTER_H__
#define I960_QUAD_REGISTER_H__
#include "types.h"
#include "NormalRegister.h"
namespace i960 {
    class QuadRegister final {
        public:
            constexpr QuadRegister(NormalRegister& lower, NormalRegister& mid, NormalRegister& high, NormalRegister& highest) noexcept : _lower(lower), _mid(mid), _upper(high), _highest(highest) { }
            void set(Ordinal lower, Ordinal mid, Ordinal upper, Ordinal highest) noexcept;
            constexpr Ordinal getLowestPart() const noexcept  { return _lower.get<Ordinal>(); }
            constexpr Ordinal getLowerPart() const noexcept   { return _mid.get<Ordinal>(); }
            constexpr Ordinal getHigherPart() const noexcept  { return _upper.get<Ordinal>(); }
            constexpr Ordinal getHighestPart() const noexcept { return _highest.get<Ordinal>(); }
        private:
            NormalRegister& _lower;
            NormalRegister& _mid;
            NormalRegister& _upper;
            NormalRegister& _highest;
    };
} // end namespace i960
namespace std {

template<size_t I>
constexpr i960::Ordinal get(const i960::QuadRegister& tr) noexcept {
    switch (I) {
        case 0:
            return tr.getLowestPart();
        case 1:
            return tr.getLowerPart();
        case 2:
            return tr.getHigherPart();
        case 3:
            return tr.getHighestPart();
        default:
            static_assert(I >= 4, "Out of range accessor");
    }
}

template<size_t I>
constexpr i960::Ordinal get(i960::QuadRegister&& tr) noexcept {
    switch (I) {
        case 0:
            return tr.getLowestPart();
        case 1:
            return tr.getLowerPart();
        case 2:
            return tr.getHigherPart();
        case 3:
            return tr.getHighestPart();
        default:
            static_assert(I >= 4, "Out of range accessor");
    }
}

} // end namespace std
#endif // end I960_QUAD_REGISTER_H__
