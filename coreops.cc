#include "types.h"
#include "coreops.h"

namespace i960 {
    Ordinal divide(Ordinal a, Ordinal b) {
        // TODO: check for zero denominator
        return a / b;
    }
    Integer divide(Integer a, Integer b) {
        // TODO: check for zero denominator
        return a / b;
    }
    Ordinal remainder(Ordinal a, Ordinal b) {
        // TODO: check for zero denominator
        return a % b;
    }
    Integer remainder(Integer a, Integer b) {
        // TODO: check for zero denominator
        return a % b;
    }
    void checkBit(ArithmeticControls& controls, Ordinal value, Ordinal position) noexcept {
        // TODO finish
    }
    Ordinal alterBit(const ArithmeticControls& controls, Ordinal value, Ordinal position) noexcept {
        // TODO Implement
       return value;
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
        // TODO implement
       return value;
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

} // end namespace i960
