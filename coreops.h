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
        if (value & mask == 0) {
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
    Ordinal addWithCarry(ArithmeticControls& ac, Ordinal src1, Ordinal src2) noexcept;
    // TODO provide signature for atadd or atomic add
    // TODO provide signature for branch and link (bal)
    // TODO provide signature for branch and link extended (balx)
    // TODO provide signature for branch (b)
    // TODO provide signature for branch extended (bx)
    // TODO provide signature for check bit and branch if clear (bbc)
    // TODO provide signature for check bit and branch if set (bbs)
    // TODO provide signature for branch if operations (be, bne, bl, ble, bg bge, bo, bno)
    // TODO provide signature for the call instruction (call)
    // TODO provide signature for the call supervisor instruction (calls)
    // TODO provide signature for the call extended instruction (callx)
    // TODO provide signature for the compare and branch operations
    void checkBit(ArithmeticControls& ac, Ordinal src, Ordinal position) noexcept;
    Ordinal alterBit(const ArithmeticControls& ac, Ordinal src, Ordinal position) noexcept;
    void compare(ArithmeticControls& ac, Integer src1, Integer src2);
    void compare(ArithmeticControls& ac, Ordinal src1, Ordinal src2);
    Integer compareAndDecrement(ArithmeticControls& ac, Integer src1, Integer src2);
    Ordinal compareAndDecrement(ArithmeticControls& ac, Ordinal src1, Ordinal src2);
    Integer compareAndIncrement(ArithmeticControls& ac, Integer src1, Integer src2);
    Ordinal compareAndIncrement(ArithmeticControls& ac, Ordinal src1, Ordinal src2);
    void conditionalCompare(ArithmeticControls& ac, Integer src1, Integer src2);
    void conditionalCompare(ArithmeticControls& ac, Ordinal src1, Ordinal src2);
    /**
     * Divides src2 by src1 and returns the result.
     * @param src1 a normal ordinal that is the denominator
     * @param src2 a long ordinal that is the numerator
     * @return a long ordinal where the lower ordinal is the remainder and the upper ordinal is the quotient
     */
    LongOrdinal extendedDivide(Ordinal src1, LongOrdinal src2) noexcept;

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
} // end namespace i960 

#endif // end I960_OPS_H__
