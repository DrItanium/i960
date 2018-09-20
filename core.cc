#include "types.h"
#include "core.h"
#include "operations.h"
#include "opcodes.h"
#include <limits>
#include <cmath>
#define __DEFAULT_THREE_ARGS__ Core::SourceRegister src1, Core::SourceRegister src2, Core::DestinationRegister dest
#define __DEFAULT_DOUBLE_WIDE_THREE_ARGS__ const DoubleRegister& src1, const DoubleRegister& src2, DoubleRegister& dest
#define __DEFAULT_TWO_ARGS__ Core::SourceRegister src, Core::DestinationRegister dest
#define __DEFAULT_DOUBLE_WIDE_TWO_ARGS__ const DoubleRegister& src, DoubleRegister& dest
#define __TWO_SOURCE_AND_INT_ARGS__ Core::SourceRegister src1, Core::SourceRegister src2, Integer targ
#define __TWO_SOURCE_REGS__ Core::SourceRegister src1, Core::SourceRegister src2
namespace i960 {
   Ordinal Core::getStackPointerAddress() const noexcept {
       return _localRegisters[StackPointerIndex].get<Ordinal>();
   }
   void Core::saveLocalRegisters() noexcept {
#warning "saveLocalRegisters unimplemented"
   }
   void Core::allocateNewLocalRegisterSet() noexcept {
#warning "allocateNewLocalRegisterSet unimplemented"
   }
   void Core::setFramePointer(Ordinal value) noexcept {
       _globalRegisters[FramePointerIndex].set(value);
   }
   Ordinal Core::getFramePointerAddress() const noexcept {
       return _globalRegisters[FramePointerIndex].get<Ordinal>() & (~0b111111);
   }
   auto Core::getPFP() noexcept -> PreviousFramePointer& {
       return _localRegisters[PreviousFramePointerIndex]._pfp;
   }
   void Core::call(Integer addr) noexcept {
       union {
           Integer _value : 22;
       } conv;
       conv._value = addr;
       auto newAddress = conv._value;
       auto tmp = (getStackPointerAddress() + 63) && (~63); // round to the next boundary
       setRegister(ReturnInstructionPointerIndex, _instructionPointer);
#warning "Code for handling multiple internal local register sets not implemented!"
       saveLocalRegisters();
       allocateNewLocalRegisterSet();
       _instructionPointer += newAddress;
       setRegister(PreviousFramePointerIndex, getFramePointerAddress());
       setFramePointer(tmp);
       setRegister(StackPointerIndex, tmp + 64);
   }
   void Core::calls(const NormalRegister& value) {
       auto callNum = value.get<ByteOrdinal>();
       if (callNum > 259) {
#warning "Raise protection length fault here"
           return;
       }
#warning "implement the rest of calls (call supervisor)"
   }

   Ordinal Core::load(Ordinal address) noexcept {
#warning "Core::load unimplemented!"
        return -1;
   }
   void Core::store(Ordinal address, Ordinal value) noexcept {
#warning "Core::store unimplemented!"
   }
   NormalRegister& Core::getRegister(ByteOrdinal index) noexcept {
       if (auto offset = (index & 0b01111) ; (index & 0b10000) == 0) {
           return _localRegisters[offset];
       } else {
           return _globalRegisters[offset];
       }
   }
   void Core::setRegister(ByteOrdinal index, const NormalRegister& other) noexcept {
       setRegister(index, other._ordinal);
   }
   void Core::callx(const NormalRegister& value) noexcept {
       auto newAddress = value._ordinal;
       Ordinal tmp = (getStackPointerAddress() + 63u) && (~63u); // round to the next boundary
       setRegister(ReturnInstructionPointerIndex, _instructionPointer);
#warning "Code for handling multiple internal local register sets not implemented!"
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
#warning "addi does not check for integer overflow"
       dest.set<Integer>(src2.get<Integer>() + src1.get<Integer>());
   }
   void Core::subo(__DEFAULT_THREE_ARGS__) noexcept {
       dest._ordinal = src2.get<Ordinal>() - src1.get<Ordinal>(); 
   }
   void Core::mulo(__DEFAULT_THREE_ARGS__) noexcept {
       dest.set(src2.get<Ordinal>() * src1.get<Ordinal>());
   }
   void Core::divo(__DEFAULT_THREE_ARGS__) noexcept {
#warning "divo does not check for divison by zero"
       dest.set(src2.get<Ordinal>() / src1.get<Ordinal>());
   }
   void Core::remo(__DEFAULT_THREE_ARGS__) noexcept {
#warning "remo does not check for divison by zero"
       dest.set(src2.get<Ordinal>() % src1.get<Ordinal>());
       
   }
   void Core::chkbit(Core::SourceRegister pos, Core::SourceRegister src) noexcept {
        _ac._conditionCode = ((src.get<Ordinal>() & (1 << (pos.get<Ordinal>() & 0b11111))) == 0) ? 0b000 : 0b010;
   }
   void Core::alterbit(Core::SourceRegister pos, Core::SourceRegister src, Core::DestinationRegister dest) noexcept {
       auto s = src.get<Ordinal>();
       auto p = pos.get<Ordinal>() & 0b11111;
       dest.set<Ordinal>(_ac._conditionCode & 0b010 == 0 ? s & (~(1 << p)) : s | (1 << p));
   }
   void Core::andOp(__DEFAULT_THREE_ARGS__) noexcept {
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

   template<typename T>
   void move(const T& src, T& dest) noexcept {
       dest.move(src);
   }

   void Core::mov(Core::SourceRegister src, Core::DestinationRegister dest) noexcept { 
       move(src, dest);
   }
   void Core::movl(Core::LongSourceRegister src, Core::LongDestinationRegister dest) noexcept { 
       move(src, dest);
   }
   void Core::movt(const TripleRegister& src, TripleRegister& dest) noexcept { 
       move(src, dest);
   }
   void Core::movq(const QuadRegister& src, QuadRegister& dest) noexcept {
       move(src, dest);
   }

   void Core::b(Integer displacement) noexcept {
       union {
           Integer _value : 24;
       } conv;
       conv._value = displacement;
       conv._value = conv._value > 8388604 ? 8388604 : conv._value;
       _instructionPointer += conv._value;
   }
   void Core::bx(Core::SourceRegister src) noexcept {
       _instructionPointer = src.get<Ordinal>();
   }
   void Core::bal(Integer displacement) noexcept {
       _globalRegisters[14]._ordinal = _instructionPointer + 4;
       b(displacement);
   }


   // Begin Instruction::REGFormat implementations
   bool Instruction::REGFormat::isFloatingPoint() const noexcept {
       return i960::isFloatingPoint(i960::Opcodes(getOpcode()));
   }
   
   RawReal Instruction::REGFormat::src1ToRealLiteral() const noexcept {
        if (auto bits = _source1; bits == 0b10000) {
            return +0.0f;
        } else if (bits == 0b10110) {
            return +1.0f;
        } else {
            return std::numeric_limits<RawReal>::quiet_NaN();
        }
   }
   RawReal Instruction::REGFormat::src2ToRealLiteral() const noexcept {
        if (auto bits = _source2; bits == 0b10000) {
            return +0.0f;
        } else if (bits == 0b10110) {
            return +1.0f;
        } else {
            return std::numeric_limits<RawReal>::quiet_NaN();
        }

   }
   RawLongReal Instruction::REGFormat::src1ToLongRealLiteral() const noexcept {
        if (auto bits = _source1; bits == 0b10000) {
            return +0.0;
        } else if (bits == 0b10110) {
            return +1.0;
        } else {
            return std::numeric_limits<RawLongReal>::quiet_NaN();
        }

   }
   RawLongReal Instruction::REGFormat::src2ToLongRealLiteral() const noexcept {
        if (auto bits = _source2; bits == 0b10000) {
            return +0.0;
        } else if (bits == 0b10110) {
            return +1.0;
        } else {
            return std::numeric_limits<RawLongReal>::quiet_NaN();
        }

   }
   bool Instruction::REGFormat::src1IsFloatingPointRegister() const noexcept { 
       return _m1 != 0 && (_source1 < 0b00100); 
   }
   bool Instruction::REGFormat::src2IsFloatingPointRegister() const noexcept { 
       return _m2 != 0 && (_source2 < 0b00100); 
   }
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

    void Core::extract(Core::SourceRegister bitpos, Core::SourceRegister len, Core::DestinationRegister srcDest) noexcept {
        srcDest._ordinal = decode(bitpos._ordinal, len._ordinal, srcDest._ordinal);
    }

    constexpr bool mostSignificantBitSet(Ordinal value) noexcept {
        return (value & 0x8000'0000) != 0;
    }
    constexpr bool mostSignificantBitClear(Ordinal value) noexcept {
        return !mostSignificantBitSet(value);
    }
    /**
     * Find the most significant set bit
     */
    void Core::scanbit(Core::SourceRegister src, Core::DestinationRegister dest) noexcept {
        auto k = src.get<Ordinal>();
        _ac._conditionCode = 0b000;
        for (int i = 31; i >= 0; --i) {
            if (mostSignificantBitSet(k)) {
                _ac._conditionCode = 0b010;
                dest.set<Ordinal>(i);
                return;
            }
            k <<= 1;
        }
        dest.set<Ordinal>(0xFFFF'FFFF);
    }
    /**
     * Find the most significant clear bit
     */
    void Core::spanbit(Core::SourceRegister src, Core::DestinationRegister dest) noexcept {
        auto k = src.get<Ordinal>();
        _ac._conditionCode = 0b000;
        for (int i = 31; i >= 0; --i) {
            if (mostSignificantBitClear(k)) {
                _ac._conditionCode = 0b010;
                dest.set<Ordinal>(i);
                return;
            }
            k <<= 1;
        }
        dest.set<Ordinal>(0xFFFF'FFFF);
    }
    void Core::modi(__DEFAULT_THREE_ARGS__) noexcept {
        auto s1 = src1.get<Integer>();
        auto s2 = src2.get<Integer>();
        NormalRegister internalRegister;
        divi(src1, src2, internalRegister);
        auto result = internalRegister.get<Integer>() * s1;
        dest.set<Integer>(s2 - result);
        if ((s2 * s1) < 0) {
            dest.set<Integer>(dest.get<Integer>() + s1);
        }
    }
    void Core::subc(__DEFAULT_THREE_ARGS__) noexcept {
		auto s1 = src1.get<Ordinal>() - 1u;
        auto s2 = src2.get<Ordinal>();
		auto carryBit = _ac._conditionCode & 0b010 != 0 ? 1u : 0u;
		// need to identify if overflow will occur
        auto result = s2 - s1;
        result += carryBit;
#warning "subc does not implement integer overflow detection, needs to be implemented"
		auto v = 0; // TODO fix this by identifying if integer subtraction would've produced an overflow and set v
		_ac._conditionCode = (carryBit << 1) | v;
        dest.set<Ordinal>(result);
	}
    void Core::ediv(Core::SourceRegister src1, Core::LongSourceRegister src2, Core::DestinationRegister remainder, Core::DestinationRegister quotient) noexcept {
        auto s2 = src2.get<LongOrdinal>();
        auto s1 = src1.get<Ordinal>();
        auto divOp = s2 / s1;
#warning "ediv does not check for divison by zero!"
        remainder.set<Ordinal>(s2 - divOp * s1);
        quotient.set<Ordinal>(divOp);
    }
    void Core::divi(__DEFAULT_THREE_ARGS__) noexcept {
#warning "divi does not check for division by zero!"
        dest.set<Integer>(src2.get<Integer>() / src1.get<Integer>());
    }

    void Core::ld(Core::SourceRegister src, Core::DestinationRegister dest) noexcept {
        // this is the base operation for load, src contains the fully computed value
        // so this will probably be an internal register in most cases.
#warning "ld not implemented!"
        dest.set<Ordinal>(load(src.get<Ordinal>()));
    }
    void Core::ldob(Core::SourceRegister src, Core::DestinationRegister dest) noexcept {
        dest.set<ByteOrdinal>(load(src.get<Ordinal>()));
    }
    void Core::ldos(Core::SourceRegister src, Core::DestinationRegister dest) noexcept {
        dest.set<ShortOrdinal>(load(src.get<Ordinal>()));
    }
    void Core::ldib(Core::SourceRegister src, Core::DestinationRegister dest) noexcept {
#warning "A special loadbyte instruction is probably necessary"
        dest.set<Integer>((ByteInteger)load(src.get<Ordinal>()));
    }
    void Core::ldis(Core::SourceRegister src, Core::DestinationRegister dest) noexcept {
#warning "A special loadshort instruction is probably necessary"
        dest.set<Integer>((ShortInteger)load(src.get<Ordinal>()));
    }

    void Core::ldl(Core::SourceRegister src, Core::LongDestinationRegister dest) noexcept {
        auto addr = src.get<Ordinal>();
        dest.set(load(addr), load(addr + 1));
    }
    void DoubleRegister::set(Ordinal lower, Ordinal upper) noexcept {
        _lower.set<Ordinal>(lower);
        _upper.set<Ordinal>(upper);
    }
    void Core::ldt(Core::SourceRegister src, TripleRegister& dest) noexcept {
        auto addr = src.get<Ordinal>();
        dest.set(load(addr), load(addr + 1), load(addr + 2));
    }
    void TripleRegister::set(Ordinal l, Ordinal m, Ordinal u) noexcept {
        _lower.set<Ordinal>(l);
        _mid.set<Ordinal>(m);
        _upper.set<Ordinal>(u);
    }
    void Core::ldq(Core::SourceRegister src, QuadRegister& dest) noexcept {
        auto addr = src.get<Ordinal>();
        dest.set(load(addr), load(addr + 1), load(addr + 2), load(addr + 3));
    }
    void QuadRegister::set(Ordinal l, Ordinal m, Ordinal u, Ordinal h) noexcept {
        _lower.set(l);
        _mid.set(m);
        _upper.set(u);
        _highest.set(h);
    }

    void Core::cmpi(Core::SourceRegister src1, Core::SourceRegister src2) noexcept { compare(src1.get<Integer>(), src2.get<Integer>()); }
    void Core::cmpo(Core::SourceRegister src1, Core::SourceRegister src2) noexcept { compare(src1.get<Ordinal>(), src2.get<Ordinal>()); }
    void Core::muli(Core::SourceRegister src1, Core::SourceRegister src2, Core::DestinationRegister dest) noexcept {
#warning "No faults raised!"
        dest.set(src2.get<Integer>() * src1.get<Integer>());
    }
    void Core::remi(Core::SourceRegister src1, Core::SourceRegister src2, Core::DestinationRegister dest) noexcept {
#warning "No divide by zero check"
        dest.set(src2.get<Integer>() % src1.get<Integer>());
    }
    void Core::stl(Core::LongSourceRegister src, Core::SourceRegister dest) noexcept {
        store(dest.get<Ordinal>(), src.getLowerHalf());
        store(dest.get<Ordinal>() + sizeof(Ordinal), src.getUpperHalf());
    }
    void Core::stt(const TripleRegister& src, Core::SourceRegister dest) noexcept {
        auto addr = dest.get<Ordinal>();
        store(addr, src.getLowerPart());
        store(addr + sizeof(Ordinal), src.getMiddlePart());
        store(addr + (2 * sizeof(Ordinal)), src.getUpperPart());
    }
    void Core::stq(const QuadRegister& src, Core::SourceRegister dest) noexcept {
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
    void Core::mark() noexcept {
#warning "mark unimplemented"
    }
    void Core::fmark() noexcept {
#warning "fmark unimplemented"
    }
    void Core::flushreg() noexcept {
        // this will nop currently as I'm saving all local registers to the 
        // stack when a call happens
    }
    void Core::subi(__DEFAULT_THREE_ARGS__) noexcept {
        dest.set<Integer>(src2.get<Integer>() - src1.get<Integer>());
    }
    void Core::modtc(__DEFAULT_THREE_ARGS__) noexcept {
#warning "unimplemented"
    }
    void Core::modpc(__DEFAULT_THREE_ARGS__) noexcept {
#warning "unimplemented"
    }
    void Core::modac(__DEFAULT_THREE_ARGS__) noexcept {
        auto tmp = _ac._value;
        auto src = src2.get<Ordinal>();
        auto mask = src1.get<Ordinal>();
        auto ac = _ac._value;
        _ac._value = (src & mask) | (ac & (~mask));
        dest.set<Ordinal>(tmp);
    }
    void Core::addc(__DEFAULT_THREE_ARGS__) noexcept {
#warning "unimplemented"
    }
    void Core::testno(Core::DestinationRegister dest) noexcept { testGeneric<TestTypes::Unordered>(dest); }
    void Core::testg(Core::DestinationRegister dest) noexcept { testGeneric<TestTypes::Greater>( dest); }
    void Core::teste(Core::DestinationRegister dest) noexcept { testGeneric<TestTypes::Equal>( dest); }
    void Core::testge(Core::DestinationRegister dest) noexcept { testGeneric<TestTypes::GreaterOrEqual>( dest); }
    void Core::testl(Core::DestinationRegister dest) noexcept { testGeneric<TestTypes::Less>( dest); }
    void Core::testne(Core::DestinationRegister dest) noexcept { testGeneric<TestTypes::NotEqual>( dest); }
    void Core::testle(Core::DestinationRegister dest) noexcept { testGeneric<TestTypes::LessOrEqual>( dest); }
    void Core::testo(Core::DestinationRegister dest) noexcept { testGeneric<TestTypes::Ordered>( dest); }
    void Core::ret() noexcept {
        auto pfp = getPFP();

        switch(pfp._returnCode) {
            case 0b000:
                // TODO
                break;
            case 0b001:
                // TODO
                break;
            case 0b010:
                // TODO
                break;
            case 0b011:
                // TODO
                break;
            case 0b110:
                // TODO
                break;
            case 0b111:
                // TODO
                break;
            case 0b100:
            case 0b101:
            default:
                throw "Undefined code!";
                
        }
    }
    void Core::be(Integer addr) noexcept { branchIfGeneric<ConditionCode::Equal>(addr); }
    void Core::bne(Integer addr) noexcept { branchIfGeneric<ConditionCode::NotEqual>(addr); }
    void Core::bl(Integer addr) noexcept { branchIfGeneric<ConditionCode::LessThan>(addr); }
    void Core::ble(Integer addr) noexcept { branchIfGeneric<ConditionCode::LessThanOrEqual>(addr); }
    void Core::bg(Integer addr) noexcept { branchIfGeneric<ConditionCode::GreaterThan>(addr); }
    void Core::bge(Integer addr) noexcept { branchIfGeneric<ConditionCode::GreaterThanOrEqual>(addr); }
    void Core::bo(Integer addr) noexcept { branchIfGeneric<ConditionCode::Ordered>(addr); }
    void Core::bno(Integer addr) noexcept { branchIfGeneric<ConditionCode::Unordered>(addr); }
    void Core::faulte(Integer addr) noexcept {
#warning "unimplemented"
    }
    void Core::faultne(Integer addr) noexcept {
#warning "unimplemented"
    }
    void Core::faultl(Integer addr) noexcept {
#warning "unimplemented"
    }
    void Core::faultle(Integer addr) noexcept {
#warning "unimplemented"
    }
    void Core::faultg(Integer addr) noexcept {
#warning "unimplemented"
    }
    void Core::faultge(Integer addr) noexcept {
#warning "unimplemented"
    }
    void Core::faulto(Integer addr) noexcept {
#warning "unimplemented"
    }
    void Core::faultno(Integer addr) noexcept {
#warning "unimplemented"
    }
    void Core::bbc(__TWO_SOURCE_AND_INT_ARGS__) noexcept {
#warning "unimplemented"
    }
    void Core::bbs(__TWO_SOURCE_AND_INT_ARGS__) noexcept {
#warning "unimplemented"
    }
    void Core::cmpobg(__TWO_SOURCE_AND_INT_ARGS__) noexcept {
#warning "unimplemented"
    }
    void Core::cmpobe(__TWO_SOURCE_AND_INT_ARGS__) noexcept {
#warning "unimplemented"
    }
    void Core::cmpobge(__TWO_SOURCE_AND_INT_ARGS__) noexcept {
#warning "unimplemented"
    }
    void Core::cmpobl(__TWO_SOURCE_AND_INT_ARGS__) noexcept {
#warning "unimplemented"
    }
    void Core::cmpobne(__TWO_SOURCE_AND_INT_ARGS__) noexcept {
#warning "unimplemented"
    }
    void Core::cmpoble(__TWO_SOURCE_AND_INT_ARGS__) noexcept {
#warning "unimplemented"
    }
    void Core::cmpibg(__TWO_SOURCE_AND_INT_ARGS__) noexcept {
#warning "unimplemented"
    }
    void Core::cmpibe(__TWO_SOURCE_AND_INT_ARGS__) noexcept {
#warning "unimplemented"
    }
    void Core::cmpibge(__TWO_SOURCE_AND_INT_ARGS__) noexcept {
#warning "unimplemented"
    }
    void Core::cmpibl(__TWO_SOURCE_AND_INT_ARGS__) noexcept {
#warning "unimplemented"
    }
    void Core::cmpibne(__TWO_SOURCE_AND_INT_ARGS__) noexcept {
#warning "unimplemented"
    }
    void Core::cmpible(__TWO_SOURCE_AND_INT_ARGS__) noexcept {
#warning "unimplemented"
    }
    void Core::cmpibo(__TWO_SOURCE_AND_INT_ARGS__) noexcept {
#warning "unimplemented"
    }
    void Core::cmpibno(__TWO_SOURCE_AND_INT_ARGS__) noexcept {
#warning "unimplemented"
    }
    void Core::balx(__DEFAULT_TWO_ARGS__) noexcept {
#warning "unimplemented"
    }
    void Core::stob(__TWO_SOURCE_REGS__) noexcept {
#warning "unimplemented"
    }
    void Core::stos(__TWO_SOURCE_REGS__) noexcept {
#warning "unimplemented"
    }
    void Core::stib(__TWO_SOURCE_REGS__) noexcept {
#warning "unimplemented"
    }
    void Core::stis(__TWO_SOURCE_REGS__) noexcept {
#warning "unimplemented"
    }
    void Core::st(__TWO_SOURCE_REGS__) noexcept {
#warning "unimplemented"
    }
    void Core::notbit(__DEFAULT_THREE_ARGS__) noexcept {
        dest.set<Ordinal>(i960::notBit(src2.get<Ordinal>(), src1.get<Ordinal>()));
    }
    void Core::notand(__DEFAULT_THREE_ARGS__) noexcept {
        dest.set<Ordinal>(i960::notAnd(src2.get<Ordinal>(), src1.get<Ordinal>()));
    }
    void Core::notor(__DEFAULT_THREE_ARGS__) noexcept {
        dest.set<Ordinal>(i960::notOr(src2.get<Ordinal>(), src1.get<Ordinal>()));
    }
    void Core::orOp(__DEFAULT_THREE_ARGS__) noexcept {
        dest.set(i960::orOp<Ordinal>(src2.get<Ordinal>(), src1.get<Ordinal>()));
    }
    void Core::nor(__DEFAULT_THREE_ARGS__) noexcept {
        dest.set(i960::nor(src2.get<Ordinal>(), src1.get<Ordinal>()));
    }
    void Core::notOp(__DEFAULT_TWO_ARGS__) noexcept {
        dest.set(i960::notOp(src.get<Ordinal>()));
    }
    void Core::ornot(__DEFAULT_THREE_ARGS__) noexcept {
        dest.set(i960::orNot(src2.get<Ordinal>(), src1.get<Ordinal>()));
    }
    void Core::nand(__DEFAULT_THREE_ARGS__) noexcept {
        dest.set(i960::nand(src2.get<Ordinal>(), src1.get<Ordinal>()));
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
    void Core::shrdi(Core::SourceRegister len, Core::SourceRegister src, Core::DestinationRegister dest) noexcept {
        auto result = src.get<Integer>() >> len.get<Integer>();
        if (result < 0) {
            result += 1;
        }
        dest.set<Integer>(result);
    }
    void Core::rotate(__DEFAULT_THREE_ARGS__) noexcept {
        dest.set<Ordinal>(i960::rotate(src2.get<Ordinal>(), src1.get<Ordinal>()));
    }
    void Core::modify(Core::SourceRegister mask, Core::SourceRegister src, Core::DestinationRegister srcDest) noexcept {
        auto s = src.get<Ordinal>();
        auto m = mask.get<Ordinal>();
        srcDest.set<Ordinal>((s & m) | (srcDest.get<Ordinal>() & (~m)));
    }
    void Core::scanbyte(__TWO_SOURCE_REGS__) noexcept {
#warning "unimplemented"
    }
    void Core::atmod(__DEFAULT_THREE_ARGS__) noexcept {
#warning "unimplemented"
    }
    void Core::atadd(__DEFAULT_THREE_ARGS__) noexcept {
#warning "unimplemented"
    }
    void Core::concmpo(__TWO_SOURCE_REGS__) noexcept {
#warning "unimplemented"
    }
    void Core::concmpi(__TWO_SOURCE_REGS__) noexcept {
#warning "unimplemented"
    }
    void Core::cmpinco(__DEFAULT_THREE_ARGS__) noexcept {
#warning "no overflow detection"
        cmpo(src1, src2);
        dest.set<Ordinal>(src2.get<Ordinal>() + 1);
    }
    void Core::cmpinci(__DEFAULT_THREE_ARGS__) noexcept {
        cmpi(src1, src2);
        dest.set<Integer>(src2.get<Integer>() + 1); // overflow suppressed
    }
    void Core::cmpdeco(__DEFAULT_THREE_ARGS__) noexcept {
#warning "no overflow detection"
        cmpo(src1, src2);
        dest.set<Ordinal>(src2.get<Ordinal>() - 1);
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
#undef __DEFAULT_TWO_ARGS__
#undef __DEFAULT_DOUBLE_WIDE_TWO_ARGS__
#undef __DEFAULT_THREE_ARGS__
#undef __DEFAULT_DOUBLE_WIDE_THREE_ARGS__
#undef __TWO_SOURCE_REGS__
} // end namespace i960
