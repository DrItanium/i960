#include <iostream>
#include <cstddef>
#include <memory>
#include "types.h"
#include "operations.h"
#include "opcodes.h"
#include <string>

void decode(i960::Ordinal value) noexcept {
    i960::Instruction inst(value);
    auto name = i960::Opcode::getDescription(inst.getOpcode());
    std::cout << std::hex << value << ": " << name.getString() << "(" << std::hex << "0x" << inst.getOpcode() << ")" << std::endl;
}
int main(int argc, char* argv[]) {
	int errorCode = 0;
    while(std::cin.good()) {
        i960::Ordinal value = 0;
        std::cin >> std::hex >> value;
        decode(value);
    }
	return errorCode;
}
