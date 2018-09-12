#include "types.h"
#include "operations.h"
#include "opcodes.h"
#include <limits>
#define __DEFAULT_THREE_ARGS__ Core::SourceRegister src1, Core::SourceRegister src2, Core::DestinationRegister dest
#define __DEFAULT_DOUBLE_WIDE_THREE_ARGS__ const DoubleRegister& src1, const DoubleRegister& src2, DoubleRegister& dest
namespace i960 {
   Ordinal Core::getStackPointerAddress() const noexcept {
       return _localRegisters[StackPointerIndex]._ordinal;
   }
   void Core::saveLocalRegisters() noexcept {
#warning "saveLocalRegisters unimplemented"
   }
   void Core::allocateNewLocalRegisterSet() noexcept {
#warning "allocateNewLocalRegisterSet unimplemented"
   }
   void Core::setFramePointer(Ordinal value) noexcept {
       _globalRegisters[FramePointerIndex]._ordinal = value;
   }
   Ordinal Core::getFramePointerAddress() const noexcept {
       return _globalRegisters[FramePointerIndex]._ordinal & (~0b111111);
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
       auto callNum = value._byteOrd;
       if (callNum > 259) {
#warning "Raise protection length fault here"
           return;
       }
#warning "implement the rest of calls (call supervisor)"
   }

   void Core::setRegister(ByteOrdinal index, Ordinal value) noexcept {
       getRegister(index)._ordinal = value;
   }
   void Core::setRegister(ByteOrdinal index, Integer value) noexcept {
       getRegister(index)._integer = value;
   }
   void Core::setRegister(ByteOrdinal index, Real value) noexcept {
       getRegister(index)._real = value;
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
       dest._ordinal = src2._ordinal + src1._ordinal; 
   }
   void Core::addi(__DEFAULT_THREE_ARGS__) noexcept {
#warning "addi does not check for integer overflow"
       dest._integer = src2._integer + src1._integer;
   }
   void Core::addr(__DEFAULT_THREE_ARGS__) noexcept {
#warning "addr does not implement any fault detection"

       dest._real._floating = (src2._real._floating + src1._real._floating);
   }
   void Core::addrl(__DEFAULT_DOUBLE_WIDE_THREE_ARGS__) noexcept {
#warning "addrl does not implement any fault detection"
       LongReal s1 = src1.get<LongReal>();
       LongReal s2 = src2.get<LongReal>();
       LongReal out(s1._floating + s2._floating);
       dest.set(out);
   }
   void Core::subo(__DEFAULT_THREE_ARGS__) noexcept {
       dest._ordinal = src2._ordinal - src1._ordinal; 
   }
   void Core::mulo(__DEFAULT_THREE_ARGS__) noexcept {
       dest._ordinal = src2._ordinal * src1._ordinal; 
   }
   void Core::divo(__DEFAULT_THREE_ARGS__) noexcept {
#warning "divo does not check for divison by zero"
       dest._ordinal = src2._ordinal / src1._ordinal;
   }
   void Core::remo(__DEFAULT_THREE_ARGS__) noexcept {
#warning "remo does not check for divison by zero"
       dest._ordinal = src2._ordinal % src1._ordinal;
       
   }
   void Core::chkbit(Core::SourceRegister pos, Core::SourceRegister src) noexcept {
        _ac._conditionCode = ((src._ordinal & (1 << (pos._ordinal & 0b11111))) == 0) ? 0b000 : 0b010;
   }
   void Core::alterbit(Core::SourceRegister pos, Core::SourceRegister src, Core::DestinationRegister dest) noexcept {
		if ((_ac._conditionCode & 0b010) == 0) {
			dest._ordinal = src._ordinal & (~(1 << (pos._ordinal & 0b11111)));
		} else {
			dest._ordinal = src._ordinal | (1 << (pos._ordinal & 0b11111));
		}
   }
   void Core::andOp(__DEFAULT_THREE_ARGS__) noexcept {
       dest._ordinal = i960::andOp<Ordinal>(src2._ordinal, src1._ordinal);
   }
   void Core::andnot(__DEFAULT_THREE_ARGS__) noexcept {
       dest._ordinal = i960::andNot(src2._ordinal, src1._ordinal);
   }

   void DoubleRegister::move(const DoubleRegister& other) noexcept {
       _lower._ordinal = other._lower._ordinal;
       _upper._ordinal = other._upper._ordinal;
   }
   void TripleRegister::move(const TripleRegister& other) noexcept {
       _lower._ordinal = other._lower._ordinal;
       _mid._ordinal = other._mid._ordinal;
       _upper._ordinal = other._upper._ordinal;

   }
   void QuadRegister::move(const QuadRegister& other) noexcept {
       _lower._ordinal = other._lower._ordinal;
       _mid._ordinal = other._mid._ordinal;
       _upper._ordinal = other._upper._ordinal;
       _highest._ordinal = other._highest._ordinal;
   }

   void Core::mov(Core::SourceRegister src, Core::DestinationRegister dest) noexcept { dest._ordinal = src._ordinal; }
   void Core::movl(Core::LongSourceRegister src, Core::LongDestinationRegister dest) noexcept { dest.move(src); }
   void Core::movt(const TripleRegister& src, TripleRegister& dest) noexcept { dest.move(src); }
   void Core::movq(const QuadRegister& src, QuadRegister& dest) noexcept { dest.move(src); }
   NormalRegister& Core::stashNewLiteral(ByteOrdinal pos, Ordinal value) noexcept {
        auto mask = pos & 0xF;
        _internalRegisters[mask]._ordinal = value;
        return _internalRegisters[mask];
   }
   NormalRegister& Core::stashNewLiteral(ByteOrdinal pos, RawReal value) noexcept {
       auto mask = pos & 0xF;
       _internalRegisters[mask]._real._floating = value;
       return _internalRegisters[mask];
   }
   DoubleRegister Core::stashNewLiteral(ByteOrdinal pos, LongOrdinal value) noexcept {
       auto mask = pos & 0b1110; // must be even
       DoubleRegister tmp(_internalRegisters[mask], _internalRegisters[mask+1]);
       tmp.set(value);
       return tmp;
   }
   DoubleRegister Core::stashNewLiteral(ByteOrdinal pos, RawLongReal value) noexcept {
       auto mask = pos & 0b1110; // must be even
       DoubleRegister tmp(_internalRegisters[mask], _internalRegisters[mask+1]);
       tmp.set(value);
       return tmp;
   }

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
#undef __DEFAULT_THREE_ARGS__
#undef __DEFAULT_DOUBLE_WIDE_THREE_ARGS__
} // end namespace i960
