#ifndef I960_TYPES_H__
#define I960_TYPES_H__
#include <cstdint>
#include <memory>
#include "archlevel.h"
namespace i960 {

    using ByteOrdinal = std::uint8_t;
    using ShortOrdinal = std::uint16_t;
    using Ordinal = std::uint32_t;
    using LongOrdinal = std::uint64_t;
    using InstructionPointer = Ordinal;
    using ByteInteger = std::int8_t;
    using ShortInteger = std::int16_t;
    using Integer = std::int32_t;
    using LongInteger = std::int64_t;

    constexpr bool isIntegerOverflow(Ordinal value) noexcept {
        return value > 0x7FFF'FFFF;
    }
    constexpr bool shouldSetCarryBit(LongOrdinal a) noexcept {
        return (a & 0xFFFF'FFFF'0000'0000) != 0;
    }
    constexpr bool shouldSetCarryBit(ShortOrdinal a) noexcept {
        return (a & 0xFF00) != 0;
    }
    constexpr bool shouldSetCarryBit(Ordinal a) noexcept {
        return (a & 0xFFFF'0000) != 0;
    }
    constexpr bool shouldSetCarryBit(ByteOrdinal a) noexcept {
        return (a & 0xF0) != 0;
    }

    union Displacement {
        Displacement(Integer value) : _value(value) { }
        ~Displacement() = default;
        Integer _value : 22;
    };
    using RawReal = float;
    using RawLongReal = double;
    using RawExtendedReal = long double;
    static_assert(sizeof(RawReal) == sizeof(Ordinal), "Real must be the same size as Ordinal");
    static_assert(sizeof(RawLongReal) == sizeof(LongOrdinal), "LongReal must be the same size as LongOrdinal");
    static_assert(sizeof(RawExtendedReal) >= 10, "ExtendedReal must be at least 10 bytes wide");
    /**
     * Part of the numerics architecture and above
     */
    struct Real {
		static constexpr Ordinal MaxExponent = 0xFF;
		static constexpr Ordinal MostSignificantFractionBit = 1 << 22;
		static constexpr Ordinal RestFractionBits = MostSignificantFractionBit - 1;
		static constexpr Ordinal SignCheckBit = 1 << 31;
		static constexpr Ordinal ZeroCheckBits = SignCheckBit - 1;
		static_assert(RestFractionBits == 0b011'1111'1111'1111'1111'1111, "Got it wrong!");
        Real() : Real(0,0,0) { }
        Real(Ordinal frac, Ordinal exponent, Ordinal flag) : fraction(frac), exponent(exponent), sign(flag) { };
        explicit Real(RawReal value) : floating(value) { }
        union {
            struct {
                Ordinal fraction : 23;
                Ordinal exponent : 8;
                Ordinal sign : 1;
            };
            Ordinal bits;
            RawReal floating;
        };
		bool isInfinity() const noexcept { return (exponent == MaxExponent) && (fraction == 0); }
		bool isPositiveInfinity() const noexcept { return isInfinity() && (sign == 0); }
		bool isNegativeInfinity() const noexcept { return isInfinity() && (sign == 1); }
		bool isNaN() const noexcept { return (fraction != 0) && (exponent == MaxExponent); }
		bool isSignalingNaN() const noexcept { return isNaN() && ((fraction & MostSignificantFractionBit) == 0); }
		bool isQuietNaN() const noexcept { return isNaN() && ((fraction & MostSignificantFractionBit) != 0); }
		bool isIndefiniteQuietNaN() const noexcept { return isQuietNaN() && ((fraction & RestFractionBits) == 0); }
		bool isNormalQuietNaN() const noexcept { return isQuietNaN() && ((fraction & RestFractionBits) != 0); }
		bool isReservedEncoding() const noexcept { return false; }
		bool isZero() const noexcept { return (bits & ZeroCheckBits) == 0; }
		bool isPositiveZero() const noexcept { return isZero() && (sign == 0); }
		bool isNegativeZero() const noexcept { return isZero() && (sign == 1); }
		bool isDenormal() const noexcept { return (exponent == 0) && (fraction != 0); }
		bool isNormal() const noexcept { return ((exponent > 0) && (exponent < MaxExponent)); }
				   
    } __attribute__((packed));
    /**
     * Part of the numerics architecture and above
     */
    struct LongReal {
		static constexpr LongOrdinal MaxExponent = 0x7FF;
		static constexpr LongOrdinal MostSignificantFractionBit = 1ul << 51;
		static constexpr LongOrdinal RestFractionBits = MostSignificantFractionBit - 1; 
		static constexpr LongOrdinal SignCheckBit = 1ul << 63;
		static constexpr LongOrdinal ZeroCheckBits = SignCheckBit - 1;
        LongReal() : LongReal(0,0) { }
        LongReal(Ordinal lower, Ordinal upper) : bits(LongOrdinal(lower) | (LongOrdinal(upper) << 32)) { }
        LongReal(LongOrdinal frac, LongOrdinal exponent, LongOrdinal sign) : fraction(frac), exponent(exponent), sign(sign) { }
        explicit LongReal(RawLongReal value) : floating(value) { }
        Ordinal lowerHalf() const noexcept { return static_cast<Ordinal>(bits & 0xFFFF'FFFF); }
        Ordinal upperHalf() const noexcept { return static_cast<Ordinal>((bits & 0xFFFF'FFFF'0000'0000) >> 32); }
        union {
            struct {
                LongOrdinal fraction : 52;
                LongOrdinal exponent : 11;
                LongOrdinal sign : 1;
            };
            LongOrdinal bits;
            RawLongReal floating;
        };
		bool isInfinity() const noexcept { return (exponent == MaxExponent) && (fraction == 0); }
		bool isPositiveInfinity() const noexcept { return isInfinity() && (sign == 0); }
		bool isNegativeInfinity() const noexcept { return isInfinity() && (sign == 1); }
		bool isNaN() const noexcept { return (fraction != 0) && (exponent == MaxExponent); }
		bool isSignalingNaN() const noexcept { return isNaN() && ((fraction & MostSignificantFractionBit) == 0); }
		bool isQuietNaN() const noexcept { return isNaN() && ((fraction & MostSignificantFractionBit) != 0); }
		bool isIndefiniteQuietNaN() const noexcept { return isQuietNaN() && ((fraction & RestFractionBits) == 0); }
		bool isNormalQuietNaN() const noexcept { return isQuietNaN() && ((fraction & RestFractionBits) != 0); }
		bool isReservedEncoding() const noexcept { return false; }
		bool isZero() const noexcept { return (bits & ZeroCheckBits) == 0; }
		bool isPositiveZero() const noexcept { return isZero() && (sign == 0); }
		bool isNegativeZero() const noexcept { return isZero() && (sign == 1); }
		bool isDenormal() const noexcept { return (exponent == 0) && (fraction != 0); }
		bool isNormal() const noexcept { return ((exponent > 0) && (exponent < MaxExponent)); }
    } __attribute__((packed));
    constexpr LongOrdinal makeLongOrdinal(Ordinal lower, Ordinal upper) noexcept {
        return LongOrdinal(lower) | (LongOrdinal(upper) << 32);
    }
    /**
     * Part of the numerics architecture and above
     */
    union ExtendedReal {
		static constexpr LongOrdinal MaxExponent = 0x7FFF;
		static constexpr LongOrdinal MostSignificantFractionBit = 1ul << 62;
		static constexpr LongOrdinal RestFractionBits = MostSignificantFractionBit - 1; 
		static constexpr ShortOrdinal SignCheckBit = 1 << 15;
		static constexpr ShortOrdinal ZeroCheckBits_Upper = SignCheckBit - 1;
        ExtendedReal() { }
        ExtendedReal(Ordinal low, Ordinal mid, Ordinal high) : lower(makeLongOrdinal(low, mid)), upper(high) { }
        explicit ExtendedReal(RawExtendedReal value) : floating(value) { }
        Ordinal lowerThird() const noexcept { return static_cast<Ordinal>(lower); }
        Ordinal middleThird() const noexcept { return static_cast<Ordinal>(lower >> 32); }
        Ordinal upperThird() const noexcept { return static_cast<Ordinal>(upper); }
		/**
		 * Combine the j field with the fraction field to get a complete value
		 * j is what is normally the implied 1 in the mantissa
		 */

		LongOrdinal getCompleteFraction() const noexcept { 
			LongOrdinal tmp = j; // get rid of the width cast problem
			return (tmp << 63) | fraction; 
		}


        struct {
            LongOrdinal fraction: 63;
            LongOrdinal j : 1;
            ShortOrdinal exponent : 15;
            ShortOrdinal sign : 1;
        }; 
        struct {
            LongOrdinal lower;
            ShortOrdinal upper;
        };
        RawExtendedReal floating;
		bool isInfinity() const noexcept { return (j == 1) && (exponent == MaxExponent) && (fraction == 0); }
		bool isPositiveInfinity() const noexcept { return isInfinity() && (sign == 0); }
		bool isNegativeInfinity() const noexcept { return isInfinity() && (sign == 1); }
		bool isNaN() const noexcept { return (j == 1) && (fraction != 0) && (exponent == MaxExponent); }
		bool isSignalingNaN() const noexcept { return isNaN() && ((fraction & MostSignificantFractionBit) == 0); }
		bool isQuietNaN() const noexcept { return isNaN() && ((fraction & MostSignificantFractionBit) != 0); }
		bool isIndefiniteQuietNaN() const noexcept { return isQuietNaN() && ((fraction & RestFractionBits) == 0); }
		bool isNormalQuietNaN() const noexcept { return isQuietNaN() && ((fraction & RestFractionBits) != 0); }
		bool isReservedEncoding() const noexcept { return (exponent != 0) && (j == 0); }
		bool isZero() const noexcept { return (lower == 0) && ((upper & ZeroCheckBits_Upper) == 0); }
		bool isPositiveZero() const noexcept { return isZero() && (sign == 0); }
		bool isNegativeZero() const noexcept { return isZero() && (sign == 1); }
		bool isDenormal() const noexcept { return (exponent == 0) && (fraction != 0); }
		bool isNormal() const noexcept { 
			// this is a little strange since the 80960 book states that there
			// is a hole in the design. However, from what I can tell it is
			// probably safe to operate the same way as the other Real types
			return ((exponent > 0) && (exponent < MaxExponent)); 
		}
    } __attribute__((packed));

    union PreviousFramePointer {
        struct {
            Ordinal returnCode : 3;
            Ordinal prereturnTrace : 1;
            Ordinal unused : 2; // 80960 ignores the lower six bits of this register
            Ordinal address : 26;
        };
        Ordinal value;
    } __attribute__((packed));
    union NormalRegister {
        private:
            template<typename T>
			static constexpr bool LegalConversion = false;
        public:
            NormalRegister(Ordinal value) : ordinal(value) { }
            NormalRegister() : NormalRegister(0u) { }
            ~NormalRegister() { ordinal = 0; }

            PreviousFramePointer pfp;
            Ordinal ordinal;
            Integer integer;
            Real real;
            ByteOrdinal byteOrd;
            ShortOrdinal shortOrd;

            template<typename T>
            T get() const noexcept {
                using K = std::decay_t<T>;
                if constexpr(std::is_same_v<K, Ordinal>) {
                    return ordinal;
                } else if constexpr(std::is_same_v<K, Integer>) {
                    return integer;
                } else if constexpr(std::is_same_v<K, ByteOrdinal>) {
                    return byteOrd;
                } else if constexpr(std::is_same_v<K, ShortOrdinal>) {
                    return shortOrd;
                } else if constexpr(std::is_same_v<K, Real>) {
                    return real;
                } else if constexpr(std::is_same_v<K, RawReal>) {
                    return real.floating;
                } else if constexpr(std::is_same_v<K, PreviousFramePointer>) {
                    return pfp;
                } else if constexpr(std::is_same_v<K, LongOrdinal>) {
                    return static_cast<LongOrdinal>(ordinal);
                } else {
                    static_assert(LegalConversion<K>, "Illegal type requested");
                }
            }
            template<typename T>
            void set(T value) noexcept {
                using K = std::decay_t<T>;
                if constexpr(std::is_same_v<K, Ordinal>) {
                    ordinal = value;
                } else if constexpr(std::is_same_v<K, Integer>) {
                    integer = value;
                } else if constexpr(std::is_same_v<K, ByteOrdinal>) {
                    byteOrd = value;
                } else if constexpr(std::is_same_v<K, ShortOrdinal>) {
                    shortOrd = value;
                } else if constexpr(std::is_same_v<K, Real>) {
                    real.floating = value.floating;
                } else if constexpr(std::is_same_v<K, RawReal>) {
                    real.floating = value;
                } else if constexpr(std::is_same_v<K, PreviousFramePointer>) {
                    ordinal = value.value;
                } else if constexpr(std::is_same_v<K, LongReal>) {
                    real.floating = value.floating;
                } else if constexpr(std::is_same_v<K, RawLongReal>) {
                    real.floating = value;
                } else if constexpr(std::is_same_v<K, ExtendedReal>) {
                    real.floating = value.floating;
                } else if constexpr(std::is_same_v<K, RawExtendedReal>) {
                    real.floating = value;
                } else {
                    static_assert(LegalConversion<K>, "Illegal type requested");
                }
            }
            void move(const NormalRegister& other) noexcept { set<Ordinal>(other.get<Ordinal>()); }
    };
	static_assert(sizeof(NormalRegister) == sizeof(Ordinal), "NormalRegister must be 32-bits wide!");
    class DoubleRegister {
        private:
            template<typename T>
            static constexpr bool LegalConversion = false;
        public:
            DoubleRegister(NormalRegister& lower, NormalRegister& upper) : _lower(lower), _upper(upper) { }
            ~DoubleRegister() = default;
            template<typename T>
                T get() const noexcept {
                    using K = std::decay_t<T>;
                    if constexpr(std::is_same_v<K, LongOrdinal>) {
                        return LongOrdinal(_lower.ordinal) | (LongOrdinal(_upper.ordinal) << 32);
                    } else if constexpr(std::is_same_v<K, LongReal>) {
                        return LongReal(_lower.ordinal, _upper.ordinal);
                    } else if constexpr (std::is_same_v<K, RawLongReal>) {
                        return get<LongReal>().floating;
                    } else {
                        static_assert(LegalConversion<K>, "Illegal type requested");
                    }
                }
            template<typename T>
                void set(T value) noexcept {
                    using K = std::decay_t<T>;
                    if constexpr(std::is_same_v<K, LongOrdinal>) {
                        _lower.ordinal = static_cast<Ordinal>(value);
                        _upper.ordinal = static_cast<Ordinal>(value >> 32);
                    } else if constexpr (std::is_same_v<K, LongReal>) {
                        _lower.ordinal = value.lowerHalf();
                        _upper.ordinal = value.upperHalf();
                    } else if constexpr (std::is_same_v<K, RawLongReal>) {
                        set<LongReal>(LongReal(value));
                    } else if constexpr(std::is_same_v<K, ExtendedReal>) {
                        set<RawLongReal>(RawLongReal(value.floating));
                    } else if constexpr(std::is_same_v<K, RawExtendedReal>) {
                        set(ExtendedReal(value));
                    } else if constexpr(std::is_same_v<K, Real>) {
                        set(value.floating);
                    } else if constexpr(std::is_same_v<K, RawReal>) {
                        set(RawLongReal(value));
                    } else {
                        static_assert(LegalConversion<K>, "Illegal type requested");
                    }
                }
            void set(Ordinal lower, Ordinal upper) noexcept;
            void move(const DoubleRegister& other) noexcept;
            Ordinal getLowerHalf() const noexcept { return _lower.get<Ordinal>(); }
            Ordinal getUpperHalf() const noexcept { return _upper.get<Ordinal>(); }

        private:
            NormalRegister& _lower;
            NormalRegister& _upper;
    };
    class TripleRegister {
        private:
            template<typename T>
            static constexpr bool LegalConversion = false;
        public:
            TripleRegister(NormalRegister& lower, NormalRegister& mid, NormalRegister& upper) : _lower(lower), _mid(mid), _upper(upper) { }
            ~TripleRegister() = default;
            template<typename T>
                T get() const noexcept {
                    using K = std::decay_t<T>;
                    if constexpr(std::is_same_v<K, ExtendedReal>) {
                        return ExtendedReal(_lower.ordinal, _mid.ordinal, _upper.ordinal);
                    } else if constexpr (std::is_same_v<K, RawExtendedReal>) {
                        return get<ExtendedReal>().floating;
                    } else {
                        static_assert(LegalConversion<K>, "Illegal type requested");
                    }
                }
            template<typename T>
                void set(T value) noexcept {
                    using K = std::decay_t<T>;
                    if constexpr (std::is_same_v<K, ExtendedReal>) {
                        _lower.ordinal = value.lowerThird();
                        _mid.ordinal = value.middleThird();
                        _upper.ordinal = value.upperThird();
                    } else if constexpr (std::is_same_v<K, RawExtendedReal>) {
                        set<ExtendedReal>(ExtendedReal(value));
                    } else {
                        static_assert(LegalConversion<K>, "Illegal type requested");
                    }
                }
            void set(Ordinal lower, Ordinal mid, Ordinal upper) noexcept;
            void move(const TripleRegister& other) noexcept;
            Ordinal getLowerPart() const noexcept { return _lower.get<Ordinal>(); }
            Ordinal getMiddlePart() const noexcept { return _mid.get<Ordinal>(); }
            Ordinal getUpperPart() const noexcept { return _upper.get<Ordinal>(); }
        private:
            NormalRegister& _lower;
            NormalRegister& _mid;
            NormalRegister& _upper;
    };
    class QuadRegister {
        public:
            QuadRegister(NormalRegister& lower, NormalRegister& mid, NormalRegister& high, NormalRegister& highest) : _lower(lower), _mid(mid), _upper(high), _highest(highest) { }
            ~QuadRegister() = default;
            void set(Ordinal lower, Ordinal mid, Ordinal upper, Ordinal highest) noexcept;
            void move(const QuadRegister& other) noexcept;
            Ordinal getLowestPart() const noexcept { return _lower.get<Ordinal>(); }
            Ordinal getLowerPart() const noexcept { return _mid.get<Ordinal>(); }
            Ordinal getHigherPart() const noexcept { return _upper.get<Ordinal>(); }
            Ordinal getHighestPart() const noexcept { return _highest.get<Ordinal>(); }
        private:
            NormalRegister& _lower;
            NormalRegister& _mid;
            NormalRegister& _upper;
            NormalRegister& _highest;
    };
    union ArithmeticControls {
        struct {
            Ordinal conditionCode : 3;
            /**
             * Used to record results from the classify real (classr and classrl)
             * and remainder real (remr and remrl) instructions. 
             */
            Ordinal arithmeticStatusField : 4;
            /**
             * Reserved, bind to zero always
             */
            Ordinal reserved0 : 1; 
            /**
             * Denotes an integer overflow happened
             */
            Ordinal integerOverflowFlag : 1;
            /**
             * Reserved, bind to zero always
             */
            Ordinal reserved1 : 3;
            /**
             * Inhibit the processor from invoking a fault handler
             * when integer overflow is detected.
             */
            Ordinal integerOverflowMask : 1;
            /**
             * Reserved, always bind to zero
             */
            Ordinal reserved2 : 2;
            /**
             * Disable faults generated by imprecise results being generated
             */
            Ordinal noImpreciseResults : 1;
            /**
             * Floating point overflow happened?
             */
            Ordinal floatingOverflowFlag : 1;
            /**
             * Floating point underflow happened?
             */
            Ordinal floatingUnderflowFlag : 1;
            /**
             * Floating point invalid operation happened?
             */
            Ordinal floatingInvalidOperationFlag : 1;
            /**
             * Floating point divide by zero happened?
             */
            Ordinal floatingZeroDivideFlag : 1;
            /**
             * Floating point rounding result was inexact?
             */
            Ordinal floatingInexactFlag : 1;
            /**
             * Reserved, always bind to zero
             */
            Ordinal reserved3 : 3;
            /**
             * Don't fault on fp overflow?
             */
            Ordinal floatingOverflowMask : 1;
            /**
             * Don't fault on fp underflow?
             */
            Ordinal floatingUnderflowMask : 1;
            /**
             * Don't fault on fp invalid operation?
             */
            Ordinal floatingInvalidOperationMask : 1;
            /**
             * Don't fault on fp div by zero?
             */
            Ordinal floatingZeroDivideMask : 1;
            /**
             * Don't fault on fp round producing an inexact representation?
             */
            Ordinal floatingInexactMask : 1;
            /**
             * Enable to produce normalized values, disable to produce unnormalized values (useful for software simulation).
             * Note that the processor will fault if it encounters denormalized fp values!
             */
            Ordinal normalizingModeFlag : 1;
            /**
             * What kind of fp rounding should be performed?
             * Modes:
             *  0: round up (towards positive infinity)
             *  1: round down (towards negative infinity)
             *  2: Round toward zero (truncate)
             *  3: Round to nearest (even)
             */
            Ordinal roundingControl : 2;
        };
        Ordinal value;
        Ordinal modify(Ordinal mask, Ordinal value) noexcept {
            if (mask == 0) {
                return value;
            } else {
                auto tmp = value;
                value = (value & mask) | (value & ~(mask));
                return tmp;
            }
        }

        // NOTE that both of these methods could return true in some cases
        // I think that the safest solution is to actually raise a fault if both
        // are true or both are false.

        bool conditionIsTrue() const noexcept {
            return conditionCode == 0b010;
        }
        bool conditionIsFalse() const noexcept {
            return conditionCode == 0b000;
        }
        bool conditionIsUnordered() const noexcept {
            return conditionCode == 0b000;
        }
        bool conditionIsGreaterThan() const noexcept {
            return conditionCode == 0b001;
        }
        bool conditionIsEqual() const noexcept {
            return conditionCode == 0b010;
        }
        bool conditionIsGreaterThanOrEqual() const noexcept {
            return conditionCode == 0b011;
        }
        bool conditionIsLessThan() const noexcept {
            return conditionCode == 0b100;
        }
        bool conditionIsNotEqual() const noexcept {
            return conditionCode == 0b101;
        }
        bool conditionIsLessThanOrEqual() const noexcept {
            return conditionCode == 0b110;
        }
        bool conditionIsOrdered() const noexcept {
            return conditionCode == 0b111;
        }
        Ordinal getConditionCode() const noexcept {
            return conditionCode;
        }
        bool roundToNearest() const noexcept {
            return roundingControl == 0b00;
        }
        bool roundDown() const noexcept {
            return roundingControl == 0b01;
        }
        bool roundUp() const noexcept {
            return roundingControl == 0b10;
        }
        bool roundTowardsZero() const noexcept {
            return roundingControl == 0b11;
        }
        bool arithmeticStatusIsZero() const noexcept {
            return (arithmeticStatusField & 0b0111) == 0;
        }
        bool arithmeticStatusIsDenormalizedNumber() const noexcept {
            return (arithmeticStatusField & 0b0111) == 1;
        }
        bool arithmeticStatusIsNormalFiniteNumber() const noexcept {
            return (arithmeticStatusField & 0b0111) == 2;
        }
        bool arithmeticStatusIsInfinity() const noexcept {
            return (arithmeticStatusField & 0b0111) == 3;
        }
        bool arithmeticStatusIsQuietNaN() const noexcept {
            return (arithmeticStatusField & 0b0111) == 4;
        }
        bool arithmeticStatusIsSignalingNaN() const noexcept {
            return (arithmeticStatusField & 0b0111) == 5;
        }
        bool arithmeticStatusIsReservedOperand() const noexcept {
            return (arithmeticStatusField & 0b0111) == 6;
        }
        bool getArithmeticStatusSign() const noexcept {
            return ((arithmeticStatusField & 0b1000) >> 3) == 1;
        }
        bool carrySet() const noexcept {
            return (conditionCode & 0b010) != 0;
        }
        Ordinal getCarryValue() const noexcept {
            return carrySet() ? 1 : 0;
        }


    };
    union ProcessControls {
#ifdef PROTECTED_ARCHITECTURE
        struct {
            Ordinal unused0 : 1;
            Ordinal multiprocessorPreempt : 1;
            Ordinal state : 2;
            Ordinal unused1 : 1;
            Ordinal nonpreemptLimit : 5;
            Ordinal addressingMode : 1;
            Ordinal checkDispatchPort : 1;
            Ordinal unused2 : 4;
            Ordinal interimPriority : 5;
            Ordinal unused3 : 10;
            Ordinal writeExternalPriority : 1;
        } _manual;
        struct {
            Ordinal traceEnable : 1;
            Ordinal executionMode : 1;
            Ordinal unused0 : 4;
            Ordinal timeSliceReschedule : 1;
            Ordinal timeSlice : 1;
            Ordinal resume : 1;
            Ordinal traceFaultPending : 1;
            Ordinal preempt : 1;
            Ordinal refault : 1;
            Ordinal state : 2;
            Ordinal unused1 : 1;
            Ordinal priority : 5;
            Ordinal internalState : 11;
        }  _automatic;
#else // !defined(PROTECTED_ARCHITECTURE)
        struct {
            Ordinal traceEnable : 1;
            Ordinal executionMode : 1;
            Ordinal unused0 : 7;
            Ordinal resume : 1;
            Ordinal traceFaultPending : 1;
            Ordinal unused1 : 2;
            Ordinal state : 1;
            Ordinal unused2 : 2;
            Ordinal priority : 5;
            Ordinal internalState : 11;
        };
#endif // end PROTECTED_ARCHITECTURE

        Ordinal value;
    } __attribute__((packed));


	static_assert(sizeof(ProcessControls) == sizeof(Ordinal), "ProcessControls is not the size of an ordinal!");
    union TraceControls {
        struct {
            Ordinal unused0 : 1;
            Ordinal instructionTraceMode : 1;
            Ordinal branchTraceMode : 1;
            Ordinal returnTraceMode : 1;
            Ordinal prereturnTraceMode : 1;
            Ordinal supervisorTraceMode : 1;
            Ordinal breakPointTraceMode : 1;
            Ordinal unused1 : 9;
            Ordinal instructionTraceEvent : 1;
            Ordinal branchTraceEvent : 1;
            Ordinal callTraceEvent : 1;
            Ordinal returnTraceEvent : 1;
            Ordinal prereturnTraceEvent: 1;
            Ordinal supervisorTraceEvent : 1;
            Ordinal breakpointTraceEvent : 1;
            Ordinal unused2 : 8;
        };
        Ordinal value;
    } __attribute__((packed));
	static_assert(sizeof(TraceControls) == sizeof(Ordinal), "TraceControls must be the size of an ordinal!");

    constexpr auto GlobalRegisterCount = 16;
    constexpr auto NumFloatingPointRegs = 4;
    constexpr auto LocalRegisterCount = 16;
    // reserved indicies
    // reserved global registers
    constexpr auto FramePointerIndex = 15;
    // reserved local registers
    constexpr auto PreviousFramePointerIndex = 0;
    constexpr auto StackPointerIndex = 1;
    constexpr auto ReturnInstructionPointerIndex = 2;




    constexpr Ordinal LargestAddress = 0xFFFF'FFFF;
    constexpr Ordinal LowestAddress = 0;
    enum class ConditionCode : Ordinal {
        False = 0b000,
        Unordered = False,
        GreaterThan = 0b001,
        Equal = 0b010,
        True = Equal,
        LessThan = 0b100,
        Ordered = 0b111,
        NotEqual = 0b101,
        LessThanOrEqual = 0b110,
        GreaterThanOrEqual = 0b011,
    };
    enum class TestTypes : Ordinal {
        Unordered = 0b000,
        Greater = 0b001,
        Equal = 0b010,
        GreaterOrEqual = 0b011,
        Less = 0b100,
        NotEqual = 0b101, 
        LessOrEqual = 0b110,
        Ordered = 0b111,
    };

    /**
     * Aritmetic status bits, four bits wide with a sign bit as the upper most 
     * bit of this field. It is taken from the sign of the value being 
     * classified.
     */
    enum ArithmeticStatusCode : Ordinal {
        Zero = 0b000,
        DenormalizedNumber,
        NormalFiniteNumber,
        QuietNaN,
        SignalingNaN,
        ReservedOperand,
    };

    enum FloatingPointRoundingControl : Ordinal {
        RoundToNearest = 0,
        RoundDown,
        RoundUp,
        Truncate,
    };

    union Instruction {
        struct REGFormat {
            Ordinal _source1 : 5;
            Ordinal _sfr : 2;
            Ordinal _opcode2 : 4;
            Ordinal _m1 : 1;
            Ordinal _m2 : 1;
            Ordinal _m3 : 1;
            Ordinal _source2 : 5;
            Ordinal _src_dest : 5;
            Ordinal _opcode : 8;
            Ordinal getOpcode() const noexcept {
                return (_opcode << 4) | _opcode2;
            }
            bool src1IsLiteral() const noexcept { return _m1 != 0; }
            bool src2IsLiteral() const noexcept { return _m2 != 0; }
            bool srcDestIsLiteral() const noexcept { return _m3 != 0; }
            ByteOrdinal src1ToIntegerLiteral() const noexcept { return _source1; }
            ByteOrdinal src2ToIntegerLiteral() const noexcept { return _source2; }
            RawReal src1ToRealLiteral() const noexcept;
            RawReal src2ToRealLiteral() const noexcept;
            RawLongReal src1ToLongRealLiteral() const noexcept;
            RawLongReal src2ToLongRealLiteral() const noexcept;
            bool isFloatingPoint() const noexcept;
            bool src1IsFloatingPointRegister() const noexcept;
            bool src2IsFloatingPointRegister() const noexcept; 
            bool src3IsFloatingPointRegister() const noexcept { return isFloatingPoint() && _m3 != 0; } 

        };
        struct COBRFormat {
            Ordinal _sfr : 1;
            Ordinal _bp : 1;
            Ordinal _displacement : 11;
            Ordinal _m1 : 1;
            Ordinal _source2 : 5; 
            Ordinal _source1 : 5;
            Ordinal _opcode : 8;
            bool src1IsLiteral() const noexcept { return _m1 != 0; }

        };
        struct CTRLFormat {
            Ordinal _sfr : 1;
            Ordinal _bp : 1;
            Ordinal _displacement : 22;
            Ordinal _opcode : 8;
        };
        union MemFormat {
            struct MEMAFormat {
                enum AddressingModes {
                    Offset = 0,
                    Abase_Plus_Offset = 1,
                };
                Ordinal _offset : 12;
                Ordinal _unused : 1;
                Ordinal _md : 1;
                Ordinal _abase : 5;
                Ordinal _src_dest : 5;
                Ordinal _opcode : 8;
                AddressingModes getAddressingMode() const noexcept {
                    return static_cast<AddressingModes>(_md);
                }
            };
            struct MEMBFormat {
                enum AddressingModes {
                    Abase = 0b0100,
                    IP_Plus_Displacement_Plus_8 = 0b0101,
                    Reserved = 0b0110,
                    Abase_Plus_Index_Times_2_Pow_Scale = 0b0111,
                    Displacement = 0b1100,
                    Abase_Plus_Displacement = 0b1101,
                    Index_Times_2_Pow_Scale_Plus_Displacement = 0b1110,
                    Abase_Plus_Index_Times_2_Pow_Scale_Plus_Displacement = 0b1111,
                };
                Ordinal _index : 5;
                Ordinal _sfr : 2;
                Ordinal _scale : 3;
                Ordinal _mode : 4;
                Ordinal _abase : 5;
                Ordinal _src_dest : 5;
                Ordinal _opcode : 8;
                AddressingModes getAddressingMode() const noexcept {
                    return static_cast<AddressingModes>(_mode);
                }
                bool has32bitDisplacement() const noexcept {
                    switch (getAddressingMode()) {
                        case AddressingModes::Abase:
                        case AddressingModes::Abase_Plus_Index_Times_2_Pow_Scale:
                        case AddressingModes::Reserved:
                            return false;
                        default:
                            return true;
                    }
                }
                ByteOrdinal getScaleFactor() const noexcept {
                    switch (_scale) {
                        case 0b000: 
                            return 1;
                        case 0b001:
                            return 2;
                        case 0b010:
                            return 4;
                        case 0b011:
                            return 8;
                        case 0b100: 
                            return 16;
                        default:
                            // TODO raise an invalid opcode fault instead
                            return 0; 
                    }
                }
            };
            bool isMemAFormat() const noexcept {
                return _mema._unused == 0;
            }
            MEMAFormat _mema;
            MEMBFormat _memb;
        };

        static_assert(sizeof(MemFormat) == sizeof(Ordinal), "MemFormat must be the size of an ordinal!");

        Instruction(Ordinal raw) : _raw(raw) { }
        Instruction() : Instruction(0) { }
        Ordinal getBaseOpcode() const noexcept {
            return 0xFF000000 & _raw >> 24;
        }
        Ordinal getOpcode() const noexcept {
            if (isRegFormat()) {
                return _reg.getOpcode();
            } else {
                return getBaseOpcode();
            }
        }
        bool isControlFormat() const noexcept {
            return getBaseOpcode() < 0x20;
        }
        bool isCompareAndBranchFormat() const noexcept {
            auto opcode = getBaseOpcode();
            return opcode >= 0x20 && opcode < 0x40;
        }
        bool isMemFormat() const noexcept {
            return getBaseOpcode() >= 0x80;
        }
        bool isRegFormat() const noexcept {
            // this is a little strange since the opcode is actually 12-bits
            // instead of 8 bits. Only use the 8bits anyway
            auto opcode = getBaseOpcode();
            return opcode >= 0x58 && opcode < 0x80;
        }
        REGFormat _reg;
        COBRFormat _cobr;
        CTRLFormat _ctrl;
        MemFormat _mem;
        Ordinal _raw;

    } __attribute__((packed));
    static_assert(sizeof(Instruction) == sizeof(Ordinal), "Instruction must be the size of an ordinal!");
#ifdef PROTECTED_ARCHITECTURE
    // Virtual addressing 
    // 32-bit address is converted to 
    // upper 20 bits are translated to the physical address of a page (4k)
    //   upper 2 bits of the 20 bits is the region identifier
    //     Four regions total, region 3 (the upper most is shared amongst all
    //     processes)
    //   lower 18 bits denote the page in that region
    // lower 12 bits represent the address within that 4k page
    // 
    // There are a total of four regions with region 3 being shared amongst all
    // processes
    /**
     * Regions zero through two are process specific
     * Region three is shared by all processes
     */
    constexpr Ordinal Region0StartAddress = LowestAddress;
    constexpr Ordinal Region0LastAddress = 0x3FFF'FFFF;
    constexpr Ordinal Region1StartAddress = 0x4000'0000;
    constexpr Ordinal Region1LastAddress = 0x7FFF'FFFF;
    constexpr Ordinal Region2StartAddress = 0x8000'0000;
    constexpr Ordinal Region2LastAddress = 0xBFFF'FFFF;
    constexpr Ordinal Region3StartAddress = 0xC000'0000;
    constexpr Ordinal Region3LastAddress = LargestAddress;

    constexpr Ordinal getRegionId(Ordinal address) noexcept {
        return (0xC000'0000 & address) >> 30;
    }
    constexpr Ordinal getPageAddress(Ordinal address) noexcept {
        return (0x3FFF'F000 & address) >> 12;
    }
    constexpr Ordinal getByteOffset(Ordinal address) noexcept {
        return (0x0000'0FFF & address);
    }
#endif // end PROTECTED_ARCHITECTURE


    constexpr Ordinal widen(ByteOrdinal value) noexcept {
        return Ordinal(value);
    }
    constexpr Integer widen(ByteInteger value) noexcept {
        return Integer(value);
    }
    constexpr Ordinal computeNextFrameStart(Ordinal currentAddress) noexcept {
        // add 1 to the masked out value to make sure we don't overrun anything
        return (currentAddress & ~(Ordinal(0b11111))) + 1; // next 64 byte frame start
    }
    constexpr Ordinal computeStackFrameStart(Ordinal framePointerAddress) noexcept {
        return framePointerAddress + 64;
    }

    /**
     * Describes the intermediate processor state from a fault or interrupt 
     * occurring during processor execution
     */
    using ResumptionRecord = ByteOrdinal[16];
    struct FaultRecord {
        Ordinal reserved = 0;
        Ordinal overrideFaultData[3];
        Ordinal faultData[3];
        union {
            Ordinal value;
            struct {
                ByteOrdinal subtype;
                ByteOrdinal reserved;
                ByteOrdinal type;
                ByteOrdinal flags;
            };
        } override;
        ProcessControls pc;
        ArithmeticControls ac;
        union {
            Ordinal value;
            struct {
                ByteOrdinal subtype;
                ByteOrdinal reserved;
                ByteOrdinal type;
                ByteOrdinal flags;
            };
        } fault;
        Ordinal faultingInstructionAddr;
    } __attribute__((packed));

    struct InterruptRecord {
        ResumptionRecord record; // optional
        ProcessControls pc;
        ArithmeticControls ac;
        union {
            Ordinal value;
            ByteOrdinal number;
        } vector;
        // I see 
    } __attribute__((packed));

    /**
     * Also known as the PRCB, it is a data structure in memory which the cpu uses to track
     * various states
     */
    struct ProcessorControlBlock {
        Ordinal reserved0 = 0;
        Ordinal processorControls;
        Ordinal reserved1 = 0;
        Ordinal currentProcessSS;
        Ordinal dispatchPortSS;
        Ordinal interruptTablePhysicalAddress;
        Ordinal interruptStackPointer;
        Ordinal reserved2 = 0;
        Ordinal region3SS;
        Ordinal systemProcedureTableSS;
        Ordinal faultTablePhysicalAddress;
        Ordinal reserved3 = 0;
        Ordinal multiprocessorPreemption[3];
        Ordinal reserved4 = 0;
        Ordinal idleTime[2];
        Ordinal systemErrorFault;
        Ordinal reserved5;
        Ordinal resumptionRecord[12];
        Ordinal systemErrorFaultRecord[11];
    } __attribute__((packed));

} // end namespace i960
#endif // end I960_TYPES_H__
