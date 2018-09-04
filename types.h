#ifndef I960_TYPES_H__
#define I960_TYPES_H__
#ifdef PROTECTED_ARCHITECTURE
    #ifndef NUMERICS_ARCHITECTURE
        // protected implies numerics
        #define NUMERICS_ARCHITECTURE
    #endif // end NUMERICS_ARCHITECTURE
#endif // end PROTECTED_ARCHITECTURE
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
#ifdef NUMERICS_ARCHITECTURE
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
#endif // end NUMERICS_ARCHITECTURE

	union TripleWord {
		struct {
			Ordinal _lower;
			Ordinal _middle;
			Ordinal _upper;
		};
#ifdef NUMERICS_ARCHITECTURE
		ExtendedReal _real;
#endif // end NUMERICS_ARCHITECTURE
	} __attribute__((packed));

	union NormalRegister {
		Ordinal _ordinal;
		Integer _integer;
#ifdef NUMERICS_ARCHITECTURE
		Real _real;
#endif // end NUMERICS_ARCHITECTURE
	};
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
			Ordinal _reserved0 : 1 ; 
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

#define MustBeSizeOfOrdinal(type, message) \
	static_assert(sizeof(type) == sizeof(Ordinal), message)

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
		};
		struct COBRFormat {
			Ordinal _sfr : 1;
			Ordinal _bp : 1;
			Ordinal _displacement : 11;
			Ordinal _m1 : 1;
			Ordinal _source2 : 5; 
			Ordinal _source1 : 5;
			Ordinal _opcode : 8;
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
						case 0b000: return 1;
						case 0b001: return 2;
						case 0b010: return 4;
						case 0b011: return 8;
						case 0b100: return 16;
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
} // end namespace i960
#endif // end I960_TYPES_H__