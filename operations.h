#ifndef I960_OPS_H__
#define I960_OPS_H__
#include "types.h"
#include <cmath>

namespace i960 {

	template<typename T>
	constexpr T andOp(T a, T b) noexcept {
		if constexpr (std::is_same<T, bool>()) {
			return a && b;
		} else {
			return a & b;
		}
	}
	template<typename T>
	constexpr T orOp(T a, T b) noexcept {
		if constexpr (std::is_same<T, bool>()) {
			return a || b;
		} else {
			return a | b;
		}
	}
	template<typename T>
	constexpr T xorOp(T a, T b) noexcept {
		return a ^ b;
	}

	template<typename T>
	constexpr T notOp(T a) noexcept {
		if constexpr (std::is_same<T, bool>()) {
			return !a;
		} else {
			return ~a;
		}
	}
	template<typename T>
	constexpr T notAnd(T a, T b) noexcept {
		return andOp(notOp(a), b);
	}

	template<typename T>
	constexpr T andNot(T a, T b) noexcept {
		return andOp(a, notOp(b));
	}

	template<typename T>
	constexpr T nor(T a, T b) noexcept {
		return andOp(notOp(a), notOp(b));
	}

	template<typename T>
	constexpr T xnor(T a, T b) noexcept {
		return orOp(notOp(orOp(a, b)), andOp(a, b));
	}

	template<typename T>
	constexpr T notOr(T a, T b) noexcept {
		return orOp(notOp(a), b);
	}

	template<typename T>
	constexpr T orNot(T a, T b) noexcept {
		return orOp(a, notOp(b));
	}

	template<typename T>
	constexpr T nand(T a, T b) noexcept {
		return orOp(notOp(a), notOp(b));
	}


    constexpr Ordinal clearBit(Ordinal value, Ordinal position) noexcept {
        Ordinal mask = ~(1 << (0b11111 & position));
        return value & mask;
    }
    constexpr Ordinal setBit(Ordinal value, Ordinal position) noexcept {
        // if the bit is not set then set it
        return value | Ordinal(1u << (0b11111 & position));
    }
    constexpr Ordinal notBit(Ordinal value, Ordinal position) noexcept {
        auto mask = Ordinal(1 << (0b11111 & position));
        if ((value & mask) == 0) {
            return setBit(value, position);
        } else {
            return clearBit(value, position);
        }
    }
    constexpr LongOrdinal multiply(LongOrdinal a, LongOrdinal b) noexcept {
        return a * b;
    }
    /** 
     * Multiplies src2 by src1 and returns the result.
     * @param src1 An Ordinal operand
     * @param src2 An Ordinal operand
     * @return A LongOrdinal which contains the result of the multiply
     */
    constexpr LongOrdinal extendedMultiply(Ordinal src1, Ordinal src2) noexcept {
        return multiply(LongOrdinal(src1), LongOrdinal(src2));
    }

    constexpr Ordinal rotate(Ordinal src, Ordinal length) noexcept {
		// taken from the wikipedia entry on circular shifts through my syn
		// project
		return Ordinal((src << length) | (src >> ((-length) & 31u)));
	}
	
	template<TestTypes t>
	Ordinal test(const ArithmeticControls& ac) noexcept {
		constexpr auto mask = Ordinal(t) & 0b111;
		if constexpr (mask == Ordinal(TestTypes::Unordered)) {
			return ac.conditionCode == 0 ? 1 : 0;
		} else {
			return (mask & ac.conditionCode) != 0 ? 1 : 0;
		}
	}
} // end namespace i960 

#endif // end I960_OPS_H__
