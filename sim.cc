#include <iostream>
#include <cstddef>
#include <memory>
#include "types.h"
#include "operations.h"
#include "opcodes.h"
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

void outputTypeInformation() {
	std::cout << "Printing out system type sizes for i960 data types:" << std::endl;
	std::cout << "sizeof(NormalRegister): " << sizeof(i960::NormalRegister) << std::endl;
	std::cout << "sizeof(ArithmeticControls): " << sizeof(i960::ArithmeticControls) << std::endl;
	std::cout << "sizeof(Instruction): " << sizeof(i960::Instruction) << std::endl;
}

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
bool testInstructionEncoding(const i960::Instruction& test, const std::string& compareAgainst) noexcept {
	std::cout << "test encoding: " << std::hex << test._raw << std::endl;
	std::cout << "\tbase opcode: 0x" << std::hex << test.getOpcode() << std::endl;
	std::cout << "\tencoded format for mema: " << std::hex << test._mem._mema._offset << std::endl;
	if (auto description = i960::Opcode::getDescription(test.getOpcode()); description.isUndefined()) {
		std::cout << "\tInstruction mnemonic: 'undefined'" << std::endl;
		return false;
	} else {
		std::string tmp(description.getString());
		std::cout << "\tInstruction mnemonic: '" << tmp << "'" << std::endl;
		return tmp == compareAgainst;
	}
}
void testInstructionEncoding() {
	using Instruction = i960::Instruction;
	auto fn = [](i960::Ordinal opcode, const std::string& compareAgainst) {
		Instruction tmp(0);
		if (opcode > 0xFF) {
			auto upperOpcode = (0xFF0 & opcode) >> 4;
			auto lowerOpcode = (0x00F & opcode) ;
			tmp._reg._opcode = upperOpcode;
			tmp._reg._opcode2 = lowerOpcode;
		} else {
			// non register kind
			tmp._raw = opcode << 24;
		}
		return testInstructionEncoding(tmp, compareAgainst);
	};
#define X(name, code) \
	std::cout << "Decoding: " << #name << std::endl; \
	if (fn(code, #name)) { \
		std::cout << "Outcome: successful!" << std::endl; \
	} else { \
		std::cout << "Outcome: failed!" << std::endl; \
		return; \
	}
#define reg(name, code, __) X(name, code);
#define mem(name, code, __) X(name, code)
#define cobr(name, code, __) X(name, code)
#define ctrl(name, code, __) X(name, code)
#include "opcodes.def"
#undef X
#undef reg
#undef mem
#undef cobr
#undef ctrl
}
int main() {
	int errorCode = 0;
	bootupMessage(std::cout);
	outputTypeInformation();
    testOverflowOfDisplacement();
	testInstructionEncoding();
	std::cout << "Shutting down..." << std::endl;

	return errorCode;
}
