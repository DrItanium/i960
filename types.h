#ifndef I960_TYPES_H__
#define I960_TYPES_H__
#include <cstdint>
#include <memory>
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
            ByteOrdinal byteOrd;
            ShortOrdinal shortOrd;
			ByteInteger byteInt;
			ShortInteger shortInt;

            template<typename T>
            T get() const noexcept {
                using K = std::decay_t<T>;
                if constexpr(std::is_same_v<K, Ordinal>) {
                    return ordinal;
                } else if constexpr(std::is_same_v<K, Integer>) {
                    return integer;
                } else if constexpr(std::is_same_v<K, ByteOrdinal>) {
                    return byteOrd;
				} else if constexpr(std::is_same_v<K, ByteInteger>) {
					return byteInt;
                } else if constexpr(std::is_same_v<K, ShortOrdinal>) {
                    return shortOrd;
				} else if constexpr(std::is_same_v<K, ShortInteger>) {
					return shortInt;
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
				} else if constexpr(std::is_same_v<K, ByteInteger>) {
					byteInt = value;
				} else if constexpr(std::is_same_v<K, ShortInteger>) {
					shortInt = value;
                } else if constexpr(std::is_same_v<K, PreviousFramePointer>) {
                    ordinal = value.value;
                } else {
                    static_assert(LegalConversion<K>, "Illegal type requested");
                }
            }
            void move(const NormalRegister& other) noexcept { set<Ordinal>(other.get<Ordinal>()); }
			ByteOrdinal mostSignificantBit() const noexcept {
				return (ordinal & 0x80000000);
			}
			bool mostSignificantBitSet() const noexcept {
				return mostSignificantBit() == 1;
			}
			bool mostSignificantBitClear() const noexcept {
				return mostSignificantBit() == 0;
			}
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
			Ordinal reserved0 : 5;
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
            Ordinal noImpreciseFaults : 1;
			Ordinal reserved3 : 16;
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

		template<Ordinal mask>
		bool conditionCodeIs() const noexcept {
			return conditionCode == mask;
		}
		template<Ordinal mask>
		bool conditionCodeBitSet() const noexcept {
			return (conditionCode & mask) != 0;
		}
#define X(title, mask) \
		bool conditionIs ## title () const noexcept { \
			return conditionCodeIs<0b010>(); \
		}
		X(True, 0b010);
		X(False, 0b000);
		X(Unordered, 0b000);
		X(Greater, 0b001);
		X(Equal, 0b010);
		X(GreaterOrEqual, 0b011);
		X(Less, 0b100);
		X(NotEqual, 0b101);
		X(LessOrEqual, 0b110);
		X(Ordered, 0b111);
#undef X
		bool shouldCarryOut() const noexcept {
			// 0b01X where X is don't care
			return conditionCode == 0b010 || conditionCode == 0b011;
		}
		bool markedAsOverflow() const noexcept {
			// 0b0X1 where X is don't care
			return conditionCode == 0b001 || conditionCode == 0b011;
		}
        Ordinal getConditionCode() const noexcept {
            return conditionCode;
        }
        bool carrySet() const noexcept {
            return (conditionCode & 0b010) != 0;
        }
        Ordinal getCarryValue() const noexcept {
            return carrySet() ? 1 : 0;
        }


    };
    union ProcessControls {
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
            Ordinal unused3 : 11;
        };
        Ordinal value;
		bool traceEnabled() const noexcept {
			return traceEnable != 0;
		}
		bool inUserMode() const noexcept {
			return executionMode == 0;
		}
		bool inSupervisorMode() const noexcept {
			return executionMode != 0;
		}
		bool traceFaultIsPending() const noexcept {
			return traceFaultPending != 0;
		}
		bool traceFaultIsNotPending() const noexcept {
			return traceFaultPending == 0;
		}
		bool isExecuting() const noexcept {
			return state == 0;
		}
		bool isInterrupted() const noexcept {
			return state != 0;
		}
		Ordinal getProcessPriority() const noexcept {
			return priority;
		}
    } __attribute__((packed));


	static_assert(sizeof(ProcessControls) == sizeof(Ordinal), "ProcessControls is not the size of an ordinal!");
    union TraceControls {
        struct {
            Ordinal unused0 : 1;
			// trace mode bits
            Ordinal instructionTraceMode : 1;
            Ordinal branchTraceMode : 1;
			Ordinal callTraceMode : 1;
            Ordinal returnTraceMode : 1;
            Ordinal prereturnTraceMode : 1;
            Ordinal supervisorTraceMode : 1;
            Ordinal markTraceMode : 1;
            Ordinal unused1 : 16;
			// hardware breakpoint event flags
			Ordinal instructionAddressBreakpoint0 : 1;
			Ordinal instructionAddressBreakpoint1 : 1;
			Ordinal dataAddressBreakpoint0 : 1;
			Ordinal dataAddressBreakpoint1 : 1;
            Ordinal unused2 : 4;
        };
        Ordinal value;
		bool traceMarked() const noexcept {
			return markTraceMode != 0;
		}
    } __attribute__((packed));
	static_assert(sizeof(TraceControls) == sizeof(Ordinal), "TraceControls must be the size of an ordinal!");

    constexpr auto GlobalRegisterCount = 16;
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
        Greater = 0b001,
        Equal = 0b010,
        Less = 0b100,
        Ordered = 0b111,
        NotEqual = 0b101,
        LessOrEqual = 0b110,
        GreaterOrEqual = 0b011,
        Unordered = False,
        True = Equal,
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
	 * Describes a register or literal in a single type. Internally
	 * the operand is a 6 bit number where bits [0,4] represent the value
	 * with the uppermost bit (bit 5) denoting if it is a literal or register
	 * reference.
	 */
	struct Operand final {
		public:
			static constexpr Ordinal encodingMask = 0b111111; 
			static constexpr Ordinal typeMask = 0b100000;
			static constexpr Ordinal valueMask = 0b011111;
			constexpr Operand(Ordinal rawValue) : _raw(rawValue & encodingMask) { }
			constexpr bool isLiteral() const noexcept { return (_raw & typeMask) != 0; }
			constexpr bool isRegister() const noexcept { return (_raw & typeMask) == 0; }
			constexpr Ordinal getValue() const noexcept { return (_raw & valueMask); }
		private:
			Ordinal _raw;
	};
	constexpr Operand operator"" _lit(unsigned long long n) {
		return Operand((Operand::valueMask & Ordinal(n) + Operand::typeMask));
	}
	constexpr Operand operator"" _greg(unsigned long long n) {
		return Operand((n & 0b1111) + 0b10000);
	}
	constexpr Operand operator"" _lreg(unsigned long long n) {
		return Operand((n & 0b1111));
	}
	constexpr Operand pfp = 0_lreg;
	constexpr Operand sp = 1_lreg;
	constexpr Operand rip = 2_lreg;
	constexpr Operand r3 = 3_lreg;
	constexpr Operand r4 = 4_lreg;
	constexpr Operand r5 = 5_lreg;
	constexpr Operand r6 = 6_lreg;
	constexpr Operand r7 = 7_lreg;
	constexpr Operand r8 = 8_lreg;
	constexpr Operand r9 = 9_lreg;
	constexpr Operand r10 = 10_lreg;
	constexpr Operand r11 = 11_lreg;
	constexpr Operand r12 = 12_lreg;
	constexpr Operand r13 = 13_lreg;
	constexpr Operand r14 = 14_lreg;
	constexpr Operand r15 = 15_lreg;
	constexpr Operand g0 = 0_greg;
	constexpr Operand g1 = 1_greg;
	constexpr Operand g2 = 2_greg;
	constexpr Operand g3 = 3_greg;
	constexpr Operand g4 = 4_greg;
	constexpr Operand g5 = 5_greg;
	constexpr Operand g6 = 6_greg;
	constexpr Operand g7 = 7_greg;
	constexpr Operand g8 = 8_greg;
	constexpr Operand g9 = 9_greg;
	constexpr Operand g10 = 10_greg;
	constexpr Operand g11 = 11_greg;
	constexpr Operand g12 = 12_greg;
	constexpr Operand g13 = 13_greg;
	constexpr Operand g14 = 14_greg;
	constexpr Operand g15 = 15_greg;
	constexpr Operand fp = 15_greg;
    union Instruction {
        struct REGFormat {
            Ordinal _source1 : 5;
            Ordinal _unused : 2;
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
            bool m1Set() const noexcept { return _m1 != 0; }
            bool m2Set() const noexcept { return _m2 != 0; }
            bool m3Set() const noexcept { return _m3 != 0; }
            bool src1IsLiteral() const noexcept { return _m1 != 0; }
            bool src2IsLiteral() const noexcept { return _m2 != 0; }
            bool srcDestIsLiteral() const noexcept { return _m3 != 0; }
            ByteOrdinal src1ToIntegerLiteral() const noexcept { return _source1; }
            ByteOrdinal src2ToIntegerLiteral() const noexcept { return _source2; }
			void encodeSrc1(const Operand& operand) noexcept {
				_source1 = operand.getValue();
				_m1 = operand.isLiteral() ? 1 : 0;
			}
			void encodeSrc2(const Operand& operand) noexcept {
				_source2 = operand.getValue();
				_m2 = operand.isLiteral() ? 1 : 0;
			}
			void encodeSrcDest(const Operand& operand) noexcept {
				_src_dest = operand.getValue();
				_m3 = operand.isLiteral() ? 1 : 0;
			}
        };
		static_assert(sizeof(REGFormat) == sizeof(Ordinal), "RegFormat sizes is does not equal Ordinal's size!");
        struct COBRFormat {
			Ordinal _unused : 2;
            Ordinal _displacement : 11;
            Ordinal _m1 : 1;
            Ordinal _source2 : 5; 
            Ordinal _source1 : 5;
            Ordinal _opcode : 8;
            auto src1IsLiteral() const noexcept { return _m1 != 0; }
			void encodeSrc1(const Operand& operand) noexcept {
				_source1 = operand.getValue();
				_m1 = operand.isLiteral() ? 1 : 0;
			}
        };
        struct CTRLFormat {
			Ordinal _unused : 2;
            Ordinal _displacement : 22;
            Ordinal _opcode : 8;
			void encodeDisplacement(Ordinal value) noexcept {
				_displacement = value;
			}
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
				bool isOffsetAddressingMode() const noexcept {
					return getAddressingMode() == AddressingModes::Offset;
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
                Ordinal _unused : 2;
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
			Ordinal getSrcDestIndex() const noexcept {
				if (isMemAFormat()) {
					return _mema._src_dest;
				} else {
					return _memb._src_dest;
				}
			}
			Ordinal getOpcode() const noexcept {
				if (isMemAFormat()) {
					return _mema._opcode;
				} else {
					return _memb._opcode;
				}
			}
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
            return (0xFF000000 & _raw) >> 24;
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

	// memory map
	namespace MemoryMap {
		// internal data ram 1 kbyte is mapped into the memory space
		constexpr Ordinal NMIVector = 0x0000'0000;
		constexpr Ordinal OptionalInterruptVectorsBegin = 0x0000'0004;
		constexpr Ordinal OptionalInterruptVectorsEnd = 0x0000'003F;
		constexpr Ordinal DataCacheUnreservedStart = 0x0000'0040;
		constexpr Ordinal DataCacheUnreservedEnd = 0x0000'03FF;
		// internal data cache end
		// normal memory begin
		constexpr Ordinal ExternalUnusedMemoryBegin = 0x0000'0400;
		constexpr Ordinal ExternalUnusedMemoryEnd = 0xFEFF'FF2F;
		constexpr Ordinal InitializationBootRecordBegin = 0xFEFF'FF30;
		constexpr Ordinal InitializationBootRecordEnd = 0xFEFF'FF5F;
		constexpr Ordinal ReservedMemoryBegin = 0xFEFF'FF60;
		constexpr Ordinal ReservedMemoryEnd = 0xFEFF'FFFF;
		constexpr Ordinal MemoryMappedRegisterSpaceBegin = 0xFF00'0000;
		constexpr Ordinal MemoryMappedRegisterSpaceEnd = 0xFFFF'FFFF;
	} // end namespace MemoryMap

	/**
	 * A block of 1024 bytes which is readable and writable but not
	 * executable. It is embedded within the processor itself and generates
	 * no external buss activity is generated when accessed.
	 */
	template<Ordinal numBytes = 1024>
	struct InternalDataRam {
		public:
			// first 64 bytes are reserved for optional interrupt vectors and the nmi vector
			constexpr static Ordinal MinimumSize = 4;
			constexpr static Ordinal TotalReservedBytes = 64;
			constexpr static Ordinal TotalByteCapacity = numBytes;
			static_assert(numBytes >= MinimumSize, "InternalDataRam must be at least 4 bytes in size");
			constexpr static Ordinal TotalUnreservedBytes  = TotalByteCapacity - TotalReservedBytes;
			constexpr static Ordinal TotalWordCapacity = TotalByteCapacity / sizeof(Ordinal);
			constexpr static Ordinal TotalReservedWords = TotalReservedBytes / sizeof(Ordinal);
			constexpr static Ordinal TotalUnreservedWords = TotalUnreservedBytes / sizeof(Ordinal);
		public:
			InternalDataRam() = default;
			~InternalDataRam() = default;
			void initialize() noexcept;
			void write(Ordinal address, Ordinal value) noexcept;
			Ordinal read(Ordinal address) const noexcept;
			constexpr Ordinal totalByteCapacity() const noexcept { return TotalByteCapacity; }
		private:
			Ordinal _nmiVector;
			Ordinal _optionalInterrupts[TotalReservedWords - 1];
			Ordinal _unreservedValues[TotalUnreservedWords];
	} __attribute__((packed));
	using JxCPUInternalDataRam = InternalDataRam<1024>;
	static_assert(sizeof(JxCPUInternalDataRam) == JxCPUInternalDataRam::TotalByteCapacity, "InternalDataRam is larger than its storage footprint!");

	struct LocalRegisterCache {

	};
} // end namespace i960
#endif // end I960_TYPES_H__
