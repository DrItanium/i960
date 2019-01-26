#include "types.h"
#include "core.h"
#include "operations.h"
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
	void Core:: test ## kind (DestinationRegister dest) noexcept { testGeneric<TestTypes:: action>(dest); } \
	void Core:: b ## kind (Integer addr) noexcept { branchIfGeneric<ConditionCode:: action > ( addr ) ; } \
    void Core:: fault ## kind () noexcept { genericFault< ConditionCode:: action > ( ) ; } \
    void Core:: sel ## kind (__DEFAULT_THREE_ARGS__) noexcept { baseSelect<ConditionCode:: action>(src1, src2, dest); } \
    void Core:: subo ## kind (__DEFAULT_THREE_ARGS__) noexcept { suboBase<ConditionCode:: action>(src1, src2, dest); } \
    void Core:: subi ## kind (__DEFAULT_THREE_ARGS__) noexcept { subiBase<ConditionCode:: action>(src1, src2, dest); } \
    void Core:: addo ## kind (__DEFAULT_THREE_ARGS__) noexcept { addoBase<ConditionCode:: action>(src1, src2, dest); } \
    void Core:: addi ## kind (__DEFAULT_THREE_ARGS__) noexcept { addiBase<ConditionCode:: action>(src1, src2, dest); } 
#include "conditional_kinds.def"
#undef X
#define X(cmpop, bop) \
	void Core:: cmpop ## bop ( __TWO_SOURCE_AND_INT_ARGS__ ) noexcept { \
		cmpop ( src1, src2 ) ; \
		bop ( targ ) ; \
	}
X(cmpo, bg);
X(cmpo, be);
X(cmpo, bge);
X(cmpo, bl);
X(cmpo, bne);
X(cmpo, ble);
X(cmpi, bg);
X(cmpi, be);
X(cmpi, bge);
X(cmpi, bl);
X(cmpi, bne);
X(cmpi, ble);
X(cmpi, bo);
X(cmpi, bno);
#undef X
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
        // TODO implement
    }
    void Core::saveFrame() noexcept {
        // TODO implement
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
	void Core::call(Integer addr) noexcept {
		union {
			Integer _value : 22;
		} conv;
		conv._value = addr;
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
	constexpr Ordinal clearLowestTwoBitsMask = ~0b11;
	constexpr Ordinal getProcedureAddress(Ordinal value) noexcept {
		return value & clearLowestTwoBitsMask;
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
	void Core::calls(const NormalRegister& value) {
        constexpr Ordinal SALIGN = 1u;
        syncf();
		auto targ = value.get<ShortOrdinal>();
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
			return _internalDataRam.read(address & clearLowestTwoBitsMask);
		} else {
			return _mem.load(address, atomic);
		}
	}
	void Core::store(Ordinal address, Ordinal value, bool atomic) noexcept {
		if (address <= _internalDataRam.LargestAddress<0>) {
			_internalDataRam.write(address & clearLowestTwoBitsMask, value);
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
	void Core::callx(const NormalRegister& value) noexcept {
		static constexpr Ordinal boundaryMarker = 63u;
		static constexpr Ordinal boundaryAlignment = 64u;
		auto newAddress = value.get<Ordinal>();
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
	void Core::addo(__DEFAULT_THREE_ARGS__) noexcept {
		dest.set<Ordinal>(src2.get<Ordinal>() + src1.get<Ordinal>());
	}
	void Core::addi(__DEFAULT_THREE_ARGS__) noexcept {
		dest.set<Integer>(src2.get<Integer>() + src1.get<Integer>());
		// check for overflow
		if ((src2.mostSignificantBit() == src1.mostSignificantBit()) && (src2.mostSignificantBit() != dest.mostSignificantBit())) {
			if (_ac.integerOverflowMask == 1) {
				_ac.integerOverflowFlag = 1;
			} else {
				generateFault(ArithmeticFaultSubtype::IntegerOverflow);
			}
		}
	}
	void Core::subo(__DEFAULT_THREE_ARGS__) noexcept {
		dest.set(src2.get<Ordinal>() - src1.get<Ordinal>());
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
	void Core::chkbit(SourceRegister pos, SourceRegister src) noexcept {
		_ac.conditionCode = ((src.get<Ordinal>() & (1 << (pos.get<Ordinal>() & 0b11111))) == 0) ? 0b000 : 0b010;
	}
	void Core::alterbit(SourceRegister pos, SourceRegister src, DestinationRegister dest) noexcept {
		if (auto p = pos.get<Ordinal>() & 0b11111; (_ac.conditionCode & 0b010)  == 0) {
			// if the condition bit is clear then we clear the given bit
			dest.set<Ordinal>(src.get<Ordinal>() & (~(1 << p)));
		} else {
			// if the condition bit is set then we set the given bit
			dest.set<Ordinal>(src.get<Ordinal>() | (1 << p));
		}
	}
	void Core::opand(__DEFAULT_THREE_ARGS__) noexcept {
		dest.set<Ordinal>(i960::andOp<Ordinal>(src2.get<Ordinal>(), src1.get<Ordinal>()));
	}
	void Core::andnot(__DEFAULT_THREE_ARGS__) noexcept {
		dest.set<Ordinal>(i960::andNot(src2.get<Ordinal>(), src1.get<Ordinal>()));
	}


	void Core::mov(const Operand& src, const Operand& dest) noexcept { 
		if (DestinationRegister d = getRegister(dest); src.isRegister()) {
			d.move(getRegister(src));
		} else {
			// in the manual the arguments are differentiated
			d.set<Ordinal>(src.getValue());
		}
	}
	void Core::movl(const Operand& src, const Operand& dest) noexcept {
		if (notDivisibleBy(src, 2) || notDivisibleBy(dest, 2)) {
			// registers are not properly aligned so watch things burn...
			// however the manual states that this is an acceptable state
			getRegister(dest).set<Integer>(-1);
			getRegister(dest.next()).set<Integer>(-1);
			generateFault(OperationFaultSubtype::InvalidOperand);
		} else if (src.isRegister()) {
			mov(src, dest);
			mov(src.next(), dest.next());
		} else {
			mov(src, dest);
			getRegister(dest.next()).set<Ordinal>(0);
		}
	}
	void Core::movt(const Operand& src, const Operand& dest) noexcept {
		DestinationRegister d0 = getRegister(dest);
		DestinationRegister d1 = getRegister(dest.next());
		DestinationRegister d2 = getRegister(dest.next().next());
		if (notDivisibleBy(src, 4) || notDivisibleBy(dest, 4)) {
			d0.set(-1);
			d1.set(-1);
			d2.set(-1);
			generateFault(OperationFaultSubtype::InvalidOperand);
		} else if (src.isRegister()) {
			d0.move(getRegister(src));
			d1.move(getRegister(src.next()));
			d2.move(getRegister(src.next().next()));
		} else {
			d0.set<Ordinal>(src.getValue());
			d1.set(0);
			d2.set(0);
		}
	}
	void Core::movq(const Operand& src, const Operand& dest) noexcept {
		DestinationRegister d0 = getRegister(dest);
		DestinationRegister d1 = getRegister(dest.next());
		DestinationRegister d2 = getRegister(dest.next().next());
		DestinationRegister d3 = getRegister(dest.next().next().next());
		if (notDivisibleBy(src, 4) || notDivisibleBy(dest, 4)) {
			d0.set(-1);
			d1.set(-1);
			d2.set(-1);
			d3.set(-1);
			generateFault(OperationFaultSubtype::InvalidOperand);
		} else if (src.isRegister()) {
			d0.move(getRegister(src));
			d1.move(getRegister(src.next()));
			d2.move(getRegister(src.next().next()));
			d3.move(getRegister(src.next().next().next()));
		} else {
			d0.set<Ordinal>(src.getValue());
			d1.set(0);
			d2.set(0);
			d3.set(0);
		}
	}
	void Core::b(Integer displacement) noexcept {
		union {
			Integer _value : 24;
		} conv;
		conv._value = displacement;
		conv._value = conv._value > 0x7F'FFFC ? 0x7F'FFFC : conv._value;
		_instructionPointer += conv._value;
		_instructionPointer &= clearLowestTwoBitsMask; // make sure the least significant two bits are clear
	}
	void Core::bx(SourceRegister src) noexcept {
		_instructionPointer = src.get<Ordinal>();
		_instructionPointer &= clearLowestTwoBitsMask;
	}
	void Core::bal(Integer displacement) noexcept {
        // this is taken from the i960mc manual since the one in the i960jx manual
        // contradicts itself. 
		_globalRegisters[14].set<Ordinal>(_instructionPointer + 4);
		b(displacement);
	}

	void Core::balx(__DEFAULT_TWO_ARGS__,Core::InstructionLength length) noexcept {
		dest.set<Ordinal>(_instructionPointer + static_cast<Ordinal>(length));
		_instructionPointer = src.get<Ordinal>();
	}
	constexpr Ordinal computeCheckBitMask(Ordinal value) noexcept {
		return 1 << (value & 0b11111);
	}
	void Core::bbc(SourceRegister bitpos, SourceRegister src, Integer targ) noexcept {
		// check bit and branch if clear
		if (auto s = src.get<Ordinal>(), mask = computeCheckBitMask(bitpos.get<Ordinal>()); (s & mask) == 0) {
			_ac.conditionCode = 0;
			union {
				Integer value : 11;
			} displacement;
			displacement.value = targ;
			_instructionPointer = _instructionPointer + 4 + (displacement.value * 4);
		} else {
			_ac.conditionCode = 0b010;
		}
	}

	void Core::bbs(SourceRegister bitpos, SourceRegister src, Integer targ) noexcept {
		// check bit and branch if set
		if (auto s = src.get<Ordinal>(), mask = computeCheckBitMask(bitpos.get<Ordinal>()); (s & mask) != 0) {
			_ac.conditionCode = 0b010;

			union {
				Integer value : 11;
			} displacement;
			displacement.value = targ;
			_instructionPointer = _instructionPointer + 4 + (displacement.value * 4);
		} else {
			_ac.conditionCode = 0;
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

	void Core::extract(SourceRegister bitpos, SourceRegister len, DestinationRegister srcDest) noexcept {
		srcDest.set<Ordinal>(decode(bitpos.get<Ordinal>(), len.get<Ordinal>(), srcDest.get<Ordinal>()));
	}

	/**
	 * Find the most significant set bit
	 */
	void Core::scanbit(SourceRegister src, DestinationRegister dest) noexcept {
		dest.set<Ordinal>(0xFFFF'FFFF);
		_ac.conditionCode = 0b000;
		for (Integer i = 31; i >= 0; --i) {
			if (auto k = 1 << i; (src.get<Ordinal>() & k) != 0) {
				_ac.conditionCode = 0b010;
				dest.set<Ordinal>(i);
				break;
			}
		}
	}
	/**
	 * Find the most significant clear bit
	 */
	void Core::spanbit(SourceRegister src, DestinationRegister dest) noexcept {
		dest.set<Ordinal>(0xFFFF'FFFF);
		_ac.conditionCode = 0b000;
		for (Integer i = 31; i >= 0; --i) {
			if (auto k = (1 << i); (src.get<Ordinal>() & k) == 0) {
				dest.set<Ordinal>(i);
				_ac.conditionCode = 0b010;
				break;
			}
		}
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

	void Core::cmpi(SourceRegister src1, SourceRegister src2) noexcept { compare(src1.get<Integer>(), src2.get<Integer>()); }
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
	void Core::syncf() noexcept {
		// this does nothing for the time being because this implementation does not execute instructions 
		// in parallel. When we get there this will become an important instruction
	}
	void Core::flushreg() noexcept {
		// this will nop currently as I'm saving all local registers to the 
		// stack when a call happens
	}
	void Core::subi(__DEFAULT_THREE_ARGS__) noexcept {
		dest.set<Integer>(src2.get<Integer>() - src1.get<Integer>());
		// check for overflow
		if ((src2.mostSignificantBit() != src1.mostSignificantBit()) && (src2.mostSignificantBit() != dest.mostSignificantBit())) {
			if (_ac.integerOverflowMask == 1) {
				_ac.integerOverflowFlag = 1;
			} else {
				generateFault(ArithmeticFaultSubtype::IntegerOverflow);
			}
		}
	}
	void Core::modtc(SourceRegister src, SourceRegister mask, DestinationRegister dest) noexcept {
		TraceControls tmp;
		tmp.value = _tc.value;
		auto temp1 = 0x00FF00FF & mask.get<Ordinal>(); // masked to prevent reserved bits from being used
		_tc.value = (temp1 & src.get<Ordinal>()) | (_tc.value & (~temp1));
		dest.set(tmp.value);
	}
	void Core::modpc(SourceRegister, SourceRegister mask, DestinationRegister srcDest) noexcept {
		// modify process controls
		auto maskVal = mask.get<Ordinal>();
		if (maskVal != 0) {
			if (_pc.inUserMode()) {
				generateFault(TypeFaultSubtype::Mismatch);
				return;
			}
			ProcessControls temp;
			temp.value = _pc.value;
			_pc.value = (maskVal & srcDest.get<Ordinal>()) | (_pc.value & (~maskVal));
			srcDest.set(temp.value);
			if (temp.priority > _pc.priority) {
				// TODO check pending interrupts
			}
			// if continue here, no interrupt to do
		} else {
			srcDest.set(_pc.value);
		}
	}
	void Core::modac(__DEFAULT_THREE_ARGS__) noexcept {
		auto tmp = _ac.value;
		_ac.value = (src2.get<Ordinal>() & src1.get<Ordinal>()) | (_ac.value & (~src1.get<Ordinal>()));
		dest.set<Ordinal>(tmp);
	}
	void Core::addc(__DEFAULT_THREE_ARGS__) noexcept {
		dest.set<Ordinal>(src2.get<Ordinal>() + src1.get<Ordinal>() + _ac.getCarryValue());
		_ac.conditionCode = 0b000; // odd that they would do this first as it breaks their action description in the manual
		auto overflowHappened = ((src2.mostSignificantBit() == src1.mostSignificantBit()) && (src2.mostSignificantBit() != dest.mostSignificantBit()));
		// combine the most significant bit of the 
		_ac.conditionCode = (dest.mostSignificantBit() << 1) + (overflowHappened ? 1 : 0);
	}
    void Core::freeCurrentRegisterSet() noexcept {
        // TODO implement
    }
    bool Core::registerSetNotAllocated(const Operand& fp) noexcept {
        // TODO implement
        return true;
    }
    void Core::retrieveFromMemory(const Operand& fp) noexcept {
        // TODO implement
    }
	void Core::ret() noexcept {
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
                        // TODO check for pending interrupts
                    }
                }();
                break;
        }
	}
	void Core::stob(__TWO_SOURCE_REGS__) noexcept {
		auto upper = load(src2.get<Ordinal>()) & 0xFFFFFF00;
		auto lower = src1.get<ByteOrdinal>();
		store(src2.get<Ordinal>(), upper | lower);
	}
	void Core::stos(__TWO_SOURCE_REGS__) noexcept {
		auto upper = load(src2.get<Ordinal>()) & 0xFFFF0000;
		auto lower = src1.get<ShortOrdinal>();
		store(src2.get<Ordinal>(), upper | lower);
	}
	void Core::stib(__TWO_SOURCE_REGS__) noexcept {
		auto upper = load(src2.get<Ordinal>()) & 0xFFFFFF00;
		auto lower = Ordinal(src1.get<Integer>() & 0x000000FF);
		store(src2.get<Ordinal>(), upper | lower);
	}
	void Core::stis(__TWO_SOURCE_REGS__) noexcept {
		// load the complete value
		auto upper = load(src2.get<Ordinal>()) & 0xFFFF0000;
		auto lower = Ordinal(src1.get<Integer>() & 0x0000FFFF);
		store(src2.get<Ordinal>(), upper | lower);
	}
	void Core::st(__TWO_SOURCE_REGS__) noexcept {
		store(src2.get<Ordinal>(), src1.get<Ordinal>());
	}
	void Core::opnot(__DEFAULT_TWO_ARGS__) noexcept {
		dest.set(~src.get<Ordinal>());
	}
	void Core::notand(__DEFAULT_THREE_ARGS__) noexcept {
		dest.set<Ordinal>((~src2.get<Ordinal>()) & src1.get<Ordinal>());
	}
	void Core::notbit(__DEFAULT_THREE_ARGS__) noexcept {
		dest.set<Ordinal>(i960::notBit(src2.get<Ordinal>(), src1.get<Ordinal>()));
	}
	void Core::notor(__DEFAULT_THREE_ARGS__) noexcept {
		dest.set<Ordinal>((~src2.get<Ordinal>()) | src1.get<Ordinal>());
	}
	void Core::opor(__DEFAULT_THREE_ARGS__) noexcept {
		dest.set(src2.get<Ordinal>() | src1.get<Ordinal>());
	}
	void Core::ornot(__DEFAULT_THREE_ARGS__) noexcept {
		dest.set(src2.get<Ordinal>() | (~src1.get<Ordinal>()));
	}
	void Core::nand(__DEFAULT_THREE_ARGS__) noexcept {
		// as shown in the manual
		dest.set((~src2.get<Ordinal>()) | (~src1.get<Ordinal>()));
	}
	void Core::nor(__DEFAULT_THREE_ARGS__) noexcept {
		dest.set((~src2.get<Ordinal>()) & (~src1.get<Ordinal>()));
	}
	void Core::shro(__DEFAULT_THREE_ARGS__) noexcept {
        if (auto shift = src1.get<Ordinal>(); shift < 32u) {
            dest.set<Ordinal>(src2.get<Ordinal>() >> shift);
        } else {
            dest.set<Ordinal>(0u);
        }
	}
	void Core::shri(__DEFAULT_THREE_ARGS__) noexcept {
		dest.set(src2.get<Integer>() >> src1.get<Integer>());
	}
	void Core::shlo(__DEFAULT_THREE_ARGS__) noexcept {
        if (auto shift = src1.get<Ordinal>(); shift < 32u) {
            dest.set<Ordinal>(src2.get<Ordinal>() << shift);
        } else {
            dest.set<Ordinal>(0u);
        }
	}
	void Core::shli(__DEFAULT_THREE_ARGS__) noexcept {
		dest.set(src2.get<Integer>() << src1.get<Integer>());
	}
	void Core::shrdi(SourceRegister len, SourceRegister src, DestinationRegister dest) noexcept {
		auto result = src.get<Integer>() >> len.get<Integer>();
		if (result < 0) {
            ++result;
		}
		dest.set<Integer>(result);
	}
	void Core::rotate(__DEFAULT_THREE_ARGS__) noexcept {
		dest.set<Ordinal>(i960::rotate(src2.get<Ordinal>(), src1.get<Ordinal>()));
	}
	void Core::modify(SourceRegister mask, SourceRegister src, DestinationRegister srcDest) noexcept {
		srcDest.set<Ordinal>((src.get<Ordinal>() & mask.get<Ordinal>()) | (srcDest.get<Ordinal>() & (~src.get<Ordinal>())));
	}
	template<Ordinal mask>
	constexpr bool maskedEquals(Ordinal src1, Ordinal src2) noexcept {
		return (src1 & mask) == (src2 & mask);
	}
	void Core::scanbyte(__TWO_SOURCE_REGS__) noexcept {
		if (auto s1 = src1.get<Ordinal>(), s2 = src2.get<Ordinal>(); 
                maskedEquals<0x000000FF>(s1, s2) ||
				maskedEquals<0x0000FF00>(s1, s2) ||
				maskedEquals<0x00FF0000>(s1, s2) ||
				maskedEquals<0xFF000000>(s1, s2)) {
			_ac.conditionCode = 0b010;
		} else {
			_ac.conditionCode = 0b000;
		}
	}
	constexpr Ordinal alignToWordBoundary(Ordinal value) noexcept {
		return value & (~0x3);
	}
	void Core::atmod(__DEFAULT_THREE_ARGS__) noexcept {
		// TODO implement
		auto srcDest = dest.get<Ordinal>();
		auto mask = src2.get<Ordinal>();
		auto fixedAddr = alignToWordBoundary(src1.get<Ordinal>());
		auto tmp = load(fixedAddr, true);
		store(fixedAddr, (srcDest & mask) | (tmp & (~mask)), true);
		dest.set<Ordinal>(tmp);
	}
	void Core::atadd(__DEFAULT_THREE_ARGS__) noexcept {
		// TODO implement atomic operations
		auto fixedAddr = alignToWordBoundary(src1.get<Ordinal>());
		auto src = src2.get<Ordinal>();
		auto tmp = load(fixedAddr, true);
		store(fixedAddr, tmp + src, true);
		dest.set<Ordinal>(tmp);
	}
	void Core::concmpo(__TWO_SOURCE_REGS__) noexcept {
		concmpBase<Ordinal>(src1, src2);
	}
	void Core::concmpi(__TWO_SOURCE_REGS__) noexcept {
		concmpBase<Integer>(src1, src2);
	}
	void Core::cmpinco(__DEFAULT_THREE_ARGS__) noexcept {
		// TODO add support for overflow detection
		cmpo(src1, src2);
		dest.set<Ordinal>(src2.get<Ordinal>() + 1);
	}
	void Core::cmpdeco(__DEFAULT_THREE_ARGS__) noexcept {
		// TODO add support for overflow detection
		cmpo(src1, src2);
		dest.set<Ordinal>(src2.get<Ordinal>() - 1);
	}
	void Core::cmpinci(__DEFAULT_THREE_ARGS__) noexcept {
		cmpi(src1, src2);
		dest.set<Integer>(src2.get<Integer>() + 1); // overflow suppressed
	}
	void Core::cmpdeci(__DEFAULT_THREE_ARGS__) noexcept {
		cmpi(src1, src2);
		dest.set<Integer>(src2.get<Integer>() - 1);
	}
	void Core::clrbit(__DEFAULT_THREE_ARGS__) noexcept {
		dest.set<Ordinal>(i960::clearBit(src2.get<Ordinal>(), src1.get<Ordinal>()));
	}
	void Core::setbit(__DEFAULT_THREE_ARGS__) noexcept {
		dest.set<Ordinal>(i960::setBit(src2.get<Ordinal>(), src1.get<Ordinal>()));
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
	void Core::fmark() noexcept {
		if (_pc.traceEnabled()) {
			generateFault(TraceFaultSubtype::Mark);
		}
	}
	void Core::mark() noexcept {
		// force mark aka generate a breakpoint trace-event
		if (_pc.traceEnabled() && _tc.traceMarked()) {
			generateFault(TraceFaultSubtype::Mark);
		}
	}
    void Core::xnor(__DEFAULT_THREE_ARGS__) noexcept {
		dest.set<Ordinal>(~(src2.get<Ordinal>() | src1.get<Ordinal>()) | (src2.get<Ordinal>() & src1.get<Ordinal>()));
    }
    void Core::opxor(__DEFAULT_THREE_ARGS__) noexcept {
		// there is an actual implementation within the manual so I'm going to
		// use that instead of the xor operator.
		dest.set<Ordinal>((src2.get<Ordinal>() | src1.get<Ordinal>()) & ~(src2.get<Ordinal>() & src1.get<Ordinal>()));
    }
	void Core::intdis() {
		// TODO implement
		if (!_pc.inSupervisorMode()) {
			generateFault(TypeFaultSubtype::Mismatch);
		}
	}
	void Core::inten() {
		// TODO implement
		if (!_pc.inSupervisorMode()) {
			generateFault(TypeFaultSubtype::Mismatch);
		}
	}

	void Core::halt(SourceRegister src1) {
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
			switch (src1.get<Ordinal>()) {
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
	void Core::bswap(SourceRegister src1, DestinationRegister src2) noexcept {
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
		auto src = src1.get<Ordinal>();
		auto rotl8 = i960::rotate(src, 8) & 0x00FF00FF; // rotate the upper 8 bits around to the bottom
		auto rotl24 = i960::rotate(src, 24) & 0xFF00FF00;
		src2.set<Ordinal>(rotl8 + rotl24);
	}
	void Core::cmpos(SourceRegister src1, SourceRegister src2) noexcept { 
		compare<ShortOrdinal>(src1.get<ShortOrdinal>(), src2.get<ShortOrdinal>());
	}
	void Core::cmpis(SourceRegister src1, SourceRegister src2) noexcept { 
		compare<ShortInteger>(src1.get<ShortInteger>(), src2.get<ShortInteger>());
	}
	void Core::cmpob(SourceRegister src1, SourceRegister src2) noexcept { 
		compare<ByteOrdinal>(src1.get<ByteOrdinal>(), src2.get<ByteOrdinal>());
	}
	void Core::cmpib(SourceRegister src1, SourceRegister src2) noexcept { 
		compare<ByteInteger>(src1.get<ByteInteger>(), src2.get<ByteInteger>());
	}
	void Core::dcctl(__DEFAULT_THREE_ARGS__) noexcept { 
		// while I don't implement a data cache myself, this instruction must
		// do something!
		// TODO something
		if (!_pc.inSupervisorMode()) {
			generateFault(TypeFaultSubtype::Mismatch);
		}
	}
	void Core::eshro(SourceRegister src1, ByteOrdinal src2Ind, DestinationRegister dest) noexcept {
		// TODO perform byte ordinal check to make sure it is even
		dest.set<Ordinal>(makeLongRegister(src2Ind).get<LongOrdinal>() >> (src1.get<Ordinal>() & 0b11111));
	}
	void Core::icctl(__DEFAULT_THREE_ARGS__) noexcept { 
		// TODO implement
		if (!_pc.inSupervisorMode()) {
			generateFault(TypeFaultSubtype::Mismatch);
		}
	}
	void Core::intctl(__DEFAULT_TWO_ARGS__) { 
		// TODO: implement
		if (!_pc.inSupervisorMode()) {
			generateFault(TypeFaultSubtype::Mismatch);
		}
	}
	void Core::sysctl(__DEFAULT_THREE_ARGS__) noexcept { 
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
		constexpr Ordinal ibrPtr = 0xFEFF'FF30;
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
		// TODO implement according to the manual
	}
#undef __DEFAULT_TWO_ARGS__
#undef __DEFAULT_DOUBLE_WIDE_TWO_ARGS__
#undef __DEFAULT_THREE_ARGS__
#undef __DEFAULT_DOUBLE_WIDE_THREE_ARGS__
#undef __TWO_SOURCE_REGS__
} // end namespace i960
