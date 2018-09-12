#include "types.h"
#include "operations.h"
#define __DEFAULT_THREE_ARGS__ Core::SourceRegister src1, Core::SourceRegister src2, Core::DestinationRegister dest
#define __DEFAULT_DOUBLE_WIDE_THREE_ARGS__ Core::SourceRegister src1Lower, Core::SourceRegister src1Upper, Core::SourceRegister src2Lower, Core::SourceRegister src2Upper, Core::DestinationRegister destLower, Core::DestinationRegister destUpper
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
   void Core::move(const NormalRegister& src, NormalRegister& dest) noexcept {
       dest._ordinal = src._ordinal;
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
       LongReal src1(src1Lower._ordinal, src1Upper._ordinal);
       LongReal src2(src2Lower._ordinal, src2Lower._ordinal);
       LongReal dest(src1._floating + src2._floating);
       destLower._ordinal = dest.lowerHalf();
       destUpper._ordinal = dest.upperHalf();
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
       dest._ordinal = i960::andNot<Ordinal>(src2._ordinal, src1._ordinal);
   }

#undef __DEFAULT_THREE_ARGS__
#undef __DEFAULT_DOUBLE_WIDE_THREE_ARGS__
} // end namespace i960
