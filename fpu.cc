#include <iostream>
#include <cstddef>

using ByteOrdinal = uint8_t;
using ShortOrdinal = uint16_t;
using Ordinal = uint32_t;
using LongOrdinal = uint64_t;

using ByteInteger = int8_t;
using ShortInteger = int16_t;
using Integer = int32_t;
using LongInteger = int64_t;

struct QuadWord {
    Ordinal _lowest;
    Ordinal _lower;
    Ordinal _higher;
    Ordinal _highest;
};
struct Real {
    Real() : Real(0,0,0) { }
    Real(Ordinal frac, Ordinal exponent, Ordinal flag) : _fraction(frac), _exponent(exponent), _flag(flag) { };
    union {
       struct {
            Ordinal _fraction : 23;
            Ordinal _exponent : 8;
            Ordinal _flag : 1;
       };
       Ordinal _value;
    };
} __attribute__((packed));
struct LongReal {
    LongReal() : LongReal(0,0) { }
    LongReal(Ordinal lower, Ordinal upper);
    LongReal(LongOrdinal frac, LongOrdinal exponent, LongOrdinal sign) : _fraction(frac), _exponent(exponent), _sign(sign) { };
    union {
        struct {
            LongOrdinal _fraction : 52;
            LongOrdinal _exponent : 11;
            LongOrdinal _sign : 1;
        };
        LongOrdinal _value;
    };
} __attribute__((packed));
struct ExtendedReal {
    ExtendedReal(LongOrdinal lower, ShortOrdinal upper) : _lower(lower), _upper(upper) { };
    ExtendedReal() : ExtendedReal(0,0) { }
    ExtendedReal(LongOrdinal frac, LongOrdinal integer, LongOrdinal exponent, LongOrdinal sign) : _fraction(frac), _integer(integer), _exponent(exponent), _sign(sign) { }
    union {
        struct {
            LongOrdinal _fraction : 63;
            LongOrdinal _integer : 1;
        };
        LongOrdinal _lower;
    };
    union {
        struct {
            ShortOrdinal _exponent : 15;
            ShortOrdinal _sign : 1;
        };
        ShortOrdinal _upper;
    };
} __attribute__((packed));

union TripleWord {
    struct {
        Ordinal _lower;
        Ordinal _middle;
        Ordinal _upper;
    };
    ExtendedReal _real;
};


union NormalRegister {
    Ordinal _ordinal;
    Integer _integer;
    Real _real;
};

union ArithmeticControls {
    struct {
        Ordinal _conditionCode : 3;
        /**
         * Used to record results from the classify real (classr and classrl)
         * and remainder real (remr and remrl) instructions. 
         */
        Ordinal _arithmeticStatusField : 4;
        Ordinal _unknown : 1 ; // TODO figure out what this is
        /**
         * Denotes an integer overflow happened
         */
        Ordinal _integerOverflowFlag : 1;
        Ordinal _unknown2 : 3;
        /**
         * Inhibit the processor from invoking a fault handler
         * when integer overflow is detected.
         */
        Ordinal _integerOverflowMask : 1;
        Ordinal _unknown3 : 3;
        /**
         * Floating point overflow happened?
         */
        Ordinal _floatingOverflowFlag : 1;
        /**
         * Floating point underflow happened?
         */
        Ordinal _floatingUnderflowFlag : 1;
        /**
         * Floating point invalid operation happened?
         */
        Ordinal _floatingInvalidOperationFlag : 1;
        /**
         * Floating point divide by zero happened?
         */
        Ordinal _floatingZeroDivideFlag : 1;
        /**
         * Floating point rounding result was inexact?
         */
        Ordinal _floatingInexactFlag : 1;
        Ordinal _unknown4 : 3;
        /**
         * Don't fault on fp overflow?
         */
        Ordinal _floatingOverflowMask : 1;
        /**
         * Don't fault on fp underflow?
         */
        Ordinal _floatingUnderflowMask : 1;
        /**
         * Don't fault on fp invalid operation?
         */
        Ordinal _floatingInvalidOperationMask : 1;
        /**
         * Don't fault on fp div by zero?
         */
        Ordinal _floatingZeroDivideMask : 1;
        /**
         * Don't fault on fp round producing an inexact representation?
         */
        Ordinal _floatingInexactMask : 1;
        /**
         * Enable to produce normalized values, disable to produce unnormalized values (useful for software simulation).
         * Note that the processor will fault if it encounters denormalized fp values!
         */
        Ordinal _normalizingModeFlag : 1;
        /**
         * What kind of fp rounding should be performed?
         * Modes:
         *  0: round up (towards positive infinity)
         *  1: round down (towards negative infinity)
         *  2: Round toward zero (truncate)
         *  3: Round to nearest (even)
         */
        Ordinal _roundingControl : 2;
    };
    Ordinal _value;
};


int main() {
    std::cout << "sizeof(ExtendedReal): " << sizeof(ExtendedReal) << std::endl;
    std::cout << "sizeof(LongReal): " << sizeof(LongReal) << std::endl;
    std::cout << "sizeof(Real): " << sizeof(Real) << std::endl;
    std::cout << "sizeof(TripleWord): " << sizeof(TripleWord) << std::endl;
    std::cout << "sizeof(QuadWord): " << sizeof(QuadWord) << std::endl;
    std::cout << "sizeof(NormalRegister): " << sizeof(NormalRegister) << std::endl;
    std::cout << "sizeof(ArithmeticControls): " << sizeof(ArithmeticControls) << std::endl;
    return 0;
}
