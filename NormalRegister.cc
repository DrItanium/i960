#include "NormalRegister.h"

namespace i960 {

void TraceControls::clear() noexcept {
    value = 0;
}

void NormalRegister::move(const NormalRegister& other) noexcept {
    set<Ordinal>(other.get<Ordinal>());
}
NormalRegister::~NormalRegister() {
    ordinal = 0;
}
} // end namespace i960
