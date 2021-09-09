#include "NormalRegister.h"

namespace i960 {

void NormalRegister::clear() noexcept {
    _value = 0;
}
void NormalRegister::move(const NormalRegister& other) noexcept {
    setValue(other.getValue());
}

void TraceControls::modify(Ordinal mask, Ordinal input) noexcept {
    auto fixedMask = 0x00FF00FF & mask; // masked to prevent reserved bits from being used
    setRawValue((fixedMask & input) | (getRawValue() & (~fixedMask)));
}

void DoubleRegister::set(Ordinal lower, Ordinal upper) noexcept {
    _lower.set<Ordinal>(lower);
    _upper.set<Ordinal>(upper);
}
} // end namespace i960
