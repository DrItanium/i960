#ifndef I960_CORE_H__
#define I960_CORE_H__
#include "types.h"
#include "NormalRegister.h"
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
        return InverseOfLowestTwoBitsPattern<Ordinal>::decodePattern(value);
    }
    constexpr bool isAlignedAddress(Ordinal value) noexcept {
        return LowestTwoBitsPattern<Ordinal>::decodePattern(value) == 0;
    }
    using LowestThreeBitsOfMajorOpcode = BitFragment<OpcodeValue, Ordinal, 0b111'0000, 4>;
    using FramePointerAddress = SameWidthFragment<Ordinal, static_cast<Ordinal>(~0b111'111)>;
    using ProcedureKindField = LowestTwoBitsPattern<Ordinal>;


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
                        return getDoubleRegister(op).get();
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
                if constexpr (std::is_same_v<K, LongOrdinal>) {
                    getDoubleRegister(srcDest.getSrcDest()).set(value);
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
            PreviousFramePointer getPFP() noexcept;
            Ordinal getStackPointerAddress() const noexcept;
            void setFramePointer(Ordinal value) noexcept;
            Ordinal getFramePointerAddress() const noexcept;
			void generateFault(ByteOrdinal faultType, ByteOrdinal faultSubtype = 0);
			template<typename T>
			void generateFault(T faultSubtype) {
				generateFault(static_cast<ByteOrdinal>(FaultAssociation<T>::ParentFaultType), static_cast<ByteOrdinal>(faultSubtype));
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
                return LowestThreeBitsOfMajorOpcode::decodePattern(value);

            }
            static constexpr Ordinal getConditionalSubtractMask(OpcodeValue value) noexcept {
                // mask is in bits 4-6 of the opcode in reg format so it is the
                // lowest three bits of the opcode like before.
                return LowestThreeBitsOfMajorOpcode::decodePattern(value);
            }
            static constexpr Ordinal getSelectMask(OpcodeValue value) noexcept {
                // mask is in bits 4-6 of the opcode in reg format so it is the
                // lowest three bits of the opcode like before.
                return LowestThreeBitsOfMajorOpcode::decodePattern(value);
            }
            template<typename T, std::enable_if_t<IsSelectOperation<std::decay_t<T>>, int> = 0> 
            void performOperation(const REGFormatInstruction& inst, T) noexcept {
                if (auto mask = getSelectMask(inst.getOpcode()); (mask & _ac.getConditionCode()) || (mask == _ac.getConditionCode())) {
                    setDest(inst, getSrc2(inst));
                } else {
                    setDest(inst, getSrc1(inst));
                }
            }

            template<typename T, std::enable_if_t<IsAddOperation<T> ||
                                                  IsSubtractOperation<T> ||
                                                  IsMultiplyOperation<T> ||
                                                  IsDivideOperation<T> ||
                                                  IsRemainderOperation<T> ||
                                                  IsModuloOperation<T>, int> = 0>
            void performOperation(const REGFormatInstruction& inst, T) noexcept {
                static_assert(IsIntegerOperation<T> || IsOrdinalOperation<T>, "Unimplemented unconditional add/subtract operation!");
                using K = std::conditional_t<IsIntegerOperation<T>, Integer, Ordinal>;
                bool updateDestination = true;
                K result = 0;
                auto s2 = getSrc2<K>(inst);
                auto s1 = getSrc1<K>(inst);
                if constexpr (IsAddOperation<T>) {
                    if constexpr (IsAddWithCarryOperation<T>) {
                        LongOrdinal intermediate = static_cast<LongOrdinal>(s1) + static_cast<LongOrdinal>(s2) + static_cast<LongOrdinal>(_ac.getCarryValue());
                        _ac.clearConditionCode();
                        auto destReg = getRegister(inst.getSrcDest());
                        // detect overflow
                        if (auto msb2 = getMostSignificantBit(s2) ; (msb2 == getMostSignificantBit(s1)) && (msb2 != destReg.mostSignificantBit())) {
                            _ac.setConditionCode(_ac.getConditionCode() | 0b001);
                        }
                        // mark carry out bit
                        _ac.setConditionCode(_ac.getConditionCode() | ((result & 0x1'0000'0000) ? 0b010 : 0b000));
                        result = static_cast<Ordinal>(intermediate); 
                    } else {
                        result = s2 + s1;
                        if constexpr (IsConditionalAddOperation<T>) {
                            auto mask = getConditionalAddMask(inst.getOpcode());
                            updateDestination = (mask & _ac.getConditionCode()) || (mask == _ac.getConditionCode());
                        }
                    }
                } else if constexpr (IsSubtractOperation<T>) {
                    result = s2 - s1;
                    if constexpr (IsConditionalSubtractOperation<T>) {
                        auto mask = getConditionalSubtractMask(inst.getOpcode());
                        updateDestination = (mask & _ac.getConditionCode()) || (mask == _ac.getConditionCode());
                    }
                } else if constexpr (IsMultiplyOperation<T>) {
                    if constexpr (std::is_same_v<std::decay_t<T>, Operation::emul>) {
                        if (inst.getSrcDest().notDivisibleBy(2)) {
                            setDest<LongOrdinal>(inst, -1);
                            generateFault(OperationFaultSubtype::InvalidOperand);
                        } else {
                            setDest<LongOrdinal>(inst, static_cast<LongOrdinal>(s2) * static_cast<LongOrdinal>(s1));
                            return;
                        }
                    } else {
                        result = s2 * s1;
                    }
                } else if constexpr (IsDivideOperation<T>) {
                    if (auto denominator = s1; denominator == 0) {
                        if constexpr (IsIntegerOperation<T>) {
                            result = -1; // undefined value
                        } else {
                            updateDestination = false;
                        }
                        generateFault(ArithmeticFaultSubtype::ZeroDivide);
                    } else {
                        auto numerator = s2;
                        if constexpr (IsIntegerOperation<T>) {
                            if (numerator == static_cast<Integer>(0x8000'0000) && denominator == -1) {
                                result = 0x8000'0000;
                                if (_ac.maskIntegerOverflow()) {
                                    _ac.setIntegerOverflowFlag(true);
                                } else {
                                    generateFault(ArithmeticFaultSubtype::IntegerOverflow);
                                }
                            }
                        }
                        result = numerator / denominator;
                        //setDest<K>(inst, numerator / denominator);
                    }
                } else if constexpr (IsRemainderOperation<T>) {
                    if (auto denominator = s1; denominator == 0) {
                        generateFault(ArithmeticFaultSubtype::ZeroDivide);
                        updateDestination = false;
                    } else {
                        auto numerator = s2;
                        result = numerator - (denominator / numerator) * denominator;
                    }
                } else if constexpr (IsModuloOperation<T>) {
                    if (s1 == 0) {
                        result = -1; // says in the manual, to assign it to an undefined value
                        generateFault(ArithmeticFaultSubtype::ZeroDivide);
                    } else {
                        result = s2 - (s2 / s1) * s1;
                        if ((s2 * s1) < 0) {
                            result = result + s1;
                        }
                    }
                } else {
                    static_assert(false_v<T>, "Unimplemented operation");
                }
                if (updateDestination) {
                    setDest<K>(inst, result);
                }
                if constexpr (CheckForOverflow<T>) {
                    if ((getMostSignificantBit(s1) == getMostSignificantBit(s2)) && 
                            (getMostSignificantBit(s2) != getMostSignificantBit(getSrc<Integer>(inst)))) {
                        if (_ac.maskIntegerOverflow()) {
                            _ac.setIntegerOverflowFlag(true);
                        } else {
                            generateFault(ArithmeticFaultSubtype::IntegerOverflow);
                        }
                    }
                }
            }
            template<typename T, std::enable_if_t<IsCompareOperation<T>, int> = 0>
            void performOperation(const REGFormatInstruction& inst, T) noexcept {
                static_assert(IsIntegerOperation<T> || IsOrdinalOperation<T>, "Unimplemented unconditional add/subtract operation!");
                if constexpr (IsByteCompareOperation<T>) {
                    using K = std::conditional_t<IsIntegerOperation<T>, ByteInteger, ByteOrdinal>;
                    compare<K>(getSrc1<K>(inst), getSrc2<K>(inst));
                } else if constexpr (IsShortCompareOperation<T>) {
                    using K = std::conditional_t<IsIntegerOperation<T>, ShortInteger, ShortOrdinal>;
                    compare<K>(getSrc1<K>(inst), getSrc2<K>(inst));
                } else if constexpr (IsPureConditionalCompare<T>) {
                    using K = std::conditional_t<IsIntegerOperation<T>, Integer, Ordinal>;
                    if (_ac.conditionCodeBitSet<0b100>()) {
                        _ac.setConditionCode(getSrc1<K>(inst) <= getSrc2<K>(inst), 0b010, 0b001);
                    }
                } else {
                    using K = std::conditional_t<IsIntegerOperation<T>, Integer, Ordinal>;
                    compare<K>(getSrc1<K>(inst), getSrc2<K>(inst));
                    if constexpr (IsCompareAndDecrementOperation<T>) {
                        setDest<K>(inst, getSrc2<K>(inst) - 1);
                    }
                    if constexpr (IsCompareAndIncrementOperation<T>) {
                        setDest<K>(inst, getSrc2<K>(inst) + 1); // overflow suppressed
                    }
                }
            }
            template<typename T, std::enable_if_t<IsShiftOperation<T>, int>  = 0>
            void performOperation(const REGFormatInstruction& inst, T) noexcept {
                static_assert(IsIntegerOperation<T> || IsOrdinalOperation<T>, "Unimplemented unconditional add/subtract operation!");
                using K = std::conditional_t<IsIntegerOperation<T>, Integer, Ordinal>;
                /// @todo shli generates an overflow fault, see 80960Jx manual for implementation details
                /// @todo use the 80960Jx manual impls for shli and shri which are much slower and at most 32 cycles
                auto s1 = getSrc1<K>(inst);
                auto s2 = getSrc2<K>(inst);
                if constexpr (IsIntegerOperation<T>) {
                    // make sure we don't shift by a negative number
                    s1 = std::abs(s1);
                    if (s1 > 32) {
                        s1 = 32u;
                    } 
                } else if constexpr (IsOrdinalOperation<T>) {
                    if (s1 >= 32u) {
                        // terminate early if we will shift all bits out and just
                        // set destination to zero
                        setDest<K>(inst, 0u);
                        return;
                    }
                } else {
                    static_assert(false_v<T>, "Shift operation neither ordinal or integer!");
                }
                K result = 0;
                if constexpr (IsShiftRightOperation<T>) {
                    result = s2 >> s1;
                } else if constexpr (IsShiftLeftOperation<T>) {
                    result = s2 << s1;
                } else {
                    static_assert(false_v<T>, "Unimplemented shift operation!");
                }
                setDest<K>(inst, result);
            }
            static constexpr Ordinal oneShiftLeft(Ordinal position) noexcept {
                return 1u << (0b11111 & position);
            }
            template<typename T, std::enable_if_t<IsBitManipulationOperation<T>, int> = 0>
            void performOperation(const REGFormatInstruction& inst, T) noexcept {
                auto s2 = getSrc2(inst);
                auto s1 = getSrc1(inst);
                Ordinal result = 0;
                if constexpr (OneShiftLeftSrc1<T>) {
                    // this comes first to support clrbit
                    s1 = oneShiftLeft(s1);
                }
                if constexpr (InvertSrc1<T>) {
                    s1 = ~s1;
                }
                if constexpr (InvertSrc2<T>) {
                    s2 = ~s2;
                }
                if constexpr (IsAndOperation<T>) {
                    result = s2 & s1;
                } else if constexpr (IsOrOperation<T>) {
                    result = s2 | s1;
                } else if constexpr (IsXorOperation<T>) {
                    if constexpr (std::is_same_v<T, Operation::opxor>){
                        // there is an actual implementation within the manual so I'm going to
                        // use that instead of the xor operator.
                        result = (s2 | s1) & ~(s2 & s1);
                    } else if constexpr (std::is_same_v<T, Operation::xnor>){
                        // there is an actual implementation within the manual so I'm going to
                        // use that instead of the xor operator.
                        result = ~(s2 | s1) | (s2 & s1);
                    } else {
                        result = s2 ^ s1;
                    }
                } else if constexpr (IsNotOperation<T>) {
                    if constexpr(IsUnaryOperation<T>) {
                        result = ~s1;
                    } else {
                        static_assert(false_v<T>, "Unsure what to do right now");
                    }
                } else if constexpr (std::is_same_v<std::decay_t<T>, Operation::alterbit>) {
                    if ((_ac.getConditionCode() & 0b010) == 0) {
                        // if the condition bit is clear then we clear the given bit
                        performOperation<Operation::clrbit>(inst, {});
                    } else {
                        // if the condition bit is set then we set the given bit
                        performOperation<Operation::setbit>(inst, {});
                    }
                    return;
                } else {
                    static_assert(false_v<T>, "Unimplemented bit manipulation operation");
                }
                setDest(inst, result);

            }
#define X(title) void performOperation(const REGFormatInstruction& inst, title ) noexcept
            X(Operation::inten);
            X(Operation::intdis);
            X(Operation::sysctl);
            X(Operation::icctl);
            X(Operation::dcctl);
            X(Operation::intctl);
            X(Operation::bswap);
            X(Operation::halt);
            X(Operation::mark);
            X(Operation::fmark);
            X(Operation::atadd);
            X(Operation::atmod);
            X(Operation::scanbyte);
            X(Operation::rotate);
            X(Operation::modac);
            X(Operation::modpc);
            X(Operation::modtc);
            X(Operation::flushreg);
            X(Operation::syncf);
            X(Operation::calls);
            X(Operation::mov);
            X(Operation::movl);
            X(Operation::movt);
            X(Operation::movq);

            X(Operation::subc);
            X(Operation::eshro);
            X(Operation::ediv);
            X(Operation::shrdi);

            X(Operation::extract);
            X(Operation::modify);
            X(Operation::chkbit);
            X(Operation::spanbit);
            X(Operation::scanbit);
#undef X


        private:
            // COBR format decls
            static constexpr Ordinal computeCheckBitMask(Ordinal value) noexcept {
                return 1 << (value & 0b11111);
            }
            template<typename T, std::enable_if_t<IsCOBRFormat<T>, int> = 0>
            void performOperation(const COBRFormatInstruction& inst, T) noexcept {
                using K = std::decay_t<T>;
                if constexpr (IsTestOperation<K>) {
                    if constexpr (std::is_same_v<K, Operation::testno>) {
                        setDest(inst, _ac.getConditionCode() == 0b000 ? 1 : 0);
                    } else {
                        setDest(inst, ((LowestThreeBitsOfMajorOpcode::decodePattern(inst.getOpcode()) & _ac.getConditionCode()) != 0) ? 1 : 0);
                    }
                } else if constexpr (IsCompareOrdinalAndBranchOperation<K> ||
                                     IsCompareIntegerAndBranchOperation<K>) {
                    static_assert(IsIntegerOperation<T> || IsOrdinalOperation<T>, "Unimplemented unconditional add/subtract operation!");
                    using Z = std::conditional_t<IsIntegerOperation<T>, Integer, Ordinal>;
                    // use variants as a side effect :D
                    compare<Z>(getSrc(inst), getSrc2(inst));
                    if (auto mask = LowestThreeBitsOfMajorOpcode::decodePattern(inst.getOpcode()); (mask & _ac.getConditionCode()) != 0) {
                        _instructionPointer += (inst.getDisplacement() * 4);
                        _instructionPointer = computeAlignedAddress(_instructionPointer);
                    }
                } else if constexpr (IsCheckBitAndBranchIfOperation<T>) {
                    auto bitpos = getSrc(inst);
                    auto src = getSrc2(inst);
                    auto mask = computeCheckBitMask(bitpos);
                    // bbc = check bit and branch if clear
                    // bbs = check bit and branch if set
                    constexpr Ordinal startingConditionCode = std::is_same_v<std::decay_t<T>, Operation::bbc> ? 0b010 : 0b000;
                    constexpr Ordinal conditionCodeOnConditionMet = std::is_same_v<std::decay_t<T>, Operation::bbc> ? 0b000 : 0b010;
                    constexpr Ordinal compareAgainst = std::is_same_v<std::decay_t<T>, Operation::bbc> ? 0 : 1;
                    _ac.setConditionCode(startingConditionCode);
                    if ((src & mask) == compareAgainst) {
                        _ac.setConditionCode(conditionCodeOnConditionMet);
                        _instructionPointer = _instructionPointer + inst.getDisplacement();
                        // clear the lowest two bits of the instruction pointer
                        _instructionPointer &= (~0b11);
                    }
                } else {
                    static_assert(false_v<K>, "Unimplemented cobr operation!");
                }
            }
        private:
            // CTRL format instructions
            void performOperation(const CTRLFormatInstruction&, Operation::b) noexcept;
            void performOperation(const CTRLFormatInstruction&, Operation::call) noexcept;
            void performOperation(const CTRLFormatInstruction&, Operation::ret) noexcept;
            void performOperation(const CTRLFormatInstruction&, Operation::bal) noexcept;
            template<typename T, std::enable_if_t<IsCTRLFormat<T>, int> = 0> 
            void performOperation(const CTRLFormatInstruction& inst, T) noexcept {
                using K = std::decay_t<T>;
                auto mask = LowestThreeBitsOfMajorOpcode::decodePattern(inst.getOpcode());
                if constexpr (IsConditionalBranchOperation<K>) {
                    auto tmp = static_cast<decltype(_instructionPointer)>(inst.getDisplacement());
                    bool condition = false;
                    if constexpr (std::is_same_v<K, Operation::bno>) {
                        condition = _ac.getConditionCode() == 0;
                    } else {
                        condition =  (mask & _ac.getConditionCode()) || (mask == _ac.getConditionCode());
                    }
                    if (condition) {
                        _instructionPointer = computeAlignedAddress(tmp + _instructionPointer);
                    }
                } else if constexpr (IsFaultOperation<K>) {
                    auto condition = false;
                    if constexpr (std::is_same_v<std::decay_t<T>, Operation::faultno>) {
                        condition = _ac.getConditionCode() == 0b000;
                    } else {
                        condition = (mask && _ac.getConditionCode()) != 0b000;
                    }
                    if (condition) {
                        generateFault(ConstraintFaultSubtype::Range);
                    }
                } else {
                    static_assert(false_v<K>, "Unimplemented ctrl format instruction");
                }
            }

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
            constexpr Ordinal computeAlignmentBoundaryConstant() const noexcept {
                // on the i960 MC the constant is four,
                // on the jx it is 1
                return (_deviceId.getSalign() * 16) - 1;
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
            TraceControls getTraceControls() noexcept { return _tc.viewAs<TraceControls>(); }
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
            NormalRegister _tc;
            //TraceControls _tc;
			MemoryInterface& _mem;
			Ordinal _prcbAddress;
			Ordinal _ctrlTable;
			// the first 1024 bytes of ram is a internal data ram cache
			// which can be read from and written to but not executed from
			const CoreInformation& _deviceId;
			// active by default but needs to be setup by processPrcb
			bool unalignedFaultEnabled = true;
            bool _frameAvailable = true;
    };

} // end namespace i960
#endif // end I960_CORE_H__
