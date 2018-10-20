#include "types.h"
#include "operations.h"
#include <cmath>
namespace i960 {
    Real convertToReal(LongInteger a) noexcept {
        return Real(RawReal(a));
    }
    Real convertToReal(Integer a) noexcept {
        return Real(RawReal(a));
    }
    Integer convertToInteger(Real a, bool truncate ) noexcept {
        // TODO lookup the rounding mode  and do something if truncate is false
        return Integer(a.floating);
    }
    LongInteger convertToLongInteger(Real a, bool truncate ) noexcept {
        // TODO lookup the rounding mode  and do something if truncate is false
        return LongInteger(a.floating);
    }
} // end namespace i960
