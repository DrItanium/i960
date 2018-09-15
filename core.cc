#include "types.h"
#include "operations.h"
#include "opcodes.h"
#include <limits>
#include <cmath>
#define __DEFAULT_THREE_ARGS__ Core::SourceRegister src1, Core::SourceRegister src2, Core::DestinationRegister dest
#define __DEFAULT_DOUBLE_WIDE_THREE_ARGS__ const DoubleRegister& src1, const DoubleRegister& src2, DoubleRegister& dest
#define __DEFAULT_TWO_ARGS__ Core::SourceRegister src, Core::DestinationRegister dest
#define __DEFAULT_DOUBLE_WIDE_TWO_ARGS__ const DoubleRegister& src, DoubleRegister& dest
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
   void Core::call(Integer addr) {
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

   Ordinal Core::load(Ordinal address) {
#warning "Core::load unimplemented!"
        return -1;
   }
   void Core::store(Ordinal address, Ordinal value) {
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

    void Core::addr(__DEFAULT_THREE_ARGS__) noexcept {
        dest.set(src2.get<RawReal>() + src1.get<RawReal>());
    }
    void Core::addrl(__DEFAULT_DOUBLE_WIDE_THREE_ARGS__) noexcept {
        dest.set(src2.get<RawLongReal>() + src1.get<RawLongReal>());
    }
    void Core::subr(__DEFAULT_THREE_ARGS__) noexcept {
        dest.set(src2.get<RawReal>() - src1.get<RawReal>());
    }
    void Core::subrl(__DEFAULT_DOUBLE_WIDE_THREE_ARGS__) noexcept {
        dest.set(src2.get<RawLongReal>() - src1.get<RawLongReal>());
    }
    void Core::mulr(__DEFAULT_THREE_ARGS__) noexcept {
        dest.set(src2.get<RawReal>() * src1.get<RawReal>());
    }
    void Core::mulrl(__DEFAULT_DOUBLE_WIDE_THREE_ARGS__) noexcept {
        dest.set(src2.get<RawLongReal>() * src1.get<RawLongReal>());
    }
    void Core::xnor(__DEFAULT_THREE_ARGS__) noexcept {
        dest.set<Ordinal>(i960::xnor<Ordinal>(src1.get<Ordinal>(), src2.get<Ordinal>()));
    }
    void Core::xorOp(__DEFAULT_THREE_ARGS__) noexcept {
        dest.set<Ordinal>(i960::xorOp<Ordinal>(src1.get<Ordinal>(), src2.get<Ordinal>()));
    }
    void Core::tanr(__DEFAULT_TWO_ARGS__) noexcept {
        dest.set<RawReal>(::tan(src.get<RawReal>()));
    }
    void Core::tanrl(__DEFAULT_DOUBLE_WIDE_TWO_ARGS__) noexcept {
        dest.set<RawLongReal>(::tan(src.get<RawLongReal>()));
    }
    void Core::cosr(__DEFAULT_TWO_ARGS__) noexcept {
        dest.set<RawReal>(::cos(src.get<RawReal>()));
    }
    void Core::cosrl(__DEFAULT_DOUBLE_WIDE_TWO_ARGS__) noexcept {
        dest.set<RawLongReal>(::cos(src.get<RawLongReal>()));
    }
    void Core::sinr(__DEFAULT_TWO_ARGS__) noexcept {
        dest.set<RawReal>(::sin(src.get<RawReal>()));
    }
    void Core::sinrl(__DEFAULT_DOUBLE_WIDE_TWO_ARGS__) noexcept {
        dest.set<RawLongReal>(::sin(src.get<RawLongReal>()));
    }
    void Core::atanr(__DEFAULT_THREE_ARGS__) noexcept {
        dest.set<RawReal>(::atan(src2.get<RawReal>() / src1.get<RawReal>()));
    }
    void Core::atanrl(__DEFAULT_DOUBLE_WIDE_THREE_ARGS__) noexcept {
        dest.set<RawLongReal>(::atan(src2.get<RawLongReal>() / src1.get<RawLongReal>()));
    }
#undef __DEFAULT_TWO_ARGS__
#undef __DEFAULT_DOUBLE_WIDE_TWO_ARGS__
#undef __DEFAULT_THREE_ARGS__
#undef __DEFAULT_DOUBLE_WIDE_THREE_ARGS__
} // end namespace i960
