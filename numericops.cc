#define NUMERICS_ARCHITECTURE
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
        return Integer(a._floating);
    }
    LongInteger convertToLongInteger(Real a, bool truncate ) noexcept {
        // TODO lookup the rounding mode  and do something if truncate is false
        return LongInteger(a._floating);
    }
	void compare(ArithmeticControls& ac, Real src1, Real src2) noexcept {
		compare<decltype(src1._floating)>(ac, src1._floating, src2._floating);
	}
	void compare(ArithmeticControls& ac, LongReal src1, LongReal src2) noexcept {
		compare<decltype(src1._floating)>(ac, src1._floating, src2._floating);
	}
} // end namespace i960
