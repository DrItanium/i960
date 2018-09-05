#define NUMERICS_ARCHITECTURE
#include "types.h"
#include "numericops.h"
#include <cmath>
namespace i960 {
#ifdef NUMERICS_ARCHITECTURE // not redundant, makes updates easier
    Real divide(Real a, Real b) noexcept {
        return a._floating / b._floating;
    }
    LongReal divide(LongReal a, LongReal b) noexcept {
        return a._floating / b._floating;
    }
    Real sine(Real value) noexcept {
        return sin(value._floating);
    }
    LongReal sine(LongReal value) noexcept {
        return sin(value._floating);
    }
    Real cosine(Real value) noexcept {
        return cos(value._floating);
    }
    LongReal cosine(LongReal value) noexcept {
        return cos(value._floating);
    }
    Real tangent(Real value) noexcept {
        return tan(value._floating);
    }
    LongReal tangent(LongReal value) noexcept {
        return tan(value._floating);
    }
    Real squareRoot(Real value) noexcept {
        return sqrt(value._floating);
    }
    LongReal squareRoot(LongReal value) noexcept {
        return sqrt(value._floating);
    }
    Real arcTangent(Real a, Real b) noexcept {
        return atan(divide(b, a)._floating);
    }
    LongReal arcTangent(LongReal a, LongReal b) noexcept {
        return atan(divide(b, a)._floating);
    }
    Real logarithmBinary(Real a) noexcept {
        return log2(a._floating);
    }
    LongReal logarithmBinary(LongReal a) noexcept {
        return log2(a._floating);
    }
    Real logarithmEpsilon(Real a, Real b) noexcept {
        return multiply(b, logarithmBinary(add(a, 1.0)));
    }
    LongReal logarithmEpsilon(LongReal a, LongReal b) noexcept {
        return multiply(b, logarithmBinary(add(a, 1.0)));
    }
    Real logarithm(Real a, Real b) noexcept {
        // taken from the i960 manuals
        return multiply(b, logarithmBinary(a));
    }
    LongReal logarithm(LongReal a, LongReal b) noexcept {
        // taken from the i960 manuals
        return multiply(b,logarithmBinary(a));
    }
    Real convertToReal(LongInteger a) noexcept {
        return Real(RawReal(a));
    }
    Real convertToReal(Integer a) noexcept {
        return Real(RawReal(a));
    }
    Integer convertToInteger(Real a, bool truncate ) noexcept {
        // TODO lookup the rounding mode  and do something if truncate is false
        return Integer(a._floating);
    }
    LongInteger convertToLongInteger(Real a, bool truncate ) noexcept {
        // TODO lookup the rounding mode  and do something if truncate is false
        return LongInteger(a._floating);
    }
#endif
} // end namespace i960
