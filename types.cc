#include "types.h"
#include <iostream>
#include "ArithmeticControls.h"
#include "DoubleRegister.h"
#include "TripleRegister.h"
#include "QuadRegister.h"

namespace i960 {



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



void DoubleRegister::set(Ordinal lower, Ordinal upper) noexcept {
    _lower.set<Ordinal>(lower);
    _upper.set<Ordinal>(upper);
}




ArithmeticControls::~ArithmeticControls() {
    value = 0;
}

} // end namespace i960
