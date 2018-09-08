#include <iostream>
#include <cstddef>
#define NUMERICS_ARCHITECTURE
#include "types.h"
#include "coreops.h"
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
int main() {
	std::cout << "sizeof(ExtendedReal): " << sizeof(i960::ExtendedReal) << std::endl;
	std::cout << "sizeof(LongReal): " << sizeof(i960::LongReal) << std::endl;
	std::cout << "sizeof(Real): " << sizeof(i960::Real) << std::endl;
	std::cout << "sizeof(TripleWord): " << sizeof(i960::TripleWord) << std::endl;
	std::cout << "sizeof(QuadWord): " << sizeof(i960::QuadWord) << std::endl;
	std::cout << "sizeof(NormalRegister): " << sizeof(i960::NormalRegister) << std::endl;
	std::cout << "sizeof(ArithmeticControls): " << sizeof(i960::ArithmeticControls) << std::endl;
	std::cout << "sizeof(Instruction): " << sizeof(i960::Instruction) << std::endl;
    std::cout << "sizeof(RawExtendedReal): " << sizeof(i960::RawExtendedReal) << std::endl;
    // test to make sure that we are doing the right thing :D
    // It seems that the 80-bit format is maintained correctly :D
    if (testResult(1.23456 + 0)) {
        std::cout << "It is 80-bits wide :D" << std::endl;
    }
    if (testResult(-0.5)) {
        std::cout << "It is 80-bits wide :D" << std::endl;
    }
	return 0;
}
