#include <iostream>
#include <cstddef>
#include <memory>
#include "types.h"
#include "opcodes.h"
#include "Instruction.h"
#include <string>
#include <sstream>
void decode(std::ostream& out, const i960::Opcode::Description& desc, const i960::REGFormatInstruction& inst) {
    if (desc.hasZeroArguments()) {
        return;
    }
    out << " " << inst.getSrc1();
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
                out << inst.getSrc2();
                break;
            default:
                out << inst.getSrcDest();
                break;
        }
    } else if (desc.hasThreeArguments()) {
        out << ", " << inst.getSrc2() << ", " << inst.getSrcDest();
    } else {
        // we need to state that the instruction is seemingly malformed
        out << " could not decode rest!";
    }
}
void decode(std::ostream& out, const i960::Opcode::Description& desc, const i960::COBRFormatInstruction& inst) {
    out << " " << inst.getSrc1();
    if (!desc.hasOneArgument()) {
        out << ", " << inst.getSrc2() << ", " << inst.getDisplacement();
    }
}
void decode(std::ostream& out, const i960::Opcode::Description& desc, const i960::MEMFormatInstruction& inst) {
    using M = std::decay_t<i960::MEMFormatInstruction::AddressingModes>;
    std::visit([&out, &desc](auto&& value) { out << std::hex << "0x" << value << ": " << desc.getString() << " "; }, inst.encode());
    switch (inst.getMode()) {
        case M::AbsoluteOffset:
            out << "0x" << inst.getOffset();
            break;
        case M::RegisterIndirectWithOffset:
            out << "0x" << inst.getOffset() << "(" << inst.getAbase() << ")";
            break;
        case M::RegisterIndirect:
            out << "(" << inst.getAbase() << ")";
            break;
        case M::IPWithDisplacement:
            out << "((ip) + " << inst.getDisplacement() << " + 8)";
            break;
        case M::RegisterIndirectWithIndex:
            out << "((" << inst.getAbase() << ") + (" << inst.getIndex() << ") * " << inst.getScale() << ")";
            break;
        case M::AbsoluteDisplacement:
            out << inst.getDisplacement();
            break;
        case M::RegisterIndirectWithDisplacement:
            out << "((" << inst.getAbase() << ") + " << inst.getDisplacement() << ")";
            break;
        case M::RegisterIndirectWithIndexAndDisplacement:
            out << "((" << inst.getAbase() << ") + (" << inst.getIndex() << " * " << inst.getScale() << ") + " << inst.getDisplacement() << ")";
            break;
        case M::IndexWithDisplacement:
            out << "((" << inst.getIndex() << " * " << inst.getScale() << ") + " << inst.getDisplacement() << ")";
            break;
        default:
            out << "ERROR: reserved (" << static_cast<uint8_t>(inst.getMode()) << ")";
            break;
    }

    out << ", " << inst.getSrcDest();
}
std::string decode(i960::Ordinal value) noexcept {
    i960::Instruction inst(value);
    auto desc = i960::Opcode::getDescription(inst.getOpcode());
    std::ostringstream out;
    std::visit(i960::overloaded {
                [&out, &desc, value](const i960::CTRLFormatInstruction& ctrl) {
                    out << std::hex << "0x" << value << ": " << desc.getString();
                    out << " " << ctrl.getDisplacement();
                },
                [&out, &desc, value](const i960::MEMFormatInstruction& dec) {
                    decode(out, desc, dec);
                },
                [&out, &desc, value](const i960::COBRFormatInstruction& dec) {
                    out << std::hex << "0x" << value << ": " << desc.getString();
                    decode(out, desc, dec);
                },
                [&out, &desc, value](const i960::REGFormatInstruction& dec) {
                    out << std::hex << "0x" << value << ": " << desc.getString();
                    decode(out, desc, dec);
                },
                [&out](const auto&) {
                    out << "bad instruction format!";
                }
            }, inst.decode());
    std::string tmp(out.str());
    return tmp;
}
int main() {
	int errorCode = 0;
    while(std::cin.good()) {
        std::cout << "> ";
        i960::Ordinal value = 0;
        std::cin >> std::hex >> value;
        if (std::cin.good()) {
            std::cout << decode(value) << std::endl;
        }
    }
	return errorCode;
}
