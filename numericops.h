#ifndef I960_OPS_H__
#define I960_OPS_H__
#include "types.h"

namespace i960 {
#ifdef NUMERICS_ARCHITECTURE
    inline Real add(Real a, Real b) noexcept { return a._floating + b._floating; }
    inline LongReal add(LongReal a, LongReal b) noexcept { return a._floating + b._floating; }
    inline Real subtract(Real a, Real b) noexcept { return a._floating - b._floating; }
    inline LongReal subtract(LongReal a, LongReal b) noexcept { return a._floating - b._floating; }
    inline Real multiply(Real a, Real b) noexcept { return a._floating * b._floating; }
    inline LongReal multiply(LongReal a, LongReal b) noexcept { return a._floating * b._floating; }
    void classify(ArithmeticControls& ac, Real a) noexcept;
    void classify(ArithmeticControls& ac, LongReal a) noexcept;
    Real divide(Real a, Real b) noexcept;
    LongReal divide(LongReal a, LongReal b) noexcept;
    Real sine(Real value) noexcept;
    LongReal sine(LongReal value) noexcept;
    Real cosine(Real value) noexcept;
    LongReal cosine(LongReal value) noexcept;
    Real tangent(Real value) noexcept;
    LongReal tangent(LongReal value) noexcept;
    Real squareRoot(Real value) noexcept;
    LongReal squareRoot(LongReal value) noexcept;
    Real arcTangent(Real a, Real b) noexcept;
    LongReal arcTangent(LongReal a, LongReal b) noexcept;
    Real logarithmBinary(Real a) noexcept;
    LongReal logarithmBinary(LongReal a) noexcept;
    Real logarithmEpsilon(Real a, Real b) noexcept;
    LongReal logarithmEpsilon(LongReal a, LongReal b) noexcept;
    Real logarithm(Real a, Real b) noexcept;
    LongReal logarithm(LongReal a, LongReal b) noexcept;
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
