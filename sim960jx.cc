#include <iostream>
#include <cstddef>
#include <memory>
#include "types.h"
#include "NormalRegister.h"
#include "ArithmeticControls.h"
#include "Operand.h"
#include "opcodes.h"
#include <string>

void bootupMessage(std::ostream& os) {
	os << "Intel 80960JX Simulator" << std::endl;
	os << std::endl; 
	os << std::endl; 
}

void outputTypeInformation(std::ostream& os) {
	os << "Printing out system type sizes for i960 data types:" << std::endl;
	os << "sizeof(NormalRegister): " << sizeof(i960::NormalRegister) << std::endl;
	os << "sizeof(ArithmeticControls): " << sizeof(i960::ArithmeticControls) << std::endl;
	os << "sizeof(Instruction): " << sizeof(i960::Instruction) << std::endl;
}

void testOverflowOfDisplacement(std::ostream& os) noexcept {
	constexpr i960::Ordinal _base = 0xFF00'0000;
    union {
        i960::Integer _displacement : 24;
        i960::Integer _value;
    } converter;
    converter._value = 0;
    converter._displacement = -8388608;

    os << "value: " << std::dec << converter._value << ", displacement: " << std::dec << converter._displacement << std::endl;
	os << "Base value: " << _base << std::endl;
	os << "Combined value: " << (_base + converter._displacement) << std::endl;
	os << "Combined value is less: " << std::boolalpha << ((_base + converter._displacement) < _base) << std::endl;
}
bool testInstructionEncoding(std::ostream& os, const i960::Instruction& test, const std::string& compareAgainst) noexcept {
	os << "test encoding: " << std::hex << test.getLowerHalf() << std::endl;
	os << "\tbase opcode: 0x" << std::hex << test.getOpcode() << std::endl;
	if (auto description = i960::Opcode::getDescription(test.getOpcode()); description.isUndefined()) {
		os << "\tInstruction mnemonic: 'undefined'" << std::endl;
		return false;
	} else {
		std::string tmp(description.getString());
		os << "\tInstruction mnemonic: '" << tmp << "'" << std::endl;
		return tmp == compareAgainst;
	}
}
void testInstructionEncoding(std::ostream& os) {
	constexpr i960::Ordinal instructions[] = {
#define X(name, code) \
		code,
#define reg(name, code, __) X(name, code)
#define mem(name, code, __) X(name, code)
#define cobr(name, code, __) X(name, code)
#define ctrl(name, code, __) X(name, code)
#include "opcodes.def"
#undef X
#undef reg
#undef mem
#undef cobr
#undef ctrl
	};
	for (auto i : instructions) {
		auto name = i960::Opcode::getDescription(i).getString();
		auto opcode = i;
		os << "Decoding: " << name << std::endl;
		os << "Outcome: ";
		i960::Instruction tmp(i960::encodeFullOpcode(0, opcode));
		if (testInstructionEncoding(os, tmp, name)) {
			os << "successful!" << std::endl;
		} else {
			os << "failed!" << std::endl;
			return;
		}
	}
}
int main() {
	int errorCode = 0;
	bootupMessage(std::cout);
	outputTypeInformation(std::cout);
    testOverflowOfDisplacement(std::cout);
	testInstructionEncoding(std::cout);
	std::cout << "Shutting down..." << std::endl;

	return errorCode;
}
