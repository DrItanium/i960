#include "types.h"
#include "core.h"
#include "operations.h"
#include "opcodes.h"
#include <limits>
#include <cmath>
#define __DEFAULT_THREE_ARGS__ SourceRegister src1, SourceRegister src2, DestinationRegister dest
#define __DEFAULT_DOUBLE_WIDE_THREE_ARGS__ const DoubleRegister& src1, const DoubleRegister& src2, DoubleRegister& dest
#define __DEFAULT_TWO_ARGS__ SourceRegister src, DestinationRegister dest
#define __DEFAULT_DOUBLE_WIDE_TWO_ARGS__ const DoubleRegister& src, DoubleRegister& dest
#define __TWO_SOURCE_AND_INT_ARGS__ SourceRegister src1, SourceRegister src2, Integer targ
#define __TWO_SOURCE_REGS__ SourceRegister src1, SourceRegister src2
namespace i960 {
	constexpr bool notDivisibleBy(ByteOrdinal value, ByteOrdinal factor) noexcept {
		return ((value % factor) != 0);
	}
#define X(kind, action) \
	void Core:: test ## kind (DestinationRegister dest) noexcept { testGeneric<TestTypes:: action>(dest); } \
	void Core:: b ## kind (Integer addr) noexcept { branchIfGeneric<ConditionCode:: action > ( addr ) ; } 
#include "conditional_kinds.def"
#undef X
#define X(kind, mask) \
	void Core:: sel ## kind (__DEFAULT_THREE_ARGS__) noexcept { \
		baseSelect<mask>(src1, src2, dest); \
	}
	X(no, 0b000);
	X(g, 0b001);
	X(e, 0b010);
	X(ge, 0b011);
	X(l, 0b100);
	X(ne, 0b101);
	X(le, 0b110);
	X(o, 0b111);
#undef X
#define X(base, kind, code) \
	void Core:: base ## kind ( __DEFAULT_THREE_ARGS__ ) noexcept { \
		base ## Base <code> ( src1, src2, dest ) ; \
	}
#define Y(base) \
	X(base, no, 0b000); \
	X(base, g, 0b001); \
	X(base, e, 0b010); \
	X(base, ge, 0b011); \
	X(base, l, 0b100); \
	X(base, ne, 0b101); \
	X(base, le, 0b110); \
	X(base, o, 0b111);
	Y(subo); Y(subi);
	Y(addo); Y(addi);
#undef Y
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
	Core::Core(MemoryInterface& mem) : _mem(mem) { }
	Ordinal Core::getStackPointerAddress() const noexcept {
		return _localRegisters[StackPointerIndex].get<Ordinal>();
	}
	void Core::saveLocalRegisters() noexcept {
		auto base = getFramePointerAddress();
		auto end = base + (sizeof(Ordinal) * LocalRegisterCount);

		for (Ordinal addr = base, i = 0; addr < end; addr += sizeof(Ordinal), ++i) {
			store(addr, _localRegisters[i].get<Ordinal>());
		}
	}
	void Core::allocateNewLocalRegisterSet() noexcept {
		// this function does nothing at this point as we are always saving locals to ram
	}
	void Core::setFramePointer(Ordinal value) noexcept {
		_globalRegisters[FramePointerIndex].set(value);
	}
	Ordinal Core::getFramePointerAddress() const noexcept {
		return _globalRegisters[FramePointerIndex].get<Ordinal>() & (~0b111111);
	}
	auto Core::getPFP() noexcept -> PreviousFramePointer& {
		return _localRegisters[PreviousFramePointerIndex].pfp;
	}
	void Core::call(Integer addr) noexcept {
		union {
			Integer _value : 22;
		} conv;
		conv._value = addr;
		auto newAddress = conv._value;
		auto tmp = (getStackPointerAddress() + 63) & (~63); // round to the next boundary
		setRegister(ReturnInstructionPointerIndex, _instructionPointer);
		saveLocalRegisters();
		allocateNewLocalRegisterSet();
		_instructionPointer += newAddress;
		setRegister(PreviousFramePointerIndex, getFramePointerAddress());
		setFramePointer(tmp);
		setRegister(StackPointerIndex, tmp + 64);
	}
	constexpr Ordinal getProcedureAddress(Ordinal value) noexcept {
		return value & (~0b11);
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
		auto targ = value.get<ShortOrdinal>();
		if (targ > 259) {
			// raise protection length fault here
			return;
		}
		Ordinal temp = 0;
		Ordinal newRRR = 0;
		// This code is 80960KB specific, I keep getting different answers :(
		auto tempPE = load(_systemProcedureTableAddress + 48 + (4 * targ));
		setRegister(ReturnInstructionPointerIndex, _instructionPointer);
		_instructionPointer = getProcedureAddress(tempPE);
		if (isLocalProcedure(tempPE) || (_pc.executionMode != 0)) {
			temp = (getStackPointerAddress() + 63) & (~63);
			newRRR = 0;
		} else {
			// I think the calls documentation for 80960kb is bugged because it references
			// structures from 80960MC...
			//
			// In 80960KB the SSP is in the Procedure Table Structure but the
			// documentation says to load from the PRCB to get the supervisor stack
			// address. That makes no sense since 12 bytes off of the PRCB
			// base is currentProcessSS (segment selector) not the supervisor
			// stack address
			//
			// The 80960 calls loads the supervisor stack from a different
			// location 
			temp = load(_systemProcedureTableAddress + 12);
			newRRR = 0b010 | _pc.traceEnable;
			_pc.executionMode = 1;
			_pc.traceEnable = temp & 0b1;
		}
		saveLocalRegisters();
		allocateNewLocalRegisterSet();
		setRegister(PreviousFramePointerIndex, getFramePointerAddress());
		getRegister(PreviousFramePointerIndex).pfp.returnCode = newRRR;
		setFramePointer(temp);
		setRegister(StackPointerIndex, temp + 64);
	}

	Ordinal Core::load(Ordinal address, bool atomic) noexcept {
		if (address <= _internalDataRam.LargestAddress<0>) {
			return _internalDataRam.read(address & (~0b11));
		} else {
			return _mem.load(address, atomic);
		}
	}
	void Core::store(Ordinal address, Ordinal value, bool atomic) noexcept {
		if (address <= _internalDataRam.LargestAddress<0>) {
			_internalDataRam.write(address & (~0b11), value);
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
	void Core::setRegister(ByteOrdinal index, const NormalRegister& other) noexcept {
		setRegister(index, other.get<Ordinal>());
	}
	void Core::callx(const NormalRegister& value) noexcept {
		auto newAddress = value.get<Ordinal>();
		Ordinal tmp = (getStackPointerAddress() + 63u) && (~63u); // round to the next boundary
		setRegister(ReturnInstructionPointerIndex, _instructionPointer);
		// Code for handling multiple internal local register sets not implemented!
		saveLocalRegisters();
		allocateNewLocalRegisterSet();
		_instructionPointer = newAddress;
		setRegister(PreviousFramePointerIndex, getFramePointerAddress());
		setFramePointer(tmp);
		setRegister(StackPointerIndex, tmp + 64u);
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
		auto s = src.get<Ordinal>();
		auto p = pos.get<Ordinal>() & 0b11111;
		if ((_ac.conditionCode & 0b010)  == 0) {
			// if the condition bit is clear then we clear the given bit
			dest.set<Ordinal>(s & (~(1 << p)));
		} else {
			// if the condition bit is set then we set the given bit
			dest.set<Ordinal>(s | (1 << p));
		}
	}
	void Core::opand(__DEFAULT_THREE_ARGS__) noexcept {
		dest.set<Ordinal>(i960::andOp<Ordinal>(src2.get<Ordinal>(), src1.get<Ordinal>()));
	}
	void Core::andnot(__DEFAULT_THREE_ARGS__) noexcept {
		dest.set<Ordinal>(i960::andNot(src2.get<Ordinal>(), src1.get<Ordinal>()));
	}

	void DoubleRegister::move(const DoubleRegister& other) noexcept {
		_lower.move(other._lower);
		_upper.move(other._upper);
	}
	void TripleRegister::move(const TripleRegister& other) noexcept {
		_lower.move(other._lower);
		_mid.move(other._mid);
		_upper.move(other._upper);
	}
	void QuadRegister::move(const QuadRegister& other) noexcept {
		_lower.move(other._lower);
		_mid.move(other._mid);
		_upper.move(other._upper);
		_highest.move(other._highest);
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
		_instructionPointer &= 0xFFFF'FFFC; // make sure the least significant two bits are clear
	}
	void Core::bx(SourceRegister src) noexcept {
		_instructionPointer = src.get<Ordinal>();
		_instructionPointer &= 0xFFFF'FFFC; // make sure the least significant two bits are clear
	}
	void Core::bal(Integer displacement) noexcept {
		_globalRegisters[14].set<Ordinal>(_instructionPointer + 4);
		b(displacement);
	}

	void Core::balx(__DEFAULT_TWO_ARGS__) noexcept {
		// TODO support 4 or 8 byte versions
		dest.set<Ordinal>(_instructionPointer + 4);
		_instructionPointer = src.get<Ordinal>();
	}

	void Core::bbc(SourceRegister bitpos, SourceRegister src, Integer targ) noexcept {

		// check bit and branch if clear
		checkBitAndBranchIf<false>(bitpos, src, targ);
	}
	void Core::bbs(SourceRegister bitpos, SourceRegister src, Integer targ) noexcept {
		// check bit and branch if set
		checkBitAndBranchIf<true>(bitpos, src, targ);
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
		auto k = src.get<Ordinal>();
		dest.set<Ordinal>(0xFFFF'FFFF);
		_ac.conditionCode = 0b000;
		for (Ordinal i = 31; i >= 0; --i) {
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
		for (Ordinal i = 31; i >= 0; --i) {
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
			auto divOp = s2 / s1;
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

	void Core::ld(SourceRegister src, DestinationRegister dest) noexcept {
		// this is the base operation for load, src contains the fully computed value
		// so this will probably be an internal register in most cases.
		dest.set<Ordinal>(load(src.get<Ordinal>()));
	}
	void Core::ldob(SourceRegister src, DestinationRegister dest) noexcept {
		dest.set<ByteOrdinal>(load(src.get<Ordinal>()));
	}
	void Core::ldos(SourceRegister src, DestinationRegister dest) noexcept {
		dest.set<ShortOrdinal>(load(src.get<Ordinal>()));
	}
	void Core::ldib(SourceRegister src, DestinationRegister dest) noexcept {
		dest.set<Integer>((ByteInteger)load(src.get<Ordinal>()));
	}
	void Core::ldis(SourceRegister src, DestinationRegister dest) noexcept {
		dest.set<Integer>((ShortInteger)load(src.get<Ordinal>()));
	}

	void Core::ldl(SourceRegister src, Ordinal srcDestIndex) noexcept {
		// TODO make sure that the srcDestIndex makes sense
		LongRegister dest = makeLongRegister(srcDestIndex);
		auto addr = src.get<Ordinal>();
		dest.set(load(addr), load(addr + 1));
	}
	void DoubleRegister::set(Ordinal lower, Ordinal upper) noexcept {
		_lower.set<Ordinal>(lower);
		_upper.set<Ordinal>(upper);
	}
	void Core::ldt(SourceRegister src, Ordinal srcDestIndex) noexcept {
		// TODO make sure that the srcDestIndex makes sense
		TripleRegister dest = makeTripleRegister(srcDestIndex);
		//TripleRegister reg(getRegister(srcDestIndex), getRegister(srcDestIndex + 1), getRegister(srcDestIndex + 2));
		auto addr = src.get<Ordinal>();
		dest.set(load(addr), load(addr + 1), load(addr + 2));
	}
	void TripleRegister::set(Ordinal l, Ordinal m, Ordinal u) noexcept {
		_lower.set<Ordinal>(l);
		_mid.set<Ordinal>(m);
		_upper.set<Ordinal>(u);
	}
	void Core::ldq(SourceRegister src, Ordinal index) noexcept {
		// TODO make sure that the srcDestIndex makes sense
		QuadRegister dest = makeQuadRegister(index);
		//QuadRegister reg(getRegister(index), getRegister(index + 1), getRegister(index + 2), getRegister(index + 3));
		auto addr = src.get<Ordinal>();
		dest.set(load(addr), load(addr + 1), load(addr + 2), load(addr + 3));
	}
	void QuadRegister::set(Ordinal l, Ordinal m, Ordinal u, Ordinal h) noexcept {
		_lower.set(l);
		_mid.set(m);
		_upper.set(u);
		_highest.set(h);
	}

	void Core::cmpi(SourceRegister src1, SourceRegister src2) noexcept { compare(src1.get<Integer>(), src2.get<Integer>()); }
	void Core::cmpo(SourceRegister src1, SourceRegister src2) noexcept { compare(src1.get<Ordinal>(), src2.get<Ordinal>()); }
	void Core::muli(SourceRegister src1, SourceRegister src2, DestinationRegister dest) noexcept {
		auto s2 = src2.get<Integer>();
		auto s1 = src1.get<Integer>();
		dest.set(s2 * s1);
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
		store(dest.get<Ordinal>(), src.getLowerHalf());
		store(dest.get<Ordinal>() + sizeof(Ordinal), src.getUpperHalf());
	}

	void Core::stt(Ordinal ind, SourceRegister dest) noexcept {
		// TODO perform fault checks
		TripleRegister src = makeTripleRegister(ind);
		auto addr = dest.get<Ordinal>();
		store(addr, src.getLowerPart());
		store(addr + sizeof(Ordinal), src.getMiddlePart());
		store(addr + (2 * sizeof(Ordinal)), src.getUpperPart());
	}

	void Core::stq(Ordinal ind, SourceRegister dest) noexcept {
		QuadRegister src = makeQuadRegister(ind);
		auto addr = dest.get<Ordinal>();
		store(addr, src.getLowestPart());
		store(addr + sizeof(Ordinal), src.getLowerPart());
		store(addr + (2 * sizeof(Ordinal)), src.getHigherPart());
		store(addr + (3 * sizeof(Ordinal)), src.getHighestPart());
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
		auto src = src2.get<Ordinal>();
		auto mask = src1.get<Ordinal>();
		auto tmp = _ac.value;
		_ac.value = (src & mask) | (_ac.value & (~mask));
		dest.set<Ordinal>(tmp);
	}
	void Core::addc(__DEFAULT_THREE_ARGS__) noexcept {
		dest.set<Ordinal>(src2.get<Ordinal>() + src1.get<Ordinal>() + _ac.getCarryValue());
		_ac.conditionCode = 0b000; // odd that they would do this first as it breaks their action description in the manual
		auto overflowHappened = ((src2.mostSignificantBit() == src1.mostSignificantBit()) && (src2.mostSignificantBit() != dest.mostSignificantBit()));
		// combine the most significant bit of the 
		_ac.conditionCode = (dest.mostSignificantBit() << 1) + (overflowHappened ? 1 : 0);
	}
	void Core::ret() noexcept {
		// TODO implement
		auto pfp = getPFP();
		auto standardRestore = [this, &pfp]() {
			setRegister(FramePointerIndex, pfp);
			// TODO implement this logic
			// free current register set
			// if register_set (FP) not allocated
			//    then retrieve from memory(FP);
			// endif
			_instructionPointer = getRegister(ReturnInstructionPointerIndex).get<Ordinal>();
		};
		auto case1 = [this, standardRestore]() {
			auto fp = getFramePointerAddress();
			auto x = load(fp - 16);
			auto y = load(fp - 12);
			standardRestore();
			_ac.value = y;
			if (_pc.executionMode != 0) {
				_pc.value = x;
			}
		};
		auto case2 = [this, standardRestore]() {
			if (_pc.executionMode == 0) {
				standardRestore();
			} else {
				_pc.traceEnable = 0;
				_pc.executionMode = 0;
				standardRestore();
			}
		};
		auto case3 = [this, standardRestore]() {
			if (_pc.executionMode == 0) {
				standardRestore();
			} else {
				_pc.traceEnable = 1;
				_pc.executionMode = 0;
				standardRestore();
			}
		};
		auto case4 = [this, standardRestore]() {
			if (_pc.executionMode != 0) {
				// free current register set
				// check pending interrupts
				// if continue here, no interrupt to do
				// enter idle state
				throw "Unimplemented!";
			} else {
				standardRestore();
			}
		};
		auto case5 = [this, standardRestore]() {
			auto fp = getFramePointerAddress();
			auto x = load(fp - 16);
			auto y = load(fp - 12);
			standardRestore();
			_ac.value = y;
			if (_pc.executionMode != 0) {
				_pc.value = x;
				// TODO check pending interrupts
			}
		};
		switch(pfp.returnCode) {
			case 0b000:
				standardRestore();
				break;
			case 0b001:
				case1();
				break;
			case 0b010:
				case2();
				break;
			case 0b011:
				case3();
				break;
			case 0b110:
				case4();
				break;
			case 0b111:
				case5();
				break;
			case 0b100:
			case 0b101:
			default:
				throw "Undefined operation";
		}
	}
	void Core::faulte() noexcept {
        if (conditionCodeIs<ConditionCode::Equal>()) {
			generateFault(ConstraintFaultSubtype::Range);
        }
	}
	void Core::faultne() noexcept {
        if (conditionCodeIs<ConditionCode::NotEqual>()) {
			generateFault(ConstraintFaultSubtype::Range);
        }
	}
	void Core::faultl() noexcept {
        if (conditionCodeIs<ConditionCode::Less>()) {
			generateFault(ConstraintFaultSubtype::Range);
        }
	}
	void Core::faultle() noexcept {
        if (conditionCodeIs<ConditionCode::LessOrEqual>()) {
			generateFault(ConstraintFaultSubtype::Range);
        }
	}
	void Core::faultg() noexcept {
        if (conditionCodeIs<ConditionCode::Greater>()) {
			generateFault(ConstraintFaultSubtype::Range);
        }
	}
	void Core::faultge() noexcept {
        if (conditionCodeIs<ConditionCode::GreaterOrEqual>()) {
			generateFault(ConstraintFaultSubtype::Range);
        }
	}
	void Core::faulto() noexcept {
        if (conditionCodeIs<ConditionCode::Ordered>()) {
			generateFault(ConstraintFaultSubtype::Range);
        }
	}
	void Core::faultno() noexcept {
        if (_ac.conditionCode == 0) {
			generateFault(ConstraintFaultSubtype::Range);
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
		dest.set<Ordinal>((~src2.get<Ordinal>()) & src2.get<Ordinal>());
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
		auto s1 = src1.get<Ordinal>();
		auto s2 = src2.get<Ordinal>();
		// as shown in the manual
		dest.set((~s2) | (~s1));
	}
	void Core::nor(__DEFAULT_THREE_ARGS__) noexcept {
		auto s1 = src1.get<Ordinal>();
		auto s2 = src2.get<Ordinal>();
		dest.set((~s2) & (~s1));
	}
	void Core::shro(__DEFAULT_THREE_ARGS__) noexcept {
		auto shift = src1.get<Ordinal>();
		dest.set<Ordinal>((shift < 32u) ? src2.get<Ordinal>() >> shift : 0u);
	}
	void Core::shri(__DEFAULT_THREE_ARGS__) noexcept {
		dest.set(src2.get<Integer>() >> src1.get<Integer>());
	}
	void Core::shlo(__DEFAULT_THREE_ARGS__) noexcept {
		auto shift = src1.get<Ordinal>();
		dest.set<Ordinal>((shift < 32u) ? src2.get<Ordinal>() << shift : 0u);
	}
	void Core::shli(__DEFAULT_THREE_ARGS__) noexcept {
		dest.set(src2.get<Integer>() << src1.get<Integer>());
	}
	void Core::shrdi(SourceRegister len, SourceRegister src, DestinationRegister dest) noexcept {
		auto result = src.get<Integer>() >> len.get<Integer>();
		if (result < 0) {
			result += 1;
		}
		dest.set<Integer>(result);
	}
	void Core::rotate(__DEFAULT_THREE_ARGS__) noexcept {
		dest.set<Ordinal>(i960::rotate(src2.get<Ordinal>(), src1.get<Ordinal>()));
	}
	void Core::modify(SourceRegister mask, SourceRegister src, DestinationRegister srcDest) noexcept {
		auto s = src.get<Ordinal>();
		auto m = mask.get<Ordinal>();
		srcDest.set<Ordinal>((s & m) | (srcDest.get<Ordinal>() & (~m)));
	}
	template<Ordinal mask>
	constexpr bool maskedEquals(Ordinal src1, Ordinal src2) noexcept {
		return (src1 & mask) == (src2 & mask);
	}
	void Core::scanbyte(__TWO_SOURCE_REGS__) noexcept {
		auto s1 = src1.get<Ordinal>();
		auto s2 = src2.get<Ordinal>();
		if (maskedEquals<0x000000FF>(s1, s2) ||
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
	void Core::reset() {
		// clear out the internal data ram
		_internalDataRam.reset();
		/* Taken from the 80960MC manual on how initialization works:
		 *
		 * 1. Assert the FAILURE output pin and perform the internal self-test.
		 * If the test passes, deassert FAILURE and continue with the step
		 * below; otherwise enter the stopped state.
		 */

		/*
		 * 2. Clear the trace controls, disable the breakpoint registers, clear
		 * the process controls, and then set the em flag in the process
		 * controls (supervisor mode). If the processor is an initialization
		 * processor, continue with the step below; otherwise enter the stopped
		 * state.
		 */
		_tc.clear();
		// disable the breakpoint registers
		_pc.clear();
		_pc.enterSupervisorMode();
		// assume that we are the initialization processor for now
		/*
		 * 3. Read eight words from memory, beginning at location 0. Clear the
		 * condition code, sum these eight words with the ADDC (add-with-carry)
		 * operation, and then add 0xFFFF'FFFF to the sum (again with addc). If
		 * the sum is 0, continue with the step below; otherwise assert the
		 * FAILURE pin and enter the stopped state.
		 */
		_ac.clear();
		auto& dest = _globalRegisters[10];
		dest.set<Ordinal>(0);
		auto& src = _globalRegisters[11];
		for (int i = 0; i < 8; ++i) {
			_initialWords[i] = load(i); // load the eight words
			src.set<Ordinal>(_initialWords[i]);
			addc(src, dest, dest);
		}
		src.set<Ordinal>(0xFFFF'FFFF);
		addc(src, dest, dest);
		if (dest.get<Ordinal>() != 0) {
			// TODO assert the FAILURE pin
			// TODO enter stop state
			return;
		}
	    /*
		 * 4. Use words 0 and 1 as the pointers to the initial data structures,
		 * and set the IP to the value of word 3. In the process controls, set
		 * the priority to 31 and the state to interrupted. Store the interrupt
		 * stack pointer in FP (g15), and begin execution.
		 */
		_systemProcedureTableAddress = _initialWords[0];
		_prcbAddress = _initialWords[1];
		_instructionPointer = _initialWords[3];
		_pc.priority = 31;
		_globalRegisters[15].set<Ordinal>(load(_prcbAddress + 24));
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
		auto s1 = src1.get<Ordinal>();
		auto s2 = src2.get<Ordinal>();
		dest.set<Ordinal>(~(s2 | s1) | (s2 & s1));
    }
    void Core::opxor(__DEFAULT_THREE_ARGS__) noexcept {
		auto s1 = src1.get<Ordinal>();
		auto s2 = src2.get<Ordinal>();
		// there is an actual implementation within the manual so I'm going to
		// use that instead of the xor operator.
		dest.set<Ordinal>((s2 | s1) & ~(s2 & s1));
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
		//LongRegister src2(getRegister(src2Ind), getRegister(src2Ind + 1));
		LongRegister src2 = makeLongRegister(src2Ind);
		dest.set<Ordinal>(src2.get<LongOrdinal>() >> (src1.get<Ordinal>() & 0b11111));
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
		// TODO implement
	}
#undef __DEFAULT_TWO_ARGS__
#undef __DEFAULT_DOUBLE_WIDE_TWO_ARGS__
#undef __DEFAULT_THREE_ARGS__
#undef __DEFAULT_DOUBLE_WIDE_THREE_ARGS__
#undef __TWO_SOURCE_REGS__
} // end namespace i960
