#define NUMERICS_ARCHITECTURE
#include "types.h"
#include "numericops.h"
#include <cmath>
namespace i960 {
#ifdef NUMERICS_ARCHITECTURE // not redundant, makes updates easier
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
