#include <iostream>
#include <cstddef>
#include <memory>
#include "types.h"
#include "operations.h"

bool testResult(i960::RawExtendedReal value) {
    union donuts {
        donuts() { };
        i960::RawExtendedReal _v;
        i960::ExtendedReal _k;
        __int128 _z;
    };
    union donuts2 {
        i960::RawExtendedReal _v;
        __int128 _k;
    } test2;
    donuts test, test3;
    test._v = value;
    test2._v = value;
    test3._z = 0;
    test3._z = __int128(test._k._upper) << 64;
    test3._z &= ~(__int128(0xFFFF) << 96);
    test3._z = test3._z | __int128(test._k._lower);
    test3._z &= ~(__int128(0xFFFF) << 96);
    std::cout << std::hex << test._k._upper << test._k._lower << std::endl;
    std::cout << std::hex << uint64_t(test2._k >> 64) << uint64_t(test2._k) << std::endl;
    std::cout << std::hex << test3._v << std::endl;
    if (test3._k._sign == 1) {
        std::cout << "negative" << std::endl;
    } else {
        std::cout << "positive" << std::endl;
    }
    return value == test3._v;
}
constexpr auto mem1G = 0x3FFF'FFFF + 1;
void bootupMessage(std::ostream& os) {
	os << "Intel 80960 Simulator" << std::endl;
	os << "Supported instruction sets:";
	if (i960::coreArchitectureSupported) {
		os << " core";
	}
	if (i960::numericsArchitectureSupported) {
		os << " numerics";
	}
	if (i960::protectedArchitectureSupported) {
		os << " protected";
	}
	if (i960::extendedArchitectureSupported) {
		os << " extended";
	}
	os << std::endl;
	std::cout << std::endl; 
	std::cout << std::endl; 
}
int main() {
	bootupMessage(std::cout);
	std::cout << "Allocating Test Memory And Randomizing" << std::endl;
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
	std::cout << "Memory Allocation and randomization complete" << std::endl << std::endl;
	std::cout << "Printing out system type sizes for i960 data types:" << std::endl;
	std::cout << "sizeof(NormalRegister): " << sizeof(i960::NormalRegister) << std::endl;
	std::cout << "sizeof(ArithmeticControls): " << sizeof(i960::ArithmeticControls) << std::endl;
	std::cout << "sizeof(Instruction): " << sizeof(i960::Instruction) << std::endl;
	std::cout << "sizeof(ExtendedReal): " << sizeof(i960::ExtendedReal) << std::endl;
	std::cout << "sizeof(LongReal): " << sizeof(i960::LongReal) << std::endl;
	std::cout << "sizeof(Real): " << sizeof(i960::Real) << std::endl;
	std::cout << "sizeof(TripleWord): " << sizeof(i960::TripleWord) << std::endl;
	std::cout << "sizeof(QuadWord): " << sizeof(i960::QuadWord) << std::endl;
    std::cout << "sizeof(RawExtendedReal): " << sizeof(i960::RawExtendedReal) << std::endl;
	std::cout << "Performing Tests Relating to the extended real floating point units" << std::endl;
    // test to make sure that we are doing the right thing :D
    // It seems that the 80-bit format is maintained correctly :D
    if (testResult(1.23456 + 0)) {
        std::cout << "It is 80-bits wide :D" << std::endl;
    }
    if (testResult(-0.5)) {
        std::cout << "It is 80-bits wide :D" << std::endl;
    }

	std::cout << "Shutting down..." << std::endl;
	return 0;
}
