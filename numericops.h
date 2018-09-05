#ifndef I960_OPS_H__
#define I960_OPS_H__
#include "types.h"
#include <cmath>

namespace i960 {
#ifdef NUMERICS_ARCHITECTURE
    inline Real add(Real a, Real b) noexcept { return Real(a._floating + b._floating); }
    inline LongReal add(LongReal a, LongReal b) noexcept { return LongReal(a._floating + b._floating); }
    inline Real subtract(Real a, Real b) noexcept { return Real(a._floating - b._floating); }
    inline LongReal subtract(LongReal a, LongReal b) noexcept { return LongReal(a._floating - b._floating); }
    inline Real multiply(Real a, Real b) noexcept { return Real(a._floating * b._floating); }
    inline LongReal multiply(LongReal a, LongReal b) noexcept { return LongReal(a._floating * b._floating); }
    template<typename T>
    inline T divide(T a, T b) noexcept {
        return T(a._floating / b._floating);
    }
    template Real divide<Real>(Real a, Real b);
    template LongReal divide<LongReal>(LongReal a, LongReal b);

    template<typename T>
    inline T sine(T value) noexcept {
        return T(::sin(value._floating));
    }
    template Real sine<Real>(Real a);
    template LongReal sine<LongReal>(LongReal a);
    template<typename T>
    inline T cosine(T value) noexcept {
        return T(::cos(value._floating));
    }
    template Real cosine<Real>(Real a);
    template LongReal cosine<LongReal>(LongReal a);
    template<typename T>
    inline T tangent(T value) noexcept {
        return T(::tan(value._floating));
    }
    template Real tangent<Real>(Real a);
    template LongReal tangent<LongReal>(LongReal a);

    template<typename T>
    inline T squareRoot(T value) noexcept {
        return T(::sqrt(value._floating));
    }
    template Real squareRoot<Real>(Real a);
    template LongReal squareRoot<LongReal>(LongReal a);

    template<typename T>
    inline T arctangent(T a, T b) noexcept {
        return T(::atan(divide(b, a)._floating));
    }

    template Real arctangent<Real>(Real a, Real b); 
    template LongReal arctangent<LongReal>(LongReal a, LongReal b); 


    template<typename T>
    inline T logarithmBinary(T a) noexcept {
        return T(::log2(a._floating));
    }
    template Real logarithmBinary<Real>(Real a); 
    template LongReal logarithmBinary<LongReal>(LongReal a); 

    template<typename T>
    inline T logarithmEpsilon(T a, T b) noexcept {
        return multiply(b, logarithmBinary(add(a, T(1.0))));
    }
    template Real logarithmEpsilon<Real>(Real a, Real b); 
    template LongReal logarithmEpsilon<LongReal>(LongReal a, LongReal b); 

    template<typename T>
    inline T logarithm(T a, T b) noexcept {
        return multiply(b, logarithmBinary(a));
    }

    template Real logarithm<Real>(Real a, Real b); 
    template LongReal logarithm<LongReal>(LongReal a, LongReal b); 

    void classify(ArithmeticControls& ac, Real a) noexcept;
    void classify(ArithmeticControls& ac, LongReal a) noexcept;
    Real convertToReal(LongInteger a) noexcept;
    Real convertToReal(Integer a) noexcept;
    Integer convertToInteger(Real a, bool truncate = false) noexcept;
    LongInteger convertToLongInteger(Real a, bool truncate = false) noexcept;
    Real round(Real a) noexcept;
    LongReal round(LongReal a) noexcept;
    Real scale(Real a, Real b) noexcept;
    LongReal scale(LongReal a, LongReal b) noexcept;
    Real remainder(Real a, Real b) noexcept;
    LongReal remainder(LongReal a, LongReal b) noexcept;
    Ordinal decimalSubtractWithCarry(ArithmeticControls& ac, Ordinal src1, Ordinal src2) noexcept;
    Ordinal decimalMoveAndTest(ArithmeticControls& ac, Ordinal src) noexcept;
    Ordinal decimalAddWithCarry(ArithmeticControls& ac, Ordinal src1, Ordinal src2) noexcept;
    Real exponent(Real src) noexcept;
    LongReal exponent(LongReal src) noexcept;
    // TODO support cpysre and cpyrsre
    void compareOrdered(ArithmeticControls& ac, Real src1, Real src2) noexcept;
    void compareOrdered(ArithmeticControls& ac, LongReal src1, LongReal src2) noexcept;
    void compare(ArithmeticControls& ac, Real src1, Real src2) noexcept;
    void compare(ArithmeticControls& ac, LongReal src1, LongReal src2) noexcept;
    // TODO signatures for the fp move instructions
    // TODO synld signature
    // TODO synmov, synmovl, synmovq signatures
#endif // end defined(NUMERICS_ARCHITECTURE)
} // end namespace i960 

#endif // end I960_OPS_H__
