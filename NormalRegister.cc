#include "NormalRegister.h"

namespace i960 {

void NormalRegister::clear() noexcept {
    _value = 0;
}
void NormalRegister::move(const NormalRegister& other) noexcept {
    setValue(other.getValue());
}
} // end namespace i960
