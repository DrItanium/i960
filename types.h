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
		void clear() noexcept {
			value = 0;
		}
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
		void clear() noexcept {
			value = 0;
		}
		bool traceEnabled() const noexcept {
			return traceEnable != 0;
		}
		bool inUserMode() const noexcept {
			return executionMode == 0;
		}
		bool inSupervisorMode() const noexcept {
			return executionMode != 0;
		}
		void enterSupervisorMode() noexcept {
			executionMode = 1;
		}
		void enterUserMode() noexcept {
			executionMode = 0;
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
		void clear() noexcept {
			value = 0;
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
			static constexpr Ordinal typeInputMask = 0b1;
			static constexpr Ordinal typeShiftAmount = 5;
			constexpr Operand(Ordinal rawValue) : _raw(rawValue & encodingMask) { }
			constexpr Operand(Ordinal type, Ordinal value) : Operand(((type & typeInputMask) << typeShiftAmount) | (value & valueMask)) { }
			constexpr bool isLiteral() const noexcept { return (_raw & typeMask) != 0; }
			constexpr bool isRegister() const noexcept { return (_raw & typeMask) == 0; }
			constexpr Ordinal getValue() const noexcept { return (_raw & valueMask); }
			constexpr operator ByteOrdinal() const noexcept { return ByteOrdinal(getValue()); }
			constexpr auto notDivisibleBy(ByteOrdinal value) const noexcept { return (((ByteOrdinal)getValue()) % value) != 0; }
			constexpr Operand next() const noexcept {
				return Operand((_raw & typeMask) != 0, getValue() + 1);
			}
		private:
			Ordinal _raw;
	};
	constexpr Operand operator"" _lit(unsigned long long n) {
		return Operand(((Operand::valueMask & Ordinal(n)) + Operand::typeMask));
	}
	constexpr Operand operator"" _gr(unsigned long long n) {
		return Operand((n & 0b1111) + 0b10000);
	}
	constexpr Operand operator"" _lr(unsigned long long n) {
		return Operand((n & 0b1111));
	}
	
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
			void encodeSrc1(const Operand& operand) noexcept {
				_source1 = operand.getValue();
				_m1 = operand.isLiteral() ? 1 : 0;
			}
			auto decodeSrc1() const noexcept {
				return Operand(_m1, _source1);
			}
			void encodeSrc2(const Operand& operand) noexcept {
				_source2 = operand.getValue();
				_m2 = operand.isLiteral() ? 1 : 0;
			}
			auto decodeSrc2() const noexcept {
				return Operand(_m2, _source2);
			}
			void encodeSrcDest(const Operand& operand) noexcept {
				_src_dest = operand.getValue();
				_m3 = operand.isLiteral() ? 1 : 0;
			}
			auto decodeSrcDest() const noexcept {
				return Operand(_m3, _src_dest);
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
			auto decodeSrc1() const noexcept {
				return Operand(_m1, _source1); 
			}
			void encodeSrc2(const Operand& operand) noexcept {
				// regardless if you give me a literal or a register, use the
				// value as is.
				_source2 = operand.getValue();
			}
			auto decodeSrc2() const noexcept {
				return Operand(0, _source2);
			}
			auto decodeDisplacement() const noexcept {
				return _displacement;
			}
        };
        struct CTRLFormat {
			Ordinal _unused : 2;
            Ordinal _displacement : 22;
            Ordinal _opcode : 8;
			void encodeDisplacement(Ordinal value) noexcept {
				_displacement = value;
			}
			auto decodeDisplacement() const noexcept {
				return _displacement;
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
				auto decodeSrcDest() const noexcept {
					return Operand(0, _src_dest);
				}
				auto decodeAbase() const noexcept {
					return Operand(0, _abase);
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
				auto decodeSrcDest() const noexcept {
					return Operand(0, _src_dest);
				}
				auto decodeAbase() const noexcept {
					return Operand(0, _abase);
				}
            };
			auto decodeSrcDest() const noexcept {
				if (isMemAFormat()) {
					return _mema.decodeSrcDest();
				} else {
					return _memb.decodeSrcDest();
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

        Instruction(Ordinal raw = 0) : _raw(raw) { }
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
	 * Physical Memory Region Control Register. Represents a 512 mbyte memory
	 * region to control
	 */
	enum class PMCONRegisterKind {
		Region0_1, 
		Region2_3, 
		Region4_5,
		Region6_7, 
		Region8_9, 
		Region10_11,
		Region12_13,
		Region14_15,
	};
	using PMCONRegisterRange = std::tuple<Ordinal, Ordinal>;
	template<PMCONRegisterKind kind>
	constexpr auto PMCONMemoryRange = false;
#define X(kind, start, end) \
	template<> \
	constexpr PMCONRegisterRange PMCONMemoryRange<PMCONRegisterKind:: kind > = std::make_tuple( start, end )
X(Region0_1,   0x0000'0000, 0x1FFF'FFFF);
X(Region2_3,   0x2000'0000, 0x3FFF'FFFF);
X(Region4_5,   0x4000'0000, 0x5FFF'FFFF);
X(Region6_7,   0x6000'0000, 0x7FFF'FFFF);
X(Region8_9,   0x8000'0000, 0x9FFF'FFFF);
X(Region10_11, 0xA000'0000, 0xBFFF'FFFF);
X(Region12_13, 0xC000'0000, 0xDFFF'FFFF);
X(Region14_15, 0xE000'0000, 0xFFFF'FFFF);
#undef X
	union PMCONRegister final {
		struct {
			Ordinal _unused0 : 22;
			Ordinal _busWidth : 2;
			Ordinal _unused1 : 8;
		};
		Ordinal _value;
		bool busWidthIs8bit() const noexcept { return _busWidth == 0b00; }
		bool busWidthIs16bit() const noexcept { return _busWidth == 0b01; }
		bool busWidthIs32bit() const noexcept { return _busWidth == 0b10; }
		bool busWidthIsUndefined() const noexcept { return _busWidth == 0b11; }
	} __attribute__((packed));
	union BCONRegister final {
		struct {
			Ordinal _configurationEntriesInControlTableValid : 1;
			Ordinal _internalRAMProtection : 1;
			Ordinal _supervisorInternalRAMProtection : 1;
		};
		Ordinal _value;
		bool pmconEntriesValid() const noexcept { return _configurationEntriesInControlTableValid != 0; }
		bool internalDataRAMProtectedFromUserModeWrites() const noexcept { return _internalRAMProtection != 0; }
		bool first64BytesProtectedFromSupervisorModeWrites() const noexcept { return _supervisorInternalRAMProtection != 0; }
	} __attribute__((packed));
	union LogicalMemoryTemplateStartingAddressRegister final {
		struct {
			Ordinal _byteOrder : 1;
			Ordinal _dataCacheEnable : 1;
			Ordinal _reserved : 10;
			/**
			 * Upper 20-bits for the starting address for a logical data
			 * template. The lower 12 bits are fixed at zero. The starting
			 * address is modulo 4 kbytes
			 */
			Ordinal _templateStartingAddress : 20;
		};
		Ordinal _value;
		bool littleEndianByteOrder() const noexcept { return _byteOrder == 0; }
		bool bigEndianByteOrder() const noexcept { return _byteOrder != 0; }
		bool dataCacheEnabled() const noexcept { return _dataCacheEnable != 0; }
		Ordinal getTemplateStartingAddress() const noexcept { return _templateStartingAddress; }
	} __attribute__((packed));
	union LogicalMemoryTemplateMaskRegister final {
		struct {
			Ordinal _logicalMemoryTemplateEnabled : 1;
			Ordinal _reserved : 11;
			Ordinal _templateAddressMask : 20;
		};
		Ordinal _value;
		bool logicalMemoryTemplateEnabled() const noexcept { return _logicalMemoryTemplateEnabled; }
		Ordinal getTemplateAddressMask() const noexcept { return _templateAddressMask; }
	} __attribute__((packed));
	union DefaultLogicalMemoryConfigurationRegister final {
		struct {
			Ordinal _byteOrder : 1;
			Ordinal _dataCacheEnable : 1;
		};
		Ordinal _value;
		bool littleEndianByteOrder() const noexcept { return _byteOrder == 0; }
		bool bigEndianByteOrder() const noexcept { return _byteOrder != 0; }
		bool dataCacheEnabled() const noexcept { return _dataCacheEnable != 0; }
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
	template<Ordinal numBytes>
	struct InternalDataRam {
		public:
			// first 64 bytes are reserved for optional interrupt vectors and the nmi vector
			constexpr static Ordinal MinimumSize = 4;
			constexpr static Ordinal TotalReservedBytes = 64;
			constexpr static Ordinal TotalByteCapacity = numBytes;
			static_assert(numBytes >= MinimumSize, "InternalDataRam must be at least 4 bytes in size");
			static_assert((numBytes & 1) == 0, "numBytes is not even!");
			static_assert(numBytes == ((numBytes >> 2) << 2), "numBytes is not a power of two value!");
			constexpr static Ordinal TotalUnreservedBytes  = TotalByteCapacity - TotalReservedBytes;
			constexpr static Ordinal TotalWordCapacity = TotalByteCapacity / sizeof(Ordinal);
			constexpr static Ordinal TotalReservedWords = TotalReservedBytes / sizeof(Ordinal);
			constexpr static Ordinal TotalUnreservedWords = TotalUnreservedBytes / sizeof(Ordinal);
			constexpr static Ordinal LegalMaskValue = TotalWordCapacity - 1;
			template<Ordinal baseAddress>
			constexpr static Ordinal LargestAddress = (baseAddress +  TotalByteCapacity) - 1;
			template<Ordinal baseAddress>
			constexpr static Ordinal SmallestAddress = baseAddress;
			
		public:
			InternalDataRam() = default;
			~InternalDataRam() = default;
			void reset() noexcept {
				// zero out memory
				for (auto i = 0u; i < TotalWordCapacity; ++i) {
					write(i, 0);
				}
			}
			void writeByte(Ordinal address, ByteOrdinal value) noexcept {
				auto localOrdinal = read(address);
				switch (address & 0b11) {
					case 0b00:
						localOrdinal = (localOrdinal & 0xFFFFFF00) | Ordinal(value);
						break;
					case 0b01:
						localOrdinal = (localOrdinal & 0xFFFF00FF) | (Ordinal(value) << 8);
						break;
					case 0b10:
						localOrdinal = (localOrdinal & 0xFF00FFFF) | (Ordinal(value) << 16);
						break;
					case 0b11:
						localOrdinal = (localOrdinal & 0x00FFFFFF) | (Ordinal(value) << 24);
						break;
					default:
						throw "Should never fire!";
				}
				write(address & (~0b11), localOrdinal);
			}
			void write(Ordinal address, Ordinal value) noexcept {
				auto actualAddress = (address >> 2) & LegalMaskValue;
				_words[actualAddress] = value;
			}
			ByteOrdinal readByte(Ordinal address) const noexcept {
				// extract out the ordinal that the byte is a part of
				auto closeValue = read(address);
				switch (address & 0b11) {
					case 0b00: 
						return ByteOrdinal(closeValue);
					case 0b01: 
						return ByteOrdinal(closeValue >> 8);
					case 0b10: 
						return ByteOrdinal(closeValue >> 16);
					case 0b11: 
						return ByteOrdinal(closeValue >> 24);
					default:
						throw "Should never be hit!";
				}
			}
			Ordinal read(Ordinal address) const noexcept {
				// the address needs to be fixed to be a multiple of four
				auto realAddress = (address >> 2) & LegalMaskValue;
				return _words[realAddress];
			}
			constexpr Ordinal totalByteCapacity() const noexcept { return TotalByteCapacity; }
		private:
			Ordinal _words[TotalUnreservedWords + TotalReservedWords];
	} __attribute__((packed));
	using JxCPUInternalDataRam = InternalDataRam<1024>;
	static_assert(sizeof(JxCPUInternalDataRam) == JxCPUInternalDataRam::TotalByteCapacity, "InternalDataRam is larger than its storage footprint!");

} // end namespace i960
#endif // end I960_TYPES_H__
