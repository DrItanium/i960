#include "Instruction.h"

namespace i960 {
    constexpr Ordinal getMajorOpcode(HalfOrdinal ordinal) noexcept {
        return decode<HalfOrdinal, Ordinal, 0x0FF0, 4>(ordinal);
    }
    constexpr Ordinal encodeOpcode(Ordinal input, HalfOrdinal opcode) noexcept {
        constexpr Ordinal majorOpcodeMask = 0xFF000000;
        return encode<Ordinal, HalfOrdinal, majorOpcodeMask, 24>(input, getMajorOpcode(opcode));
    }
    constexpr Ordinal getMinorOpcode(HalfOrdinal ordinal) noexcept {
        return decode<Ordinal, Ordinal, 0x000F>(ordinal);
    }
    constexpr Ordinal encodeMinorOpcode(Ordinal input, HalfOrdinal opcode) noexcept {
        constexpr Ordinal minorOpcodeMask = 0b1111'00'00000;
        return encode<Ordinal, HalfOrdinal, minorOpcodeMask, 7>(input, getMinorOpcode(opcode));
    }
    constexpr Ordinal MEMAoffsetMask = 0xFFF;
    constexpr Ordinal MEMBscaleMask = 0x380;
    constexpr ByteOrdinal decodeMask(Ordinal value) noexcept {
        // upper two bits of the mode are shared between types, thus we should do the 
        // mask of 0x3C and make a four bit type code in all cases. However, we also
        // have a 2-bit unused field inside of MEMB that we should use for sanity checking
        constexpr auto toggleBit = 0b010000;
        constexpr auto memAModeMask = 0b110000;
        constexpr auto memBExtraMask = 0x30;
        constexpr auto mask = 0x3C00;
        constexpr auto shift = 6;
        if (auto shiftedValue = decode<Ordinal, ByteOrdinal, mask, shift>(value); shiftedValue & toggleBit) {
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
        return decode<Ordinal, Operand, Mask, Shift>(input);
    }
    constexpr auto decodeSrc2(Ordinal input) noexcept {
        constexpr Ordinal Mask = 0x000C7000;
        constexpr Ordinal Shift = 14;
        return decode<Ordinal, Operand, Mask, Shift>(input);
    }
    constexpr auto decodeSrc1(Ordinal input) noexcept {
        constexpr Ordinal Mask = 0x1F;
        return decode<Ordinal, ByteOrdinal, Mask>(input);
    }
    MEMFormatInstruction::MEMFormatInstruction(const DecodedInstruction& inst) : Base(inst), 
    _srcDest(decodeSrcDest(inst.getLowerHalf())),
    _abase(decodeSrc2(inst.getLowerHalf())),
    _mode(decodeMask(inst.getLowerHalf())),
    _offset(decode<Ordinal, ByteOrdinal, MEMAoffsetMask>(inst.getLowerHalf())),
    _scale(decode<Ordinal, ByteOrdinal, MEMBscaleMask, 7>(inst.getLowerHalf())),
    _index(decodeSrc1(inst.getLowerHalf())),
    _displacement(inst.getUpperHalf()) { }

    EncodedInstruction
    MEMFormatInstruction::encode() const noexcept {
        /// @todo implement
        return 0u;
    }


    CTRLFormatInstruction::CTRLFormatInstruction(const DecodedInstruction& inst) : Base(inst), 
    _displacement(decode<Ordinal, Ordinal, 0x00FFFFFC, 2>(inst.getLowerHalf())),
    _t(decode<Ordinal, bool, 0b10>(inst.getLowerHalf())) {
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
        return decode<Ordinal, ByteOrdinal, 0b1'0000'0000'0000, 10>(value) |
               decode<Ordinal, ByteOrdinal, 0b11>(value);
    }
    COBRFormatInstruction::COBRFormatInstruction(const DecodedInstruction& inst) : Base(inst),
    _source1(decodeSrcDest(inst.getLowerHalf())),
    _source2(decodeSrc2(inst.getLowerHalf())),
    _displacement(decode<Ordinal, Ordinal, 0b1111'1111'1100, 2>(inst.getLowerHalf())),
    _flags(computeCOBRFlags(inst.getLowerHalf())),
    _bitpos(decodeSrcDest(inst.getLowerHalf()))
    { }

    EncodedInstruction
    COBRFormatInstruction::encode() const noexcept {
        /// @todo implement
        return 0u;
    }
    constexpr ByteOrdinal computeREGFlags(Ordinal value) noexcept {
        auto lowerTwo = decode<Ordinal, ByteOrdinal, 0b11'00000, 5>(value);
        auto upperThree = decode<Ordinal, ByteOrdinal, 0b111'0000'00'00000, 8>(value);
        return lowerTwo | upperThree;
    }

    REGFormatInstruction::REGFormatInstruction(const DecodedInstruction& inst) : Base(inst),
    _srcDest(decodeSrcDest(inst.getLowerHalf())),
    _src2(decodeSrc2(inst.getLowerHalf())),
    _src1(decodeSrc1(inst.getLowerHalf())),
    _flags(computeREGFlags(inst.getLowerHalf())),
    _bitpos(decodeSrc1(inst.getLowerHalf())) { }

    EncodedInstruction
    REGFormatInstruction::encode() const noexcept {
        /// @todo implement
        return 0u;
    }

} // end namespace i960
