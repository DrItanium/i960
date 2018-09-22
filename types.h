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
#define MustBeSizeOfOrdinal(type, message) \
    static_assert(sizeof(type) == sizeof(Ordinal), message)

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
        Real() : Real(0,0,0) { }
        Real(Ordinal frac, Ordinal exponent, Ordinal flag) : _fraction(frac), _exponent(exponent), _flag(flag) { };
        explicit Real(RawReal value) : _floating(value) { }
        union {
            struct {
                Ordinal _fraction : 23;
                Ordinal _exponent : 8;
                Ordinal _flag : 1;
            };
            Ordinal _bits;
            RawReal _floating;
        };
    } __attribute__((packed));
    /**
     * Part of the numerics architecture and above
     */
    struct LongReal {
        LongReal() : LongReal(0,0) { }
        LongReal(Ordinal lower, Ordinal upper) : _bits(LongOrdinal(lower) | (LongOrdinal(upper) << 32)) { }
        LongReal(LongOrdinal frac, LongOrdinal exponent, LongOrdinal sign) : _fraction(frac), _exponent(exponent), _sign(sign) { }
        explicit LongReal(RawLongReal value) : _floating(value) { }
        Ordinal lowerHalf() const noexcept { return static_cast<Ordinal>(_bits & 0xFFFF'FFFF); }
        Ordinal upperHalf() const noexcept { return static_cast<Ordinal>((_bits & 0xFFFF'FFFF'0000'0000) >> 32); }
        union {
            struct {
                LongOrdinal _fraction : 52;
                LongOrdinal _exponent : 11;
                LongOrdinal _sign : 1;
            };
            LongOrdinal _bits;
            RawLongReal _floating;
        };
    } __attribute__((packed));
    constexpr LongOrdinal makeLongOrdinal(Ordinal lower, Ordinal upper) noexcept {
        return LongOrdinal(lower) | (LongOrdinal(upper) << 32);
    }
    /**
     * Part of the numerics architecture and above
     */
    union ExtendedReal {
        ExtendedReal() { }
        ExtendedReal(Ordinal low, Ordinal mid, Ordinal high) : _lower(makeLongOrdinal(low, mid)), _upper(high) { }
        explicit ExtendedReal(RawExtendedReal value) : _floating(value) { }
        Ordinal lowerThird() const noexcept { return static_cast<Ordinal>(_lower); }
        Ordinal middleThird() const noexcept { return static_cast<Ordinal>(_lower >> 32); }
        Ordinal upperThird() const noexcept { return static_cast<Ordinal>(_upper); }

        struct {
            LongOrdinal _fraction: 63;
            LongOrdinal _j : 1;
            ShortOrdinal _exponent : 15;
            ShortOrdinal _sign : 1;
        }; 
        struct {
            LongOrdinal _lower;
            ShortOrdinal _upper;
        };
        RawExtendedReal _floating;
    } __attribute__((packed));

    union PreviousFramePointer {
        struct {
            Ordinal _returnCode : 3;
            Ordinal _prereturnTrace : 1;
            Ordinal _unused : 2; // 80960 ignores the lower six bits of this register
            Ordinal _address : 26;
        };
        Ordinal _value;
    } __attribute__((packed));
    union NormalRegister {
        private:
            template<typename T>
                static constexpr bool LegalConversion = false;
        public:
            NormalRegister(Ordinal value) : _ordinal(value) { }
            NormalRegister() : NormalRegister(0u) { }
            ~NormalRegister() { _ordinal = 0; }

            PreviousFramePointer _pfp;
            Ordinal _ordinal;
            Integer _integer;
            Real _real;
            ByteOrdinal _byteOrd;
            ShortOrdinal _shortOrd;

            template<typename T>
                T get() const noexcept {
                    using K = std::decay_t<T>;
                    if constexpr(std::is_same_v<K, Ordinal>) {
                        return _ordinal;
                    } else if constexpr(std::is_same_v<K, Integer>) {
                        return _integer;
                    } else if constexpr(std::is_same_v<K, ByteOrdinal>) {
                        return _byteOrd;
                    } else if constexpr(std::is_same_v<K, ShortOrdinal>) {
                        return _shortOrd;
                    } else if constexpr(std::is_same_v<K, Real>) {
                        return _real;
                    } else if constexpr(std::is_same_v<K, RawReal>) {
                        return _real._floating;
                    } else if constexpr(std::is_same_v<K, PreviousFramePointer>) {
                        return _pfp;
                    } else if constexpr(std::is_same_v<K, LongOrdinal>) {
                        return static_cast<LongOrdinal>(_ordinal);
                    } else {
                        static_assert(LegalConversion<K>, "Illegal type requested");
                    }
                }
            template<typename T>
                void set(T value) noexcept {
                    using K = std::decay_t<T>;
                    if constexpr(std::is_same_v<K, Ordinal>) {
                        _ordinal = value;
                    } else if constexpr(std::is_same_v<K, Integer>) {
                        _integer = value;
                    } else if constexpr(std::is_same_v<K, ByteOrdinal>) {
                        _byteOrd = value;
                    } else if constexpr(std::is_same_v<K, ShortOrdinal>) {
                        _shortOrd = value;
                    } else if constexpr(std::is_same_v<K, Real>) {
                        _real._floating = value._floating;
                    } else if constexpr(std::is_same_v<K, RawReal>) {
                        _real._floating = value;
                    } else if constexpr(std::is_same_v<K, PreviousFramePointer>) {
                        _ordinal = value._value;
                    } else {
                        static_assert(LegalConversion<K>, "Illegal type requested");
                    }
                }
            void move(const NormalRegister& other) noexcept { set<Ordinal>(other.get<Ordinal>()); }
    };
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
                        return LongOrdinal(_lower._ordinal) | (LongOrdinal(_upper._ordinal) << 32);
                    } else if constexpr(std::is_same_v<K, LongReal>) {
                        return LongReal(_lower._ordinal, _upper._ordinal);
                    } else if constexpr (std::is_same_v<K, RawLongReal>) {
                        return get<LongReal>()._floating;
                    } else {
                        static_assert(LegalConversion<K>, "Illegal type requested");
                    }
                }
            template<typename T>
                void set(T value) noexcept {
                    using K = std::decay_t<T>;
                    if constexpr(std::is_same_v<K, LongOrdinal>) {
                        _lower._ordinal = static_cast<Ordinal>(value);
                        _upper._ordinal = static_cast<Ordinal>(value >> 32);
                    } else if constexpr (std::is_same_v<K, LongReal>) {
                        _lower._ordinal = value.lowerHalf();
                        _upper._ordinal = value.upperHalf();
                    } else if constexpr (std::is_same_v<K, RawLongReal>) {
                        set<LongReal>(LongReal(value));
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
                        return ExtendedReal(_lower._ordinal, _mid._ordinal, _upper._ordinal);
                    } else if constexpr (std::is_same_v<K, RawExtendedReal>) {
                        return get<ExtendedReal>()._floating;
                    } else {
                        static_assert(LegalConversion<K>, "Illegal type requested");
                    }
                }
            template<typename T>
                void set(T value) noexcept {
                    using K = std::decay_t<T>;
                    if constexpr (std::is_same_v<K, ExtendedReal>) {
                        _lower._ordinal = value.lowerThird();
                        _mid._ordinal = value.middleThird();
                        _upper._ordinal = value.upperThird();
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
    MustBeSizeOfOrdinal(NormalRegister, "NormalRegister must be 32-bits wide!");
    union ArithmeticControls {
        struct {
            Ordinal _conditionCode : 3;
            /**
             * Used to record results from the classify real (classr and classrl)
             * and remainder real (remr and remrl) instructions. 
             */
            Ordinal _arithmeticStatusField : 4;
            /**
             * Reserved, bind to zero always
             */
            Ordinal _reserved0 : 1; 
            /**
             * Denotes an integer overflow happened
             */
            Ordinal _integerOverflowFlag : 1;
            /**
             * Reserved, bind to zero always
             */
            Ordinal _reserved1 : 3;
            /**
             * Inhibit the processor from invoking a fault handler
             * when integer overflow is detected.
             */
            Ordinal _integerOverflowMask : 1;
            /**
             * Reserved, always bind to zero
             */
            Ordinal _reserved2 : 2;
            /**
             * Disable faults generated by imprecise results being generated
             */
            Ordinal _noImpreciseResults : 1;
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
            /**
             * Reserved, always bind to zero
             */
            Ordinal _reserved3 : 3;
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
        Ordinal modify(Ordinal mask, Ordinal value) noexcept {
            if (mask == 0) {
                return _value;
            } else {
                auto tmp = _value;
                _value = (value & mask) | (_value & ~(mask));
                return tmp;
            }
        }

        // NOTE that both of these methods could return true in some cases
        // I think that the safest solution is to actually raise a fault if both
        // are true or both are false.

        bool conditionIsTrue() const noexcept {
            return _conditionCode == 0b010;
        }
        bool conditionIsFalse() const noexcept {
            return _conditionCode == 0b000;
        }
        bool conditionIsUnordered() const noexcept {
            return _conditionCode == 0b000;
        }
        bool conditionIsGreaterThan() const noexcept {
            return _conditionCode == 0b001;
        }
        bool conditionIsEqual() const noexcept {
            return _conditionCode == 0b010;
        }
        bool conditionIsGreaterThanOrEqual() const noexcept {
            return _conditionCode == 0b011;
        }
        bool conditionIsLessThan() const noexcept {
            return _conditionCode == 0b100;
        }
        bool conditionIsNotEqual() const noexcept {
            return _conditionCode == 0b101;
        }
        bool conditionIsLessThanOrEqual() const noexcept {
            return _conditionCode == 0b110;
        }
        bool conditionIsOrdered() const noexcept {
            return _conditionCode == 0b111;
        }
        Ordinal getConditionCode() const noexcept {
            return _conditionCode;
        }
        bool roundToNearest() const noexcept {
            return _roundingControl == 0b00;
        }
        bool roundDown() const noexcept {
            return _roundingControl == 0b01;
        }
        bool roundUp() const noexcept {
            return _roundingControl == 0b10;
        }
        bool roundTowardsZero() const noexcept {
            return _roundingControl == 0b11;
        }
        bool arithmeticStatusIsZero() const noexcept {
            return (_arithmeticStatusField & 0b0111) == 0;
        }
        bool arithmeticStatusIsDenormalizedNumber() const noexcept {
            return (_arithmeticStatusField & 0b0111) == 1;
        }
        bool arithmeticStatusIsNormalFiniteNumber() const noexcept {
            return (_arithmeticStatusField & 0b0111) == 2;
        }
        bool arithmeticStatusIsInfinity() const noexcept {
            return (_arithmeticStatusField & 0b0111) == 3;
        }
        bool arithmeticStatusIsQuietNaN() const noexcept {
            return (_arithmeticStatusField & 0b0111) == 4;
        }
        bool arithmeticStatusIsSignalingNaN() const noexcept {
            return (_arithmeticStatusField & 0b0111) == 5;
        }
        bool arithmeticStatusIsReservedOperand() const noexcept {
            return (_arithmeticStatusField & 0b0111) == 6;
        }
        bool getArithmeticStatusSign() const noexcept {
            return ((_arithmeticStatusField & 0b1000) >> 3) == 1;
        }
        bool carrySet() const noexcept {
            return (_conditionCode & 0b010) != 0;
        }
        Ordinal getCarryValue() const noexcept {
            return carrySet() ? 1 : 0;
        }


    };
    using InstructionPointer = Ordinal;
    union ProcessControls {
#ifdef PROTECTED_ARCHITECTURE
        struct {
            Ordinal _unused0 : 1;
            Ordinal _multiprocessorPreempt : 1;
            Ordinal _state : 2;
            Ordinal _unused1 : 1;
            Ordinal _nonpreemptLimit : 5;
            Ordinal _addressingMode : 1;
            Ordinal _checkDispatchPort : 1;
            Ordinal _unused2 : 4;
            Ordinal _interimPriority : 5;
            Ordinal _unused3 : 10;
            Ordinal _writeExternalPriority : 1;
        } _manual;
        struct {
            Ordinal _traceEnable : 1;
            Ordinal _executionMode : 1;
            Ordinal _unused0 : 4;
            Ordinal _timeSliceReschedule : 1;
            Ordinal _timeSlice : 1;
            Ordinal _resume : 1;
            Ordinal _traceFaultPending : 1;
            Ordinal _preempt : 1;
            Ordinal _refault : 1;
            Ordinal _state : 2;
            Ordinal _unused1 : 1;
            Ordinal _priority : 5;
            Ordinal _internalState : 11;
        }  _automatic;
#else // !defined(PROTECTED_ARCHITECTURE)
        struct {
            Ordinal _traceEnable : 1;
            Ordinal _executionMode : 1;
            Ordinal _unused0 : 7;
            Ordinal _resume : 1;
            Ordinal _traceFaultPending : 1;
            Ordinal _unused1 : 2;
            Ordinal _state : 1;
            Ordinal _unused2 : 2;
            Ordinal _priority : 5;
            Ordinal _internalState : 11;
        };
#endif // end PROTECTED_ARCHITECTURE

        Ordinal _value;
    } __attribute__((packed));


    MustBeSizeOfOrdinal(ProcessControls, "ProcessControls is not the size of an ordinal!");
    union TraceControls {
        struct {
            Ordinal _unused0 : 1;
            Ordinal _instructionTraceMode : 1;
            Ordinal _branchTraceMode : 1;
            Ordinal _returnTraceMode : 1;
            Ordinal _prereturnTraceMode : 1;
            Ordinal _supervisorTraceMode : 1;
            Ordinal _breakPointTraceMode : 1;
            Ordinal _unused1 : 9;
            Ordinal _instructionTraceEvent : 1;
            Ordinal _branchTraceEvent : 1;
            Ordinal _callTraceEvent : 1;
            Ordinal _returnTraceEvent : 1;
            Ordinal _prereturnTraceEvent: 1;
            Ordinal _supervisorTraceEvent : 1;
            Ordinal _breakpointTraceEvent : 1;
            Ordinal _unused2 : 8;
        };
        Ordinal _value;
    } __attribute__((packed));

    MustBeSizeOfOrdinal(TraceControls, "TraceControls must be the size of an ordinal!");

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

        MustBeSizeOfOrdinal(MemFormat, "MemFormat must be the size of an ordinal!");

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
    MustBeSizeOfOrdinal(Instruction, "Instruction must be the size of an ordinal!");
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

} // end namespace i960
#endif // end I960_TYPES_H__
