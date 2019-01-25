#include <iostream>
#include <cstddef>
#include <memory>
#include "types.h"
#include "operations.h"
#include "opcodes.h"
#include <string>
#include <sstream>
void decode(std::ostream& out, const i960::Opcode::Description& desc, const i960::Instruction::REGFormat& inst) {
    if (desc.hasZeroArguments()) {
        return;
    }
    i960::Operand src1(inst.decodeSrc1());
    i960::Operand src2(inst.decodeSrc2());
    i960::Operand srcDest(inst.decodeSrcDest());
    out << " " << src1;
    if (desc.hasOneArgument()) {
        // we are done now
        return;
    }
    if (desc.hasTwoArguments()) {
        out << ", ";
        switch (desc) {
            case i960::Opcode::cmpos:
            case i960::Opcode::cmpob:
            case i960::Opcode::cmpis:
            case i960::Opcode::cmpib:
            case i960::Opcode::scanbyte:
            case i960::Opcode::bswap:
                out << src2;
                break;
            default:
                out << srcDest;
                break;
        }
    } else if (desc.hasThreeArguments()) {
        out << ", " << src2 << ", " << srcDest;
    } else {
        // we need to state that the instruction is seemingly malformed
        out << " could not decode rest!";
    }
}
void decode(std::ostream& out, const i960::Opcode::Description& desc, const i960::Instruction::COBRFormat& inst) {
    i960::Operand src1(inst.decodeSrc1());
    out << " " << src1;
    if (!desc.hasOneArgument()) {
        i960::Operand src2(inst.decodeSrc1());
        out << ", " << src2 << ", " << inst.decodeDisplacement();
    }
}
void decode(std::ostream& out, const i960::Opcode::Description& desc, const i960::Instruction::MemFormat::MEMBFormat& inst) {
    out << "0x";
    using E = std::decay_t<decltype(inst)>::AddressingModes;
    auto index = inst._index;
    auto scale = inst.getScaleFactor();
    i960::Ordinal displacement = 0u;
    if (inst.has32bitDisplacement()) {
        std::cin >> std::hex >> displacement;
        if (!std::cin.good()) {
            out << "Bad input!!!" << std::endl;
            return;
        } else {
            out << std::hex << displacement << " ";
        }
    }
    out << std::hex << inst._raw << ": " << desc.getString() << " ";
    i960::Operand abase(inst.decodeAbase());
    switch(inst.getAddressingMode()) {
        case E::Abase:
            out << "(" << abase << ")";
            break;
        case E::IP_Plus_Displacement_Plus_8:
            out << "((ip) + " << displacement << " + 8)";
            break;
        case E::Abase_Plus_Index_Times_2_Pow_Scale:
            out << "((" << abase << ") + (" << index << ") * " << scale << ")"; 
            break;
        case E::Displacement:
            out << displacement;
            break;
        case E::Abase_Plus_Displacement:
            out << "((" << abase << ") + " << displacement << ")";
            break;
        case E::Index_Times_2_Pow_Scale_Plus_Displacement:
            out << "((" << index << ") * " << scale << " + " << displacement << ")";
            break;
        case E::Abase_Plus_Index_Times_2_Pow_Scale_Plus_Displacement:
            out << "((" << abase << ") + (" << index << ") * " << scale << " + " << displacement << ")"; 
            break;
        default:
            out << "ERROR: reserved!!!";
            break;
    }
    out << ", " << i960::Operand(inst.decodeSrcDest());
}
void decode(std::ostream& out, const i960::Opcode::Description& desc, const i960::Instruction::MemFormat& inst) {
    if (inst.isMemAFormat()) {
        auto ma = inst._mema;
        i960::Operand srcDest(inst.decodeSrcDest());
        out << std::hex << "0x" << ma._raw << ": " << desc.getString() << " 0x" << ma._offset;
        if (!ma.isOffsetAddressingMode()) {
            out << "(" << i960::Operand(ma.decodeAbase()) << ")";
        }
        out << ", " << srcDest;
    } else {
        decode(out, desc, inst._memb);
    }
}
std::string decode(i960::Ordinal value) noexcept {
    i960::Instruction inst(value);
    auto desc = i960::Opcode::getDescription(inst.getOpcode());
    std::ostringstream out;
    if (inst.isRegFormat()) {
        out << std::hex << "0x" << value << ": " << desc.getString();
        decode(out, desc, inst._reg);
    } else if (inst.isControlFormat()) {
        out << std::hex << "0x" << value << ": " << desc.getString();
        out << " " << inst._ctrl.decodeDisplacement();
    } else if (inst.isCompareAndBranchFormat()) { 
        out << std::hex << "0x" << value << ": " << desc.getString();
        decode(out, desc, inst._cobr);
    } else if (inst.isMemFormat()) {
        decode(out, desc, inst._mem);
    } else {
        out << " unknown instruction class... unable to decode further!";
    }
    std::string tmp(out.str());
    return tmp;
}
int main() {
	int errorCode = 0;
    while(std::cin.good()) {
        i960::Ordinal value = 0;
        std::cin >> std::hex >> value;
        if (std::cin.good()) {
            std::cout << decode(value) << std::endl;
        }
    }
	return errorCode;
}
