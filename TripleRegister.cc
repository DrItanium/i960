#include "types.h"
#include "TripleRegister.h"

namespace i960 {
void TripleRegister::set(Ordinal l, Ordinal m, Ordinal u) noexcept {
    _lower.set<Ordinal>(l);
    _mid.set<Ordinal>(m);
    _upper.set<Ordinal>(u);
}
}
