#include "types.h"
#include "ArithmeticControls.h"

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







ArithmeticControls::~ArithmeticControls() {
    value = 0;
}

} // end namespace i960
