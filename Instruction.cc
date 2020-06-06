#include "Instruction.h"

namespace i960 {
    struct ModeEncoderDecoder final {
        public:
            using SliceType = ByteOrdinal;
            static constexpr ByteOrdinal decode(Ordinal value) noexcept {
                // bits [10,13] are the entire mask.
                constexpr auto memABModeMask = 0b11'1100'0000'0000;
                if (auto typeSelect = i960::decode<Ordinal, ByteOrdinal, memABModeMask, 10>(value); (typeSelect & 0b0100) == 0) {
                    // MEMA format
                    return (typeSelect & 0b1100); // lower two bits are part of the offset field
                } else {
                    return typeSelect;
                }
            }
            static constexpr Ordinal encode(Ordinal value, ByteOrdinal mode) noexcept {
                if (MEMFormatInstruction::isMEMBFormat(mode)) {
                    constexpr Ordinal mask = 0b1111'000'00'00000;
                    return i960::encode<Ordinal, decltype(mode),  mask, 10>(value, mode);
                    // we really only have four bits to deal with in this case
                } else {
                    constexpr Ordinal mask = 0b11'000000'000000;
                    return i960::encode<Ordinal, decltype(mode), mask, 10>(value, mode);
                }
            }
        public:
            ModeEncoderDecoder() = delete;
            ~ModeEncoderDecoder() = delete;
            ModeEncoderDecoder(const ModeEncoderDecoder&) = delete;
            ModeEncoderDecoder(ModeEncoderDecoder&&) = delete;
            ModeEncoderDecoder& operator=(const ModeEncoderDecoder&) = delete;
            ModeEncoderDecoder& operator=(ModeEncoderDecoder&&) = delete;
    };
    constexpr ByteOrdinal decodeMode(Ordinal value) noexcept {
        return ModeEncoderDecoder::decode(value);
    }
    constexpr Ordinal encodeMode(Ordinal value, ByteOrdinal mode) noexcept {
        return ModeEncoderDecoder::encode(value, mode);
    }
    template<typename R = Operand>
    constexpr auto decodeSrcDest(Ordinal input) noexcept {
        using Decoder = BitFragment<decltype(input), R, HasSrcDest::EncoderDecoder::Mask, HasSrcDest::EncoderDecoder::Shift>;
        return Decoder::decodePattern(input);
    }
    template<Ordinal mask>
    constexpr auto decodeSrcDest(Ordinal input, Ordinal modeBits) noexcept {
        return Operand((modeBits & mask) != 0, decodeSrcDest<Ordinal>(input));
    }
    constexpr Ordinal encodeSrcDest(Ordinal value, Operand input) noexcept {
        constexpr OrdinalToByteOrdinalField<HasSrcDest::EncoderDecoder::Mask, HasSrcDest::EncoderDecoder::Shift> fragment;
        return fragment.encode(value, input.getValue());
    }
    constexpr auto encodeSrc2(Ordinal value, Operand input) noexcept {
        constexpr Ordinal Mask = HasSrc2::EncoderDecoder::Mask;
        constexpr Ordinal Shift = HasSrc2::EncoderDecoder::Shift;
        return encode<Ordinal, ByteOrdinal, Mask, Shift>(value, input.getValue());
    }

    constexpr auto encodeSrc1(Ordinal value, Operand input) noexcept {
        constexpr Ordinal Mask = 0x1F;
        return encode<Ordinal, ByteOrdinal, Mask, 0>(value, input.getValue());
    }

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
    static_assert(encodeSrcDest(0, 1_lr) == 0x00080000);
    static_assert(encodeSrc2(0, 1_lr) == (0x00080000 >> 5));
    static_assert(encodeSrc1(0, 1_lr) == 1);
    MEMFormatInstruction::MEMFormatInstruction(const Instruction& inst) : Base(inst), 
    HasSrcDest(inst),
    HasSrc1(inst),
    HasSrc2(inst),
    _mode(ModeEncoderDecoder::decode(inst.getLowerHalf())),
    _offset(MEMAOffsetPattern::decodePattern(inst.getLowerHalf())),
    _scale(MEMBScalePattern::decodePattern(inst.getLowerHalf())),
    _displacement(inst.getUpperHalf()),
    _target(Operation::translate(inst.getOpcode(), Operation::MEMClass())) { }

    EncodedInstruction
    MEMFormatInstruction::encode() const noexcept {
        template<typename ... Others>
        using PartialEncoder = i960::BinaryEncoderDecoder<SingleEncodedInstructionValue, 
              HasSrcDest::EncoderDecoder,
              ModeEncoderDecoder,
              HasSrc2::EncoderDecoder,
              Others...>;
        if (isMEMAFormat()) {
            using MEMAFormatEncoder = PartialEncoder<MEMAOffsetPattern>;
            return MEMAFormatEncoder::encode(encodeMajorOpcode(getOpcode()),
                    getSrcDest(),
                    getRawMode(),
                    getSrc2(), // abase
                    _offset);
        } else if (isMEMBFormat()) {
            using MEMBFormatEncoder = PartialEncoder<MEMBScalePattern, 
                  HasSrc1::EncoderDecoder,
                  BitPattern<SingleEncodedInstructionValue, SingleEncodedInstructionValue, 0b11'00000, 5>>; // zero out bits 5 and 6
            auto instruction = MEMBFormatEncoder::encode(
                    encodeMajorOpcode(getOpcode()),
                    getSrcDest(),
                    getRawMode(),
                    getSrc2(),
                    _scale, 
                    getIndex(),
                    0); // zero out bits 5 and 6
            if (isDoubleWide()) {
                // need to make the instruction double wide so the instruction
                // we've been encoding is the lower half
                LongOrdinal doubleWideInstruction = instruction;
                LongOrdinal doubleWideDisplacement = _displacement;
                doubleWideDisplacement <<= 32; // after sign extension has taken place then move displacement up into the upper half 
                return doubleWideDisplacement | doubleWideInstruction;
            } else {
                return instruction;
            }
        } else {
            return static_cast<Ordinal>(0xFFFF'FFFF);
        }
    }

    CTRLFormatInstruction::CTRLFormatInstruction(const Instruction& inst) : Base(inst), 
    _displacement(CTRLFormatInstruction::DisplacementPattern::decodePattern(inst.getLowerHalf())),
    _t(CTRLFormatInstruction::TPattern::decodePattern(inst.getLowerHalf())),
    _target(Operation::translate(inst.getOpcode(), Operation::CTRLClass())) { }

    EncodedInstruction
    CTRLFormatInstruction::encode() const noexcept {
        // mask out the least significant bit to follow the ctrl format
        return CTRLFormatInstruction::DisplacementPattern::encodePattern(
                CTRLFormatInstruction::TPattern::encodePattern(encodeMajorOpcode(0, getOpcode()), _t), _displacement) & 0xFFFF'FFFE;
    }
    using COBRDisplacementPattern = BitPattern<Ordinal, Integer, 0b1111'1111'1100, 0>;
    COBRFormatInstruction::COBRFormatInstruction(const Instruction& inst) : Base(inst),
    Flags(inst),
    HasSrcDest(decodeSrcDest<0b100>(inst.getLowerHalf(), Flags::getValue())),
    HasSrc2(inst),
    _displacement(COBRDisplacementPattern::decodePattern(inst.getLowerHalf())), // we want to make a 12-bit number out of this
    _target(Operation::translate(inst.getOpcode(), Operation::COBRClass()))
    { }

    EncodedInstruction
    COBRFormatInstruction::encode() const noexcept {
        using PartialEncoderPattern = i960::BinaryEncoderDecoder<SingleEncodedInstructionValue,
              HasSrcDest::EncoderDecoder, 
              HasSrc2::EncoderDecoder,
              COBRDisplacementPattern>;
        return Flags::encode(PartialEncoderPattern::encode(encodeMajorOpcode(0, getOpcode()), getSrc1(), getSrc2(), _displacement));
    }

    REGFormatInstruction::REGFormatInstruction(const Instruction& inst) : Base(inst), 
    Flags(inst),
    HasSrcDest(decodeSrcDest<0b10000>(inst.getLowerHalf(), Flags::getValue())),
    HasSrc2(decodeSrc2<0b01000>(inst.getLowerHalf(), Flags::getValue())),
    HasSrc1(decodeSrc1<0b00100>(inst.getLowerHalf(), Flags::getValue())),
    _target(Operation::translate(inst.getOpcode(), Operation::REGClass())) { }

    EncodedInstruction
    REGFormatInstruction::encode() const noexcept {
        using PartialEncoderPattern = 
            i960::BinaryEncoderDecoder<SingleEncodedInstructionValue,
            HasSrcDest::EncoderDecoder,
            HasSrc2::EncoderDecoder,
            HasSrc1::EncoderDecoder>;
        return Flags::encode(PartialEncoderPattern::encode(encodeFullOpcode(0, getOpcode()), getSrcDest(), getSrc2(), getSrc1()));
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
