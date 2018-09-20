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
    template Real divide<Real>(Real, Real);
    template LongReal divide<LongReal>(LongReal, LongReal);

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
	//void compare(ArithmeticControls& ac, Real src1, Real src2) noexcept;
	//void compare(ArithmeticControls& ac, LongReal src1, LongReal src2) noexcept;
    bool isUnordered(Real r) noexcept;
    bool isUnordered(LongReal r) noexcept;
    bool isUnordered(ExtendedReal r) noexcept;
	template<TestTypes t>
	Ordinal test(const ArithmeticControls& ac) noexcept {
		constexpr auto mask = Ordinal(t) & 0b111;
		if constexpr (mask == Ordinal(TestTypes::Unordered)) {
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
