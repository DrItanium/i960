#include "Instruction.h"

namespace i960 {
    constexpr Ordinal MEMAoffsetMask = 0xFFF;
    constexpr Ordinal MEMBscaleMask = 0x380;
    constexpr ByteOrdinal decodeMask(Ordinal value) noexcept {
        // bits [10,13] are the entire mask.
        constexpr auto memABModeMask = 0b11'1100'0000'0000;
        if (auto typeSelect = decode<Ordinal, ByteOrdinal, memABModeMask, 10>(value); (typeSelect & 0b0100) == 0) {
            // MEMA format
            return (typeSelect & 0b1100); // lower two bits are part of the offset field
        } else {
            return typeSelect;
        }
    }
    constexpr Ordinal encodeMode(Ordinal value, ByteOrdinal mode) noexcept {
        if (MEMFormatInstruction::isMEMBFormat(mode)) {
            constexpr Ordinal mask = 0b1111'000'00'00000;
            return encode<Ordinal, decltype(mode),  mask, 10>(value, mode);
            // we really only have four bits to deal with in this case
        } else {
            constexpr Ordinal mask = 0b11'000000'000000;
            return encode<Ordinal, decltype(mode), mask, 10>(value, mode);
        }
    }
    template<typename R = Operand>
    constexpr auto decodeSrcDest(Ordinal input) noexcept {
        constexpr BitFragment<decltype(input), R, HasSrcDest::EncoderDecoder::Mask, HasSrcDest::EncoderDecoder::Shift> srcDestMask;
        return srcDestMask.decode(input);
    }
    template<Ordinal mask>
    constexpr auto decodeSrcDest(Ordinal input, Ordinal modeBits) noexcept {
        return Operand((modeBits & mask) != 0, decodeSrcDest<Ordinal>(input));
    }
    constexpr Ordinal encodeSrcDest(Ordinal value, Operand input) noexcept {
        constexpr OrdinalToByteOrdinalField<HasSrcDest::EncoderDecoder::Mask, HasSrcDest::EncoderDecoder::Shift> fragment;
        return fragment.encode(value, input.getValue());
    }
    static_assert(encodeSrcDest(0, 1_lr) == 0x00080000);
    constexpr auto encodeSrc2(Ordinal value, Operand input) noexcept {
        constexpr Ordinal Mask = HasSrc2::EncoderDecoder::Mask;
        constexpr Ordinal Shift = HasSrc2::EncoderDecoder::Shift;
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
        constexpr Ordinal Mask = HasSrc1::EncoderDecoder::Mask;
        constexpr Ordinal Shift = HasSrc1::EncoderDecoder::Shift;
        return decode<Ordinal, R, Mask, Shift>(input);
    }
    template<Ordinal mask>
    constexpr auto decodeSrc1(Ordinal value, Ordinal modeBits) noexcept {
        return Operand((modeBits & mask) != 0, decodeSrc1<Ordinal>(value));
    }
    template<typename R = Operand>
    constexpr auto decodeSrc2(Ordinal input) noexcept {
        constexpr Ordinal Mask = HasSrc2::EncoderDecoder::Mask;
        constexpr Ordinal Shift = HasSrc2::EncoderDecoder::Shift;
        return decode<Ordinal, R, Mask, Shift>(input);
    }
    template<Ordinal mask>
    constexpr auto decodeSrc2(Ordinal input, Ordinal modeBits) noexcept {
        return Operand((modeBits & mask) != 0, decodeSrc2<Ordinal>(input));
    }
    MEMFormatInstruction::MEMFormatInstruction(const Instruction& inst) : Base(inst), 
    HasSrcDest(inst),
    HasSrc1(inst),
    HasSrc2(inst),
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
        return instruction;
    }


    CTRLFormatInstruction::CTRLFormatInstruction(const Instruction& inst) : Base(inst), 
    _displacement(decode<Ordinal, Integer, 0x00FFFFFC, 0>(inst.getLowerHalf())),
    _t(decode<Ordinal, bool, 0b10>(inst.getLowerHalf())),
    _target(Operation::translate(inst.getOpcode(), Operation::CTRLClass())) { }

    EncodedInstruction
    CTRLFormatInstruction::encode() const noexcept {
        // mask out the least significant bit to follow the ctrl format
        return i960::encode<Ordinal, Integer, 0x00FFFFFC, 0>(
                i960::encode<Ordinal, bool, 0b10, 1>(encodeMajorOpcode(0, getOpcode()), _t),
                _displacement) & 0xFFFF'FFFE;
    }
    COBRFormatInstruction::COBRFormatInstruction(const Instruction& inst) : Base(inst),
    Flags(inst),
    HasSrcDest(decodeSrcDest<0b100>(inst.getLowerHalf(), Flags::getValue())),
    HasSrc2(inst),
    _displacement(i960::decode<Ordinal, Integer, 0b1111'1111'1100, 0>(inst.getLowerHalf())), // we want to make a 12-bit number out of this
    _target(Operation::translate(inst.getOpcode(), Operation::COBRClass()))
    { }

    EncodedInstruction
    COBRFormatInstruction::encode() const noexcept {
        auto instruction = encodeMajorOpcode(0, getOpcode());
        instruction = encodeSrcDest(instruction, getSrc1());
        instruction = encodeSrc2(instruction, getSrc2());
        instruction = i960::encode<Ordinal, Integer, 0b1111'1111'1100, 0>(instruction, _displacement);
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
