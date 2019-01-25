#include <iostream>
#include <cstddef>
#include <memory>
#include "types.h"
#include "operations.h"
#include "opcodes.h"
#include <string>
void decode(const i960::Opcode::Description& desc, const decltype(i960::Instruction::_reg)& inst) {
    if (desc.hasZeroArguments()) {
        return;
    }
    i960::Operand src1(inst.decodeSrc1());
    i960::Operand src2(inst.decodeSrc2());
    i960::Operand srcDest(inst.decodeSrcDest());
    std::cout << " " << src1;
    if (desc.hasOneArgument()) {
        // we are done now
        return;
    }
    if (desc.hasTwoArguments()) {
        std::cout << ", ";
        switch (desc) {
            case i960::Opcode::cmpos:
            case i960::Opcode::cmpob:
            case i960::Opcode::cmpis:
            case i960::Opcode::cmpib:
            case i960::Opcode::scanbyte:
            case i960::Opcode::bswap:
                std::cout << src2;
                break;
            default:
                std::cout << srcDest;
                break;
        }
    } else if (desc.hasThreeArguments()) {
        std::cout << src1 << ", " << src2 << ", " << srcDest;
    } else {
        // we need to state that the instruction is seemingly malformed
        std::cout << " could not decode rest!";
    }
}
void decode(const i960::Opcode::Description& desc, const i960::Instruction& inst) noexcept {
    if (inst.isRegFormat()) {
        decode(desc, inst._reg);
    } else if (inst.isControlFormat()) {
    } else if (inst.isCompareAndBranchFormat()) { 
    } else if (inst.isMemFormat()) {
    } else {
        std::cout << " unknown instruction class... unable to decode further!";
    }

}
void decode(i960::Ordinal value) noexcept {
    i960::Instruction inst(value);
    auto desc = i960::Opcode::getDescription(inst.getOpcode());
    std::cout << std::hex << "0x" << value << ": " << desc.getString();
    decode(desc, inst);
    std::cout << std::endl;
}
int main(int argc, char* argv[]) {
	int errorCode = 0;
    do {
        i960::Ordinal value = 0;
        std::cin >> std::hex >> value;
        decode(value);
    } while(std::cin.good());
	return errorCode;
}
