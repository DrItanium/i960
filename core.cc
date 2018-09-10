#include "types.h"

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
   void Core::call(Ordinal newAddress) {
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
   void Core::callx(Ordinal newAddress) noexcept {
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
   void Core::calls(ByteOrdinal callNum) {
       if (callNum > 259) {
#warning "Raise protection length fault here"
           return;
       }
#warning "implement the rest of calls (call supervisor)"
   }

   void Core::setRegister(ByteOrdinal index, Ordinal value) noexcept {
       if (auto offset = (index & 0b01111) ; (index & 0b10000) == 0) {
            _localRegisters[offset]._ordinal = value;
       } else {
            _globalRegisters[offset]._ordinal = value;
       }
   }
   void Core::setRegister(ByteOrdinal index, Integer value) noexcept {
       if (auto offset = (index & 0b01111) ; (index & 0b10000) == 0) {
            _localRegisters[offset]._integer = value;
       } else {
            _globalRegisters[offset]._integer = value;
       }
   }
   void Core::setRegister(ByteOrdinal index, Real value) noexcept {
       if (auto offset = (index & 0b01111) ; (index & 0b10000) == 0) {
            _localRegisters[offset]._real = value;
       } else {
            _globalRegisters[offset]._real = value;
       }
   }
   Ordinal Core::load(Ordinal address) {
#warning "Core::load unimplemented!"
        return -1;
   }
   void Core::store(Ordinal address, Ordinal value) {
#warning "Core::store unimplemented!"
   }


} // end namespace i960
