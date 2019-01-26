#include "types.h"
#include "DoubleRegister.h"

namespace i960 {
void DoubleRegister::set(Ordinal lower, Ordinal upper) noexcept {
    _lower.set<Ordinal>(lower);
    _upper.set<Ordinal>(upper);
}
}
