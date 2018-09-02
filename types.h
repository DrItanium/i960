#ifndef I960_TYPES_H__
#define I960_TYPES_H__
#include <cstddef>
namespace i960 {
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
	/**
	 * Part of the numerics architecture and above
	 */
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
	/**
	 * Part of the numerics architecture and above
	 */
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
	/**
	 * Part of the numerics architecture and above
	 */
    struct ExtendedReal {
        ExtendedReal(LongOrdinal lower, ShortOrdinal upper) : _lower(lower), _upper(upper) { }
        ExtendedReal() : ExtendedReal(0,0) { }
		union {
			LongOrdinal _lower;
			struct {
				LongOrdinal _fraction: 63;
				LongOrdinal _j : 1;
			};
		}; 
		union {
			ShortOrdinal _upper;
			struct {
				ShortOrdinal _exponent : 15;
				ShortOrdinal _sign : 1;
			};
		};
    } __attribute__((packed));

    union TripleWord {
        struct {
            Ordinal _lower;
            Ordinal _middle;
            Ordinal _upper;
        };
        ExtendedReal _real;
    } __attribute__((packed));

    union NormalRegister {
        Ordinal _ordinal;
        Integer _integer;
        Real _real;
    };
}
#endif
