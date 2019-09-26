#ifndef I960_CORE_H__
#define I960_CORE_H__
#define __DEFAULT_THREE_ARGS__ SourceRegister src1, SourceRegister src2, DestinationRegister dest
#define __DEFAULT_TWO_ARGS__ SourceRegister src, DestinationRegister dest
#define __DEFAULT_DOUBLE_WIDE_THREE_ARGS__ LongSourceRegister src1, LongSourceRegister src2, LongDestinationRegister dest
#define __DEFAULT_DOUBLE_WIDE_TWO_ARGS__ LongSourceRegister src, LongDestinationRegister dest
#define __GEN_DEFAULT_THREE_ARG_SIGS__(name) void name (__DEFAULT_THREE_ARGS__) noexcept
#define __TWO_SOURCE_REGS__ SourceRegister src, SourceRegister dest
#include "types.h"
#include "NormalRegister.h"
#include "DoubleRegister.h"
#include "TripleRegister.h"
#include "QuadRegister.h"
#include "ArithmeticControls.h"
#include "ProcessControls.h"
#include "memiface.h"
#include "Operand.h"
#include "Instruction.h"
#include "InternalDataRam.h"
#include "ConditionCode.h"
#include "ProcessorControlBlock.h"
#include "MemoryMap.h"
#include "StartupRecord.h"
#include "opcodes.h"
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
    
    using RegisterWindow = NormalRegister[16];
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
		private:
			FaultAssociation() = delete;
			FaultAssociation(const FaultAssociation&) = delete;
			FaultAssociation(FaultAssociation&&) = delete;
			~FaultAssociation() = delete;
            FaultAssociation& operator=(const FaultAssociation&) = delete;
            FaultAssociation& operator=(FaultAssociation&&) = delete;
	};
#define X(type, parent) \
	template<> \
	class FaultAssociation< type > final { \
		public: \
			static constexpr auto ParentFaultType = FaultType:: parent ; \
			FaultAssociation() = delete; \
			FaultAssociation(const FaultAssociation&) = delete; \
			FaultAssociation(FaultAssociation&&) = delete; \
			~FaultAssociation() = delete; \
            FaultAssociation& operator=(const FaultAssociation&) = delete; \
            FaultAssociation& operator=(FaultAssociation&&) = delete; \
	}
	X(TraceFaultSubtype, Trace);
	X(OperationFaultSubtype, Operation);
	X(ArithmeticFaultSubtype, Arithmetic);
	X(ConstraintFaultSubtype, Constraint);
	X(ProtectionFaultSubtype, Protection);
	X(TypeFaultSubtype, Type);
#undef X

	constexpr Ordinal clearLowestTwoBitsMask = ~0b11;
    constexpr Ordinal computeAlignedAddress(Ordinal value) noexcept {
        return value & clearLowestTwoBitsMask;
    }
    class Core {
        public:
            static constexpr auto targetSeries = ProcessorSeries::Jx;
            using PRCB = i960::ProcessorControlBlock_t<targetSeries>;
            using IBR = i960::StartupRecord_t<targetSeries>;
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
            Ordinal getSupervisorStackPointerBase() noexcept;
            Ordinal load(Ordinal address, bool atomic = false) noexcept;
            LongOrdinal loadDouble(Ordinal address, bool atomic = false) noexcept;
            void store(Ordinal address, Ordinal value, bool atomic = false) noexcept;
            inline void store(const NormalRegister& addr, const NormalRegister&  value, bool atomic = false) noexcept { 
                store(addr.get<Ordinal>(), value.get<Ordinal>(), atomic);
            }


            inline void store(const NormalRegister& addr, Ordinal value) noexcept {
                store(addr.get<Ordinal>(), value);
            }
            template<typename T>
            void setRegister(const Operand& op, T value) noexcept {
                setRegister<T>(static_cast<ByteOrdinal>(op), value);
            }
            inline auto& getRegister(const Operand& op) noexcept { return getRegister(static_cast<ByteOrdinal>(op)); }
            inline auto getDoubleRegister(const Operand& op) noexcept { return DoubleRegister(getRegister(op), getRegister(op.next())); }
            inline auto getTripleRegister(const Operand& op) noexcept { return TripleRegister(getRegister(op), getRegister(op.next()), getRegister(op.next().next())); }
            inline auto getQuadRegister(const Operand& op) noexcept { return QuadRegister(getRegister(op), getRegister(op.next()), getRegister(op.next().next()), getRegister(op.next().next().next())); }
            inline SourceRegister& getRegister(const Operand& op) const noexcept { return getRegister(static_cast<ByteOrdinal>(op)); }

            template<typename T>
            void setRegister(ByteOrdinal index, T value) noexcept {
                getRegister(index).set<T>(value);
            }
            inline void setRegister(ByteOrdinal index, SourceRegister other) noexcept {
                setRegister(index, other.get<Ordinal>());
            }
            inline void setRegister(const Operand& index, SourceRegister other) noexcept {
                setRegister(static_cast<ByteOrdinal>(index), other);
            }
            NormalRegister& getRegister(ByteOrdinal index) noexcept;
            template<typename T>
            T getRegisterValue(const Operand& op) noexcept {
                using K = std::decay_t<T>;
                if (op.isRegister()) {
                    if constexpr (std::is_same_v<K, LongOrdinal> ||
                            std::is_saem_v<K, LongInteger>) {
                        return getDoubleRegister(op).get<T>();
                    } else {
                        return getRegister(op).get<T>();
                    }
                } else {
                    return static_cast<T>(op.getValue());
                }
            }

            template<typename R = Ordinal>
            R getSrc1(const HasSrc1& src1) noexcept {
                return getRegisterValue<R>(src1.getSrc1());
            }
            template<typename R = Ordinal>
            R getSrc2(const HasSrc2& src2) noexcept {
                return getRegisterValue<R>(src2.getSrc2());
            }
            template<typename R = Ordinal>
            R getSrc(const HasSrcDest& srcDest) noexcept {
                return getRegisterValue<R>(srcDest.getSrcDest());
            }
            inline NormalRegister& getDest(const HasSrcDest& srcDest) noexcept {
                return getRegister(srcDest.getSrcDest());
            }
            template<typename T>
            void setDest(const HasSrcDest& srcDest, T value) noexcept {
                using K = std::decay_t<T>;
                if constexpr (std::is_same_v<K, LongOrdinal> ||
                              std::is_same_v<K, LongInteger>) {
                    getDoubleRegister(srcDest.getSrcDest()).set<T>(value);
                } else {
                    getDest(inst).set<T>(value);
                }
            }
            inline void setDest(const HasSrcDest& srcDest, Operand lower, Operand upper) noexcept {
                getDoubleRegister(srcDest.getSrcDest()).set(lower, upper);
            }
        private:
            inline auto load(const NormalRegister& reg, bool atomic = false) noexcept { return load(reg.get<Ordinal>(), atomic); }
            inline auto load(const Operand& op, bool atomic = false) noexcept { return load(getRegister(op), atomic); }
            inline void store(const Operand& addr, const Operand& value, bool atomic = false) noexcept {
                store(getRegister(addr), getRegister(value), atomic);
            }
            inline void store(const Operand& op, Ordinal value) noexcept {
                store(getRegister(op), value);
            }
			LongRegister makeLongRegister(ByteOrdinal index) noexcept;
            inline auto makeLongRegister(const Operand& base) noexcept { return makeLongRegister(static_cast<ByteOrdinal>(base)); }
			TripleRegister makeTripleRegister(ByteOrdinal index) noexcept;
            inline auto makeTripleRegister(const Operand& index) noexcept { return makeTripleRegister(static_cast<ByteOrdinal>(index)); }
			QuadRegister makeQuadRegister(ByteOrdinal index) noexcept;
            inline auto makeQuadRegister(const Operand& index) noexcept { return makeQuadRegister(static_cast<ByteOrdinal>(index)); }
            PreviousFramePointer& getPFP() noexcept;
            Ordinal getStackPointerAddress() const noexcept;
            void setFramePointer(Ordinal value) noexcept;
            Ordinal getFramePointerAddress() const noexcept;
			void generateFault(ByteOrdinal faultType, ByteOrdinal faultSubtype = 0);
			template<typename T>
			void generateFault(T faultSubtype) {
				generateFault(ByteOrdinal(FaultAssociation<T>::ParentFaultType), ByteOrdinal(faultSubtype));
			}
        private:
            // core dispatch logic
            template<typename T>
            void performOperation(const T&, std::monostate) {
                throw "Bad operation!";
            }
            template<typename T, typename K>
            void performOperation(const T&, K) noexcept {
                throw "UNIMPLEMENTED!";
            }

            template<typename T>
            void dispatchOperation(const T& inst, const typename T::OpcodeList& targetInstruction) {
                std::visit([&inst, this](auto&& value) { return performOperation(inst, value); }, targetInstruction);
            }
            template<typename T>
            void dispatchOperation(const T& inst) {
                std::visit([&inst, this](auto&& value) {
                            using K = std::decay_t<decltype(value)>;
                            if constexpr (std::is_same_v<K, std::monostate>) {
                                generateFault(OperationFaultSubtype::InvalidOpcode);
                            } else {
                                performOperation(inst, value);
                            }
                        }, inst.getTarget());
            }
#define X(name, code, kind) \
            void performOperation(const kind ## FormatInstruction& , Operation:: name ) noexcept ;
#define reg(name, code, __) X(name, code, REG)
#define mem(name, code, __) X(name, code, MEM)
#define cobr(name, code, __) X(name, code, COBR)
#define ctrl(name, code, __) X(name, code, CTRL)
#include "opcodes.def"
#undef X
#undef reg
#undef mem
#undef cobr
#undef ctrl
            // begin core architecture
            void callx(SourceRegister value) noexcept;
            void calls(SourceRegister value);
            __GEN_DEFAULT_THREE_ARG_SIGS__(addo);
            __GEN_DEFAULT_THREE_ARG_SIGS__(addi);
            __GEN_DEFAULT_THREE_ARG_SIGS__(opand);
            __GEN_DEFAULT_THREE_ARG_SIGS__(andnot);
            void atadd(__DEFAULT_THREE_ARGS__) noexcept; // TODO add other forms of atadd
            void atmod(SourceRegister src, SourceRegister mask, DestinationRegister srcDest) noexcept; // TODO check out other forms of this instruction
            // compare and branch instructions as well
            // faults too
            void clrbit(SourceRegister pos, SourceRegister src, DestinationRegister dest) noexcept; // TODO look into the various forms further
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
			__GEN_DEFAULT_THREE_ARG_SIGS__(dcctl);
			void halt(SourceRegister src1);
            void cmpos(SourceRegister src1, SourceRegister src2) noexcept;
            void cmpis(SourceRegister src1, SourceRegister src2) noexcept;
            void cmpob(SourceRegister src1, SourceRegister src2) noexcept;
            void cmpib(SourceRegister src1, SourceRegister src2) noexcept;
			void bswap(SourceRegister src1, DestinationRegister src2) noexcept;
		private:
			// templated bodies
            template<typename T>
            void concmpBase(T src1, T src2) noexcept {
				if (_ac.conditionCodeBitSet<0b100>()) {
                    if (src1 <= src2) {
						_ac.conditionCode = 0b010;
					} else {
						_ac.conditionCode = 0b001;
					}
				}
            }
			template<typename T>
			void concmpBase(SourceRegister src1, SourceRegister src2) noexcept {
                concmpBase<T>(src1.get<T>(), src2.get<T>());
			}
            template<ConditionCode cc>
            constexpr bool conditionCodeIs() const noexcept {
                if constexpr (cc == ConditionCode::Unconditional) {
                    return true;
                } else if constexpr (cc == ConditionCode::False) {
                    return _ac.conditionCodeIs<Ordinal(cc)>();
                } else {
                    return _ac.conditionCodeBitSet<Ordinal(cc)>();
                }
            }
            template<TestTypes t>
            void testGeneric(DestinationRegister dest) noexcept {
                dest.set<Ordinal>(conditionCodeIs<t>() ? 1u : 0u);
            }
            template<ConditionCode cc>
            void branchIfGeneric(Integer displacement) noexcept {
                if (conditionCodeIs<cc>()) {
                    static constexpr auto checkMask = 0x7F'FFFC;
                    union {
                        Integer _value : 24;
                    } conv;
                    conv._value = displacement;
                    conv._value = conv._value > checkMask ? checkMask : conv._value;
                    _instructionPointer += conv._value;
                    _instructionPointer = computeAlignedAddress(_instructionPointer); // make sure the least significant two bits are clear
                }
            }
            template<typename T>
            void compare(T src1, T src2) noexcept {
				// saw this nifty trick from a CppCon talk about 
				// performance improvements. Reduces the number of
				// assignments but also makes modification easier in
				// the future if necessary
				_ac.conditionCode = [src1, src2]() noexcept {
								if (src1 < src2) {
									return 0b100;
								} else if (src1 == src2) {
									return 0b010;
								} else {
									return 0b001;
								}
				}();
            }
			template<ConditionCode code>
			constexpr bool genericCondCheck() noexcept {
                if constexpr (code == ConditionCode::Unconditional) {
                    // we're in a don't care situation so pass the conditional check
                    return true;
                } else {
                    return _ac.conditionCodeBitSet<static_cast<Ordinal>(code)>() || _ac.conditionCodeIs<static_cast<Ordinal>(code)>();
                }
			}
			template<ConditionCode mask>
			void baseSelect(__DEFAULT_THREE_ARGS__) noexcept {
                if (genericCondCheck<mask>()) {
                    dest.set<Ordinal>(src2.get<Ordinal>());
                } else {
                    dest.set<Ordinal>(src1.get<Ordinal>());
                }
			}
            template<ConditionCode mask>
            void baseSelect(const REGFormatInstruction& inst) noexcept {
                setDest(inst, genericCondCheck<mask>() ? getSrc2(inst) : getSrc1(inst));
            }
			template<ConditionCode mask>
			void addoBase(const REGFormatInstruction& inst) noexcept {
                if (genericCondCheck<mask>()) {
                    setDest(inst, getSrc1(inst) +
                                  getSrc2(inst));
				}
			}
            template<ConditionCode mask>
            void addiBase(const REGFormatInstruction& inst) noexcept {
                auto s1 = getSrc1<Integer>(inst);
                auto s2 = getSrc2<Integer>(inst);
                if (genericCondCheck<mask>()) {
                    setDest<Integer>(inst, s1 + s2);
                }
				// according to the docs, the arithmetic overflow always is
				// computed even if the addition is not performed
                if ((getMostSignificantBit(s1) == getMostSignificantBit(s2)) && 
                    (getMostSignificantBit(s2) != getMostSignificantBit(getSrc<Integer>(inst)))) {
					if (_ac.integerOverflowMask == 1) {
						_ac.integerOverflowFlag = 1;
					} else {
						generateFault(ArithmeticFaultSubtype::IntegerOverflow);
					}
				}
			}
            template<ConditionCode mask>
            void suboBase(const REGFormatInstruction& inst) noexcept {
                if (genericCondCheck<mask>()) {
                    setDest(inst, getSrc2<Ordinal>(inst) - getSrc1<Ordinal>(inst));
                }
            }
			template<ConditionCode mask>
			void subiBase(const REGFormatInstruction& inst) noexcept {
                auto s1 = getSrc1<Integer>(inst);
                auto s2 = getSrc2<Integer>(inst);
				if (genericCondCheck<mask>()) {
                    setDest<Integer>(inst, s2 - s1);
				}
				// according to the docs, the arithmetic overflow always is
				// computed even if the subtraction is not performed
                if ((getMostSignificantBit(s2) != getMostSignificantBit(s1)) &&
                    (getMostSignificantBit(s2) != getMostSignificantBit(getSrc<Integer>(inst)))) {
					if (_ac.integerOverflowMask == 1) {
						_ac.integerOverflowFlag = 1;
					} else {
						generateFault(ArithmeticFaultSubtype::IntegerOverflow);
					}
                }
			}
		private:
            template<ConditionCode code>
            void genericFault() noexcept {
                if (conditionCodeIs<code>()) {
                    generateFault(ConstraintFaultSubtype::Range);
                }
            }
			// auto generated routines
#define X(kind, __) \
            void b ## kind (Integer) noexcept;
#include "conditional_kinds.def"
#undef X
		private:
			void dispatch(const Instruction& decodedInstruction) noexcept;
            bool cycle();
            Instruction readInstruction();
        private:
            void saveFrame() noexcept;
            void allocateNewFrame() noexcept;
            void saveLocalRegisters() noexcept;
            void allocateNewLocalRegisterSet() noexcept;
            void freeCurrentRegisterSet() noexcept;
            bool registerSetNotAllocated(const Operand& fp) noexcept;
            void retrieveFromMemory(const Operand& fp) noexcept;
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
			// active by default but needs to be setup by processPrcb
			bool unalignedFaultEnabled = true;
            NormalRegister _temporary0;
            NormalRegister _temporary1;
            NormalRegister _temporary2;
            bool _frameAvailable = true;
    };

}
#undef __TWO_SOURCE_REGS__
#undef __GEN_DEFAULT_THREE_ARG_SIGS__
#undef __DEFAULT_THREE_ARGS__
#undef __DEFAULT_DOUBLE_WIDE_THREE_ARGS__
#undef __DEFAULT_TWO_ARGS__
#undef __DEFAULT_DOUBLE_WIDE_TWO_ARGS__

#endif // end I960_CORE_H__
