#include "types.h"

namespace i960 {

void ProcessControls::clear() noexcept {
    value = 0;
}
void ProcessControls::enterSupervisorMode() noexcept {
    executionMode = 1;
}
void ProcessControls::enterUserMode() noexcept {
    executionMode = 0;
}

void TraceControls::clear() noexcept {
    value = 0;
}

void NormalRegister::move(const NormalRegister& other) noexcept {
    set<Ordinal>(other.get<Ordinal>());
}

void ArithmeticControls::clear() noexcept {
    value = 0;
}


Ordinal ArithmeticControls::modify(Ordinal mask, Ordinal value) noexcept {
    if (mask == 0) {
        return value;
    } else {
        auto tmp = value;
        value = (value & mask) | (value & ~(mask));
        return tmp;
    }
}

NormalRegister::~NormalRegister() {
    ordinal = 0;
}


} // end namespace i960
