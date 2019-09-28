#ifndef I960_CORE_H__
#define I960_CORE_H__
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
	struct FaultAssociation< type > final { \
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

    constexpr Ordinal unusedAddressBits = 0b11;
	constexpr Ordinal clearLowestTwoBitsMask = ~unusedAddressBits;
    constexpr Ordinal computeAlignedAddress(Ordinal value) noexcept {
        return value & clearLowestTwoBitsMask;
    }
    constexpr bool isAlignedAddress(Ordinal value) noexcept {
        return (value & unusedAddressBits) == 0;
    }

    constexpr Ordinal lowestThreeBitsOfMajorOpcode(OpcodeValue v) noexcept {
        return (v & 0b111'0000) >> 4;
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
                    if constexpr (std::is_same_v<K, LongOrdinal>) {
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
                    getDest(srcDest).set<T>(value);
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
            void performOperation(const T&, std::monostate) noexcept {
                generateFault(OperationFaultSubtype::InvalidOpcode);
            }

            template<typename T>
            void dispatchOperation(const T& inst, const typename T::OpcodeList& targetInstruction) noexcept {
                std::visit([&inst, this](auto&& value) { return performOperation(inst, value); }, targetInstruction);
            }
            template<typename T>
            void dispatchOperation(const T& inst) noexcept {
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
#define cobr(name, code, __) //X(name, code, COBR)
#define ctrl(name, code, __) //X(name, code, CTRL)
#include "opcodes.def"
#undef X
#undef reg
#undef mem
#undef cobr
#undef ctrl
        private:
            // COBR format decls
            using TestOperation = std::variant<Operation::testg,
                                               Operation::teste,
                                               Operation::testge,
                                               Operation::testl,
                                               Operation::testne,
                                               Operation::testle,
                                               Operation::testo>;
            using CompareOrdinalAndBranchOperation = std::variant<Operation::cmpobg, 
                                                                  Operation::cmpobe, 
                                                                  Operation::cmpobge, 
                                                                  Operation::cmpobl,
                                                                  Operation::cmpobne,
                                                                  Operation::cmpoble>;
            using CompareIntegerAndBranchOperation = std::variant<Operation::cmpibg, 
                                                                  Operation::cmpibe, 
                                                                  Operation::cmpibge, 
                                                                  Operation::cmpibl,
                                                                  Operation::cmpibne,
                                                                  Operation::cmpible,
                                                                  Operation::cmpibo, // always branches
                                                                  Operation::cmpibno>; // never branches
            void performOperation(const COBRFormatInstruction& inst, Operation::bbc) noexcept;
            void performOperation(const COBRFormatInstruction& inst, Operation::bbs) noexcept;
            void performOperation(const COBRFormatInstruction&, Operation::testno) noexcept;
            void performOperation(const COBRFormatInstruction&, TestOperation) noexcept;
            void performOperation(const COBRFormatInstruction&, CompareOrdinalAndBranchOperation) noexcept;
            void performOperation(const COBRFormatInstruction&, CompareIntegerAndBranchOperation) noexcept;
        private:
            // CTRL format instructions
            using ConditionalBranchOperation = std::variant<Operation::bg,
                                                            Operation::be,
                                                            Operation::bge,
                                                            Operation::bl,
                                                            Operation::bne,
                                                            Operation::ble,
                                                            Operation::bo>;
            using FaultOperation = std::variant<Operation::faultg,
                                                Operation::faulte,
                                                Operation::faultge,
                                                Operation::faultl,
                                                Operation::faultne,
                                                Operation::faultle,
                                                Operation::faulto>;
            void performOperation(const CTRLFormatInstruction&, Operation::b) noexcept;
            void performOperation(const CTRLFormatInstruction&, Operation::call) noexcept;
            void performOperation(const CTRLFormatInstruction&, Operation::ret) noexcept;
            void performOperation(const CTRLFormatInstruction&, Operation::bal) noexcept;
            void performOperation(const CTRLFormatInstruction&, Operation::bno) noexcept;
            void performOperation(const CTRLFormatInstruction&, Operation::faultno) noexcept;
            void performOperation(const CTRLFormatInstruction&, ConditionalBranchOperation) noexcept;
            void performOperation(const CTRLFormatInstruction&, FaultOperation) noexcept;

        private:
            void syncf() noexcept;
		private:
			// templated bodies
            template<typename T>
            void concmpBase(T src1, T src2) noexcept {
				if (_ac.conditionCodeBitSet<0b100>()) {
                    if (src1 <= src2) {
                        _ac.setConditionCode(0b010);
					} else {
                        _ac.setConditionCode(0b001);
					}
				}
            }
            template<ConditionCode cc>
            constexpr bool conditionCodeIs() const noexcept {
                if constexpr (cc == ConditionCode::Unconditional) {
                    return true;
                } else if constexpr (cc == ConditionCode::False) {
                    return _ac.conditionCodeIs<static_cast<Ordinal>(cc)>();
                } else {
                    return _ac.conditionCodeBitSet<static_cast<Ordinal>(cc)>();
                }
            }
            template<typename T>
            void compare(T src1, T src2) noexcept {
				// saw this nifty trick from a CppCon talk about 
				// performance improvements. Reduces the number of
				// assignments but also makes modification easier in
				// the future if necessary
                _ac.setConditionCode([src1, src2]() noexcept {
								if (src1 < src2) {
									return 0b100;
								} else if (src1 == src2) {
									return 0b010;
								} else {
									return 0b001;
								}
				}());
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
                    if (_ac.maskIntegerOverflow()) {
                        _ac.setIntegerOverflowFlag(true);
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
                    if (_ac.maskIntegerOverflow()) {
                        _ac.setIntegerOverflowFlag(true);
					} else {
						generateFault(ArithmeticFaultSubtype::IntegerOverflow);
					}
                }
			}
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

} // end namespace i960
#endif // end I960_CORE_H__
