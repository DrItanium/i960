#ifndef I960_TRIPLE_REGISTER_H__
#define I960_TRIPLE_REGISTER_H__
#include "types.h"
#include "NormalRegister.h"
namespace i960 {
    class TripleRegister final {
        public:
            constexpr TripleRegister(NormalRegister& lower, NormalRegister& mid, NormalRegister& upper) noexcept : _lower(lower), _mid(mid), _upper(upper) { }
            void set(Ordinal lower, Ordinal mid, Ordinal upper) noexcept;
            [[nodiscard]] constexpr Ordinal getLowerPart() const noexcept  { return _lower.get<Ordinal>(); }
            [[nodiscard]] constexpr Ordinal getMiddlePart() const noexcept { return _mid.get<Ordinal>(); }
            [[nodiscard]] constexpr Ordinal getUpperPart() const noexcept  { return _upper.get<Ordinal>(); }
        private:
            NormalRegister& _lower;
            NormalRegister& _mid;
            NormalRegister& _upper;
    };

} // end namespace i960
namespace std {

template<size_t I>
constexpr i960::Ordinal get(const i960::TripleRegister& tr) noexcept {
    switch (I) {
        case 0:
            return tr.getLowerPart();
        case 1:
            return tr.getMiddlePart();
        case 2:
            return tr.getUpperPart();
        default:
            static_assert(I >= 3, "Out of range accessor");
    }
}

template<size_t I>
constexpr i960::Ordinal get(i960::TripleRegister&& tr) noexcept {
    switch (I) {
        case 0:
            return tr.getLowerPart();
        case 1:
            return tr.getMiddlePart();
        case 2:
            return tr.getUpperPart();
        default:
            static_assert(I >= 3, "Out of range accessor");
    }
}

} // end namespace std
#endif // end I960_TRIPLE_REGISTER_H__
