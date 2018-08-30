#include <iostream>
#include "types.h"

#include "regs.h"




int main() {
    std::cout << "sizeof(ExtendedReal): " << sizeof(ExtendedReal) << std::endl;
    std::cout << "sizeof(LongReal): " << sizeof(LongReal) << std::endl;
    std::cout << "sizeof(Real): " << sizeof(Real) << std::endl;
    std::cout << "sizeof(TripleWord): " << sizeof(TripleWord) << std::endl;
    std::cout << "sizeof(QuadWord): " << sizeof(QuadWord) << std::endl;
    std::cout << "sizeof(NormalRegister): " << sizeof(NormalRegister) << std::endl;
    std::cout << "sizeof(ArithmeticControls): " << sizeof(ArithmeticControls) << std::endl;
    return 0;
}
