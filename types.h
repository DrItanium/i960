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
                } else if constexpr(std::is_same_v<K, PreviousFramePointer>) {
                    ordinal = value.value;
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
		bool conditionCodeBitSet() const noexcept {
			return (conditionCode & mask) == 0;
		}
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
            Ordinal internalState : 11;
        };
        Ordinal value;
		bool traceEnabled() const noexcept {
			return traceEnable != 0;
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
            bool m1Set() const noexcept { return _m1 != 0; }
            bool m2Set() const noexcept { return _m2 != 0; }
            bool m3Set() const noexcept { return _m3 != 0; }
            bool src1IsLiteral() const noexcept { return _m1 != 0; }
            bool src2IsLiteral() const noexcept { return _m2 != 0; }
            bool srcDestIsLiteral() const noexcept { return _m3 != 0; }
            ByteOrdinal src1ToIntegerLiteral() const noexcept { return _source1; }
            ByteOrdinal src2ToIntegerLiteral() const noexcept { return _source2; }
        };
        struct COBRFormat {
            Ordinal _sfr : 1;
            Ordinal _bp : 1;
            Ordinal _displacement : 11;
            Ordinal _m1 : 1;
            Ordinal _source2 : 5; 
            Ordinal _source1 : 5;
            Ordinal _opcode : 8;
            auto src1IsLiteral() const noexcept { return _m1 != 0; }
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
