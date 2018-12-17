#include <iostream>
#include <cstddef>
#include <memory>
#include "types.h"
#include "operations.h"
#include <string>

constexpr auto mem1G = 0x3FFF'FFFF + 1;
void bootupMessage(std::ostream& os) {
	os << "Intel 80960JX Simulator" << std::endl;
	std::cout << std::endl; 
	std::cout << std::endl; 
}
template<typename T>
bool testInstructionResult(T expected, T value, const std::string& instruction) {
	if (expected != value) {
		std::cout << "Testing instruction: " << instruction << " expected: " << std::dec << expected;
		std::cout << " and got " << std::dec << value << " result: FAIL" << std::endl;
		return false;
	} 
	return true;
}
/*
template<> 
bool testInstructionResult<i960::Real>(i960::Real expected, i960::Real value, const std::string& instruction) {
	if (expected._floating != value._floating) {
		std::cout << "Testing instruction: " << instruction << " expected: " << std::dec << expected._floating;
		std::cout << " and got " << std::dec << value._floating << " result: FAIL" << std::endl;
		return false;
	} 
	return true;
}

template<> 
bool testInstructionResult<i960::LongReal>(i960::LongReal expected, i960::LongReal value, const std::string& instruction) {
	if (expected._floating != value._floating) {
		std::cout << "Testing instruction: " << instruction << " expected: " << std::dec << expected._floating;
		std::cout << " and got " << std::dec << value._floating << " result: FAIL" << std::endl;
		return false;
	} 
	return true;
}
bool testArithmeticOperationsOrdinal(i960::Ordinal a, i960::Ordinal b) noexcept {
	i960::ArithmeticControls ac;
	return testInstructionResult<i960::Ordinal>(a + b, i960::add(a, b), "addo") && 
	testInstructionResult<i960::Ordinal>(a - b, i960::subtract(a, b), "subo") &&
	testInstructionResult<i960::Ordinal>(a * b, i960::multiply(a, b), "mulo") &&
	testInstructionResult<i960::Ordinal>(a / b, i960::divide(ac, a, b), "divo") && 
	testInstructionResult<i960::Ordinal>(a % b, i960::remainder(ac, a, b), "remo");
}
bool testArithmeticOperationsInteger(i960::Integer a, i960::Integer b) noexcept {
	i960::ArithmeticControls ac;
	return testInstructionResult<i960::Integer>(a + b, i960::add(a, b), "addi") &&
	testInstructionResult<i960::Integer>(a - b, i960::subtract(a, b), "subi") &&
	testInstructionResult<i960::Integer>(a * b, i960::multiply(a, b), "muli") &&
	testInstructionResult<i960::Integer>(a / b, i960::divide(ac, a, b), "divi") &&
	testInstructionResult<i960::Integer>(a % b, i960::remainder(ac, a, b), "remi");
}

bool testArithmeticOperationsReal(i960::RawReal a, i960::RawReal b) noexcept {
	i960::ArithmeticControls ac;
	i960::Real c(a);
	i960::Real d(b);
	return testInstructionResult<i960::Real>(i960::Real(a + b), i960::add(c, d), "addr") &&
	testInstructionResult<i960::Real>(i960::Real(a - b), i960::subtract(c, d), "subr") &&
	testInstructionResult<i960::Real>(i960::Real(a * b), i960::multiply(c, d), "mulr") &&
	testInstructionResult<i960::Real>(i960::Real(a / b), i960::divide(c, d), "divr");
}
bool testArithmeticOperationsLongReal(i960::RawLongReal a, i960::RawLongReal b) noexcept {
	i960::ArithmeticControls ac;
	i960::LongReal c(a);
	i960::LongReal d(b);
	return testInstructionResult<i960::LongReal>(i960::LongReal(a + b), i960::add(c, d), "addrl") &&
	testInstructionResult<i960::LongReal>(i960::LongReal(a - b), i960::subtract(c, d), "subrl") &&
	testInstructionResult<i960::LongReal>(i960::LongReal(a * b), i960::multiply(c, d), "mulrl") &&
	testInstructionResult<i960::LongReal>(i960::LongReal(a / b), i960::divide(c, d), "divrl");
}
bool testLogicalOperations(i960::Ordinal a, i960::Ordinal b) noexcept {
	i960::ArithmeticControls ac;
	return testInstructionResult<i960::Ordinal>(a & b, i960::andOp(a, b), "and") &&
	testInstructionResult<i960::Ordinal>((~a) & b, i960::notAnd(a, b), "notand") &&
	testInstructionResult<i960::Ordinal>((a) & (~b), i960::andNot(a, b), "andnot") &&
	testInstructionResult<i960::Ordinal>(a | b, i960::orOp(a, b), "or") &&
	testInstructionResult<i960::Ordinal>((~a) | b, i960::notOr(a, b), "notor") &&
	testInstructionResult<i960::Ordinal>((a) | (~b), i960::orNot(a, b), "ornot") &&
	testInstructionResult<i960::Ordinal>((a | b) & (~(a & b)), i960::xorOp(a, b), "xor") &&
	testInstructionResult<i960::Ordinal>((~(a | b)) | ((a & b)), i960::xnor(a, b), "xnor") &&
	testInstructionResult<i960::Ordinal>((~a) | (~b), i960::nand(a, b), "nand") &&
	testInstructionResult<i960::Ordinal>((~a) & (~b), i960::nor(a, b), "nor");
}
bool testShiftOperationsOrdinal(i960::Ordinal a, i960::Ordinal b) { 
	return testInstructionResult<i960::Ordinal>(a << b, i960::shiftLeft(a, b), "shlo") &&
		testInstructionResult<i960::Ordinal>(a >> b, i960::shiftRight(a, b), "shro");
}
bool testShiftOperationsInteger(i960::Integer a, i960::Integer b) {
	return testInstructionResult<i960::Integer>(a << b, i960::shiftLeft(a, b), "shli") &&
		testInstructionResult<i960::Integer>(a >> b, i960::shiftRight(a, b), "shri");
}

bool performTests() {
	return testArithmeticOperationsOrdinal(1u, 1u) &&
	testArithmeticOperationsOrdinal(10u, 100u) &&
	testArithmeticOperationsOrdinal(10u, 500u) &&
	testArithmeticOperationsInteger(-1, -1) &&
	testArithmeticOperationsInteger(-10, -50) &&
	testLogicalOperations(0xFDED, 0x1000) &&
	testLogicalOperations(0x1234FDED, 0x56789ABC) &&
	testShiftOperationsInteger(-127, 1) &&
	testShiftOperationsOrdinal(127, 1) &&
	testInstructionResult<i960::Ordinal>((0xfdedu << 5u) | (0xFDEDu >> (32 - 5)), i960::rotate(0xfded, 5), "rotate") &&
	testArithmeticOperationsReal(1.0f, 2.0f) &&
	testArithmeticOperationsReal(10.0f, 30.0f) &&
	testArithmeticOperationsLongReal(1.0, 2.0) &&
	testArithmeticOperationsLongReal(10.0, 30.0);
}
*/

void outputTypeInformation() {
	std::cout << "Printing out system type sizes for i960 data types:" << std::endl;
	std::cout << "sizeof(NormalRegister): " << sizeof(i960::NormalRegister) << std::endl;
	std::cout << "sizeof(ArithmeticControls): " << sizeof(i960::ArithmeticControls) << std::endl;
	std::cout << "sizeof(Instruction): " << sizeof(i960::Instruction) << std::endl;
}
void testMemoryAllocation() {
	std::cout << "Allocating Test Memory And Randomizing" << std::endl;
    // allocate 1 gb of space in each region max
    auto region0 = std::make_unique<i960::ByteOrdinal[]>(mem1G);
    auto region1 = std::make_unique<i960::ByteOrdinal[]>(mem1G);
    auto region2 = std::make_unique<i960::ByteOrdinal[]>(mem1G);
    auto region3 = std::make_unique<i960::ByteOrdinal[]>(mem1G);
	for (int i = 0; i < mem1G; ++i) {
		region0[i] = 0x12;
		region1[i] = 0x34;
		region2[i] = 0x56;
		region3[i] = 0x78;
	}
	std::cout << "Byte Writing Complete" << std::endl;
	std::cout << "Starting Long Ordinal Writes" << std::endl;
	i960::LongOrdinal* r0 = (i960::LongOrdinal*)region0.get();
	i960::LongOrdinal* r1 = (i960::LongOrdinal*)region1.get();
	i960::LongOrdinal* r2 = (i960::LongOrdinal*)region2.get();
	i960::LongOrdinal* r3 = (i960::LongOrdinal*)region3.get();
    for(int i = 0; i < (mem1G / sizeof(i960::LongOrdinal)); ++i) {
		r0[i] = 0x123456789ABCDEF0;
		r1[i] = 0x0123456789ABCDEF;
		r2[i] = 0xF0123456789ABCDE;
		r3[i] = 0xEF0123456789ABCD;
    }
	std::cout << "Memory Allocation and randomization complete" << std::endl << std::endl;
}


/*
int performInstructionTests() {
	int errorCode = 0;
	std::cout << "Performing instruction tests" << std::endl;
	if(!performTests()) {
		std::cout << "Tests failed!" << std::endl;
		errorCode = 1;
	} else {
		std::cout << "Tests passed!" << std::endl;
	}
	return errorCode;

}
*/
void testOverflowOfDisplacement() noexcept {
    union {
        i960::Integer _displacement : 24;
        i960::Integer _value;
    } converter;
    converter._value = 0;
    converter._displacement = -8388608;

    std::cout << "value: " << std::dec << converter._value << ", displacement: " << std::dec << converter._displacement << std::endl;
	i960::Ordinal _base = 0xFF00'0000;
	std::cout << "Base value: " << _base << std::endl;
	std::cout << "Combined value: " << (_base + converter._displacement) << std::endl;
	std::cout << "Combined value is less: " << std::boolalpha << ((_base + converter._displacement) < _base) << std::endl;
}
void testInstructionEncoding() {
	using Instruction = i960::Instruction;
	Instruction test(127);
	std::cout << "test encoding: " << std::hex << test._raw << std::endl;
	std::cout << "\tencoded format for mema: " << std::hex << test._mem._mema._offset << std::endl;
}
int main() {
	int errorCode = 0;
	bootupMessage(std::cout);
	testMemoryAllocation();
	outputTypeInformation();
	//errorCode = performInstructionTests();
    testOverflowOfDisplacement();
	testInstructionEncoding();
	std::cout << "Shutting down..." << std::endl;

	return errorCode;
}
