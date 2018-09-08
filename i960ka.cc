#include <iostream>
#include <cstddef>
#include <memory>
#include "types.h"
#include "operations.h"

constexpr auto mem1G = 0x3FFF'FFFF + 1;
int main() {
    // allocate 1 gb of space in each region max
    auto region0 = std::make_unique<i960::ByteOrdinal[]>(mem1G);
    auto region1 = std::make_unique<i960::ByteOrdinal[]>(mem1G);
    auto region2 = std::make_unique<i960::ByteOrdinal[]>(mem1G);
    auto region3 = std::make_unique<i960::ByteOrdinal[]>(mem1G);
    for(int i = 0; i < mem1G; ++i) {
        region0[i] = 0x12;
        region1[i] = 0x34;
        region2[i] = 0x56;
        region3[i] = 0x78;
    }
	std::cout << "sizeof(TripleWord): " << sizeof(i960::TripleWord) << std::endl;
	std::cout << "sizeof(QuadWord): " << sizeof(i960::QuadWord) << std::endl;
	std::cout << "sizeof(NormalRegister): " << sizeof(i960::NormalRegister) << std::endl;
	std::cout << "sizeof(ArithmeticControls): " << sizeof(i960::ArithmeticControls) << std::endl;
	std::cout << "sizeof(Instruction): " << sizeof(i960::Instruction) << std::endl;

    char donuts;
    std::cin >>  donuts;
	return 0;
}
