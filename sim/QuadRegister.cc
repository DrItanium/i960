#include "types.h"
#include "QuadRegister.h"

namespace i960 {
void QuadRegister::set(Ordinal l, Ordinal m, Ordinal u, Ordinal h) noexcept {
    _lower.set(l);
    _mid.set(m);
    _upper.set(u);
    _highest.set(h);
}
}
