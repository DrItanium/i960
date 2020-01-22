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
    class Instruction {
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
            /**
             * Extract the 8-bit opcode as a 16-bit opcode to make it common to
             * REG format instructions; The lower 4 bits will be zero unless it
             * is a reg format instruction; The upper most four bits will also 
             * be zero
             * @return the 16-bit opcode
             */
            constexpr OpcodeValue getStandardOpcode() const noexcept {
                return ((_enc >> 20) & 0x0FF0);
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
                return getStandardOpcode() | ((_enc >> 7) & 0xF);
            }
            template<OpcodeValue start, OpcodeValue finish>
            constexpr auto opcodeIsInRange() const noexcept {
                auto standardOpcode = getStandardOpcode();
                return (standardOpcode >= start) &&
                    (standardOpcode < finish);
            }
            constexpr auto isREGFormat() const noexcept {
                return opcodeIsInRange<0x580, 0x800>();
            }
            constexpr auto isCOBRFormat() const noexcept {
                return opcodeIsInRange<0x200, 0x400>();
            }
            constexpr auto isCTRLFormat() const noexcept {
                return getStandardOpcode() < 0x200;
            }
            constexpr auto isMEMFormat() const noexcept {
                return (getStandardOpcode() >= 0x800);
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
            constexpr auto getValue() const noexcept { return _flags; }
        private:
            ByteOrdinal _flags;
    };
    class REGFlags : public GenericFlags {
        public:
            using Parent = GenericFlags;
            static constexpr ByteOrdinal decode(SingleEncodedInstructionValue value) noexcept {
                auto lowerTwo = i960::decode<Ordinal, ByteOrdinal, 0b11'00000, 5>(value);
                auto upperThree = i960::decode<Ordinal, ByteOrdinal, 0b111'0000'00'00000, 8>(value);
                return lowerTwo | upperThree;
            }
        public:
            constexpr REGFlags(const Instruction& inst) noexcept : Parent(decode(inst.getLowerHalf())) { }
            constexpr bool getM1()  const noexcept { return getFlag<0b10000>(); }
            constexpr bool getM2()  const noexcept { return getFlag<0b01000>(); }
            constexpr bool getM3()  const noexcept { return getFlag<0b00100>(); }
            constexpr bool getSF1() const noexcept { return getFlag<0b00010>(); }
            constexpr bool getSF2() const noexcept { return getFlag<0b00001>(); }
            constexpr SingleEncodedInstructionValue encode(SingleEncodedInstructionValue value) const noexcept {
                auto lowerTwo = i960::encode<SingleEncodedInstructionValue, ByteOrdinal, 0b11'00000, 5>(value, getValue());
                auto upperThree = i960::encode<SingleEncodedInstructionValue, ByteOrdinal, 0b111'0000'00'00000, 8>(value, getValue());
                return lowerTwo | upperThree;
            }
    };
    class HasSrc1 {
        public:
            constexpr explicit HasSrc1(Operand op) : _src1(op) { }
            ~HasSrc1() = default;
            constexpr auto getSrc1() const noexcept { return _src1; }
            void setSrc1(Operand op) noexcept { _src1 = op; }
            /// @todo insert encode operation here
        private:
            Operand _src1;
    };
    class HasSrc2 {
        public:
            constexpr explicit HasSrc2(Operand op) : _src2(op) { }
            ~HasSrc2() = default;
            constexpr auto getSrc2() const noexcept { return _src2; }
            void setSrc2(Operand op) noexcept { _src2 = op; }
        private:
            Operand _src2;
    };
    class HasSrcDest {
        public:
            constexpr explicit HasSrcDest(Operand op) : _srcDest(op) { }
            ~HasSrcDest() = default;
            constexpr auto getSrcDest() const noexcept { return _srcDest; }
            void setSrcDest(Operand op) noexcept { _srcDest = op; }
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
            static constexpr ByteOrdinal decode(SingleEncodedInstructionValue value) noexcept {
                constexpr ShiftMaskPair<decltype(value)> Part1 { 0b1'0000'0000'0000 , 10 };
                constexpr ShiftMaskPair<decltype(value)> Part2 { 0b11, 0 };
                return i960::decode<SingleEncodedInstructionValue, ByteOrdinal, Part1>(value) |
                       i960::decode<SingleEncodedInstructionValue, ByteOrdinal, Part2>(value);
            }
        public:
            constexpr COBRFlags(const Instruction& inst) : Parent(decode(inst.getLowerHalf())) { }
            constexpr bool getM1() const noexcept { return getFlag<0b100>(); }
            constexpr bool getT()  const noexcept { return getFlag<0b010>(); }
            constexpr bool getS2() const noexcept { return getFlag<0b001>(); }
            constexpr SingleEncodedInstructionValue encode(SingleEncodedInstructionValue value) const noexcept {
                return i960::encode<Ordinal, ByteOrdinal, 0b1'0000'0000'0000, 10>(value, getValue()) |
                    i960::encode<Ordinal, ByteOrdinal, 0b11, 0>(value, getValue());
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
                AbsoluteOffset = 0b00'0000, // offset
                AbsoluteDisplacement = 0b11'0000, // displacement
                RegisterIndirect = 0b01'0000,
                RegisterIndirectWithOffset = 0b10'0000,
                RegisterIndirectWithDisplacement = 0b11'0100,
                RegisterIndirectWithIndex = 0b01'1100,
                RegisterIndirectWithIndexAndDisplacement = 0b11'1100,
                IndexWithDisplacement = 0b11'1000,
                IPWithDisplacement = 0b01'0100,
                Bad = 0xFF,
            };
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
        constexpr Ordinal majorOpcodeMask = 0xFF000000;
        return encode<Ordinal, HalfOrdinal, majorOpcodeMask, 24>(input, getMajorOpcode(opcode));
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
