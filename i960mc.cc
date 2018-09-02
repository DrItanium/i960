#include <iostream>
#include "types.h"
#include "regs.h"
#include "iac.h"




int main() {
    std::cout << "sizeof(ExtendedReal): " << sizeof(i960::ExtendedReal) << std::endl;
    std::cout << "sizeof(LongReal): " << sizeof(i960::LongReal) << std::endl;
    std::cout << "sizeof(Real): " << sizeof(i960::Real) << std::endl;
    std::cout << "sizeof(TripleWord): " << sizeof(i960::TripleWord) << std::endl;
    std::cout << "sizeof(QuadWord): " << sizeof(i960::QuadWord) << std::endl;
    std::cout << "sizeof(NormalRegister): " << sizeof(i960::NormalRegister) << std::endl;
    std::cout << "sizeof(ArithmeticControls): " << sizeof(i960::ArithmeticControls) << std::endl;
    return 0;
}
