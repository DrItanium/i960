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
#endif // end defined(NUMERICS_ARCHITECTURE)
} // end namespace i960 

#endif // end I960_OPS_H__
