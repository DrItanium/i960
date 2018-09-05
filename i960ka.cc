#include <iostream>
#include <cstddef>
#include "types.h"
#include "coreops.h"

int main() {
	std::cout << "sizeof(TripleWord): " << sizeof(i960::TripleWord) << std::endl;
	std::cout << "sizeof(QuadWord): " << sizeof(i960::QuadWord) << std::endl;
	std::cout << "sizeof(NormalRegister): " << sizeof(i960::NormalRegister) << std::endl;
	std::cout << "sizeof(ArithmeticControls): " << sizeof(i960::ArithmeticControls) << std::endl;
	std::cout << "sizeof(Instruction): " << sizeof(i960::Instruction) << std::endl;
	return 0;
}
