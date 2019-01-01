#ifndef I960_CORE_H__
#define I960_CORE_H__
#define __DEFAULT_THREE_ARGS__ SourceRegister src1, SourceRegister src2, DestinationRegister dest
#define __DEFAULT_TWO_ARGS__ SourceRegister src, DestinationRegister dest
#define __DEFAULT_DOUBLE_WIDE_THREE_ARGS__ LongSourceRegister src1, LongSourceRegister src2, LongDestinationRegister dest
#define __DEFAULT_DOUBLE_WIDE_TWO_ARGS__ LongSourceRegister src, LongDestinationRegister dest
#define __GEN_DEFAULT_THREE_ARG_SIGS__(name) void name (__DEFAULT_THREE_ARGS__) noexcept
#define __TWO_SOURCE_REGS__ SourceRegister src, SourceRegister dest
#include "types.h"
#include "memiface.h"
#include <cmath>
#include <math.h>
#include <variant>
#include <functional>
namespace i960 {
    using Register = NormalRegister;
    using LongRegister = DoubleRegister;
    using SourceRegister = const Register&;
    using DestinationRegister = Register&;
    using LongSourceRegister = const LongRegister&;
    using LongDestinationRegister = LongRegister&;
    
    using RegisterWindow = NormalRegister[LocalRegisterCount];
	enum class FaultType : ByteOrdinal {
		Override = 0x0,
		Parallel = 0x0,
		Trace = 0x1,
		Operation = 0x2,
		Arithmetic = 0x3,
		Constraint = 0x5,
		Protection = 0x7,
		Type = 0xA,
	};
	enum class TraceFaultSubtype : ByteOrdinal {
		Instruction = 0x02,
		Branch = 0x04,
		Call = 0x08,
		Return = 0x10,
		PreReturn = 0x20,
		Supervisor = 0x40,
		Mark = 0x80,
	};
	enum class OperationFaultSubtype : ByteOrdinal {
		InvalidOpcode = 0x1,
		Unimplemented = 0x2,
		Unaligned = 0x3,
		InvalidOperand = 0x4,
	};
	enum class ArithmeticFaultSubtype : ByteOrdinal {
		IntegerOverflow = 0x1,
		ZeroDivide = 0x2,
	};
	enum class ConstraintFaultSubtype : ByteOrdinal {
		Range = 0x1,
	};
	enum class ProtectionFaultSubtype : ByteOrdinal {
		Length = 0x1,
	};
	enum class TypeFaultSubtype : ByteOrdinal {
		Mismatch = 0x1,
	};
	template<typename T>
	class FaultAssociation final {
		public:
		private:
			FaultAssociation() = delete;
			FaultAssociation(const FaultAssociation&) = delete;
			FaultAssociation(FaultAssociation&&) = delete;
			~FaultAssociation() = delete;
	};
#define X(type, parent) \
	template<> \
	class FaultAssociation< type > final { \
		public: \
			static constexpr auto ParentFaultType = FaultType:: parent ; \
		private: \
			FaultAssociation() = delete; \
			FaultAssociation(const FaultAssociation&) = delete; \
			FaultAssociation(FaultAssociation&&) = delete; \
			~FaultAssociation() = delete; \
	}
	X(TraceFaultSubtype, Trace);
	X(OperationFaultSubtype, Operation);
	X(ArithmeticFaultSubtype, Arithmetic);
	X(ConstraintFaultSubtype, Constraint);
	X(ProtectionFaultSubtype, Protection);
	X(TypeFaultSubtype, Type);
#undef X
	class CoreInformation final {
		public:
			enum class CoreVoltageKind {
				V3_3,
				V5_0,
			};
			constexpr CoreInformation(const char* str, 
					Ordinal devId, 
					CoreVoltageKind voltage, 
					Ordinal icacheSize, 
					Ordinal dcacheSize) noexcept :
				_str(str), 
				_devId(devId), 
				_voltage(voltage), 
				_icacheSize(icacheSize), 
				_dcacheSize(dcacheSize) { 
				}
			constexpr auto getString() const noexcept { return _str; }
			constexpr auto getDeviceId() const noexcept { return _devId; }
			constexpr auto getVoltage() const noexcept { return _voltage; }
			constexpr auto getInstructionCacheSize() const noexcept { return _icacheSize; }
			constexpr auto getDataCacheSize() const noexcept { return _dcacheSize; }
			constexpr auto getVersion() const noexcept { return (_devId & 0xF000'0000) >> 28; }
			constexpr auto getProductType() const noexcept { return (_devId & 0x0FE0'0000) >> 21; }
			constexpr auto getGeneration() const noexcept { return (_devId & 0x001E'0000) >> 17; }
			constexpr auto getModel() const noexcept { return (_devId & 0x0001'F000) >> 12; }
			constexpr auto getManufacturer() const noexcept { return (_devId & 0x0000'0FFE) >> 1; }
		private:
			const char* _str;
			Ordinal _devId;
			CoreVoltageKind _voltage;
			Ordinal _icacheSize;
			Ordinal _dcacheSize;
	};
	constexpr CoreInformation cpu80L960JA("80L960JA", 0x0082'1013, CoreInformation::CoreVoltageKind::V3_3, 2048u, 1024u);
	constexpr CoreInformation cpu80960JF("80960JF", 0x0882'0013, CoreInformation::CoreVoltageKind::V5_0, 4096u, 2048u);
	static_assert(cpu80960JF.getGeneration() == 0b0001, "Bad generation check!");
	static_assert(cpu80960JF.getModel() == 0, "Bad model check!");
	static_assert(cpu80960JF.getProductType() == 0b1000100, "Bad product type check!");
	static_assert(cpu80960JF.getManufacturer() == 0b0000'0001'001, "Bad manufacturer check!");
	constexpr CoreInformation cpu80L960JF("80L960JF", 0x0082'0013, CoreInformation::CoreVoltageKind::V3_3, 4096u, 2048u);
	constexpr CoreInformation cpu80960JD("80960JD", 0x0882'0013, CoreInformation::CoreVoltageKind::V5_0, 4096u, 2048u);

	
    class Core {
        public:
			Core(const CoreInformation& info, MemoryInterface& mem);
			/**
			 * Invoked by the external RESET pin, initializes the core.
			 */
			void reset();
			void initializeProcessor();
			void processPrcb();
        private:
			Ordinal getFaultTableBaseAddress() noexcept;
			Ordinal getControlTableBaseAddress() noexcept;
			Ordinal getACRegisterInitialImage() noexcept;
			Ordinal getFaultConfigurationWord() noexcept;
			Ordinal getInterruptTableBaseAddress() noexcept;
			Ordinal getSystemProcedureTableBaseAddress() noexcept;
			Ordinal getInterruptStackPointer() noexcept;
			Ordinal getInstructionCacheConfigurationWord() noexcept;
			Ordinal getRegisterCacheConfigurationWord() noexcept;
			Ordinal getPRCBPointer() noexcept;
			Ordinal getFirstInstructionPointer() noexcept;
			void generateFault(ByteOrdinal faultType, ByteOrdinal faultSubtype = 0);
			template<typename T>
			void generateFault(T faultSubtype) {
				generateFault(ByteOrdinal(FaultAssociation<T>::ParentFaultType), ByteOrdinal(faultSubtype));
			}
            /** 
             * perform a call
             */
            void call(Integer displacement) noexcept;
            Ordinal load(Ordinal address, bool atomic = false) noexcept;
            void store(Ordinal address, Ordinal value, bool atomic = false) noexcept;

            void saveLocalRegisters() noexcept;
            void allocateNewLocalRegisterSet() noexcept;
            template<typename T>
            void setRegister(ByteOrdinal index, T value) noexcept {
                getRegister(index).set<T>(value);
            }
            void setRegister(ByteOrdinal index, SourceRegister other) noexcept;
            NormalRegister& getRegister(ByteOrdinal index) noexcept;
			LongRegister makeLongRegister(ByteOrdinal index) noexcept;
			TripleRegister makeTripleRegister(ByteOrdinal index) noexcept;
			QuadRegister makeQuadRegister(ByteOrdinal index) noexcept;
            PreviousFramePointer& getPFP() noexcept;
            Ordinal getStackPointerAddress() const noexcept;
            void setFramePointer(Ordinal value) noexcept;
            Ordinal getFramePointerAddress() const noexcept;
            // begin core architecture
            void callx(SourceRegister value) noexcept;
            void calls(SourceRegister value);
            __GEN_DEFAULT_THREE_ARG_SIGS__(addc);
            __GEN_DEFAULT_THREE_ARG_SIGS__(addo);
            __GEN_DEFAULT_THREE_ARG_SIGS__(addi);
            void chkbit(SourceRegister pos, SourceRegister src) noexcept;
            __GEN_DEFAULT_THREE_ARG_SIGS__(alterbit);
            __GEN_DEFAULT_THREE_ARG_SIGS__(opand);
            __GEN_DEFAULT_THREE_ARG_SIGS__(andnot);
            void atadd(__DEFAULT_THREE_ARGS__) noexcept; // TODO add other forms of atadd
            void atmod(SourceRegister src, SourceRegister mask, DestinationRegister srcDest) noexcept; // TODO check out other forms of this instruction
            void b(Integer displacement) noexcept;
            void bx(SourceRegister targ) noexcept; // TODO check these two instructions out for more variants
            void bal(Integer displacement) noexcept;
            void balx(__DEFAULT_TWO_ARGS__) noexcept; // TODO check these two instructions out for more variants
            void bbc(SourceRegister pos, SourceRegister src, Integer targ) noexcept; 
            void bbs(SourceRegister pos, SourceRegister src, Integer targ) noexcept;
            // compare and branch instructions as well
            // faults too
            void clrbit(SourceRegister pos, SourceRegister src, DestinationRegister dest) noexcept; // TODO look into the various forms further
            void cmpi(SourceRegister src1, SourceRegister src2) noexcept;
            void cmpo(SourceRegister src1, SourceRegister src2) noexcept;
            __GEN_DEFAULT_THREE_ARG_SIGS__(cmpdeci);
            __GEN_DEFAULT_THREE_ARG_SIGS__(cmpdeco);
            __GEN_DEFAULT_THREE_ARG_SIGS__(cmpinci);
            __GEN_DEFAULT_THREE_ARG_SIGS__(cmpinco);
            void concmpi(SourceRegister src1, SourceRegister src2) noexcept;
            void concmpo(SourceRegister src1, SourceRegister src2) noexcept;
            __GEN_DEFAULT_THREE_ARG_SIGS__(divo);
            __GEN_DEFAULT_THREE_ARG_SIGS__(divi);
			void ediv(SourceRegister src1, ByteOrdinal src2Ind, ByteOrdinal destInd) noexcept;
			void emul(SourceRegister src1, SourceRegister src2, ByteOrdinal destIndex) noexcept;
            __GEN_DEFAULT_THREE_ARG_SIGS__(extract);
            void fill(SourceRegister dst, SourceRegister value, SourceRegister len) noexcept;
            void flushreg() noexcept;
            void fmark() noexcept;
            void ld(__DEFAULT_TWO_ARGS__) noexcept;
            void ldob(__DEFAULT_TWO_ARGS__) noexcept;
            void ldos(__DEFAULT_TWO_ARGS__) noexcept;
            void ldib(__DEFAULT_TWO_ARGS__) noexcept;
            void ldis(__DEFAULT_TWO_ARGS__) noexcept;
			void ldl(SourceRegister src, Ordinal srcDestIndex) noexcept;
			void ldt(SourceRegister src, Ordinal srcDestIndex) noexcept;
			void ldq(SourceRegister src, Ordinal index) noexcept;
            void lda(__DEFAULT_TWO_ARGS__) noexcept;
            void mark() noexcept;
            __GEN_DEFAULT_THREE_ARG_SIGS__(modac);
            __GEN_DEFAULT_THREE_ARG_SIGS__(modi);
            __GEN_DEFAULT_THREE_ARG_SIGS__(modify);
            __GEN_DEFAULT_THREE_ARG_SIGS__(modpc);
            __GEN_DEFAULT_THREE_ARG_SIGS__(modtc);
            void mov(const Operand& src, const Operand& dest) noexcept;
            void movl(const Operand& src, const Operand& dest) noexcept;
            void movt(const Operand& src, const Operand& dest) noexcept;
            void movq(const Operand& src, const Operand& dest) noexcept;
            __GEN_DEFAULT_THREE_ARG_SIGS__(mulo);
            __GEN_DEFAULT_THREE_ARG_SIGS__(muli);
            __GEN_DEFAULT_THREE_ARG_SIGS__(nand);
            __GEN_DEFAULT_THREE_ARG_SIGS__(nor);
            void opnot(__DEFAULT_TWO_ARGS__) noexcept;
            __GEN_DEFAULT_THREE_ARG_SIGS__(notand);
            __GEN_DEFAULT_THREE_ARG_SIGS__(notbit);
            __GEN_DEFAULT_THREE_ARG_SIGS__(notor);
            __GEN_DEFAULT_THREE_ARG_SIGS__(opor);
            __GEN_DEFAULT_THREE_ARG_SIGS__(ornot);
            __GEN_DEFAULT_THREE_ARG_SIGS__(remo);
            __GEN_DEFAULT_THREE_ARG_SIGS__(remi);
            void resumeprcs(SourceRegister src) noexcept;
            void ret() noexcept;
            void rotate(__DEFAULT_THREE_ARGS__) noexcept;
            void scanbyte(SourceRegister src1, SourceRegister src2) noexcept;
            void scanbit(__DEFAULT_TWO_ARGS__) noexcept;
            void setbit(__DEFAULT_THREE_ARGS__) noexcept;
            void shlo(__DEFAULT_THREE_ARGS__) noexcept;
            void shro(__DEFAULT_THREE_ARGS__) noexcept;
            void shli(__DEFAULT_THREE_ARGS__) noexcept;
            void shri(__DEFAULT_THREE_ARGS__) noexcept;
            void shrdi(__DEFAULT_THREE_ARGS__) noexcept;
            void spanbit(__DEFAULT_TWO_ARGS__) noexcept;
            void st(__TWO_SOURCE_REGS__) noexcept;
            void stob(__TWO_SOURCE_REGS__) noexcept;
            void stos(__TWO_SOURCE_REGS__) noexcept;
            void stib(__TWO_SOURCE_REGS__) noexcept;
            void stis(__TWO_SOURCE_REGS__) noexcept;
			void stl(Ordinal ind, SourceRegister dest) noexcept;
            void stt(Ordinal ind, SourceRegister dest) noexcept;
            void stq(Ordinal ind, SourceRegister dest) noexcept;
            __GEN_DEFAULT_THREE_ARG_SIGS__(subc); 
            __GEN_DEFAULT_THREE_ARG_SIGS__(subo);
            __GEN_DEFAULT_THREE_ARG_SIGS__(subi);
            void syncf() noexcept;
            __GEN_DEFAULT_THREE_ARG_SIGS__(xnor);
            __GEN_DEFAULT_THREE_ARG_SIGS__(opxor);
			__GEN_DEFAULT_THREE_ARG_SIGS__(sysctl);
			void inten();
			void intdis();
			void intctl(__DEFAULT_TWO_ARGS__);
			__GEN_DEFAULT_THREE_ARG_SIGS__(icctl);
			void eshro(SourceRegister src1, ByteOrdinal src2Ind, DestinationRegister dest) noexcept;
			__GEN_DEFAULT_THREE_ARG_SIGS__(eshro);
			__GEN_DEFAULT_THREE_ARG_SIGS__(dcctl);
			void halt(SourceRegister src1);
            void cmpos(SourceRegister src1, SourceRegister src2) noexcept;
            void cmpis(SourceRegister src1, SourceRegister src2) noexcept;
            void cmpob(SourceRegister src1, SourceRegister src2) noexcept;
            void cmpib(SourceRegister src1, SourceRegister src2) noexcept;
			void bswap(SourceRegister src1, DestinationRegister src2) noexcept;
			void baseSelect(bool condition, __DEFAULT_THREE_ARGS__) noexcept;
		private:
			// templated bodies
			template<typename T>
			void concmpBase(SourceRegister src1, SourceRegister src2) noexcept {
				if (_ac.conditionCodeBitSet<0b100>()) {
					if (auto s1 = src1.get<T>(), s2 = src2.get<T>(); s1 <= s2) {
						_ac.conditionCode = 0b010;
					} else {
						_ac.conditionCode = 0b001;
					}
				}
			}
            template<TestTypes t>
            void testGeneric(DestinationRegister dest) noexcept {
                dest.set<Ordinal>((_ac.conditionCode & (Ordinal(t))) != 0 ? 1 : 0);
            }
            template<ConditionCode cc>
            bool conditionCodeIs() const noexcept {
                return (_ac.conditionCode & static_cast<Ordinal>(cc)) != 0;
            }
            template<ConditionCode cc>
            void branchIfGeneric(Integer addr) noexcept {
                if constexpr (cc == ConditionCode::Unordered) {
                    if (_ac.conditionCode == 0) {
                        b(addr);
                    }
                } else {
                    if (((Ordinal(cc)) & _ac.conditionCode) != 0) {
                        b(addr);
                    }
                }
            }
            template<typename T>
            void compare(T src1, T src2) noexcept {
                if (src1 < src2) {
                    _ac.conditionCode = 0b100;
                } else if (src1 == src2) {
                    _ac.conditionCode = 0b010;
                } else {
                    _ac.conditionCode = 0b001;
                }
            }
			template<Ordinal mask>
			void baseSelect(__DEFAULT_THREE_ARGS__) noexcept {
				if (((mask & _ac.conditionCode) != 0) || (mask == _ac.conditionCode)) {
					dest.set<Ordinal>(src2.get<Ordinal>());
				} else {
					dest.set<Ordinal>(src1.get<Ordinal>());
				}
			}
			template<Ordinal mask>
			void addoBase(__DEFAULT_THREE_ARGS__) noexcept {
				if (((mask & _ac.conditionCode) != 0) || (mask == _ac.conditionCode)) {
					addo(src1, src2, dest);
				}
			}
			template<Ordinal mask>
			void addiBase(__DEFAULT_THREE_ARGS__) noexcept {
				if (((mask & _ac.conditionCode) != 0) || (mask == _ac.conditionCode)) {
					dest.set<Integer>(src1.get<Integer>() + src2.get<Integer>());
				}
				// according to the docs, the arithmetic overflow always is
				// computed even if the addition is not performed
				if ((src2.mostSignificantBit() == src1.mostSignificantBit()) && (src2.mostSignificantBit() != dest.mostSignificantBit())) {
					if (_ac.integerOverflowMask == 1) {
						_ac.integerOverflowFlag = 1;
					} else {
						generateFault(ArithmeticFaultSubtype::IntegerOverflow);
					}
				}
			}
			template<Ordinal mask>
			void suboBase(__DEFAULT_THREE_ARGS__) noexcept {
				if (((mask & _ac.conditionCode) != 0) || (mask == _ac.conditionCode)) {
					subo(src1, src2, dest);
				}
			}
			template<Ordinal mask>
			void subiBase(__DEFAULT_THREE_ARGS__) noexcept {
				if (((mask & _ac.conditionCode) != 0) || (mask == _ac.conditionCode)) {
					dest.set<Integer>(src2.get<Integer>() - src1.get<Integer>());
				}
				// according to the docs, the arithmetic overflow always is
				// computed even if the subtraction is not performed
				if ((src2.mostSignificantBit() != src1.mostSignificantBit()) && (src2.mostSignificantBit() != dest.mostSignificantBit())) {
					if (_ac.integerOverflowMask == 1) {
						_ac.integerOverflowFlag = 1;
					} else {
						generateFault(ArithmeticFaultSubtype::IntegerOverflow);
					}
				}
			}
			template<bool checkIfSet>
			static constexpr bool checkIfBitIs(Ordinal s, Ordinal mask) noexcept {
				if constexpr (auto maskedValue = s & mask; checkIfSet) {
					return maskedValue == 1;
				} else {
					return maskedValue == 0;
				}
			}
			template<bool branchOnSet>
			void checkBitAndBranchIf(SourceRegister bitpos, SourceRegister src, Integer targ) noexcept {
				// check bit and branch if clear
				auto shiftAmount = bitpos.get<Ordinal>() & 0b11111;
				auto mask = 1 << shiftAmount;
				if (auto s = src.get<Ordinal>(); checkIfBitIs<branchOnSet>(s, mask)) {
					if constexpr (branchOnSet) {
						_ac.conditionCode = 0b010;
					} else {
						_ac.conditionCode = 0;
					}

					union {
						Integer value : 11;
					} displacement;
					displacement.value = targ;
					_instructionPointer = _instructionPointer + 4 + (displacement.value * 4);
				} else {
					if constexpr (branchOnSet) {
						_ac.conditionCode = 0;
					} else {
						_ac.conditionCode = 0b010;
					}
				}
			}
		private:
			// auto generated routines
#define X(kind, __) \
            void b ## kind (Integer) noexcept; \
            void cmpib ## kind ( SourceRegister, SourceRegister, Integer) noexcept; \
            void fault ## kind () noexcept; \
            void test ## kind ( DestinationRegister) noexcept;
#define Y(kind) void cmp ## kind ( SourceRegister, SourceRegister, Integer) noexcept;
#include "conditional_kinds.def"
			Y(obe)
			Y(obne)
			Y(obl)
			Y(oble)
			Y(obg)
			Y(obge)
#undef X
#undef Y
#define X(kind, __) \
			__GEN_DEFAULT_THREE_ARG_SIGS__(addo ## kind) ; \
			__GEN_DEFAULT_THREE_ARG_SIGS__(addi ## kind) ; \
			__GEN_DEFAULT_THREE_ARG_SIGS__(subo ## kind) ; \
			__GEN_DEFAULT_THREE_ARG_SIGS__(subi ## kind) ; \
			__GEN_DEFAULT_THREE_ARG_SIGS__(sel ## kind) ;
#include "conditional_kinds.def"
#undef X 
		private:
			void dispatch(const Instruction& decodedInstruction) noexcept;
        private:
            RegisterWindow _globalRegisters;
            // The hardware implementations use register sets, however
            // to start with, we should just follow the logic as is and 
            // just save the contents of the registers to the stack the logic
            // is always sound to do it this way
            RegisterWindow _localRegisters;
            ArithmeticControls _ac;
            Ordinal _instructionPointer;
            ProcessControls _pc;
            TraceControls _tc;
			MemoryInterface& _mem;
			Ordinal _prcbAddress;
			Ordinal _ctrlTable;
			// the first 1024 bytes of ram is a internal data ram cache
			// which can be read from and written to but not executed from
			i960::JxCPUInternalDataRam _internalDataRam;
			const CoreInformation& _deviceId;
    };

}
#undef __TWO_SOURCE_REGS__
#undef __GEN_DEFAULT_THREE_ARG_SIGS__
#undef __DEFAULT_THREE_ARGS__
#undef __DEFAULT_DOUBLE_WIDE_THREE_ARGS__
#undef __DEFAULT_TWO_ARGS__
#undef __DEFAULT_DOUBLE_WIDE_TWO_ARGS__

#endif // end I960_CORE_H__
