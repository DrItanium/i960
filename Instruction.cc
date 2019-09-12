#include "Instruction.h"

namespace i960 {
    constexpr Ordinal getMajorOpcode(HalfOrdinal ordinal) noexcept {
        return (0x0FF0 & ordinal) >> 4;
    }
    constexpr Ordinal encodeOpcode(Ordinal input, HalfOrdinal opcode) noexcept {
        constexpr Ordinal majorOpcodeMask = 0xFF000000;
        return (input & (~majorOpcodeMask)) | ((getMajorOpcode(opcode) << 24) & majorOpcodeMask);
    }
    constexpr Ordinal getMinorOpcode(HalfOrdinal ordinal) noexcept {
        return (0x000F & ordinal);
    }
    constexpr Ordinal encodeMinorOpcode(Ordinal input, HalfOrdinal opcode) noexcept {
        constexpr Ordinal minorOpcodeMask = 0b1111'00'00000;
        return (input & (~minorOpcodeMask)) | ((getMinorOpcode(opcode) << 7) & minorOpcodeMask);
    }
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
    constexpr auto decodeSrcDest(Ordinal input) noexcept {
        constexpr Ordinal Mask = 0x00F80000;
        constexpr Ordinal Shift = 19;
        return (input & Mask) >> Shift;
    }
    constexpr auto decodeSrc2(Ordinal input) noexcept {
        constexpr Ordinal Mask = 0x000C7000;
        constexpr Ordinal Shift = 14;
        return (input & Mask) >> Shift;
    }
    MEMFormatInstruction::MEMFormatInstruction(const DecodedInstruction& inst) : Base(inst), 
    _srcDest(decodeSrcDest(inst.getLowerHalf())),
    _abase(decodeSrc2(inst.getLowerHalf())),
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
    constexpr ByteOrdinal computeCOBRFlags(Ordinal value) noexcept {
        return static_cast<ByteOrdinal>(((0b1'0000'0000'0000 & value) >> 10) | (0b11 & value));
    }
    COBRFormatInstruction::COBRFormatInstruction(const DecodedInstruction& inst) : Base(inst),
    _source1(decodeSrcDest(inst.getLowerHalf())),
    _source2(decodeSrc2(inst.getLowerHalf())),
    _displacement((0b1111'1111'1100 & inst.getLowerHalf()) >> 2),
    _flags(computeCOBRFlags(inst.getLowerHalf())),
    _bitpos(decodeSrcDest(inst.getLowerHalf()))
    { }

    EncodedInstruction
    COBRFormatInstruction::encode() const noexcept {
        /// @todo implement
        return 0u;
    }
    constexpr ByteOrdinal computeREGFlags(Ordinal value) noexcept {
        auto lowerTwo = (0b1100000 & value) >> 5;
        auto upperThree = (0b111'0000'00'00000 & value) >> 8;
        return static_cast<ByteOrdinal>(lowerTwo | upperThree);
    }

    REGFormatInstruction::REGFormatInstruction(const DecodedInstruction& inst) : Base(inst),
    _srcDest(decodeSrcDest(inst.getLowerHalf())),
    _src2(decodeSrc2(inst.getLowerHalf())),
    _src1((MEMBindexMask & inst.getLowerHalf())),
    _flags(computeREGFlags(inst.getLowerHalf())),
    _bitpos((MEMBindexMask & inst.getLowerHalf())) { }

    EncodedInstruction
    REGFormatInstruction::encode() const noexcept {
        /// @todo implement
        return 0u;
    }

} // end namespace i960
