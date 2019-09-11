#include "Instruction.h"

namespace i960 {
    constexpr ByteOrdinal decodeMask(Ordinal value) noexcept {
        // okay, we are looking a four bit type code with bit 3 being the bit we toggle on
        constexpr auto toggleBit = 0b0100;
        constexpr auto memAModeMask = 0b1100;
        constexpr auto mask = 0x3C00;
        constexpr auto shift = 8;
        if (auto shiftedValue = (value & modeMask) >> modeShift; shiftedValue & toggleBit) {
            // MEMB type operation
            return shiftedValue;
        } else {
            return shiftedValue & memAModeMask;
        }
    }
    MEMFormatInstruction::MEMFormatInstruction(const DecodedInstruction& inst) :
                Base(inst), 
                _srcDest((inst.getLowerHalf() & srcDestMask) >> 19),
                _abase((inst.getLowerHalf() & abaseMask) >> 14),
                _mode(decodeMask(inst.getLowerHalf()) { }
} // end namespace i960
