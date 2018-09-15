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
    constexpr auto GlobalRegisterIndexStartsAt = 0b10000;
    constexpr auto LocalRegisterIndexStartsAt = 0b00000;
    constexpr auto FloatingPointIndexStartsAt = 0b00000;




    constexpr Ordinal LargestAddress = 0xFFFF'FFFF;
    constexpr Ordinal LowestAddress = 0;

    constexpr Ordinal ConditionCodeTrue = 0b010;
    constexpr Ordinal ConditionCodeFalse = 0b000;
    constexpr Ordinal ConditionCodeUnordered = 0b000;
    constexpr Ordinal ConditionCodeGreaterThan = 0b001;
    constexpr Ordinal ConditionCodeEqual = 0b010;
    constexpr Ordinal ConditionCodeLessThan = 0b100;
    constexpr Ordinal ConditionCodeOrdered = 0b111;
    constexpr Ordinal ConditionCodeNotEqual = 0b101;
    constexpr Ordinal ConditionCodeLessThanOrEqual = 0b110;
    constexpr Ordinal ConditionCodeGreaterThanOrEqual = 0b011;

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


    namespace IAC {
        struct Message {
            union {
                Ordinal _word0;
                struct {
                    Ordinal _field2 : 16;
                    Ordinal _field1 : 8;
                    Ordinal _MsgType : 8;
                };
            };
            Ordinal _field3;
            Ordinal _field4;
            Ordinal _field5;
        };
        static_assert(sizeof(Message) == 16, "Message is not of the correct size!");
        /**
         * The address that a processor can use to send itself IAC messages
         */
        constexpr Ordinal ProcessorLoopback = 0xFF000010;
        /**
         * Start of IAC memory space
         */
        constexpr Ordinal MessageSpaceStart = 0xFF000000;
        constexpr Ordinal MessageSpaceEnd = 0xFFFFFFF0;
        constexpr Ordinal ProcessorIdMask = 0b1111111111;
        constexpr Ordinal ProcessorPriorityMask = 0b11111;

        constexpr Ordinal computeProcessorAddress(Ordinal processorIdent, Ordinal priorityLevel) noexcept {
            // first mask the pieces
            constexpr Ordinal internalBitIdentifier = 0b0011000000 << 4;
            // lowest four bits are always zero
            auto maskedIdent = (ProcessorIdMask & processorIdent) << 14;
            auto maskedPriority = internalBitIdentifier | ((ProcessorPriorityMask & priorityLevel) << 4);
            return MessageSpaceStart | maskedIdent | maskedPriority;
        }

        enum MessageType {
            /**
             * Field1 is an interrupt vector. Generates or posts an interrupt.
             */
            Interrupt,
            /**
             * Tests for a pending interrupt
             */
            TestPendingInterrupts,
            /**
             * Field3 is an address. Stores two words beginning at this address;
             * the values are the addresses of the two initialization data
             * structures
             */
            StoreSystemBase,
            /**
             * Invalidates all entries in the instruction cache
             */
            PurgeInstructionCache,

            /**
             * Store into the special break point registers. Field3 and Field4 are
             * the values stored into the breakpoint registers. Bit 0 of the values
             * are ignored because instructions are word aligned. Bit 1 indicates
             * whether the breakpoint is enabled (0) or disabled (1).
             */
            SetBreakpointRegister,
            /**
             * Puts the processor into the stopped state
             */
            Freeze,
            /**
             * Puts the processor at step 2 of the initialization sequence.
             */
            ContinueInitialization,
            /**
             * Field3, field4, and field5 contain, respectively, the
             * addresses expected in memory locations 0, 4, and 12 during
             * initialization. Puts the processor at step 4 of the initialization
             * sequence, using these addresses instead of those from memory
             */
            ReinitializeProcessor,
        };

        union InterruptControlRegister {
            Ordinal _value;
            /**
             * The set of "pins" that the chip responds on
             * Int3's vector index must be a higher priority 2 which must be higher
             * than 1 which must be higher than 0.
             */
            struct {
                ByteOrdinal _int0Vector;
                ByteOrdinal _int1Vector;
                ByteOrdinal _int2Vector;
                ByteOrdinal _int3Vector;
            };
        };
        /**
         * Memory mapped location of the Interrupt control register. Can only be
         * read to and written from using synmov and synld instructions
         */
        constexpr Ordinal InterruptControlRegisterMappedAddress = 0xFF000004;
    } // end namespace IAC
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
            struct {
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
            } _mema;
            struct {
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
            } _memb;
            bool isMemAFormat() const noexcept {
                return _mema._unused == 0;
            }
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
    class Core {
        public:
            using SourceRegister = const NormalRegister&;
            using DestinationRegister = NormalRegister&;
            using LongSourceRegister = const DoubleRegister&;
            using LongDestinationRegister = DoubleRegister&;
            using RegisterWindow = NormalRegister[LocalRegisterCount];
            Core() = default;
            ~Core() = default;
            // TODO finish this once we have all the other sub components implemented behind the
            // scenes
            /** 
             * perform a call
             */
            virtual void call(Integer displacement);
            virtual Ordinal load(Ordinal address);
            virtual void store(Ordinal address, Ordinal value);

            void saveLocalRegisters() noexcept;
            void allocateNewLocalRegisterSet() noexcept;
            void setRegister(ByteOrdinal index, Integer value) noexcept;
            void setRegister(ByteOrdinal index, Ordinal value) noexcept;
            void setRegister(ByteOrdinal index, Real value) noexcept;
            void setRegister(ByteOrdinal index, SourceRegister other) noexcept;
            NormalRegister& getRegister(ByteOrdinal index) noexcept;
            Ordinal getStackPointerAddress() const noexcept;
            void setFramePointer(Ordinal value) noexcept;
            Ordinal getFramePointerAddress() const noexcept;
        private:
#define __DEFAULT_THREE_ARGS__ SourceRegister src1, SourceRegister src2, DestinationRegister dest
#define __DEFAULT_TWO_ARGS__ SourceRegister src, DestinationRegister dest
#define __DEFAULT_DOUBLE_WIDE_THREE_ARGS__ LongSourceRegister src1, LongSourceRegister src2, LongDestinationRegister dest
#define __DEFAULT_DOUBLE_WIDE_TWO_ARGS__ LongSourceRegister src, LongDestinationRegister dest
#define __GENERATE_DEFAULT_THREE_ARG_SIGS__(name) void name (__DEFAULT_THREE_ARGS__) noexcept

            void callx(SourceRegister value) noexcept;
            void calls(SourceRegister value);
            __GENERATE_DEFAULT_THREE_ARG_SIGS__(addc);
            __GENERATE_DEFAULT_THREE_ARG_SIGS__(addo);
            __GENERATE_DEFAULT_THREE_ARG_SIGS__(addi);
            void addr(__DEFAULT_THREE_ARGS__) noexcept;
            void addrl(__DEFAULT_DOUBLE_WIDE_THREE_ARGS__) noexcept;
            void chkbit(SourceRegister pos, SourceRegister src) noexcept;
            __GENERATE_DEFAULT_THREE_ARG_SIGS__(alterbit);
            __GENERATE_DEFAULT_THREE_ARG_SIGS__(andOp);
            __GENERATE_DEFAULT_THREE_ARG_SIGS__(andnot);
            void atadd(__DEFAULT_THREE_ARGS__) noexcept; // TODO add other forms of atadd
            void atan(__DEFAULT_THREE_ARGS__) noexcept;
            void atanrl(__DEFAULT_DOUBLE_WIDE_THREE_ARGS__) noexcept;
            void atmod(SourceRegister src, SourceRegister mask, DestinationRegister srcDest) noexcept; // TODO check out other forms of this instruction
            void b(Integer displacement) noexcept;
            void bx(SourceRegister targ) noexcept; // TODO check these two instructions out for more variants
            void bal(Integer displacement) noexcept;
            void balx(__DEFAULT_TWO_ARGS__) noexcept; // TODO check these two instructions out for more variants
            void bbc(SourceRegister pos, SourceRegister src, Integer targ) noexcept; 
            void bbs(SourceRegister pos, SourceRegister src, Integer targ) noexcept;
            void be(Integer displacement) noexcept;
            void bne(Integer displacement) noexcept;
            void bl(Integer displacement) noexcept;
            void ble(Integer displacement) noexcept;
            void bg(Integer displacement) noexcept;
            void bge(Integer displacement) noexcept;
            void bo(Integer displacement) noexcept;
            void bno(Integer displacement) noexcept;
            void classr(SourceRegister src) noexcept;
            void classrl(SourceRegister srcLower, SourceRegister srcUpper) noexcept;
            void clrbit(SourceRegister pos, SourceRegister src, DestinationRegister dest) noexcept; // TODO look into the various forms further
            void cmpi(SourceRegister src1, SourceRegister src2) noexcept;
            void cmpo(SourceRegister src1, SourceRegister src2) noexcept;
            __GENERATE_DEFAULT_THREE_ARG_SIGS__(cmpdeci);
            __GENERATE_DEFAULT_THREE_ARG_SIGS__(cmpdeco);
            __GENERATE_DEFAULT_THREE_ARG_SIGS__(cmpinci);
            __GENERATE_DEFAULT_THREE_ARG_SIGS__(cmpinco);
            void cmpor(SourceRegister src1, SourceRegister src2) noexcept;
            void cmporl(SourceRegister src1Lower, SourceRegister src1Upper, SourceRegister src2Lower, SourceRegister src2Upper) noexcept;
            void cmpr(SourceRegister src1, SourceRegister src2) noexcept;
            void cmpstr(SourceRegister src1, SourceRegister src2, SourceRegister len) noexcept;
            // compare and branch instructions
            void cmpibe(SourceRegister src1, SourceRegister src2, Integer targ) noexcept;
            void cmpibne(SourceRegister src1, SourceRegister src2, Integer targ) noexcept;
            void cmpibl(SourceRegister src1, SourceRegister src2, Integer targ) noexcept;
            void cmpible(SourceRegister src1, SourceRegister src2, Integer targ) noexcept;
            void cmpibg(SourceRegister src1, SourceRegister src2, Integer targ) noexcept;
            void cmpibge(SourceRegister src1, SourceRegister src2, Integer targ) noexcept;
            void cmpibo(SourceRegister src1, SourceRegister src2, Integer targ) noexcept;
            void cmpibno(SourceRegister src1, SourceRegister src2, Integer targ) noexcept;
            void cmpobe(SourceRegister src1, SourceRegister src2, Integer targ) noexcept;
            void cmpobne(SourceRegister src1, SourceRegister src2, Integer targ) noexcept;
            void cmpobl(SourceRegister src1, SourceRegister src2, Integer targ) noexcept;
            void cmpoble(SourceRegister src1, SourceRegister src2, Integer targ) noexcept;
            void cmpobg(SourceRegister src1, SourceRegister src2, Integer targ) noexcept;
            void cmpobge(SourceRegister src1, SourceRegister src2, Integer targ) noexcept;
            void concompi(SourceRegister src1, SourceRegister src2) noexcept;
            void concompo(SourceRegister src1, SourceRegister src2) noexcept;
            void condrec(SourceRegister src, DestinationRegister dest) noexcept;
            void condwait(SourceRegister src) noexcept;
            void cosr(__DEFAULT_TWO_ARGS__) noexcept;
            void cosrl(__DEFAULT_DOUBLE_WIDE_TWO_ARGS__) noexcept;
            void cpysre(__DEFAULT_THREE_ARGS__) noexcept; // TODO fix the signature of this function
            void cpyrsre(__DEFAULT_THREE_ARGS__) noexcept; // TODO fix the signature of this function
            void cvtilr(LongSourceRegister src, ExtendedReal& dest) noexcept;
            void cvtir(SourceRegister src, ExtendedReal& dest) noexcept;
            void cvtri(__DEFAULT_TWO_ARGS__) noexcept; // TODO fix this function as it deals with floating point registers
            void cvtril(SourceRegister src, LongDestinationRegister dest) noexcept; // TODO fix this function as it deals with floating point registers
            void cvtzri(__DEFAULT_TWO_ARGS__) noexcept; // TODO fix this function as it deals with floating point registers
            void cvtzril(SourceRegister src, LongDestinationRegister dest) noexcept; // TODO fix this function as it deals with floating point registers
            __GENERATE_DEFAULT_THREE_ARG_SIGS__(daddc);
            __GENERATE_DEFAULT_THREE_ARG_SIGS__(divo);
            __GENERATE_DEFAULT_THREE_ARG_SIGS__(divi);
            void divr(__DEFAULT_THREE_ARGS__) noexcept; // TODO divr and divrl do not support extended registers yet
            void divrl(__DEFAULT_DOUBLE_WIDE_THREE_ARGS__) noexcept;
            void dmovt(SourceRegister src, DestinationRegister dest) noexcept;
            __GENERATE_DEFAULT_THREE_ARG_SIGS__(dsubc);
            void ediv(SourceRegister src1, LongSourceRegister src2, DestinationRegister remainder, DestinationRegister quotient) noexcept;
            void emul(SourceRegister src1, SourceRegister src2, LongDestinationRegister dest) noexcept;
            void expr(__DEFAULT_TWO_ARGS__) noexcept;
            void exprl(LongSourceRegister src, LongDestinationRegister dest) noexcept;
            __GENERATE_DEFAULT_THREE_ARG_SIGS__(extract);
            void faulte() noexcept;
            void faultne() noexcept;
            void faultl() noexcept;
            void faultle() noexcept;
            void faultg() noexcept;
            void faultge() noexcept;
            void faulto() noexcept;
            void faultno() noexcept;
            void fill(SourceRegister dst, SourceRegister value, SourceRegister len) noexcept;
            void flushreg() noexcept;
            void fmark() noexcept;
            void inspacc(__DEFAULT_TWO_ARGS__) noexcept;
            void ld(__DEFAULT_TWO_ARGS__) noexcept;
            void ldob(__DEFAULT_TWO_ARGS__) noexcept;
            void ldos(__DEFAULT_TWO_ARGS__) noexcept;
            void ldib(__DEFAULT_TWO_ARGS__) noexcept;
            void ldis(__DEFAULT_TWO_ARGS__) noexcept;
            void ldl(SourceRegister src, LongDestinationRegister dest) noexcept;
            void ldt(SourceRegister src, TripleRegister& dest) noexcept;
            void ldq(SourceRegister src, QuadRegister& dest) noexcept;
            void lda(__DEFAULT_TWO_ARGS__) noexcept;
            void ldphy(__DEFAULT_TWO_ARGS__) noexcept;
            void ldtime(DestinationRegister dest) noexcept;
            void logbnr(__DEFAULT_TWO_ARGS__) noexcept;
            void logbnrl(LongSourceRegister src, LongDestinationRegister dest) noexcept;
            void logepr(__DEFAULT_THREE_ARGS__) noexcept;
            void logeprl(__DEFAULT_DOUBLE_WIDE_THREE_ARGS__) noexcept;
            void logr(__DEFAULT_THREE_ARGS__) noexcept;
            void logrl(__DEFAULT_DOUBLE_WIDE_THREE_ARGS__) noexcept;
            void mark() noexcept;
            __GENERATE_DEFAULT_THREE_ARG_SIGS__(modifyac);
            __GENERATE_DEFAULT_THREE_ARG_SIGS__(modi);
            __GENERATE_DEFAULT_THREE_ARG_SIGS__(modify);
            __GENERATE_DEFAULT_THREE_ARG_SIGS__(modpc);
            __GENERATE_DEFAULT_THREE_ARG_SIGS__(modtc);
            void mov(__DEFAULT_TWO_ARGS__) noexcept;
            void movl(LongSourceRegister src, LongDestinationRegister dest) noexcept;
            void movt(const TripleRegister& src, TripleRegister& dest) noexcept;
            void movq(const QuadRegister& src, QuadRegister& dest) noexcept;
            void movqstr(SourceRegister dst, SourceRegister src, SourceRegister len) noexcept;
            void movr(__DEFAULT_TWO_ARGS__) noexcept;
            void movrl(LongSourceRegister src, LongDestinationRegister dest) noexcept;
            void movre(const TripleRegister& src, TripleRegister& dest) noexcept;
            void movstr(SourceRegister dst, SourceRegister src, SourceRegister len) noexcept;
            __GENERATE_DEFAULT_THREE_ARG_SIGS__(mulo);
            __GENERATE_DEFAULT_THREE_ARG_SIGS__(muli);
            void mulr(__DEFAULT_THREE_ARGS__) noexcept;
            void mulrl(__DEFAULT_DOUBLE_WIDE_THREE_ARGS__) noexcept;
            __GENERATE_DEFAULT_THREE_ARG_SIGS__(nand);
            __GENERATE_DEFAULT_THREE_ARG_SIGS__(nor);
            void notOp(__DEFAULT_TWO_ARGS__) noexcept;
            __GENERATE_DEFAULT_THREE_ARG_SIGS__(notand);
            __GENERATE_DEFAULT_THREE_ARG_SIGS__(notbit);
            __GENERATE_DEFAULT_THREE_ARG_SIGS__(notor);
            __GENERATE_DEFAULT_THREE_ARG_SIGS__(orOp);
            __GENERATE_DEFAULT_THREE_ARG_SIGS__(ornot);
            void receive(__DEFAULT_TWO_ARGS__) noexcept;
            __GENERATE_DEFAULT_THREE_ARG_SIGS__(remo);
            __GENERATE_DEFAULT_THREE_ARG_SIGS__(remi);
            __GENERATE_DEFAULT_THREE_ARG_SIGS__(remr);
            void remrl(__DEFAULT_DOUBLE_WIDE_THREE_ARGS__) noexcept;
            void resumeprcs(SourceRegister src) noexcept;
            void ret() noexcept;
            void rotate(__DEFAULT_THREE_ARGS__) noexcept;
            void roundr(__DEFAULT_TWO_ARGS__) noexcept;
            void roundrl(LongSourceRegister src, LongDestinationRegister dest) noexcept;
            void saveprcs() noexcept;
            void scaler(__DEFAULT_THREE_ARGS__) noexcept;
            void scalerl(LongSourceRegister src1, LongSourceRegister src2, LongDestinationRegister dest) noexcept;
            void scanbit(__DEFAULT_TWO_ARGS__) noexcept;
            void scanbyte(SourceRegister src1, SourceRegister src2) noexcept;
            void schedprcs(SourceRegister src) noexcept;
            void send(SourceRegister dest, SourceRegister src1, SourceRegister src2) noexcept;
            void sendserv(SourceRegister src) noexcept;
            void setbit(__DEFAULT_THREE_ARGS__) noexcept;
            void shlo(__DEFAULT_THREE_ARGS__) noexcept;
            void shro(__DEFAULT_THREE_ARGS__) noexcept;
            void shli(__DEFAULT_THREE_ARGS__) noexcept;
            void shri(__DEFAULT_THREE_ARGS__) noexcept;
            void shrdi(__DEFAULT_THREE_ARGS__) noexcept;
            void signal(SourceRegister src) noexcept;
            void sinr(__DEFAULT_TWO_ARGS__) noexcept; 
            void sinrl(__DEFAULT_DOUBLE_WIDE_TWO_ARGS__) noexcept; 
            void spanbit(__DEFAULT_TWO_ARGS__) noexcept;
            void sqrtr(__DEFAULT_TWO_ARGS__) noexcept;
            void sqrtrl(LongSourceRegister src, LongDestinationRegister dest) noexcept;
            void st(__DEFAULT_TWO_ARGS__) noexcept;
            void stob(__DEFAULT_TWO_ARGS__) noexcept;
            void stos(__DEFAULT_TWO_ARGS__) noexcept;
            void stib(__DEFAULT_TWO_ARGS__) noexcept;
            void stis(__DEFAULT_TWO_ARGS__) noexcept;
            void stl(SourceRegister src, LongDestinationRegister dest) noexcept;
            void stt(SourceRegister src, TripleRegister& dest) noexcept;
            void stq(SourceRegister src, QuadRegister& dest) noexcept;
            __GENERATE_DEFAULT_THREE_ARG_SIGS__(subc); 
            __GENERATE_DEFAULT_THREE_ARG_SIGS__(subo);
            __GENERATE_DEFAULT_THREE_ARG_SIGS__(subi);
            void subr(__DEFAULT_THREE_ARGS__) noexcept;
            void subrl(LongSourceRegister src1, LongSourceRegister src2, LongDestinationRegister dest) noexcept;
            void syncf() noexcept;
            void synld(__DEFAULT_TWO_ARGS__) noexcept;
            void synmov(__DEFAULT_TWO_ARGS__) noexcept;
            void synmovl(__DEFAULT_TWO_ARGS__) noexcept;
            void synmovq(__DEFAULT_TWO_ARGS__) noexcept;
            void tanr(__DEFAULT_TWO_ARGS__) noexcept;
            void tanrl(LongSourceRegister src, LongDestinationRegister dest) noexcept;
            void teste(DestinationRegister dest) noexcept;
            void testne(DestinationRegister dest) noexcept;
            void testl(DestinationRegister dest) noexcept;
            void testle(DestinationRegister dest) noexcept;
            void testg(DestinationRegister dest) noexcept;
            void testge(DestinationRegister dest) noexcept;
            void testo(DestinationRegister dest) noexcept;
            void testno(DestinationRegister dest) noexcept;
            void wait(SourceRegister src) noexcept;
            __GENERATE_DEFAULT_THREE_ARG_SIGS__(xnor);
            __GENERATE_DEFAULT_THREE_ARG_SIGS__(xorOp);

#undef __GENERATE_DEFAULT_THREE_ARG_SIGS__
#undef __DEFAULT_THREE_ARGS__
#undef __DEFAULT_DOUBLE_WIDE_THREE_ARGS__
#undef __DEFAULT_TWO_ARGS__
#undef __DEFAULT_DOUBLE_WIDE_TWO_ARGS__
        private:
            RegisterWindow _globalRegisters;
            // The hardware implementations use register sets, however
            // to start with, we should just follow the logic as is and 
            // just save the contents of the registers to the stack the logic
            // is always sound to do it this way
#warning "No register window sets implemented at this point in time"
            RegisterWindow _localRegisters;
            ArithmeticControls _ac;
            Ordinal _instructionPointer;
            ProcessControls _pc;
            TraceControls _tc;
            NormalRegister _sfr[32]; // not implemented in the documentation I have
            ExtendedReal _floatingPointRegisters[NumFloatingPointRegs];
            NormalRegister _internalRegisters[8]; // for internal conversion purposes to make decoding regular
    };

} // end namespace i960
#endif // end I960_TYPES_H__
