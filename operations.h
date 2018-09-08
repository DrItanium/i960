#ifndef I960_OPS_H__
#define I960_OPS_H__
#include "types.h"
#include <cmath>

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
    Ordinal divide(ArithmeticControls& ac, Ordinal a, Ordinal b);
    Integer divide(ArithmeticControls& ac, Integer a, Integer b);
    Ordinal remainder(ArithmeticControls& ac, Ordinal a, Ordinal b);
    Integer remainder(ArithmeticControls& ac, Integer a, Integer b);
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
        if ((value & mask) == 0) {
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
    /**
     * Find the most significant set bit
     */
    Ordinal scanBit(ArithmeticControls& ac, Ordinal value) noexcept;
    /**
     * Find the most significant clear bit
     */
    Ordinal spanBit(ArithmeticControls& ac, Ordinal value) noexcept;
    void scanByte(ArithmeticControls& ac, Ordinal src1, Ordinal src2) noexcept;
    Ordinal addWithCarry(ArithmeticControls& ac, Ordinal src1, Ordinal src2) noexcept;
    void checkBit(ArithmeticControls& ac, Ordinal src, Ordinal position) noexcept;
    Ordinal alterBit(const ArithmeticControls& ac, Ordinal src, Ordinal position) noexcept;
    template<typename T>
    void compare(ArithmeticControls& ac, T src1, T src2) noexcept {
        if (src1 < src2) {
            ac._conditionCode = 0b100;
        } else if (src1 == src2) {
            ac._conditionCode = 0b010;
        } else {
            ac._conditionCode = 0b001;
        }
    }
    template void compare<Integer>(ArithmeticControls& ac, Integer src1, Integer src2) noexcept;
    template void compare<Ordinal>(ArithmeticControls& ac, Ordinal src1, Ordinal src2) noexcept;
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

    Integer modulo(ArithmeticControls& ac, Integer src1, Integer src2) noexcept;

    constexpr Ordinal rotate(Ordinal src, Ordinal length) noexcept {
		// taken from the wikipedia entry on circular shifts through my syn
		// project
		return Ordinal((src << length) | (src >> ((-length) & 31u)));
	}
    Ordinal subtractWithCarry(ArithmeticControls& ac, Ordinal src1, Ordinal src2) noexcept;
	
    inline Real add(Real a, Real b) noexcept { return Real(a._floating + b._floating); }
    inline LongReal add(LongReal a, LongReal b) noexcept { return LongReal(a._floating + b._floating); }
    inline Real subtract(Real a, Real b) noexcept { return Real(a._floating - b._floating); }
    inline LongReal subtract(LongReal a, LongReal b) noexcept { return LongReal(a._floating - b._floating); }
    inline Real multiply(Real a, Real b) noexcept { return Real(a._floating * b._floating); }
    inline LongReal multiply(LongReal a, LongReal b) noexcept { return LongReal(a._floating * b._floating); }
    template<typename T>
    inline T divide(T a, T b) noexcept {
        return T(a._floating / b._floating);
    }
    template Real divide<Real>(Real a, Real b);
    template LongReal divide<LongReal>(LongReal a, LongReal b);

    template<typename T>
    inline T sine(T value) noexcept {
        return T(::sin(value._floating));
    }
    template Real sine<Real>(Real a);
    template LongReal sine<LongReal>(LongReal a);
    template<typename T>
    inline T cosine(T value) noexcept {
        return T(::cos(value._floating));
    }
    template Real cosine<Real>(Real a);
    template LongReal cosine<LongReal>(LongReal a);
    template<typename T>
    inline T tangent(T value) noexcept {
        return T(::tan(value._floating));
    }
    template Real tangent<Real>(Real a);
    template LongReal tangent<LongReal>(LongReal a);

    template<typename T>
    inline T squareRoot(T value) noexcept {
        return T(::sqrt(value._floating));
    }
    template Real squareRoot<Real>(Real a);
    template LongReal squareRoot<LongReal>(LongReal a);

    template<typename T>
    inline T arctangent(T a, T b) noexcept {
        return T(::atan(divide(b, a)._floating));
    }

    template Real arctangent<Real>(Real a, Real b); 
    template LongReal arctangent<LongReal>(LongReal a, LongReal b); 


    template<typename T>
    inline T logarithmBinary(T a) noexcept {
        return T(::log2(a._floating));
    }
    template Real logarithmBinary<Real>(Real a); 
    template LongReal logarithmBinary<LongReal>(LongReal a); 

    template<typename T>
    inline T logarithmEpsilon(T a, T b) noexcept {
        return multiply(b, logarithmBinary(add(a, T(1.0))));
    }
    template Real logarithmEpsilon<Real>(Real a, Real b); 
    template LongReal logarithmEpsilon<LongReal>(LongReal a, LongReal b); 

    template<typename T>
    inline T logarithm(T a, T b) noexcept {
        return multiply(b, logarithmBinary(a));
    }

    template Real logarithm<Real>(Real a, Real b); 
    template LongReal logarithm<LongReal>(LongReal a, LongReal b); 

    void classify(ArithmeticControls& ac, Real a) noexcept;
    void classify(ArithmeticControls& ac, LongReal a) noexcept;
    Real convertToReal(LongInteger a) noexcept;
    Real convertToReal(Integer a) noexcept;
    Integer convertToInteger(Real a, bool truncate = false) noexcept;
    LongInteger convertToLongInteger(Real a, bool truncate = false) noexcept;
    Real round(Real a) noexcept;
    LongReal round(LongReal a) noexcept;
    Real scale(Real a, Real b) noexcept;
    LongReal scale(LongReal a, LongReal b) noexcept;
    Real remainder(Real a, Real b) noexcept;
    LongReal remainder(LongReal a, LongReal b) noexcept;
    Ordinal decimalSubtractWithCarry(ArithmeticControls& ac, Ordinal src1, Ordinal src2) noexcept;
    Ordinal decimalMoveAndTest(ArithmeticControls& ac, Ordinal src) noexcept;
    Ordinal decimalAddWithCarry(ArithmeticControls& ac, Ordinal src1, Ordinal src2) noexcept;
    Real exponent(Real src) noexcept;
    LongReal exponent(LongReal src) noexcept;
    void compareOrdered(ArithmeticControls& ac, Real src1, Real src2) noexcept;
    void compareOrdered(ArithmeticControls& ac, LongReal src1, LongReal src2) noexcept;
	void compare(ArithmeticControls& ac, Real src1, Real src2) noexcept;
	void compare(ArithmeticControls& ac, LongReal src1, LongReal src2) noexcept;
    bool isUnordered(Real r) noexcept;
    bool isUnordered(LongReal r) noexcept;
    bool isUnordered(ExtendedReal r) noexcept;
	enum TestTypes {
		Unordered = 0b000,
		Greater = 0b001,
		Equal = 0b010,
		GreaterOrEqual = 0b011,
		Less = 0b100,
		NotEqual = 0b101, 
		LessOrEqual = 0b110,
		Ordered = 0b111,
	};
	template<TestTypes t>
	Ordinal test(const ArithmeticControls& ac) noexcept {
		constexpr auto mask = t & 0b111;
		if constexpr (mask == TestTypes::Unordered) {
			return ac._conditionCode == 0 ? 1 : 0;
		} else {
			return (mask & ac._conditionCode) != 0 ? 1 : 0;
		}
	}
    // TODO provide signatures for the fault instructions
    // TODO provide signatures for the load instructions
    // TODO provide signatures for the store instructions
    // TODO provide signature for the flushreg instruction
    // TODO provide signature for the fmark instruction
    // TODO provide signature for the mark instruction
    // TODO provide signature for the load address instruction
    // TODO provide signature for the modac instruction
    // TODO provide signature for the modpc instruction
    // TODO provide signature for the modtc instruction
    // TODO provide signatures for the move instructions
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
    // TODO provide a signature for the ret instruction
    // TODO provide a signature for the shift right dividing integer instruction
    // TODO syncf signature
    // TODO signatures for the fp move instructions
    // TODO synld signature
    // TODO synmov, synmovl, synmovq signatures
    // TODO support cpysre and cpyrsre
} // end namespace i960 

#endif // end I960_OPS_H__
