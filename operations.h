#ifndef I960_OPS_H__
#define I960_OPS_H__
#include "types.h"
#include <cmath>

namespace i960 {

	
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
