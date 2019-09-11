#include "Instruction.h"

namespace i960 {
    constexpr Ordinal MEMsrcDestMask = 0x00F80000;
    constexpr Ordinal MEMsrcDestShift = 19;
    constexpr Ordinal MEMabaseMask   = 0x0007C000;
    constexpr Ordinal MEMabaseShift = 14;
    constexpr Ordinal MEMAoffsetMask = 0xFFF;
    constexpr Ordinal MEMBscaleMask = 0x380;
    constexpr Ordinal MEMBindexMask = 0x1F;
    constexpr ByteOrdinal decodeMask(Ordinal value) noexcept {
        // upper two bits of the mode are shared between types, thus we should do the 
        // mask of 0x3C and make a four bit type code in all cases. However, we also
        // have a 2-bit unused field inside of MEMB that we should use for sanity checking
        constexpr auto toggleBit = 0b010000;
        constexpr auto memAModeMask = 0b110000;
        constexpr auto memBExtraMask = 0x30;
        constexpr auto mask = 0x3C00;
        constexpr auto shift = 6;
        if (auto shiftedValue = (value & mask) >> shift; shiftedValue & toggleBit) {
            // MEMB
            return shiftedValue | (value | memBExtraMask);
        } else {
            // MEMA
            return shiftedValue & memAModeMask;
        }
    }
    MEMFormatInstruction::MEMFormatInstruction(const DecodedInstruction& inst) : Base(inst), 
    _srcDest((inst.getLowerHalf() & MEMsrcDestMask) >> 19),
    _abase((inst.getLowerHalf() & MEMabaseMask) >> 14),
    _mode(decodeMask(inst.getLowerHalf())),
    _offset(inst.getLowerHalf() & MEMAoffsetMask),
    _scale((inst.getLowerHalf() & MEMBscaleMask) >> 7),
    _index((inst.getLowerHalf() & MEMBindexMask)),
    _displacement(inst.getUpperHalf()) { }

    EncodedInstruction
    MEMFormatInstruction::encode() const noexcept {
        /// @todo implement
        return 0u;
    }


    CTRLFormatInstruction::CTRLFormatInstruction(const DecodedInstruction& inst) : Base(inst), 
    _displacement((inst.getLowerHalf() & 0x00FFFFFC) >> 2),
    _t((0b10 & inst.getLowerHalf()) != 0) {
        if ((inst.getLowerHalf() & 1) != 0) {
            /// @todo throw an exception
        }
    }

    EncodedInstruction
    CTRLFormatInstruction::encode() const noexcept {
        /// @todo implement
        return 0u;
    }


} // end namespace i960
