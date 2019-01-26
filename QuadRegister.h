#ifndef I960_QUAD_REGISTER_H__
#define I960_QUAD_REGISTER_H__
#include "types.h"
#include "NormalRegister.h"
namespace i960 {
    class QuadRegister {
        public:
            QuadRegister(NormalRegister& lower, NormalRegister& mid, NormalRegister& high, NormalRegister& highest) : _lower(lower), _mid(mid), _upper(high), _highest(highest) { }
            ~QuadRegister() = default;
            void set(Ordinal lower, Ordinal mid, Ordinal upper, Ordinal highest) noexcept;
            Ordinal getLowestPart() const noexcept  { return _lower.get<Ordinal>(); }
            Ordinal getLowerPart() const noexcept   { return _mid.get<Ordinal>(); }
            Ordinal getHigherPart() const noexcept  { return _upper.get<Ordinal>(); }
            Ordinal getHighestPart() const noexcept { return _highest.get<Ordinal>(); }
        private:
            NormalRegister& _lower;
            NormalRegister& _mid;
            NormalRegister& _upper;
            NormalRegister& _highest;
    };
} // end namespace i960
#endif // end I960_QUAD_REGISTER_H__
