#include "types.h"
#include "core.h"
#include "NormalRegister.h"
#include "DoubleRegister.h"
#include "TripleRegister.h"
#include "QuadRegister.h"
#include "opcodes.h"
#include "Operand.h"
#include "InternalDataRam.h"
#include <limits>
#include <cmath>
#define __DEFAULT_THREE_ARGS__ SourceRegister src1, SourceRegister src2, DestinationRegister dest
#define __DEFAULT_DOUBLE_WIDE_THREE_ARGS__ const DoubleRegister& src1, const DoubleRegister& src2, DoubleRegister& dest
#define __DEFAULT_TWO_ARGS__ SourceRegister src, DestinationRegister dest
#define __DEFAULT_DOUBLE_WIDE_TWO_ARGS__ const DoubleRegister& src, DoubleRegister& dest
#define __TWO_SOURCE_AND_INT_ARGS__ SourceRegister src1, SourceRegister src2, Integer targ
#define __TWO_SOURCE_REGS__ SourceRegister src1, SourceRegister src2
namespace i960 {
	constexpr Ordinal xorOperation(Ordinal src2, Ordinal src1) noexcept {
		return (src2 | src1) & ~(src2 & src1);
	}
	constexpr Ordinal oneShiftLeft(Ordinal position) noexcept {
		return 1u << (0b11111 & position);
	}
    template<Ordinal index>
    constexpr Ordinal RegisterIndex = index * sizeof(Ordinal);
	constexpr bool notDivisibleBy(ByteOrdinal value, ByteOrdinal factor) noexcept {
		return ((value % factor) != 0);
	}
    constexpr Ordinal computeNextFrameStart(Ordinal currentAddress) noexcept {
        // add 1 to the masked out value to make sure we don't overrun anything
        return (currentAddress & ~(Ordinal(0b11111))) + 1; // next 64 byte frame start
    }
    constexpr Ordinal computeStackFrameStart(Ordinal framePointerAddress) noexcept {
        return framePointerAddress + 64;
    }
#define X(kind, action) \
	void Core:: b ## kind (Integer addr) noexcept { branchIfGeneric<ConditionCode:: action > ( addr ) ; } \
    void Core:: performOperation(const REGFormatInstruction& inst, Operation:: subi ## kind ) noexcept { subiBase<ConditionCode:: action>(inst); } \
    void Core:: performOperation(const REGFormatInstruction& inst, Operation:: subo ## kind ) noexcept { suboBase<ConditionCode:: action>(inst); } \
    void Core:: performOperation(const COBRFormatInstruction& inst, Operation:: test ## kind ) noexcept { testGeneric<TestTypes:: action> ( getDest(inst) ); } \
    void Core:: performOperation(const CTRLFormatInstruction&, Operation:: fault ## kind ) noexcept { genericFault<ConditionCode:: action > ( ); } \
    void Core:: performOperation(const CTRLFormatInstruction& inst, Operation:: b ## kind ) noexcept { branchIfGeneric<ConditionCode:: action > ( inst.getDisplacement() ); } \
    void Core:: performOperation(const REGFormatInstruction& inst, Operation :: sel ## kind ) noexcept { baseSelect<ConditionCode:: action>(inst); } \
    void Core:: performOperation(const REGFormatInstruction& inst, Operation:: addo ## kind ) noexcept { addoBase<ConditionCode:: action>(inst) ; } \
    void Core:: performOperation(const REGFormatInstruction& inst, Operation:: addi ## kind ) noexcept { addiBase<ConditionCode:: action>(inst) ; }
#include "conditional_kinds.def"
#undef X
#define CompareOrdinalAndBranch(bop) \
    void Core:: performOperation (const COBRFormatInstruction& inst, Operation:: cmpo ## bop ) noexcept { \
        compare<Ordinal>(getSrc(inst), getSrc2(inst)); \
        bop ( inst.getDisplacement() ); \
    }
#define CompareIntegerAndBranch(bop) \
    void Core:: performOperation (const COBRFormatInstruction& inst, Operation:: cmpi ## bop ) noexcept { \
        compare<Integer>(getSrc<Integer>(inst), getSrc2<Integer>(inst)); \
        bop ( inst.getDisplacement() ); \
    }
CompareOrdinalAndBranch(bg);
CompareOrdinalAndBranch(be);
CompareOrdinalAndBranch(bge);
CompareOrdinalAndBranch(bl);
CompareOrdinalAndBranch(bne);
CompareOrdinalAndBranch(ble);
CompareIntegerAndBranch(bg);
CompareIntegerAndBranch(be);
CompareIntegerAndBranch(bge);
CompareIntegerAndBranch(bl);
CompareIntegerAndBranch(bne);
CompareIntegerAndBranch(ble);
CompareIntegerAndBranch(bo);
CompareIntegerAndBranch(bno);
#undef CompareOrdinalAndBranch
#undef CompareIntegerAndBranch
	Core::Core(const CoreInformation& info, MemoryInterface& mem) : _mem(mem), _deviceId(info) { }
	Ordinal Core::getStackPointerAddress() const noexcept {
        return _localRegisters[SP.getOffset()].get<Ordinal>();
	}
	void Core::saveLocalRegisters() noexcept {
        static constexpr auto LocalRegisterCount = 16;
		auto base = getFramePointerAddress();
		auto end = base + (sizeof(Ordinal) * LocalRegisterCount);

		for (Ordinal addr = base, i = 0; addr < end; addr += sizeof(Ordinal), ++i) {
			store(addr, _localRegisters[i].get<Ordinal>());
		}
	}
	void Core::allocateNewLocalRegisterSet() noexcept {
		// this function does nothing at this point as we are always saving locals to ram
	}
    void Core::allocateNewFrame() noexcept {
        /// @todo implement
    }
    void Core::saveFrame() noexcept {
        /// @todo implement
    }
    Ordinal Core::getSupervisorStackPointerBase() noexcept {
        return getSystemProcedureTableBaseAddress() + 12;
    }
	void Core::setFramePointer(Ordinal value) noexcept {
		_globalRegisters[FP.getOffset()].set(value);
	}
	Ordinal Core::getFramePointerAddress() const noexcept {
		return _globalRegisters[FP.getOffset()].get<Ordinal>() & (~0b111111);
	}
	auto Core::getPFP() noexcept -> PreviousFramePointer& {
		return _localRegisters[PFP.getOffset()].pfp;
	}
	constexpr Ordinal getProcedureAddress(Ordinal value) noexcept {
        return computeAlignedAddress(value);
	}
	constexpr Ordinal getProcedureKind(Ordinal value) noexcept {
		return value & 0b11;
	}
	constexpr bool isLocalProcedure(Ordinal value) noexcept {
		return getProcedureKind(value) == 0;
	}
	constexpr bool isSupervisorProcedure(Ordinal value) noexcept {
		return getProcedureKind(value) == 0b10;
	}
    void Core::performOperation(const REGFormatInstruction& inst, Operation::calls) noexcept {
        constexpr Ordinal SALIGN = 1u;
        syncf();
        auto targ = getSrc1<ShortOrdinal>(inst);
		if (targ > 259) {
			generateFault(ProtectionFaultSubtype::Length);
			return;
		}
        NormalRegister tempPE(load(getSystemProcedureTableBaseAddress() + 48 + (4 * targ)));
		// TODO reimplement for the i960 jx
        if (_frameAvailable) {
            allocateNewFrame();
        } else {
            saveFrame();
            allocateNewFrame();
            // local register references now refer to new frame
        }
        setRegister(RIP, _instructionPointer);
        _instructionPointer = tempPE.ordinal;
        NormalRegister temp;
        if ((temp.pe.isLocal()) || (_pc.inSupervisorMode())) {
            // local call or supervisor call from supervisor mode
            // in the manual:
            // temp = (SP + (SALIGN*16 -1)) & ~(SALIGN * 16 - 1)
            // where SALIGN=1 on i960 Jx processors
            // round stack pointer to the next boundary
            temp.ordinal = (getStackPointerAddress() + (SALIGN * 16 - 1)) & ~(SALIGN * 16 - 1);
            temp.pfp.returnCode = 0b000;
        } else {
            // supervisor call from user mode
            temp.ordinal = getSupervisorStackPointerBase();
            temp.pfp.returnCode = 0b010 | _pc.traceEnable;
            _pc.enterSupervisorMode();
            _pc.traceEnable = temp.pc.traceEnable;
        }
        setRegister(PFP, getRegister(FP).get<Ordinal>());
        getPFP().returnCode = temp.pfp.returnCode;
        setRegister(FP, temp.ordinal);
        setRegister(SP, temp.ordinal + 64);
	}

	Ordinal Core::load(Ordinal address, bool atomic) noexcept {
		if (address <= _internalDataRam.LargestAddress<0>) {
			return _internalDataRam.read(computeAlignedAddress(address));
		} else {
			return _mem.load(address, atomic);
		}
	}
    LongOrdinal Core::loadDouble(Ordinal address, bool atomic) noexcept {
        if (address <= _internalDataRam.LargestAddress<0>) {
            return _internalDataRam.readDouble(computeAlignedAddress(address));
        } else {
            return _mem.loadDouble(address, atomic);
        }
    }
	void Core::store(Ordinal address, Ordinal value, bool atomic) noexcept {
		if (address <= _internalDataRam.LargestAddress<0>) {
			_internalDataRam.write(computeAlignedAddress(address), value);
		} else {
			_mem.store(address, value, atomic);
		}
	}
	NormalRegister& Core::getRegister(ByteOrdinal index) noexcept {
		if (auto offset = (index & 0b01111) ; (index & 0b10000) == 0) {
			return _localRegisters[offset];
		} else {
			return _globalRegisters[offset];
		}
	}
    void Core::performOperation(const MEMFormatInstruction& inst, Operation::callx) noexcept {
		static constexpr Ordinal boundaryMarker = 63u;
		static constexpr Ordinal boundaryAlignment = 64u;
        // TODO, compute the effective address as needed from the REG
        auto newAddress = getSrc(inst);
		Ordinal tmp = (getStackPointerAddress() + boundaryMarker) && (~boundaryMarker); // round to the next boundary
		setRegister(RIP, _instructionPointer);
		// Code for handling multiple internal local register sets not implemented!
		saveLocalRegisters();
		allocateNewLocalRegisterSet();
		_instructionPointer = newAddress;
		setRegister(PFP, getFramePointerAddress());
		setFramePointer(tmp);
		setRegister(SP, tmp + boundaryAlignment);
	}
    void Core::performOperation(const REGFormatInstruction& inst, Operation::addo) noexcept {
        addoBase<ConditionCode::Unconditional>(inst);
    }
    void Core::performOperation(const REGFormatInstruction& inst, Operation::addi) noexcept {
        addiBase<ConditionCode::Unconditional>(inst);
	}
    void Core::performOperation(const REGFormatInstruction& inst, Operation::subo) noexcept {
        suboBase<ConditionCode::Unconditional>(inst);
    }
	void Core::mulo(__DEFAULT_THREE_ARGS__) noexcept {
		dest.set(src2.get<Ordinal>() * src1.get<Ordinal>());
	}
	void Core::divo(__DEFAULT_THREE_ARGS__) noexcept {
		if (auto denominator = src1.get<Ordinal>(); denominator == 0) {
			generateFault(ArithmeticFaultSubtype::ZeroDivide);
		} else {
			dest.set(src2.get<Ordinal>() / denominator);
		}
	}
	void Core::remo(__DEFAULT_THREE_ARGS__) noexcept {
		if (auto denominator = src1.get<Ordinal>(); denominator == 0) {
			generateFault(ArithmeticFaultSubtype::ZeroDivide);
		} else {
			// as defined in the manual
			auto numerator = src2.get<Ordinal>();
			dest.set(numerator - (denominator / numerator) * denominator);
		}
	}
    void Core::performOperation(const REGFormatInstruction& inst, Operation::chkbit) noexcept {
        auto bitpos = getSrc1(inst);
        auto src = getSrc2(inst);
		_ac.conditionCode = ((src & (1 << (bitpos & 0b11111))) == 0) ? 0b000 : 0b010;
	}
    void Core::performOperation(const REGFormatInstruction& inst, Operation::alterbit) noexcept {
        Ordinal result = 0u;
        if (auto bitpos = getSrc1(inst), src = getSrc2(inst); (_ac.conditionCode & 0b010) == 0) {
			// if the condition bit is clear then we clear the given bit
            result = src & (~(1 << bitpos));
        } else {
			// if the condition bit is set then we set the given bit
            result = src | (1 << bitpos);
        }
        setDest(inst, result);
    }
    void Core::performOperation(const REGFormatInstruction& inst, Operation::opand) noexcept {
        setDest(inst, getSrc2<Ordinal>(inst) & 
                      getSrc1<Ordinal>(inst));
	}
    void Core::performOperation(const REGFormatInstruction& inst, Operation::andnot) noexcept {
        setDest(inst, (getSrc2(inst)) & (~getSrc1(inst)));
	}

    void Core::performOperation(const REGFormatInstruction& inst, Operation::mov) noexcept {
        setDest(inst, getSrc1(inst));
    }
    void Core::performOperation(const REGFormatInstruction& inst, Operation::movl) noexcept {
        if (auto src1 = inst.getSrc1(), dst = inst.getSrcDest(); 
                notDivisibleBy(src1, 2) ||
                notDivisibleBy(dst, 2)) {
            setRegister<Integer>(dst, -1);
            setRegister<Integer>(dst.next(), -1);
            generateFault(OperationFaultSubtype::InvalidOperand);
        } else if (src1.isLiteral()) {
            setRegister<Ordinal>(dst, src1.getValue());
            setRegister<Ordinal>(dst.next(), 0);
        } else {
            // src1 is a register
            setRegister<Ordinal>(dst, getRegisterValue<Ordinal>(src1));
            setRegister<Ordinal>(dst.next(), getRegisterValue<Ordinal>(src1.next()));
        }
    }
    void Core::performOperation(const REGFormatInstruction& inst, Operation::movt) noexcept {
        if (auto src1 = inst.getSrc1(), dst = inst.getSrcDest(); 
                notDivisibleBy(src1, 4) ||
                notDivisibleBy(dst, 4)) {
            setRegister<Integer>(dst, -1);
            setRegister<Integer>(dst.next(), -1);
            setRegister<Integer>(dst.next().next(), -1);
            generateFault(OperationFaultSubtype::InvalidOperand);
        } else if (src1.isLiteral()) {
            setRegister<Ordinal>(dst, src1.getValue());
            setRegister<Ordinal>(dst.next(), 0);
            setRegister<Ordinal>(dst.next().next(), 0);
        } else {
            // src1 is a register
            setRegister<Ordinal>(dst, getRegisterValue<Ordinal>(src1));
            setRegister<Ordinal>(dst.next(), getRegisterValue<Ordinal>(src1.next()));
            setRegister<Ordinal>(dst.next().next(), getRegisterValue<Ordinal>(src1.next().next()));
        }
    }
    void Core::performOperation(const REGFormatInstruction& inst, Operation::movq) noexcept {
        if (auto src1 = inst.getSrc1(), dst = inst.getSrcDest(); 
                notDivisibleBy(src1, 4) ||
                notDivisibleBy(dst, 4)) {
            setRegister<Integer>(dst, -1);
            setRegister<Integer>(dst.next(), -1);
            setRegister<Integer>(dst.next().next(), -1);
            setRegister<Integer>(dst.next().next().next(), -1);
            generateFault(OperationFaultSubtype::InvalidOperand);
        } else if (src1.isLiteral()) {
            setRegister<Ordinal>(dst, src1.getValue());
            setRegister<Ordinal>(dst.next(), 0);
            setRegister<Ordinal>(dst.next().next(), 0);
            setRegister<Ordinal>(dst.next().next().next(), 0);
        } else {
            // src1 is a register
            setRegister<Ordinal>(dst, getRegisterValue<Ordinal>(src1));
            setRegister<Ordinal>(dst.next(), getRegisterValue<Ordinal>(src1.next()));
            setRegister<Ordinal>(dst.next().next(), getRegisterValue<Ordinal>(src1.next().next()));
            setRegister<Ordinal>(dst.next().next().next(), getRegisterValue<Ordinal>(src1.next().next().next()));
        }
    }
	void Core::performOperation(const CTRLFormatInstruction& inst, Operation::b) noexcept {
        branchIfGeneric<ConditionCode::Unconditional>(inst.getDisplacement());
	}
    void Core::performOperation(const MEMFormatInstruction& inst, Operation::bx) noexcept {
        _instructionPointer = getSrc(inst);
        _instructionPointer = computeAlignedAddress(_instructionPointer); // make sure the least significant two bits are clear
    }
    void Core::performOperation(const CTRLFormatInstruction& inst, Operation::bal) noexcept {
		_globalRegisters[14].set<Ordinal>(_instructionPointer + 4);
        branchIfGeneric<ConditionCode::Unconditional>(inst.getDisplacement());
    }

	constexpr Ordinal computeCheckBitMask(Ordinal value) noexcept {
		return 1 << (value & 0b11111);
	}
    void Core::performOperation(const COBRFormatInstruction& inst, Operation::bbc) noexcept {
        // check bit and branch if clear
        auto bitpos = getSrc(inst);
        auto src = getSrc2(inst);
        auto mask = computeCheckBitMask(bitpos);
        _ac.conditionCode = 0b010; // update condition code to not taken result since it will always be done
        if ((src & mask) == 0) {
            _ac.conditionCode = 0b000;
            _instructionPointer = _instructionPointer + inst.getDisplacement();
            // clear the lowest two bits of the instruction pointer
            _instructionPointer &= (~0b11);
        } 
    }
    void Core::performOperation(const COBRFormatInstruction& inst, Operation::bbs) noexcept {
        // check bit and branch if set
        auto bitpos = getSrc(inst);
        auto src = getSrc2(inst);
        auto mask = computeCheckBitMask(bitpos);
        _ac.conditionCode = 0b000;
        if ((src & mask) == 1) {
            _ac.conditionCode = 0b010;
            _instructionPointer = _instructionPointer + inst.getDisplacement();
            // clear the lowest two bits of the instruction pointer
            _instructionPointer &= (~0b11);
        } 
    }

	// Begin Instruction::REGFormat implementations
	/**
	 * Shifts a specified bit field in value right and fills the bits to the left of
	 * the shifted bit field with zeros. 
	 * @param value Data to be shifted
	 * @param position specifies the least significant bit of the bit field to be shifted
	 * @param length specifies the length of the bit field
	 * @return A value where the bitfield in value is shifted and zeros are put in place of all other values
	 */
	constexpr Ordinal decode(Ordinal value, Ordinal position, Ordinal length) noexcept {
		auto shifted = value >> (position & 0b11111);
		auto mask = ((1 << length) - 1); // taken from the i960 documentation
		return shifted & mask;
	}


    void Core::performOperation(const REGFormatInstruction& inst, Operation::extract) noexcept {
        setDest(inst, decode(getSrc1(inst), getSrc2(inst), getSrc(inst)));
    }
    constexpr Ordinal largestOrdinal = 0xFFFF'FFFF;
    void Core::performOperation(const REGFormatInstruction& inst, Operation::scanbit) noexcept {
	//void Core::scanbit(SourceRegister src, DestinationRegister dest) noexcept {
        // find the most significant set bit
        auto result = largestOrdinal;
		_ac.conditionCode = 0b000;
        // while the psuedo-code in the programmers manual talks about setting
        // the destination to all ones if src is equal to zero, there is no short 
        // circuit in the action section for not iterating through the loop when
        // src is zero. A small optimization
        if (auto src = getSrc1(inst); src != 0) {
            for (Integer i = 31; i >= 0; --i) {
                if (auto k = 1 << i; (src & k) != 0) {
                    _ac.conditionCode = 0b010;
                    result = i;
                    break;
                }
            }
        }
        setDest(inst, result);
	}
    void Core::performOperation(const REGFormatInstruction& inst, Operation::spanbit) noexcept {
        /**
         * Find the most significant clear bit
         */
        auto result = largestOrdinal;
        _ac.conditionCode = 0b000;
        if (auto src = getSrc1(inst); src != largestOrdinal) {
            for (Integer i = 31; i >= 0; --i) {
                if (auto k = (1 << i); (src & k) == 0) {
                    result = i;
                    _ac.conditionCode = 0b010;
                    break;
                }
            }
        }
        setDest(inst, result);
	}
	void Core::modi(__DEFAULT_THREE_ARGS__) noexcept {
		if (auto s1 = src1.get<Integer>(); s1 == 0) {
			dest.set<Integer>(-1); // says in the manual, to assign it to an undefined value
			generateFault(ArithmeticFaultSubtype::ZeroDivide);
		} else {
			auto s2 = src2.get<Integer>();
			dest.set<Integer>(s2 - (s2 / s1) * s1);
			if ((s2 * s1) < 0) {
				dest.set<Integer>(dest.get<Integer>() + s1);
			}
		}
	}
	void Core::subc(__DEFAULT_THREE_ARGS__) noexcept {
		auto s1 = src1.get<Ordinal>() - 1u;
		auto s2 = src2.get<Ordinal>();
		auto carryBit = (_ac.conditionCode & 0b010) >> 1;
		dest.set<Ordinal>(s2 - s1 + carryBit);
		_ac.conditionCode = 0b000;
		if ((src2.mostSignificantBit() == src1.mostSignificantBit()) && (src2.mostSignificantBit() != dest.mostSignificantBit())) {
			// we've overflowed
			_ac.conditionCode = 0b001;
		}
		_ac.conditionCode += (dest.mostSignificantBit() << 1);
	}
	void Core::ediv(SourceRegister src1, ByteOrdinal src2Ind, ByteOrdinal destInd) noexcept {
		// TODO make sure that src2 and dest indicies are even
		LongRegister src2 = makeLongRegister(src2Ind);
		LongRegister dest = makeLongRegister(destInd);
		if (auto s1 = src1.get<LongOrdinal>(); s1 == 0) {
			dest.set<LongOrdinal>(-1); // set to undefined value
			generateFault(OperationFaultSubtype::InvalidOperand);
		} else if (src1.get<Ordinal>() == 0) {
			dest.set<LongOrdinal>(-1);
			generateFault(ArithmeticFaultSubtype::ZeroDivide);
		} else {
			auto s2 = src2.get<LongOrdinal>();
			// lower contains remainder, upper contains quotient
			auto quotient = s2 / s1;
			auto remainder = s2 - (s2 / s1) * s1;
			dest.set(remainder, quotient);
		}
	}
	void Core::divi(__DEFAULT_THREE_ARGS__) noexcept {
		if (auto denominator = src1.get<Integer>(); denominator == 0) {
			dest.set<Integer>(-1);
			generateFault(ArithmeticFaultSubtype::ZeroDivide);
		} else if (auto numerator = src2.get<Integer>(); (numerator == 0x8000'0000) && (denominator == -1)) {
			// this one is a little strange, the manual states -2**31
			// which is just 0x8000'0000 in signed integer, no clue why they
			// described it like that. So I'm just going to put 0x8000'0000
			dest.set<Integer>(0x8000'0000);
			if (_ac.integerOverflowMask == 1) {
				_ac.integerOverflowFlag = 1;
			} else {
				generateFault(ArithmeticFaultSubtype::IntegerOverflow);
			}
		} else {
			dest.set<Integer>(numerator / denominator);
		}
	}
	constexpr Ordinal getLowestTwoBits(Ordinal address) noexcept {
		return address & 0b11;
	}
	constexpr Ordinal getLowestBit(Ordinal address) noexcept {
		return address & 0b1;
	}
	void Core::ld(SourceRegister src, DestinationRegister dest) noexcept {
		// this is the base operation for load, src contains the fully computed value
		// so this will probably be an internal register in most cases.
		auto effectiveAddress = src.get<Ordinal>();
		dest.set<Ordinal>(load(effectiveAddress));
		if ((getLowestTwoBits(effectiveAddress) != 0) && unalignedFaultEnabled) {
			generateFault(OperationFaultSubtype::Unaligned);
		}
	}
	constexpr Ordinal getByteOrdinalMostSignificantBit(Ordinal value) noexcept {
		return value & 0b1000'0000;
	}
	constexpr Ordinal getShortOrdinalMostSignificantBit(Ordinal value) noexcept {
		return value & 0b1000'0000'0000'0000;
	}
	constexpr Ordinal maskToByteOrdinal(Ordinal value) noexcept {
		return value & 0xFF;
	}
	constexpr Ordinal maskToShortOrdinal(Ordinal value) noexcept {
		return value & 0xFFFF;
	}
	void Core::ldob(SourceRegister src, DestinationRegister dest) noexcept {
		dest.set<Ordinal>(maskToByteOrdinal(load(src.get<Ordinal>())));
	}
	void Core::ldos(SourceRegister src, DestinationRegister dest) noexcept {
		auto effectiveAddress = src.get<Ordinal>();
		auto value = maskToShortOrdinal(load(effectiveAddress));
		dest.set<Ordinal>(value);
		if ((getLowestBit(effectiveAddress) != 0) && unalignedFaultEnabled) {
			generateFault(OperationFaultSubtype::Unaligned);
		}
		
	}
	void Core::ldib(SourceRegister src, DestinationRegister dest) noexcept {
		auto effectiveAddress = src.get<Ordinal>();
		auto result = (maskToByteOrdinal(load(effectiveAddress)));
		if (getByteOrdinalMostSignificantBit(result) != 0) {
			result += 0xFFFF'FF00; // 
		}
		dest.set<Ordinal>(result);
	}
	void Core::ldis(SourceRegister src, DestinationRegister dest) noexcept {
		auto effectiveAddress = src.get<Ordinal>();
		auto value = maskToShortOrdinal(load(effectiveAddress));
		if (getShortOrdinalMostSignificantBit(value) != 0) {
			value += 0xFFFF'0000;
		}
		dest.set<Ordinal>(value);
		if ((getLowestBit(effectiveAddress) != 0) && unalignedFaultEnabled) {
			generateFault(OperationFaultSubtype::Unaligned);
		}
	}
	constexpr Ordinal getLowestThreeBits(Ordinal value) noexcept {
		return value & 0b111;
	}
	void Core::ldl(SourceRegister src, Ordinal srcDestIndex) noexcept {
		if ((srcDestIndex % 2) != 0) {
			generateFault(OperationFaultSubtype::InvalidOperand);
		} else {
			LongRegister dest = makeLongRegister(srcDestIndex);
			auto addr = src.get<Ordinal>();
			dest.set(load(addr), load(addr + RegisterIndex<1>));
			if ((getLowestThreeBits(addr) != 0) && unalignedFaultEnabled) {
				generateFault(OperationFaultSubtype::Unaligned);
			}
		}
	}
	constexpr Ordinal getLowestFourBits(Ordinal value) noexcept {
		return value & 0b1111;
	}
	void Core::ldt(SourceRegister src, Ordinal srcDestIndex) noexcept {
		if ((srcDestIndex % 4) != 0) {
			generateFault(OperationFaultSubtype::InvalidOperand);
		} else {
			TripleRegister dest = makeTripleRegister(srcDestIndex);
			auto addr = src.get<Ordinal>();
			dest.set(load(addr), load(addr + RegisterIndex<1>), load(addr + RegisterIndex<2>));
			if ((getLowestFourBits(addr) != 0) && unalignedFaultEnabled) {
				generateFault(OperationFaultSubtype::Unaligned);
			}
		}
	}
	void Core::ldq(SourceRegister src, Ordinal index) noexcept {
		if ((index % 4) != 0) {
			generateFault(OperationFaultSubtype::InvalidOperand);
		} else {
			QuadRegister dest = makeQuadRegister(index);
			auto addr = src.get<Ordinal>();
			dest.set(load(addr), load(addr + RegisterIndex<1>), load(addr + RegisterIndex<2>), load(addr + RegisterIndex<3>));
			if ((getLowestFourBits(addr) != 0) && unalignedFaultEnabled) {
				generateFault(OperationFaultSubtype::Unaligned);
			}
		}
	}

    void Core::performOperation(const REGFormatInstruction& inst, Operation::cmpi) noexcept {
        cmpi(getRegister(inst.getSrc1()), getRegister(inst.getSrc2()));
    }
	void Core::cmpi(SourceRegister src1, SourceRegister src2) noexcept { compare(src1.get<Integer>(), src2.get<Integer>()); }
    void Core::performOperation(const REGFormatInstruction& inst, Operation::cmpo) noexcept {
        compare<Ordinal>(getSrc1(inst), getSrc2(inst));
    }
	void Core::cmpo(SourceRegister src1, SourceRegister src2) noexcept { compare(src1.get<Ordinal>(), src2.get<Ordinal>()); }
	void Core::muli(SourceRegister src1, SourceRegister src2, DestinationRegister dest) noexcept {
		dest.set(src2.get<Integer>() * src1.get<Integer>());
		if ((src2.mostSignificantBit() == src1.mostSignificantBit()) && (src2.mostSignificantBit() != dest.mostSignificantBit())) {
			if (_ac.integerOverflowMask == 1) {
				_ac.integerOverflowFlag = 1;
			} else {
				generateFault(ArithmeticFaultSubtype::IntegerOverflow);
			}
		}
	}
	void Core::remi(SourceRegister src1, SourceRegister src2, DestinationRegister dest) noexcept {
		if (auto denominator = src1.get<Integer>(); denominator == 0) {
			generateFault(ArithmeticFaultSubtype::ZeroDivide);
		} else {
			// as defined in the manual
			auto numerator = src2.get<Integer>();
			dest.set(numerator - (denominator / numerator) * denominator);
		}
	}
	void Core::stl(Ordinal ind, SourceRegister dest) noexcept {
		// TODO check for valid register
		LongRegister src = makeLongRegister(ind);
		store(dest.get<Ordinal>() + RegisterIndex<0>, src.getLowerHalf());
		store(dest.get<Ordinal>() + RegisterIndex<1>, src.getUpperHalf());
	}

	void Core::stt(Ordinal ind, SourceRegister dest) noexcept {
		// TODO perform fault checks
		TripleRegister src = makeTripleRegister(ind);
		auto addr = dest.get<Ordinal>();
		store(addr + RegisterIndex<0>, src.getLowerPart());
		store(addr + RegisterIndex<1>, src.getMiddlePart());
        store(addr + RegisterIndex<2>, src.getUpperPart());
	}

	void Core::stq(Ordinal ind, SourceRegister dest) noexcept {
		QuadRegister src = makeQuadRegister(ind);
		auto addr = dest.get<Ordinal>();
		store(addr + RegisterIndex<0>, src.getLowestPart());
		store(addr + RegisterIndex<1>, src.getLowerPart());
		store(addr + RegisterIndex<2>, src.getHigherPart());
		store(addr + RegisterIndex<3>, src.getHighestPart());
	}
    void Core::performOperation(const REGFormatInstruction&, Operation::syncf) noexcept {
        syncf();
    }
    void Core::syncf() noexcept {
		// this does nothing for the time being because this implementation does not execute instructions 
		// in parallel. When we get there this will become an important instruction
	}

    void Core::performOperation(const REGFormatInstruction&, Operation::flushreg) noexcept {
		// this will nop currently as I'm saving all local registers to the 
		// stack when a call happens
	}
    void Core::performOperation(const REGFormatInstruction& inst, Operation::subi) noexcept {
        subiBase<ConditionCode::Unconditional>(inst);
    }
    void Core::performOperation(const REGFormatInstruction& inst, Operation::modtc) noexcept {
        // the instruction has its arguments reversed for some reason...
        auto mask = getSrc(inst);
        auto src2 = getSrc2(inst);
		TraceControls tmp;
		tmp.value = _tc.value;
		auto temp1 = 0x00FF00FF & mask; // masked to prevent reserved bits from being used
		_tc.value = (temp1 & src2) | (_tc.value & (~temp1));
        setRegister(inst.getSrc1(), tmp.value);
	}
    void Core::performOperation(const REGFormatInstruction& inst, Operation::modpc) noexcept {
		// modify process controls
		if (auto maskVal = getSrc2(inst); maskVal != 0) {
			if (_pc.inUserMode()) {
				generateFault(TypeFaultSubtype::Mismatch);
				return;
			}
			ProcessControls temp;
			temp.value = _pc.value;
			_pc.value = (maskVal & getSrc(inst)) | (_pc.value & (~maskVal));
            setDest(inst, temp.value);
			if (temp.priority > _pc.priority) {
				// TODO check pending interrupts
			}
			// if continue here, no interrupt to do
		} else {
            setDest(inst, _pc.value);
		}
	}
    void Core::performOperation(const REGFormatInstruction& inst, Operation::modac) noexcept {
	//void Core::modac(__DEFAULT_THREE_ARGS__) noexcept {
        auto mask = getSrc(inst);
        auto src = getSrc2(inst);
        auto dest = getRegister(inst.getSrc1());
        
		auto tmp = _ac.value;
        _ac.value = (src & mask) | (tmp & (~mask));
        setRegister<Ordinal>(inst.getSrc1(), tmp);
	}
    constexpr auto mostSignificantBit(Ordinal input) noexcept {
        return (input & 0x8000'0000);
    }
    void Core::performOperation(const REGFormatInstruction& inst, Operation::addc) noexcept {
        auto src1Value = getRegisterValue<Ordinal>(inst.getSrc1());
        auto src2Value = getRegisterValue<Ordinal>(inst.getSrc2());
        auto destReg = getRegister(inst.getSrcDest());
        LongOrdinal result = static_cast<LongOrdinal>(src1Value) + static_cast<LongOrdinal>(src2Value) + _ac.getCarryValue();
        destReg.set<Ordinal>(static_cast<Ordinal>(result));
        _ac.conditionCode = 0b000; // odd that they would do this first as it breaks their action description in the manual
        if (auto msb2 = mostSignificantBit(src2Value) ; (msb2 == mostSignificantBit(src1Value)) && (msb2 != destReg.mostSignificantBit())) {
            _ac.conditionCode |= 0b001; 
        }
        _ac.conditionCode |= ((result >> 31 & 0b10));
    }
    void Core::freeCurrentRegisterSet() noexcept {
        /// @todo implement
    }
    bool Core::registerSetNotAllocated(const Operand& fp) noexcept {
        /// @todo implement
        return true;
    }
    void Core::retrieveFromMemory(const Operand& fp) noexcept {
        /// @todo implement
    }
	void Core::performOperation(const CTRLFormatInstruction& inst, Operation::ret) noexcept {
        syncf();
        auto pfp = getPFP();
        if (pfp.prereturnTrace && _pc.traceEnable && _tc.prereturnTraceMode) {
            pfp.prereturnTrace = 0;
            generateFault(TraceFaultSubtype::PreReturn);
            return;
        }
        auto getIPFP = [this]() {
            setRegister(FP, getRegister(PFP).ordinal);
            freeCurrentRegisterSet();
            if (registerSetNotAllocated(FP)) {
                retrieveFromMemory(FP);
            }
            _instructionPointer = getRegister(RIP).ordinal;
        };
        switch (pfp.returnCode) {
            case 0b000: // local return
                // get_FP_and_IP();
                break;
            case 0b001: // fault return
                [this, getIPFP]() {
                    auto tempa = load(getRegister(FP).ordinal - 16);
                    auto tempb = load(getRegister(FP).ordinal - 12);
                    getIPFP();
                    _ac.value = tempb;
                    if (_pc.inSupervisorMode()) {
                        _pc.value = tempa;
                    }
                }();
                break;
            case 0b010: // supervisor return, trace on return disabled
                [this,getIPFP]() {
                    if (!_pc.inSupervisorMode()) {
                        getIPFP();
                    } else {
                        _pc.traceEnable = 0;
                        _pc.enterUserMode();
                        getIPFP();
                    }
                }();
                break;
            case 0b100: // reserved - unpredictable behavior
            case 0b101: // reserved - unpredictable behavior
            case 0b110: // reserved - unpredictable behavior
                generateFault(OperationFaultSubtype::Unimplemented);
                break;
            case 0b111: // interrupt return
                [this, getIPFP]() {
                    auto tempa = load(getRegister(FP).ordinal - 16);
                    auto tempb = load(getRegister(FP).ordinal - 12);
                    getIPFP();
                    _ac.value = tempb;
                    if (_pc.inSupervisorMode()) {
                        _pc.value = tempa;
                        /// @todo check for pending interrupts
                    }
                }();
                break;
        }
	}
    void Core::performOperation(const REGFormatInstruction& inst, Operation::opnot) noexcept {
        setDest(inst, ~getSrc1(inst));
    }
    void Core::performOperation(const REGFormatInstruction& inst, Operation::notand) noexcept {
        setDest(inst, (~getSrc2(inst)) & getSrc1(inst));
    }
    void Core::performOperation(const REGFormatInstruction& inst, Operation::notbit) noexcept {
        setDest(inst, xorOperation(getSrc2(inst), oneShiftLeft(getSrc1(inst))));
    }
    void Core::performOperation(const REGFormatInstruction& inst, Operation::notor) noexcept {
        setDest(inst, (~getSrc2(inst)) | getSrc1(inst));
    }
    void Core::performOperation(const REGFormatInstruction& inst, Operation::opor) noexcept {
        setDest(inst, (getSrc2(inst) | getSrc1(inst)));
    }
    void Core::performOperation(const REGFormatInstruction& inst, Operation::ornot) noexcept {
        setDest(inst, (getSrc2(inst) | (~getSrc1(inst))));
    }
    void Core::performOperation(const REGFormatInstruction& inst, Operation::nand) noexcept {
		// as shown in the manual
        setDest(inst, (~getSrc2(inst)) | (~getSrc1(inst)));
    }
    void Core::performOperation(const REGFormatInstruction& inst, Operation::nor) noexcept {
        setDest(inst, (~getSrc2(inst)) & (~getSrc1(inst)));
	}
    void Core::performOperation(const REGFormatInstruction& inst, Operation::shro) noexcept {
        auto result = 0u;
        if (auto shift = getSrc1(inst); shift < 32u) {
            result = getSrc2(inst) >> shift;
        }
        setDest(inst, result);
    }
    void Core::performOperation(const REGFormatInstruction& inst, Operation::shri) noexcept {
        // we want src1 to be an ordinal to prevent shifting by negative numbers
        setDest(inst, getSrc2<Integer>(inst) >> std::abs(getSrc1<Integer>(inst)));
    }
    void Core::performOperation(const REGFormatInstruction& inst, Operation::shlo) noexcept {
        auto result = 0u;
        if (auto shift = getSrc1(inst); shift < 32u) {
            result = getSrc2(inst) << shift;
        } 
        setDest(inst, result);
	}
    void Core::performOperation(const REGFormatInstruction& inst, Operation::shli) noexcept {
        setDest(inst, getSrc2<Integer>(inst) << std::abs(getSrc1<Integer>(inst)));
	}
    void Core::performOperation(const REGFormatInstruction& inst, Operation::shrdi) noexcept {
        auto src = getSrc2<Integer>(inst);
        auto len = std::abs(getSrc1<Integer>(inst));
        auto result = src >> len;
        if (src < 0 && result < 0) {
            ++result;
        }
        setDest<Integer>(inst, result);
    }
	constexpr Ordinal rotateOperation(Ordinal src, Ordinal length) noexcept {
		return (src << length) | (src >> ((-length) & 31u));
	}
    void Core::performOperation(const REGFormatInstruction& inst, Operation::rotate) noexcept {
        auto src = getSrc2(inst);
        auto length = getSrc1(inst);
        setDest(inst, rotateOperation(src, length));
	}
    void Core::performOperation(const REGFormatInstruction& inst, Operation::modify) noexcept {
        auto mask = getSrc1(inst);
        auto src = getSrc2(inst);
        auto sdVal = getSrc(inst);
        setDest(inst, (src & mask) | (sdVal & (~mask)));
	}
	template<Ordinal mask>
	constexpr bool maskedEquals(Ordinal src1, Ordinal src2) noexcept {
		return (src1 & mask) == (src2 & mask);
	}
    void Core::performOperation(const REGFormatInstruction& inst, Operation::scanbyte) noexcept {
        _ac.conditionCode = 0b000;
		if (auto s1 = getSrc1(inst), 
                 s2 = getSrc2(inst); 
                maskedEquals<0x000000FF>(s1, s2) ||
				maskedEquals<0x0000FF00>(s1, s2) ||
				maskedEquals<0x00FF0000>(s1, s2) ||
				maskedEquals<0xFF000000>(s1, s2)) {
			_ac.conditionCode = 0b010;
		} 
	}
	constexpr Ordinal alignToWordBoundary(Ordinal value) noexcept {
		return value & (~0x3);
	}
    void Core::performOperation(const REGFormatInstruction& inst, Operation::atmod) noexcept {
		// TODO implement
		auto srcDest = getSrc(inst);
		auto mask = getSrc2(inst);
		auto fixedAddr = alignToWordBoundary(getSrc1(inst));
		auto tmp = load(fixedAddr, true);
		store(fixedAddr, (srcDest & mask) | (tmp & (~mask)), true);
        setDest(inst, tmp);
	}
    void Core::performOperation(const REGFormatInstruction& inst, Operation::atadd) noexcept {
		// TODO implement atomic operations
		auto fixedAddr = alignToWordBoundary(getSrc1(inst));
		auto src = getSrc2(inst);
		auto tmp = load(fixedAddr, true);
		store(fixedAddr, tmp + src, true);
        setDest(inst, tmp);
	}
    void Core::performOperation(const REGFormatInstruction& inst, Operation::concmpo) noexcept {
        concmpBase(getSrc1(inst), getSrc2(inst));
    }
    void Core::performOperation(const REGFormatInstruction& inst, Operation::concmpi) noexcept {
        concmpBase(getSrc1<Integer>(inst), getSrc2<Integer>(inst));
	}
    void Core::performOperation(const REGFormatInstruction& inst, Operation::cmpinco) noexcept {
		/// @todo add support for overflow detection
        compare<Ordinal>(getSrc1(inst), getSrc2(inst));
        setDest(inst, getSrc2(inst) + 1);
	}
    void Core::performOperation(const REGFormatInstruction& inst, Operation::cmpdeco) noexcept {
        /// @todo add support for overflow detection
        compare<Ordinal>(getSrc1(inst), getSrc2(inst));
        setDest(inst, getSrc2(inst) - 1);
	}
    void Core::performOperation(const REGFormatInstruction& inst, Operation::cmpinci) noexcept {
        compare<Integer>(getSrc1<Integer>(inst), getSrc2<Integer>(inst));
        setDest<Integer>(inst, getSrc2<Integer>(inst) + 1); // overflow suppressed
	}
    void Core::performOperation(const REGFormatInstruction& inst, Operation::cmpdeci) noexcept {
        compare<Integer>(getSrc1<Integer>(inst), getSrc2<Integer>(inst));
        setDest<Integer>(inst, getSrc2<Integer>(inst) - 1);
	}
    void Core::performOperation(const REGFormatInstruction& inst, Operation::clrbit) noexcept {
        setDest(inst, getSrc2(inst) & ~oneShiftLeft(getSrc1(inst)));
	}
    void Core::performOperation(const REGFormatInstruction& inst, Operation::setbit) noexcept {
        setDest(inst, getSrc2(inst) | oneShiftLeft(getSrc1(inst)));
	}
	void Core::emul(SourceRegister src1, SourceRegister src2, ByteOrdinal destIndex) noexcept {
		// TODO perform double register validity check
		LongRegister dest = makeLongRegister(destIndex);
		dest.set<LongOrdinal>(src2.get<LongOrdinal>() * src1.get<LongOrdinal>());
	}
	void Core::lda(SourceRegister src, DestinationRegister dest) noexcept {
		// already computed this value by proxy of decoding
		dest.move(src);
	}
    void Core::performOperation(const REGFormatInstruction&, Operation::fmark) noexcept {
        /// @todo see if we have to do anything else
		if (_pc.traceEnabled()) {
			generateFault(TraceFaultSubtype::Mark);
		}
	}
    void Core::performOperation(const REGFormatInstruction&, Operation::mark) noexcept {
		// force mark aka generate a breakpoint trace-event
		if (_pc.traceEnabled() && _tc.traceMarked()) {
			generateFault(TraceFaultSubtype::Mark);
		}
	}
    void Core::performOperation(const REGFormatInstruction& inst, Operation::xnor) noexcept {
        auto s2 = getSrc2(inst);
		auto s1 = getSrc1(inst);
        setDest(inst, ~(s2 | s1) | (s2 & s1));
    }
    void Core::performOperation(const REGFormatInstruction& inst, Operation::opxor) noexcept {
    //void Core::opxor(__DEFAULT_THREE_ARGS__) noexcept {
		// there is an actual implementation within the manual so I'm going to
		// use that instead of the xor operator.
		auto s2 = getSrc2(inst);
        auto s1 = getSrc1(inst);
        setDest(inst, xorOperation(s2, s1));
    }
    void Core::performOperation(const REGFormatInstruction&, Operation::intdis) noexcept {
		// TODO implement
		if (!_pc.inSupervisorMode()) {
			generateFault(TypeFaultSubtype::Mismatch);
		}
	}
    void Core::performOperation(const REGFormatInstruction&, Operation::inten) noexcept {
		// TODO implement
		if (!_pc.inSupervisorMode()) {
			generateFault(TypeFaultSubtype::Mismatch);
		}
	}
    void Core::performOperation(const REGFormatInstruction& inst, Operation::halt) noexcept {
		// TODO finish implementing this
		// From the i960Jx manual:
		// causes the processor to enter HALT mode which is described in
		// Chapter 16, HALT MODE. Entry into Halt mode allows the interrupt
		// enable state to be conditionally changed based on the value of src1
		// if src1 == 0 then Disable interrupts and halt
		// if src1 == 1 then Enable interrupts and halt
		// if src1 == 2 then Use current interrupt enable state and halt
		//
		// The processor exits Halt mode on a hardware reset or upon receipt of
		// an interrupt that should be delivered based on teh current process
		// priority. After executing the interrupt that forced the processor
		// out of Halt mode, execution resumes at the instruction immediately
		// after the halt instruction. The processor must be in supervisor mode
		// to use this instruction
		syncf(); // implicit syncf
		if (!_pc.inSupervisorMode()) {
			generateFault(TypeFaultSubtype::Mismatch);
		} else {
			switch (getSrc1(inst)) {
				case 0: // Disable interrupts. Clear ICON.gie
					// globalInterruptEnable = false; 
					break;
				case 1: // enable interrupts. Set ICON.gie
					// globalInterruptEnable = true;
					break;
				case 2: // use the current interrupt enable state.
					break;
				default:
					// TODO generate an Operation.Invalid_Operand fault
					break;
			}
		}
		// ensure_bus_is_quiescient; // WAT!??
		// enter_HALT_mode; // TODO
	}
    void Core::performOperation(const REGFormatInstruction& inst, Operation::bswap) noexcept {
		// Taken from the i960 Jx reference manual:
		// alter the order of bytes in a word, reversing its "endianness."
		//
		// Copies bytes 3:0 of src1 to src2 reversing order of the bytes. Byte
		// 0 of src1 becomes byte 3 of src2, byte 1 of src1 becomes byte 2 of
		// src2, etc.
		//
		// Example: 
		// 	              # g8 = 0x89ABCDEF
		// 	bswap g8, g10 # reverse byte order
		// 				  # g10 = 0xEFCDAB89
		//
		// 	action: dst = (rotate_left(src, 8) & 0x00FF00FF) +
		// 				  (rotate_left(src, 24) & 0xFF00FF00)
		auto src = getSrc1(inst);
		auto rotl8 = rotateOperation(src, 8) & 0x00FF00FF; // rotate the upper 8 bits around to the bottom
		auto rotl24 = rotateOperation(src, 24) & 0xFF00FF00;
        setDest(inst, rotl8 + rotl24);
	}
    void Core::performOperation(const REGFormatInstruction& inst, Operation::cmpos) noexcept {
        compare<ShortOrdinal>(getSrc1<ShortOrdinal>(inst), getSrc2<ShortOrdinal>(inst));
	}
    void Core::performOperation(const REGFormatInstruction& inst, Operation::cmpis) noexcept {
        compare<ShortInteger>(getSrc1<ShortInteger>(inst), getSrc2<ShortInteger>(inst));
	}
    void Core::performOperation(const REGFormatInstruction& inst, Operation::cmpob) noexcept {
        compare<ByteOrdinal>(getSrc1<ByteOrdinal>(inst), getSrc2<ByteOrdinal>(inst));
	}
    void Core::performOperation(const REGFormatInstruction& inst, Operation::cmpib) noexcept {
        compare<ByteInteger>(getSrc1<ByteInteger>(inst), getSrc2<ByteInteger>(inst));
	}
    void Core::performOperation(const REGFormatInstruction& inst, Operation::dcctl) noexcept {
		// while I don't implement a data cache myself, this instruction must
		// do something!
		// TODO something
		if (!_pc.inSupervisorMode()) {
			generateFault(TypeFaultSubtype::Mismatch);
		}
	}
    void Core::performOperation(const REGFormatInstruction& inst, Operation::eshro) noexcept {
        /// @todo reimplement the following
		/// dest.set<Ordinal>(makeLongRegister(src2Ind).get<LongOrdinal>() >> (src1.get<Ordinal>() & 0b11111));
    }
    void Core::performOperation(const REGFormatInstruction& inst, Operation::icctl) noexcept {
		// TODO implement
		if (!_pc.inSupervisorMode()) {
			generateFault(TypeFaultSubtype::Mismatch);
		}
	}
    void Core::performOperation(const REGFormatInstruction& inst, Operation::intctl) noexcept {
		// TODO: implement
		if (!_pc.inSupervisorMode()) {
			generateFault(TypeFaultSubtype::Mismatch);
		}
	}
    void Core::performOperation(const REGFormatInstruction& inst, Operation::sysctl) noexcept {
		// TODO: implement
		if (!_pc.inSupervisorMode()) {
			generateFault(TypeFaultSubtype::Mismatch);
		}
	}
	QuadRegister Core::makeQuadRegister(ByteOrdinal index) noexcept {
		return QuadRegister(getRegister(index), getRegister(index + 1), getRegister(index + 2), getRegister(index + 3));
	}
	TripleRegister Core::makeTripleRegister(ByteOrdinal index) noexcept {
		return TripleRegister(getRegister(index), getRegister(index + 1), getRegister(index + 2));
	}
	LongRegister Core::makeLongRegister(ByteOrdinal index) noexcept {
		return LongRegister(getRegister(index), getRegister(index + 1));
	}
	void Core::generateFault(ByteOrdinal faultType, ByteOrdinal faultSubtype) {
		// get the fault table base address
	//	auto faultTableBaseAddress = getFaultTableBaseAddress();
	}
	Ordinal Core::getFaultTableBaseAddress() noexcept {
		return load(_prcbAddress + 0x00);
	}
	Ordinal Core::getControlTableBaseAddress() noexcept {
		return load(_prcbAddress + 0x04);
	}
	Ordinal Core::getACRegisterInitialImage() noexcept {
		return load(_prcbAddress + 0x08);
	}
	Ordinal Core::getFaultConfigurationWord() noexcept {
		return load(_prcbAddress + 0x0C);
	}
	Ordinal Core::getInterruptTableBaseAddress() noexcept {
		return load(_prcbAddress + 0x10);
	}
	Ordinal Core::getSystemProcedureTableBaseAddress() noexcept {
		return load(_prcbAddress + 0x14);
	}
	Ordinal Core::getInterruptStackPointer() noexcept {
		// _prcbAddress + 24 is reserved in the manual
		return load(_prcbAddress + 0x1C);
	}
	Ordinal Core::getInstructionCacheConfigurationWord() noexcept {
		return load(_prcbAddress + 0x20);
	}
	Ordinal Core::getRegisterCacheConfigurationWord() noexcept {
		return load(_prcbAddress + 0x24);
	}
	void Core::reset() {
		_internalDataRam.reset();
		initializeProcessor();
	}
	void Core::initializeProcessor() {
		// taken from the manual but modified to reflect future implementation
		// data.
		// _failPin = true;
		// restoreFullCacheMode();
		// _icache.disable();
		// _icache.invalidate();
		// _dcache.disable();
		// _dcache.invalidate();
		// _bcon.ctv = 0; // selects _pmcon14_15 to control all accesses
		// _pmcon14_15 = 0; // selects 8-bit bus width
		/// Exit reset state and start init 
		// if (stestOnRisingEdgeOfReset) {
		//   _status = bist(); // bist does not return if it fails
		// }
		// _failPin = false;
		_pc.value = 0x001F2002; 
        constexpr auto ibrPtr = StartupRecordAddress<targetSeries>;
		// read pmcon14_15 image in ibr 
		// _failPin = true;
		// _imsk = 0;
		// _dlmcon.dcen = 0;
		// _lmmr0.lmte = 0;
		// _lmmr1.lmte = 0;
		// _dlmcon.be = load(ibrPtr + 0xc] >> 7;
		// _pmcon14_15.byte2 = 0xc0 & load(ibrPtr + 0x08);
		// compute checksum on boot record
		auto carry = 0u;
		auto checksum = 0xFFFF'FFFF;
		for (auto i = 0; i < 6; ++i) {
			// carry is carry out from previous add
			checksum = load(ibrPtr + 16 + (i * 4)) + checksum + carry;
		}
		if (checksum != 0) {
			constexpr Ordinal failMsg = 0xFEFF'FF64; // fail bus confidence test
			//auto dummy = load(failMsg); // do load with address = failMsg
			// loop forever with FAIL pin true (hardware does this)
			// for (;;);
		} else {
			// failPin = false;
		}
		// process prcb and control table
		_prcbAddress = load(ibrPtr + 0x14);
		_ctrlTable = load(ibrPtr + 0x04);
		processPrcb();
		_instructionPointer = load(ibrPtr + 0x10);
		setRegister(0_gr, _deviceId.getDeviceId());
	}
	void Core::processPrcb() {
        // Taken from the manual
        // PRCB_mmr = prcb_ptr;
        // reset_state(data_ram); // It is unpredictable whether the data ram keeps its prior contents
        // fault_table = memory[PRCB_mmr];
        // ctrl_table = memory[PRCB_mmr + 0x4];
        // ac = memory[PRCB_mmr + 0x8];
        // fault_config = memory[PRCB_mmr + 0xc];
        // if (1 & (fault_config >> 30)) {
        //  generate_fault_on_unaligned_access = false;
        // } else {
        //  generate_fault_on_unaligned_access = true;
        // }
        // /** Load Interrupt table and cache nmi vector entry in data RAM **/
        // Reset_block_NMI;
        // interrupt_table = memory[PRCB_mmr + 0x10];
        // memory[0] = memory[interrupt_table + (248*4) + 4];
        //
        // /** process system procedure table **/
        // sysproc = memory[PRCB_mmr + 0x14];
        // temp = memory[sysproc + 0xc];
        // SSP_mmr = (~0x3) & temp;
        // SSP.te = 1 & temp;
        //
        // /** initialize isp, fp, sp, and pfp **/
        // ISM_mmr = memory[PRCB_mmr+0x1c];
        // FP = ISP_mmr;
        // SP = FP + 64;
        // PFP = FP;
        //
        // /** initialize instruction cache **/
        // iccw = memory[PRCB_mmr+0x20];
        // if (1 & (iccw >> 16)) {
        //  enable(I_cache);
        // }
        //
        // /** Configure Local Register Cache **/
        // programmed_limit = (7 & memory[PRCB_mmr + 0x24] >> 8) );
        // config_reg_cache(programmed_limit);
        //
        // /** load_control_table. Note breakpoints and BPCON are excluded here **/
        // load_control_table(ctrl_table + 0x10, ctrl_table + 0x58);
        // load_control_table(ctrl_table + 0x68, ctrl_table + 0x6c);
        // IBP0 = 0x0;
        // IBP1 = 0x0;
        // DAB0 = 0x0;
        // DAB1 = 0x0;
        //
        // /** initialize timers **/
        // TMR0.tc = 0;
        // TMR1.tc = 0;
        // TMR0.enable = 0;
        // TMR1.enable = 0;
        // TMR0.sup = 0;
        // TMR1.sup = 0;
        // TMR0.reload = 0;
        // TMR1.reload = 0;
        // TMR0.csel = 0;
        // TMR1.csel = 0;
        // return;
	}
    bool Core::cycle() {
        dispatch(readInstruction());
        return true;
    }
    Instruction Core::readInstruction() {
        // we need to pull the lower half, and then check and see if it is actually a double byte design
        auto address = computeAlignedAddress(_instructionPointer);
        // load two instructions at a time but we will increment by the number
        // of arguments used
        Instruction basic(load(address), load(address + 0b100));
#if 0
        if (basic.isTwoOrdinalInstruction()) {
            // load the second value
            basic._second = load(address + 0b100);
            // advance the instruction pointer by two
            _instructionPointer += 0b1000;
        } else {
            // advance the instruction pointer by one
            _instructionPointer += 0b100;
        }
#endif
        /// @todo implement code to identify how many words of the loaded instruction are actually used and increment ip by that
        
        return basic;
    }
#undef __DEFAULT_TWO_ARGS__
#undef __DEFAULT_DOUBLE_WIDE_TWO_ARGS__
#undef __DEFAULT_THREE_ARGS__
#undef __DEFAULT_DOUBLE_WIDE_THREE_ARGS__
#undef __TWO_SOURCE_REGS__
    void 
    Core::performOperation(const CTRLFormatInstruction& inst, Operation::call) noexcept {
        /// @todo see if this remasking is overkill
		union {
			Integer _value : 22;
		} conv;
		conv._value = inst.getDisplacement();
		auto newAddress = conv._value;
		auto tmp = (getStackPointerAddress() + 63) & (~63); // round to the next boundary
		setRegister(RIP, _instructionPointer);
		saveLocalRegisters();
		allocateNewLocalRegisterSet();
		_instructionPointer += newAddress;
		setRegister(PFP, getFramePointerAddress());
		setFramePointer(tmp);
		setRegister(SP, tmp + 64);
    }
} // end namespace i960
