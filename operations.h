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

    constexpr Ordinal oneShiftLeft(Ordinal position) noexcept {
        return 1u << (0b11111 & position);
    }
    constexpr Ordinal clearBit(Ordinal value, Ordinal position) noexcept {
        return value & ~oneShiftLeft(position);
    }
    constexpr Ordinal setBit(Ordinal value, Ordinal position) noexcept {
        // if the bit is not set then set it
        return value | oneShiftLeft(position);
    }
    constexpr Ordinal notBit(Ordinal value, Ordinal position) noexcept {
        auto mask = oneShiftLeft(position);
        if ((value & mask) == 0) {
            return setBit(value, position);
        } else {
            return clearBit(value, position);
        }
    }

    constexpr Ordinal rotate(Ordinal src, Ordinal length) noexcept {
		// taken from the wikipedia entry on circular shifts through my syn
		// project
		return Ordinal((src << length) | (src >> ((-length) & 31u)));
	}
	
} // end namespace i960 

#endif // end I960_OPS_H__
