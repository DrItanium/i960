#include "types.h"
#include "operations.h"

namespace i960 {
    Ordinal divide(ArithmeticControls& ac, Ordinal a, Ordinal b) {
        // TODO: check for zero denominator
        return a / b;
    }
    Integer divide(ArithmeticControls& ac, Integer a, Integer b) {
        // TODO: check for zero denominator
        return a / b;
    }
    Ordinal remainder(ArithmeticControls& ac, Ordinal a, Ordinal b) {
        // TODO: check for zero denominator
        return a % b;
    }
    Integer remainder(ArithmeticControls& ac, Integer a, Integer b) {
        // TODO: check for zero denominator
        return a % b;
    }
    void checkBit(ArithmeticControls& controls, Ordinal value, Ordinal position) noexcept {
        controls._conditionCode = ((value & (1 << (position & 0b11111))) == 0) ? 0b000 : 0b010;
    }
    Ordinal alterBit(const ArithmeticControls& controls, Ordinal value, Ordinal position) noexcept {
		if ((controls._conditionCode & 0b010) == 0) {
			return value & (~(1 << (position & 0b11111)));
		} else {
			return value | (1 << (position & 0b11111));
		}
    }

    /**
     * Shifts a specified bit field in value right and fills the bits to the left of
     * the shifted bit field with zeros. 
     * @param value Data to be shifted
     * @param position specifies the least significant bit of the bit field to be shifted
     * @param length specifies the length of the bit field
     * @return A value where the bitfield in value is shifted and zeros are put in place of all other values
     */
    Ordinal extract(Ordinal value, Ordinal position, Ordinal length) noexcept {
		auto shifted = value >> (position & 0b11111);
		auto mask = ((1 << length) - 1); // taken from the i960 documentation
		return shifted & mask;

    }
    constexpr bool mostSignificantBitSet(Ordinal value) noexcept {
        return (value & 0x8000'0000) != 0;
    }
    constexpr bool mostSignificantBitClear(Ordinal value) noexcept {
        return !mostSignificantBitSet(value);
    }
    /**
     * Find the most significant set bit
     */
    Ordinal scanBit(ArithmeticControls& ac, Ordinal value) noexcept {
        auto k = value;
        ac._conditionCode = 0b000;
        for (int i = 31; i >= 0; --i) {
            if (mostSignificantBitSet(k)) {
                ac._conditionCode = 0b010;
                return Ordinal(i);
            }
            k <<= 1;
        }
        return 0xFFFF'FFFF;
    }
    /**
     * Find the most significant clear bit
     */
    Ordinal spanBit(ArithmeticControls& ac, Ordinal value) noexcept {
        auto k = value;
        ac._conditionCode = 0b000;
        for (int i = 31; i >= 0; --i) {
            if (mostSignificantBitClear(k)) {
                ac._conditionCode = 0b010;
                return Ordinal(i);
            }
            k <<= 1;
        }
        return 0xFFFF'FFFF;
    }
    Ordinal subtractWithCarry(ArithmeticControls& ac, Ordinal src1, Ordinal src2) noexcept {
		Ordinal s1 = src1 - 1;
		Ordinal carryBit = ac._conditionCode & 0b010 != 0 ? 1 : 0;
		// need to identify if overflow will occur
		auto result = src2 - s1 + carryBit;
#warning "subtractWithCarry does not implement integer overflow detection, needs to be implemented"
		auto v = 0; // TODO fix this by identifying if integer subtraction would've produced an overflow and set v
		ac._conditionCode = (carryBit << 1) | v;
		return result;
	}
    Integer modulo(ArithmeticControls& ac, Integer src1, Integer src2) noexcept {
		// taken from the i960 manual
		auto result = subtract(src2, multiply(divide(ac, src2, src1), src1));
		if ((src2 * src1) < 0) {
			result = result + src1;
		}
		return result;
	}
    LongOrdinal extendedDivide(Ordinal src1, LongOrdinal src2) noexcept {
		Ordinal remainder = (src2 - (src2 / src1) * src1);
		Ordinal quotient = src2 / src1;
		return (LongOrdinal(quotient) << 32) | LongOrdinal(remainder);
	}

} // end namespace i960
