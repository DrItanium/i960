#include <iostream>
#include <iomanip>
#include <cstddef>
#include <memory>
#include "types.h"
#include "opcodes.h"
#include "Instruction.h"
#include <string>
#include <sstream>

std::string
binary(i960::Ordinal value) noexcept 
{
    std::ostringstream sstream;
    auto tracker = value;
    constexpr i960::Ordinal Mask = 0x8000'0000;
    for (int i = 0; i < 32; ++i) {
        sstream << ((Mask & tracker) ? "1" : "0");
        tracker <<= 1;
    }
    auto str = sstream.str();
    return str;
}
std::string
binary(i960::LongOrdinal value) noexcept 
{
    std::ostringstream sstream;
    auto tracker = value;
    constexpr i960::LongOrdinal Mask = 0x8000'0000'0000'0000;
    for (int i = 0; i < 64; ++i) {
        sstream << ((Mask & tracker) ? "1" : "0");
        tracker <<= 1;
    }
    auto str = sstream.str();
    return str;
}
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
    auto encoded = inst.encode();
    std::visit([&out, &desc](auto&& value) { 
                out << std::hex << "0x" << value << ": " << desc.getString() << " "; 
            }, encoded);
    constexpr auto computeProperScaleFactor = [](auto scale) -> int {
        switch (scale) {
            case 0b000:
                return 1;
            case 0b001:
                return 2;
            case 0b010:
                return 4;
            case 0b011:
                return 8;
            case 0b100:
                return 16;
            default:
                return -666; // an obviously wrong value
        }
    };
    auto properScale = computeProperScaleFactor(inst.getScale());
    auto abase = inst.getAbase();
    auto offset = inst.getOffset();
    auto index = inst.getIndex();
    out << inst.getSrcDest() << ", ";
    switch (inst.getMode()) {
        case M::AbsoluteOffset: // MEMA Form of Absolute [0,2048]
            out << "0x" << std::hex << offset;
            break;
        case M::AbsoluteDisplacement: // MEMB Form of Absolute, [-2^31, (2^31)-1]
            out << "0x" << std::hex << inst.getDisplacement();
            break;
        case M::RegisterIndirect:
            out << "(" << abase << ")";
            break;
        case M::RegisterIndirectWithOffset: // MEMA Form of Register Indirect with Offset
            out << "0x" << std::hex << offset << "(" << abase << ")";
            break;
        case M::RegisterIndirectWithDisplacement: // MEMBForm of Register Indirect with Offset (use displacement)
            out << "0x" << std::hex << inst.getDisplacement() << "(" << abase << ")";
            break;
        case M::RegisterIndirectWithIndex: // MEMA Form of Register Indirect with Index
            out << "(" << abase << ")[" << index << "*" << std::dec << properScale << "]";
            break;
        case M::RegisterIndirectWithIndexAndDisplacement:
            out << "0x" << std::hex << inst.getDisplacement() << "(" << abase << ")[" << index << "*" << std::dec << properScale << "]";
            break;
        case M::IndexWithDisplacement:
            out << "(" << index << " * " << std::dec << properScale << ") + 0x" << std::hex << inst.getDisplacement();
            break;
        case M::IPWithDisplacement:
            out << "0x" << std::hex << inst.getDisplacement() << "(ip)";
            break;
        default:
            out << "ERROR: reserved (" << static_cast<int>(inst.getRawMode() >> 2) << ")";
            break;
    }

    out << "  (0b";
    std::visit([&out](auto&& value) {
                out << binary(value);
            }, encoded);
    out << ")";
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
