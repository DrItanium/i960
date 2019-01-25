#include <iostream>
#include <cstddef>
#include <memory>
#include "types.h"
#include "operations.h"
#include "opcodes.h"
#include <string>
void decode(const i960::Opcode::Description& desc, const i960::Instruction::REGFormat& inst) {
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
void decode(const i960::Opcode::Description& desc, const i960::Instruction::CTRLFormat& inst) {
    std::cout << " " << inst._ctrl.decodeDisplacement();
}
void decode(const i960::Opcode::Description& desc, const i960::Instruction::COBRFormat& inst) {
    i960::Operand src1(inst.decodeSrc1());
        std::cout << " " << src1;
    if (!desc.hasOneArgument()) {
        i960::Operand src2(inst.decodeSrc1());
        std::cout << ", " << src2 << ", " << inst.decodeDisplacement();
    }
}
void decode(const i960::Opcode::Description& desc, const i960::Instruction::MemFormat& inst) {
    // TODO dispatch into mema and memb formats
}
void decode(i960::Ordinal value) noexcept {
    i960::Instruction inst(value);
    auto desc = i960::Opcode::getDescription(inst.getOpcode());
    std::cout << std::hex << "0x" << value << ": " << desc.getString();
    if (inst.isRegFormat()) {
        decode(desc, inst._reg);
    } else if (inst.isControlFormat()) {
        decode(desc, inst._ctrl);
    } else if (inst.isCompareAndBranchFormat()) { 
        decode(desc, inst._cobr);
    } else if (inst.isMemFormat()) {
        decode(desc, inst._mem);
    } else {
        std::cout << " unknown instruction class... unable to decode further!";
    }
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
