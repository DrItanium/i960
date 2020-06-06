#ifndef I960_INSTRUCTION_H__
#define I960_INSTRUCTION_H__
#include "types.h"
#include "Operand.h"
#include "opcodes.h"
#include <variant>
#include <optional>
namespace i960 {
    /// @todo implement a flags class that makes extracting bits pretty easy
    using SingleEncodedInstructionValue = Ordinal;
    using DoubleEncodedInstructionValue = LongOrdinal;
    using EncodedInstruction = std::variant<SingleEncodedInstructionValue, DoubleEncodedInstructionValue>;
    class REGFormatInstruction;
    class MEMFormatInstruction;
    class COBRFormatInstruction;
    class CTRLFormatInstruction;
    using DecodedInstruction = std::variant<std::monostate, REGFormatInstruction, MEMFormatInstruction, COBRFormatInstruction, CTRLFormatInstruction>;
    using OpcodeNumericRange = std::tuple<OpcodeValue, OpcodeValue>;
    class Instruction {
        public:
            static constexpr OpcodeNumericRange REGFormat { 0x580, 0x800 };
            static constexpr OpcodeNumericRange COBRFormat { 0x200, 0x400 };
            static constexpr OpcodeNumericRange CTRLFormat { 0x000, 0x200 };
            static constexpr OpcodeNumericRange MEMFormat { 0x800, 0x1000 };
            using StandardOpcodeManipulator = OrdinalBitPattern<OpcodeValue, 0xFF00'0000, 20>;
            using ExtendedOpcodeManipulator = OrdinalBitPattern<OpcodeValue, 0b11110000000, 7>;
            using InstructionToOpcode = BinaryEncoderDecoder<SingleEncodedInstructionValue, StandardOpcodeManipulator, ExtendedOpcodeManipulator>;
            using OpcodeValueStandardOpcode = SameWidthFragment<OpcodeValue, 0xFF0>;
            using OpcodeValueExtendedOpcode = SameWidthFragment<OpcodeValue, 0x00F>;
            using OpcodeValueGenerator = BinaryEncoderDecoder<OpcodeValue, OpcodeValueStandardOpcode, OpcodeValueExtendedOpcode>;
        public:
            /**
             * Construct a 64-bit opcode, the second EncodedInstructionValue may not
             * be used depending on the instruction.
             * @param first The first 32-bits
             * @param second The second 32-bits (not used in all cases)
             */
            constexpr Instruction(SingleEncodedInstructionValue first, 
                    SingleEncodedInstructionValue second) : _enc(first), _second(second) { }
            /**
             * Construct a 32-bit only opcode as though it is a 64-bit opcode
             * (upper 32-bits are set to zero)
             * @param first the lower 32-bits
             */
            constexpr explicit Instruction(SingleEncodedInstructionValue first) : Instruction(first, 0) { }
            constexpr explicit Instruction(DoubleEncodedInstructionValue value) : 
                Instruction(static_cast<SingleEncodedInstructionValue>(value), 
                            static_cast<SingleEncodedInstructionValue>(value >> 32)) { }
            ~Instruction() = default;
            constexpr auto getLowerHalf() const noexcept { return _enc; }
            constexpr auto getUpperHalf() const noexcept { return _second; }
            constexpr auto getOpcodeComponents() const noexcept {
                return InstructionToOpcode::decode(_enc);
            }
            /**
             * Extract the 8-bit opcode as a 16-bit opcode to make it common to
             * REG format instructions; The lower 4 bits will be zero unless it
             * is a reg format instruction; The upper most four bits will also 
             * be zero
             * @return the 16-bit opcode
             */
            constexpr OpcodeValue getStandardOpcode() const noexcept {
                return StandardOpcodeManipulator::decodePattern(_enc);
            }
            constexpr OpcodeValue getExtendedOpcode() const noexcept {
                // according to the documents, reg format opcodes 
                // are made up of 
                // Opcode [0,3] -> bits [7,10]
                // Opcode [4,11] -> bits [24, 31]
                // thus we get a 12-bit opcode out of it
                // (opcode 11-4), (src/dest), (src2), (mode), (opcode 3-0),
                // (special flags), (src1)
                //  we want to shift the lower four bits to become the lowest four bits
                //  so 58:0 becomes 0x0580 and 58:C -> 0x058C
                return OpcodeValueGenerator::encode(getStandardOpcode(), ExtendedOpcodeManipulator::decodePattern(_enc));

            }
            template<OpcodeNumericRange range>
            constexpr auto opcodeIsInRange() const noexcept {
                auto standardOpcode = getStandardOpcode();
                return (standardOpcode >= std::get<0>(range)) && (standardOpcode < std::get<1>(range));
            }
            constexpr auto isREGFormat() const noexcept {
                //return opcodeIsInRange<0x580, 0x800>();
                return opcodeIsInRange<REGFormat>();
            }
            constexpr auto isCOBRFormat() const noexcept {
                //return opcodeIsInRange<0x200, 0x400>();
                return opcodeIsInRange<COBRFormat>();
            }
            constexpr auto isCTRLFormat() const noexcept {
                //return getStandardOpcode() < 0x200;
                return opcodeIsInRange<CTRLFormat>();
            }
            constexpr auto isMEMFormat() const noexcept {
                //return (getStandardOpcode() >= 0x800);
                return opcodeIsInRange<MEMFormat>();
            }
            constexpr auto getOpcode() const noexcept {
                if (isREGFormat()) {
                    return getExtendedOpcode();
                } else {
                    return getStandardOpcode();
                }
            }

            DecodedInstruction decode() const noexcept;

        private:
            SingleEncodedInstructionValue _enc, 
                                          _second;

    };
    class GenericFormatInstruction {
        public:
            constexpr GenericFormatInstruction(const Instruction& inst) : _opcode(inst.getOpcode()) { }
            virtual ~GenericFormatInstruction() = default;
            constexpr auto getOpcode() const noexcept { return _opcode; }
            inline void setOpcode(OpcodeValue opcode) noexcept { _opcode = opcode; }
            virtual EncodedInstruction encode() const noexcept = 0;
        private:
            OpcodeValue _opcode;
    };
    class GenericFlags {
        public:
            constexpr GenericFlags(ByteOrdinal flags) noexcept : _flags(flags) { }
            template<ByteOrdinal mask = 0xFF>
            constexpr auto getFlag() const noexcept { return (_flags & mask) != 0; }
            template<typename Pattern>
            constexpr auto getFlag() const noexcept { return Pattern::decodePattern(_flags) != 0; }
            template<uint8_t position>
            constexpr auto getFlagAtPosition() const noexcept { return ByteOrdinalFlagPattern<position>::decodePattern(_flags); }
            constexpr auto getValue() const noexcept { return _flags; }
        private:
            ByteOrdinal _flags;
    };

    class REGFlags : public GenericFlags {
        public:
            using Parent = GenericFlags;
            using LowerTwoFlagBits = BitPattern<SingleEncodedInstructionValue, ByteOrdinal, 0b11'00000, 5>;
            using UpperThreeFlagBits = BitPattern<SingleEncodedInstructionValue, ByteOrdinal, 0b111'0000'00'00000 , 9 >;
            using FlagsFromInstruction = BinaryEncoderDecoder<SingleEncodedInstructionValue, LowerTwoFlagBits, UpperThreeFlagBits>;
            static constexpr ByteOrdinal decode(SingleEncodedInstructionValue value) noexcept {
                auto tup = FlagsFromInstruction::decode(value);
                return std::get<0>(tup) | std::get<1>(tup);
            }
            static_assert(LowerTwoFlagBits::encodePattern(0b11) == 0b11'00000);
            static_assert(UpperThreeFlagBits::encodePattern(0b11100) == 0b111'0000'00'00000);
            static_assert((static_cast<Ordinal>(0b11100) << 9) == (static_cast<Ordinal>(0b111'0000'00'00000)));
            static_assert(FlagsFromInstruction::encode(0, 0b11, 0b11100) == 0b111'0000'11'00000);
        public:
            constexpr REGFlags(const Instruction& inst) noexcept : Parent(decode(inst.getLowerHalf())) { }
            constexpr bool getM1()  const noexcept { return getFlagAtPosition<4>(); }
            constexpr bool getM2()  const noexcept { return getFlagAtPosition<3>(); }
            constexpr bool getM3()  const noexcept { return getFlagAtPosition<2>(); }
            constexpr bool getSF1() const noexcept { return getFlagAtPosition<1>(); }
            constexpr bool getSF2() const noexcept { return getFlagAtPosition<0>(); }
            constexpr SingleEncodedInstructionValue encode(SingleEncodedInstructionValue value) const noexcept {
                return FlagsFromInstruction::encode(value, getValue(), getValue());
            }
    };
    class HasSrc1 {
        public:
            using EncoderDecoder = BitPattern<SingleEncodedInstructionValue, Ordinal, 0x1F, 0>;
        public:
            constexpr explicit HasSrc1(Operand op) : _src1(op) { }
            constexpr explicit HasSrc1(const Instruction& inst) : _src1(EncoderDecoder::decodePattern(inst.getLowerHalf())) { }
            ~HasSrc1() = default;
            constexpr auto getSrc1() const noexcept { return _src1; }
            void setSrc1(Operand op) noexcept { _src1 = op; }
            constexpr SingleEncodedInstructionValue encode(SingleEncodedInstructionValue value) const noexcept {
                return EncoderDecoder::encodePattern(value, _src1);
            }
            /// @todo insert encode operation here
        private:
            Operand _src1;
    };
    class HasSrc2 {
        // constexpr Ordinal Mask = 0x0007C000;
        // constexpr Ordinal Shift = 14;
        public:
            template<typename R = Ordinal>
            using GenericEncoderDecoder = BitPattern<SingleEncodedInstructionValue, R, 0x000'7C'000, 14>;
            using EncoderDecoder = GenericEncoderDecoder<Ordinal>;
        public:
            constexpr explicit HasSrc2(Operand op) : _src2(op) { }
            constexpr explicit HasSrc2(const Instruction& inst) : _src2(EncoderDecoder::decodePattern(inst.getLowerHalf())) { }
            ~HasSrc2() = default;
            constexpr auto getSrc2() const noexcept { return _src2; }
            void setSrc2(Operand op) noexcept { _src2 = op; }
            constexpr SingleEncodedInstructionValue encode(SingleEncodedInstructionValue value) const noexcept {
                return EncoderDecoder::encodePattern(value, _src2);
            }
        private:
            Operand _src2;
    };
    class HasSrcDest {
        public:
            using EncoderDecoder = BitPattern<SingleEncodedInstructionValue, Ordinal, 0x00F80000, 19>;
        public:
            constexpr explicit HasSrcDest(Operand op) : _srcDest(op) { }
            constexpr explicit HasSrcDest(const Instruction& inst) : _srcDest(EncoderDecoder::decodePattern(inst.getLowerHalf())) { }
            ~HasSrcDest() = default;
            constexpr auto getSrcDest() const noexcept { return _srcDest; }
            void setSrcDest(Operand op) noexcept { _srcDest = op; }
            constexpr SingleEncodedInstructionValue encode(SingleEncodedInstructionValue value) const noexcept {
                return EncoderDecoder::encodePattern(value, _srcDest);
            }
        private:
            Operand _srcDest;
    };
    class REGFormatInstruction : public GenericFormatInstruction, public REGFlags, public HasSrcDest, public HasSrc2, public HasSrc1 {
        public:
            using Base = GenericFormatInstruction;
            using OpcodeList = Operation::REG;
            using Flags = REGFlags;
        public:
            REGFormatInstruction(const Instruction& inst);
            ~REGFormatInstruction() override = default;
            constexpr auto getBitPos() const noexcept { return getSrc1().getValue(); }
            /// @todo implement setters
            EncodedInstruction encode() const noexcept override;
            const auto& getTarget() const noexcept { return _target; }
        private:
            OpcodeList _target;
    };
    class COBRFlags : public GenericFlags {
        public:
            using Parent = GenericFlags;
            static constexpr BitFragment<SingleEncodedInstructionValue, ByteOrdinal, 0b1'0000'0000'0000, 10> Part1{};
            static constexpr BitFragment<SingleEncodedInstructionValue, ByteOrdinal, 0b11, 0> Part2{};
            static constexpr ByteOrdinal decode(SingleEncodedInstructionValue value) noexcept {
                return Part1.decode(value) | Part2.decode(value);
            }
        public:
            constexpr COBRFlags(const Instruction& inst) : Parent(decode(inst.getLowerHalf())) { }
            constexpr bool getM1() const noexcept { return getFlag<0b100>(); }
            constexpr bool getT()  const noexcept { return getFlag<0b010>(); }
            constexpr bool getS2() const noexcept { return getFlag<0b001>(); }
            constexpr SingleEncodedInstructionValue encode(SingleEncodedInstructionValue value) const noexcept {
                return Part1.encode(value, getValue()) | Part2.encode(value, getValue());
            }
    };
    class COBRFormatInstruction : public GenericFormatInstruction, public COBRFlags, public HasSrcDest, public HasSrc2 {
        public:
            using Base = GenericFormatInstruction;
            using OpcodeList = Operation::COBR;
            using Flags = COBRFlags;
        public:
            COBRFormatInstruction(const Instruction&);
            ~COBRFormatInstruction() override = default;
            constexpr auto getSrc1() const noexcept { return getSrcDest(); }
            constexpr auto getDisplacement() const noexcept { return _displacement; }
            constexpr auto getBitPos() const noexcept { return getSrc1().getValue(); }
            EncodedInstruction encode() const noexcept override;
        private:
            // meant to be a signed 12-bit value with the lowest two bits always zero
            Integer _displacement : 12;
        public:
            const auto& getTarget() const noexcept { return _target; }
        private:
            OpcodeList _target;
    };
    class CTRLFormatInstruction : public GenericFormatInstruction {
        public:
            using Base = GenericFormatInstruction;
            using OpcodeList = Operation::CTRL;
            using DisplacementPattern = BitPattern<Ordinal, Integer, 0x00FFFFFC, 0>;
            using TPattern = BitPattern<Ordinal, bool, 0b10, 0>;
        public:
            CTRLFormatInstruction(const Instruction&);
            ~CTRLFormatInstruction() override = default;
            constexpr auto getDisplacement() const noexcept { return _displacement; }
            constexpr auto getT() const noexcept { return _t; }
            EncodedInstruction encode() const noexcept override;
        private:
            // signed 24-bit value since the lowest two bits are always zero
            Integer _displacement : 24;
            bool _t;
        public:
            const auto& getTarget() const noexcept { return _target; }
        private:
            OpcodeList _target;
    };
    /**
     * Generic super type for MEM class instructions
     */
    class MEMFormatInstruction : public GenericFormatInstruction, public HasSrcDest, public HasSrc1, public HasSrc2 {
        public:
            using Base = GenericFormatInstruction;
            using OpcodeList = Operation::MEM;
            /** 
             * A 6-bit code that describes what addressing mode to perform. The
             * common upper two bits are used internally to construct the raw mode
             * value correctly. The lower four bits are actually specific to MEMB type
             * instructions with the lowest two bits made up of bits 6,5 in a MEMB
             * instruction as a sanity check (according to the manuals they should always
             * be zero). In reality the mode should be a 4-bit code but expanding to 6-bits
             * solves quite a few problems.
             */
            enum class AddressingModes : ByteOrdinal {
                // MEMA Modes
                AbsoluteOffset = 0b0000, // offset
                RegisterIndirectWithOffset = 0b1000,
                // MEMB Modes
                RegisterIndirect = 0b0100,
                AbsoluteDisplacement = 0b1100,
                RegisterIndirectWithDisplacement = 0b1101,
                RegisterIndirectWithIndex = 0b0111,
                RegisterIndirectWithIndexAndDisplacement = 0b1111,
                IndexWithDisplacement = 0b1110,
                IPWithDisplacement = 0b0101,
                Bad = 0xFF,
            };
            static constexpr auto isMEMAFormat(AddressingModes mode) noexcept {
                switch (mode) {
                    case AddressingModes::AbsoluteOffset:
                    case AddressingModes::RegisterIndirectWithOffset:
                        return true;
                    default:
                        return false;
                }
            }
            static constexpr auto isMEMAFormat(ByteOrdinal mode) noexcept {
                return isMEMAFormat(static_cast<AddressingModes>(mode));
            }
            static constexpr auto isMEMBFormat(AddressingModes mode) noexcept {
                switch (mode) {
                    case AddressingModes::RegisterIndirect:
                    case AddressingModes::AbsoluteDisplacement:
                    case AddressingModes::RegisterIndirectWithDisplacement:
                    case AddressingModes::RegisterIndirectWithIndex:
                    case AddressingModes::RegisterIndirectWithIndexAndDisplacement:
                    case AddressingModes::IndexWithDisplacement:
                    case AddressingModes::IPWithDisplacement:
                        return true;
                    default:
                        return false;
                }
            }
            static constexpr auto isMEMBFormat(ByteOrdinal mode) noexcept {
                return isMEMBFormat(static_cast<AddressingModes>(mode));
            }
        public:
            //instruction = i960::encode<Ordinal, Ordinal, 0b1111'1111'1111, 0>(instruction, _offset);
            using MEMAOffsetPattern = BitPattern<Ordinal, Ordinal, 0b1111'1111'1111, 0>;
            using MEMBScalePattern = BitPattern<Ordinal, ByteOrdinal, 0x380, 7>;
        public:
            MEMFormatInstruction(const Instruction& inst);
            ~MEMFormatInstruction() override = default;
            constexpr auto getAbase() const noexcept { return getSrc2(); }
            constexpr auto getRawMode() const noexcept { return _mode; }
            constexpr auto getMode() const noexcept { 
                auto mode = static_cast<AddressingModes>(_mode);
                switch (mode) {
                    case AddressingModes::AbsoluteOffset:
                    case AddressingModes::AbsoluteDisplacement:
                    case AddressingModes::RegisterIndirect:
                    case AddressingModes::RegisterIndirectWithOffset:
                    case AddressingModes::RegisterIndirectWithDisplacement:
                    case AddressingModes::RegisterIndirectWithIndex:
                    case AddressingModes::RegisterIndirectWithIndexAndDisplacement:
                    case AddressingModes::IndexWithDisplacement:
                    case AddressingModes::IPWithDisplacement:
                        return mode;
                    default:
                        // Disallow modes that are not explicitly defined by the enumeration.
                        return AddressingModes::Bad;
                }
            }
            constexpr auto isOffsetAddressingMode() const noexcept { return getMode() == AddressingModes::AbsoluteOffset; }
            EncodedInstruction encode() const noexcept override;
            constexpr auto getOffset() const noexcept { return _offset; }
            constexpr auto getScale() const noexcept { return _scale; }
            constexpr auto getIndex() const noexcept { return getSrc1(); }
            constexpr auto getDisplacement() const noexcept { return _displacement; }
            constexpr auto isDoubleWide() const noexcept { 
                switch (getMode()) {
                    case AddressingModes::AbsoluteOffset:
                    case AddressingModes::RegisterIndirect:
                    case AddressingModes::RegisterIndirectWithOffset:
                    case AddressingModes::RegisterIndirectWithIndex:
                        return false;
                    default:
                        return true;
                }
            }
            constexpr auto isMEMAFormat() const noexcept { return isMEMAFormat(_mode); }
            constexpr auto isMEMBFormat() const noexcept { return isMEMBFormat(_mode); }
            bool srcDestIsSrc() const noexcept;
            bool srcDestIsDest() const noexcept;
            bool srcDestIsUnused() const noexcept;
            /// @todo add effective address computation support
        private:
            /// @todo convert this to be part of the code structure instead of wasting space
            ByteOrdinal _mode;
            // MEMA Specific Fields 
            Ordinal _offset : 12;
            // MEMB Specific Fields
            ByteOrdinal _scale;
            Integer _displacement;
        public:
            const auto& getTarget() const noexcept { return _target; }
        private:
            OpcodeList _target;
    };
    constexpr Ordinal getMajorOpcode(HalfOrdinal ordinal) noexcept {
        return decode<HalfOrdinal, Ordinal, 0x0FF0, 4>(ordinal);
    }
    constexpr Ordinal encodeMajorOpcode(Ordinal input, HalfOrdinal opcode) noexcept {
        constexpr ShiftMaskPair<decltype(input)> MajorOpcode { 0xFF00'0000, 24 };
        return encode<Ordinal, HalfOrdinal, MajorOpcode>(input, getMajorOpcode(opcode));
    }
    constexpr Ordinal encodeMajorOpcode(HalfOrdinal opcode) noexcept {
        return encodeMajorOpcode(0, opcode);
    }
    constexpr Ordinal getMinorOpcode(HalfOrdinal ordinal) noexcept {
        return decode<Ordinal, Ordinal, 0x000F>(ordinal);
    }
    constexpr Ordinal encodeMinorOpcode(Ordinal input, HalfOrdinal opcode) noexcept {
        constexpr Ordinal minorOpcodeMask = 0b1111'00'00000;
        return encode<Ordinal, HalfOrdinal, minorOpcodeMask, 7>(input, getMinorOpcode(opcode));
    }
    constexpr Ordinal encodeFullOpcode(Ordinal input, HalfOrdinal opcode) noexcept {
        return encodeMajorOpcode(encodeMinorOpcode(input, opcode), opcode);
    }

} // end namespace i960
#endif // end I960_INSTRUCTION_H__
