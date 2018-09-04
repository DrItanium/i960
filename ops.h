#ifndef I960_OPS_H__
#define I960_OPS_H__
#include "types.h"

namespace i960 {

    constexpr Ordinal add(Ordinal a, Ordinal b) noexcept {
        return a + b;
    }
    constexpr Integer add(Integer a, Integer b) noexcept {
        return a + b;
    }
    constexpr Ordinal subtract(Ordinal a, Ordinal b) noexcept {
        return a - b;
    }
    constexpr Integer subtract(Integer a, Integer b) noexcept {
        return a - b;
    }
    constexpr Ordinal multiply(Ordinal a, Ordinal b) noexcept {
        return a * b;
    }
    constexpr Integer multiply(Integer a, Integer b) noexcept {
        return a * b;
    }
    Ordinal divide(Ordinal a, Ordinal b);
    Integer divide(Integer a, Integer b);
    Ordinal remainder(Ordinal a, Ordinal b);
    Integer remainder(Integer a, Integer b);
    constexpr Ordinal shiftLeft(Ordinal a, Ordinal b) noexcept {
        return a << b;
    }
    constexpr Integer shiftLeft(Integer a, Integer b) noexcept {
        return a << b;
    }
    constexpr Ordinal shiftRight(Ordinal a, Ordinal b) noexcept {
        return a >> b;
    }
    constexpr Integer shiftRight(Integer a, Integer b) noexcept {
        return a >> b;
    }

    constexpr bool andOp(bool a, bool b) noexcept {
        return a && b;
    }
    constexpr bool notAnd(bool a, bool b) noexcept {
        return (!a) && b;
    }
    constexpr bool andNot(bool a, bool b) noexcept {
        return a && (!b);
    }
    constexpr bool xorOp(bool a, bool b) noexcept {
        return a ^ b;
    }
    constexpr bool orOp(bool a, bool b) noexcept {
        return a || b;
    }
    constexpr bool nor(bool a, bool b) noexcept {
        return (!a) || (!b);
    }
    constexpr bool xnor(bool a, bool b) noexcept {
        return a == b;
    }
    constexpr bool notOp(bool a) noexcept {
        return !a;
    }
    constexpr bool notOr(bool a, bool b) noexcept {
        return (!a) || b;
    }
    constexpr bool orNot(bool a, bool b) noexcept {
        return a || (!b);
    }
    constexpr bool nand(bool a, bool b) {
        return (!a) || (!b);
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
        if (Ordinal value = mask & value; value == 0) {
            return setBit(value, position);
        } else {
            return clearBit(value, position);
        }
    }
    void checkBit(ArithmeticControls& controls, Ordinal value, Ordinal position) noexcept;
    Ordinal alterBit(const ArithmeticControls& controls, Ordinal value, Ordinal position) noexcept;
    /**
     * Shifts a specified bit field in value right and fills the bits to the left of
     * the shifted bit field with zeros. 
     * @param value Data to be shifted
     * @param position specifies the least significant bit of the bit field to be shifted
     * @param length specifies the length of the bit field
     * @return A value where the bitfield in value is shifted and zeros are put in place of all other values
     */
    Ordinal extract(Ordinal value, Ordinal position, Ordinal length) noexcept;
    constexpr Ordinal modify(Ordinal value, Ordinal inject, Ordinal mask) noexcept {
        return (inject & mask) | (value & (~mask));
    }
    // TODO implement chkbit and alterbit once the control structures have been implemented
    /**
     * Find the most significant set bit
     */
    Ordinal scanBit(Ordinal value) noexcept;
    /**
     * Find the most significant clear bit
     */
    Ordinal spanBit(Ordinal value) noexcept;
#ifdef NUMERICS_ARCHITECTURE
    constexpr Real add(Real a, Real b) noexcept { return a + b; }
    constexpr LongReal add(LongReal a, LongReal b) noexcept { return a + b; }
    constexpr Real subtract(Real a, Real b) noexcept { return a - b; }
    constexpr LongReal subtract(LongReal a, LongReal b) noexcept { return a - b; }
    constexpr Real multiply(Real a, Real b) noexcept { return a * b; }
    constexpr LongReal multiply(LongReal a, LongReal b) noexcept { return a * b; }
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
#endif // end defined(NUMERICS_ARCHITECTURE)


} // end namespace i960 

#endif // end I960_OPS_H__
