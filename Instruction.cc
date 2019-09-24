#include "Instruction.h"

namespace i960 {
    constexpr Ordinal MEMAoffsetMask = 0xFFF;
    constexpr Ordinal MEMBscaleMask = 0x380;
    template<typename T>
    constexpr bool isMEMBFormat(T value) noexcept {
        constexpr auto toggleBit = 0b010000;
        return value & toggleBit;
    }
    template<typename T>
    constexpr bool isMEMAFormat(T value) noexcept {
        return !isMEMBFormat(value);
    }
    constexpr ByteOrdinal decodeMask(Ordinal value) noexcept {
        // upper two bits of the mode are shared between types, thus we should do the 
        // mask of 0x3C and make a four bit type code in all cases. However, we also
        // have a 2-bit unused field inside of MEMB that we should use for sanity checking
        constexpr auto memAModeMask = 0b110000;
        constexpr auto memBExtraMask = 0x30;
        constexpr auto mask = 0x3C00;
        constexpr auto shift = 6;
        if (auto shiftedValue = decode<Ordinal, ByteOrdinal, mask, shift>(value); isMEMBFormat(shiftedValue)) {
            // MEMB
            return shiftedValue | (value | memBExtraMask);
        } else {
            // MEMA
            return shiftedValue & memAModeMask;
        }
    }
    constexpr Ordinal encodeMode(Ordinal value, ByteOrdinal mode) noexcept {
        if (auto maskedOutValue = static_cast<Ordinal>(mode); isMEMBFormat(mode)) {
            constexpr Ordinal mask = 0b1111'000'00'00000;
            // we really only have four bits to deal with in this case
            return (value & ~mask) | (((maskedOutValue >> 2) << 10) & mask);
        } else {
            // we must process the upper two bits
            constexpr Ordinal mask = 0b11'000000'000000;
            return (value & ~mask) | (((maskedOutValue >> 4) << 12) & mask);
        }
    }
    template<typename R = Operand>
    constexpr auto decodeSrcDest(Ordinal input) noexcept {
        constexpr Ordinal Mask = 0x00F80000;
        constexpr Ordinal Shift = 19;
        return decode<Ordinal, R, Mask, Shift>(input);
    }
    template<Ordinal mask>
    constexpr auto decodeSrcDest(Ordinal input, Ordinal modeBits) noexcept {
        return Operand((modeBits & mask) != 0, decodeSrcDest<Ordinal>(input));
    }
    constexpr Ordinal encodeSrcDest(Ordinal value, Operand input) noexcept {
        constexpr Ordinal Mask = 0x00F80000;
        constexpr Ordinal Shift = 19;
        return encode<Ordinal, ByteOrdinal, Mask, Shift>(value, input.getValue());
    }
    static_assert(encodeSrcDest(0, 1_lr) == 0x00080000);
    constexpr auto encodeSrc2(Ordinal value, Operand input) noexcept {
        constexpr Ordinal Mask = 0x000C7000;
        constexpr Ordinal Shift = 14;
        return encode<Ordinal, ByteOrdinal, Mask, Shift>(value, input.getValue());
    }
    static_assert(encodeSrc2(0, 1_lr) == (0x00080000 >> 5));

    constexpr auto encodeSrc1(Ordinal value, Operand input) noexcept {
        constexpr Ordinal Mask = 0x1F;
        return encode<Ordinal, ByteOrdinal, Mask, 0>(value, input.getValue());
    }
    static_assert(encodeSrc1(0, 1_lr) == 1);

    template<typename R = ByteOrdinal>
    constexpr auto decodeSrc1(Ordinal input) noexcept {
        constexpr Ordinal Mask = 0x1F;
        return decode<Ordinal, R, Mask>(input);
    }
    template<Ordinal mask>
    constexpr auto decodeSrc1(Ordinal value, Ordinal modeBits) noexcept {
        return Operand((modeBits & mask) != 0, decodeSrc1<Ordinal>(value));
    }
    template<typename R = Operand>
    constexpr auto decodeSrc2(Ordinal input) noexcept {
        constexpr Ordinal Mask = 0x000C7000;
        constexpr Ordinal Shift = 14;
        return decode<Ordinal, R, Mask, Shift>(input);
    }
    template<Ordinal mask>
    constexpr auto decodeSrc2(Ordinal input, Ordinal modeBits) noexcept {
        return Operand((modeBits & mask) != 0, decodeSrc2<Ordinal>(input));
    }
    MEMFormatInstruction::MEMFormatInstruction(const Instruction& inst) : Base(inst), 
    HasSrcDest(decodeSrcDest(inst.getLowerHalf())),
    HasSrc2(decodeSrc2(inst.getLowerHalf())),
    HasSrc1(decodeSrc1(inst.getLowerHalf())),
    _mode(decodeMask(inst.getLowerHalf())),
    _offset(decode<Ordinal, ByteOrdinal, MEMAoffsetMask>(inst.getLowerHalf())),
    _scale(decode<Ordinal, ByteOrdinal, MEMBscaleMask, 7>(inst.getLowerHalf())),
    _displacement(inst.getUpperHalf()),
    _target(Operation::translate(inst.getOpcode(), Operation::MEMClass())) { }

    EncodedInstruction
    MEMFormatInstruction::encode() const noexcept {
        auto instruction = encodeMajorOpcode(getOpcode());
        instruction = encodeSrcDest(instruction, getSrcDest());
        instruction = encodeSrc2(instruction, getAbase());
        /// @todo implement
        return 0u;
    }


    CTRLFormatInstruction::CTRLFormatInstruction(const Instruction& inst) : Base(inst), 
    _displacement(decode<Ordinal, Integer, 0x00FFFFFC, 2>(inst.getLowerHalf())),
    _t(decode<Ordinal, bool, 0b10>(inst.getLowerHalf())),
    _target(Operation::translate(inst.getOpcode(), Operation::CTRLClass())) { }

    EncodedInstruction
    CTRLFormatInstruction::encode() const noexcept {
        // mask out the least significant bit to follow the ctrl format
        return i960::encode<Ordinal, Ordinal, 0x00FFFFFC, 2>(
                i960::encode<Ordinal, bool, 0b10, 1>(encodeMajorOpcode(0, getOpcode()), _t),
                _displacement) & 0xFFFF'FFFE;
    }
    COBRFormatInstruction::COBRFormatInstruction(const Instruction& inst) : Base(inst),
    Flags(inst),
    HasSrcDest(decodeSrcDest<0b100>(inst.getLowerHalf(), Flags::getValue())),
    HasSrc2(decodeSrc2(inst.getLowerHalf())),
    _displacement(i960::decode<Ordinal, Ordinal, 0b1111'1111'1100, 2>(inst.getLowerHalf())),
    _target(Operation::translate(inst.getOpcode(), Operation::COBRClass()))
    { }

    EncodedInstruction
    COBRFormatInstruction::encode() const noexcept {
        auto instruction = encodeMajorOpcode(0, getOpcode());
        instruction = encodeSrcDest(instruction, getSrc1());
        instruction = encodeSrc2(instruction, getSrc2());
        instruction = i960::encode<Ordinal, Ordinal, 0b1111'1111'1100, 2>(instruction, _displacement);
        return Flags::encode(instruction);
    }

    REGFormatInstruction::REGFormatInstruction(const Instruction& inst) : Base(inst), 
    Flags(inst),
    HasSrcDest(decodeSrcDest<0b10000>(inst.getLowerHalf(), Flags::getValue())),
    HasSrc2(decodeSrc2<0b01000>(inst.getLowerHalf(), Flags::getValue())),
    HasSrc1(decodeSrc1<0b00100>(inst.getLowerHalf(), Flags::getValue())),
    _target(Operation::translate(inst.getOpcode(), Operation::REGClass())) { }

    EncodedInstruction
    REGFormatInstruction::encode() const noexcept {
        auto instruction = encodeFullOpcode(0, getOpcode());
        instruction = encodeSrcDest(instruction, getSrcDest());
        instruction = encodeSrc2(instruction, getSrc2());
        instruction = encodeSrc1(instruction, getSrc1());
        return Flags::encode(instruction);
    }

    DecodedInstruction 
    Instruction::decode() const noexcept {
        if (isREGFormat()) {
            return REGFormatInstruction(*this);
        } else if (isMEMFormat()) {
            return MEMFormatInstruction(*this);
        } else if (isCOBRFormat()) {
            return COBRFormatInstruction(*this);
        } else if (isCTRLFormat()) {
            return CTRLFormatInstruction(*this);
        } else {
            return std::monostate();
        }
    }

} // end namespace i960
