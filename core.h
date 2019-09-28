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
        private:
            // MEM format instructions
            void performOperation(const MEMFormatInstruction& inst, Operation::bx)    noexcept;
            void performOperation(const MEMFormatInstruction& inst, Operation::balx)  noexcept;
            void performOperation(const MEMFormatInstruction& inst, Operation::callx) noexcept;
            void performOperation(const MEMFormatInstruction& inst, Operation::lda)   noexcept;
            void performOperation(const MEMFormatInstruction& inst, Operation::ld)    noexcept;
            void performOperation(const MEMFormatInstruction& inst, Operation::ldl)   noexcept;
            void performOperation(const MEMFormatInstruction& inst, Operation::ldt)   noexcept;
            void performOperation(const MEMFormatInstruction& inst, Operation::ldq)   noexcept;
            void performOperation(const MEMFormatInstruction& inst, Operation::ldos)  noexcept;
            void performOperation(const MEMFormatInstruction& inst, Operation::ldis)  noexcept;
            void performOperation(const MEMFormatInstruction& inst, Operation::ldob)  noexcept;
            void performOperation(const MEMFormatInstruction& inst, Operation::ldib)  noexcept;
            void performOperation(const MEMFormatInstruction& inst, Operation::st)    noexcept;
            void performOperation(const MEMFormatInstruction& inst, Operation::stl)   noexcept;
            void performOperation(const MEMFormatInstruction& inst, Operation::stt)   noexcept;
            void performOperation(const MEMFormatInstruction& inst, Operation::stq)   noexcept;
            void performOperation(const MEMFormatInstruction& inst, Operation::stos)  noexcept;
            void performOperation(const MEMFormatInstruction& inst, Operation::stis)  noexcept;
            void performOperation(const MEMFormatInstruction& inst, Operation::stob)  noexcept;
            void performOperation(const MEMFormatInstruction& inst, Operation::stib)  noexcept;

        private:
            // REG format instructions
            static constexpr Ordinal getConditionalAddMask(OpcodeValue value) noexcept {
                // mask is in bits 4-6 of the opcode in reg format so it is the
                // lowest three bits of the opcode like before.
                return lowestThreeBitsOfMajorOpcode(value);
            }
            static constexpr Ordinal getConditionalSubtractMask(OpcodeValue value) noexcept {
                // mask is in bits 4-6 of the opcode in reg format so it is the
                // lowest three bits of the opcode like before.
                return lowestThreeBitsOfMajorOpcode(value);
            }
            static constexpr Ordinal getSelectMask(OpcodeValue value) noexcept {
                // mask is in bits 4-6 of the opcode in reg format so it is the
                // lowest three bits of the opcode like before.
                return lowestThreeBitsOfMajorOpcode(value);
            }
            using ConditionalAddOrdinalOperation = std::variant<Operation::addono,
                                                                Operation::addog,
                                                                Operation::addoe,
                                                                Operation::addoge,
                                                                Operation::addol,
                                                                Operation::addone,
                                                                Operation::addole,
                                                                Operation::addoo>;
            using ConditionalAddIntegerOperation = std::variant<Operation::addino,
                                                                Operation::addig,
                                                                Operation::addie,
                                                                Operation::addige,
                                                                Operation::addil,
                                                                Operation::addine,
                                                                Operation::addile,
                                                                Operation::addio>;
            using ConditionalSubtractOrdinalOperation = std::variant<Operation::subono,
                                                                Operation::subog,
                                                                Operation::suboe,
                                                                Operation::suboge,
                                                                Operation::subol,
                                                                Operation::subone,
                                                                Operation::subole,
                                                                Operation::suboo>;
            using ConditionalSubtractIntegerOperation = std::variant<Operation::subino,
                                                                Operation::subig,
                                                                Operation::subie,
                                                                Operation::subige,
                                                                Operation::subil,
                                                                Operation::subine,
                                                                Operation::subile,
                                                                Operation::subio>;
            using SelectOperation = std::variant<Operation::selno,
                                                 Operation::selg,
                                                 Operation::sele,
                                                 Operation::selge,
                                                 Operation::sell,
                                                 Operation::selne,
                                                 Operation::selle,
                                                 Operation::selo>;

#define X(title) void performOperation(const REGFormatInstruction& inst, title ) noexcept
            X(SelectOperation);
            X(ConditionalAddIntegerOperation);
            X(ConditionalAddOrdinalOperation);
            X(ConditionalSubtractIntegerOperation);
            X(ConditionalSubtractOrdinalOperation);
            X(Operation::inten);
            X(Operation::intdis);
            X(Operation::sysctl);
            X(Operation::icctl);
            X(Operation::dcctl);
            X(Operation::intctl);
            X(Operation::eshro);
            X(Operation::cmpib); 
            X(Operation::cmpob);
            X(Operation::cmpis); 
            X(Operation::cmpos);
            X(Operation::cmpi);  
            X(Operation::cmpo);
            X(Operation::bswap);
            X(Operation::halt);
            X(Operation::opxor);
            X(Operation::nand);
            X(Operation::xnor);
            X(Operation::mark);
            X(Operation::fmark);
            X(Operation::emul);
            X(Operation::setbit);
            X(Operation::chkbit);
            X(Operation::cmpdeco); 
            X(Operation::cmpdeci);
            X(Operation::concmpi); 
            X(Operation::concmpo);
            X(Operation::atadd);
            X(Operation::atmod);
            X(Operation::scanbyte);
            X(Operation::modify);
            X(Operation::clrbit);
            X(Operation::cmpinci); 
            X(Operation::cmpinco);
            X(Operation::rotate);
            X(Operation::shrdi);
            X(Operation::shli); 
            X(Operation::shlo);
            X(Operation::shri);
            X(Operation::shro);
            X(Operation::nor);
            X(Operation::ornot);
            X(Operation::notor);
            X(Operation::opor);
            X(Operation::notbit);
            X(Operation::notand);
            X(Operation::opnot);
            X(Operation::addc);
            X(Operation::modac);
            X(Operation::modpc);
            X(Operation::subi);
            X(Operation::subo);
            X(Operation::muli);
            X(Operation::mulo);
            X(Operation::divi);
            X(Operation::divo);
            X(Operation::modtc);
            X(Operation::remi);
            X(Operation::remo);
            X(Operation::ediv);
            X(Operation::flushreg);
            X(Operation::syncf);
            X(Operation::modi);
            X(Operation::spanbit);
            X(Operation::scanbit);
            X(Operation::calls);
            X(Operation::addo);
            X(Operation::addi);
            X(Operation::alterbit);
            X(Operation::opand);
            X(Operation::andnot);
            X(Operation::mov);
            X(Operation::movl);
            X(Operation::movt);
            X(Operation::movq);
            X(Operation::extract);
            X(Operation::subc);
#undef X
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
