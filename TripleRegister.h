#ifndef I960_TRIPLE_REGISTER_H__
#define I960_TRIPLE_REGISTER_H__
#include "types.h"
#include "NormalRegister.h"
namespace i960 {
    class TripleRegister final {
        public:
            TripleRegister(NormalRegister& lower, NormalRegister& mid, NormalRegister& upper) noexcept : _lower(lower), _mid(mid), _upper(upper) { }
            ~TripleRegister() = default;
            void set(Ordinal lower, Ordinal mid, Ordinal upper) noexcept;
            Ordinal getLowerPart() const noexcept  { return _lower.get<Ordinal>(); }
            Ordinal getMiddlePart() const noexcept { return _mid.get<Ordinal>(); }
            Ordinal getUpperPart() const noexcept  { return _upper.get<Ordinal>(); }
        private:
            NormalRegister& _lower;
            NormalRegister& _mid;
            NormalRegister& _upper;
    };

} // end namespace i960
#endif // end I960_TRIPLE_REGISTER_H__
