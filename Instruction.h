#ifndef I960_INSTRUCTION_H__
#define I960_INSTRUCTION_H__
#include "types.h"
#include "Operand.h"
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
    using DecodedInstruction = std::variant<REGFormatInstruction, MEMFormatInstruction, COBRFormatInstruction, CTRLFormatInstruction>;
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
            constexpr auto isREGFormat() const noexcept {
                auto standardOpcode = getStandardOpcode();
                return (standardOpcode >= 0x580) &&
                       (standardOpcode < 0x800);
            }
            constexpr auto isCOBRFormat() const noexcept {
                auto standardOpcode = getStandardOpcode();
                return (standardOpcode >= 0x200) &&
                       (standardOpcode < 0x400);
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

            DecodedInstruction decode();

        private:
            SingleEncodedInstructionValue _enc, 
                                          _second;

    };
    class GenericFormatInstruction {
        public:
            GenericFormatInstruction(const Instruction& inst) : _opcode(inst.getOpcode()) { }
            virtual ~GenericFormatInstruction() = default;
            constexpr auto getOpcode() const noexcept { return _opcode; }
            inline void setOpcode(OpcodeValue opcode) noexcept { _opcode = opcode; }
            virtual EncodedInstruction encode() const noexcept = 0;
        private:
            OpcodeValue _opcode;
    };
    class REGFormatInstruction : public GenericFormatInstruction {
        public:
            using Base = GenericFormatInstruction;
        public:
            REGFormatInstruction(const Instruction& inst);
            ~REGFormatInstruction() override = default;
            constexpr auto getSrc1() const noexcept { return _src1; }
            constexpr auto getSrc2() const noexcept { return _src2; }
            constexpr auto getSrcDest() const noexcept { return _srcDest; }
            constexpr auto getBitPos() const noexcept { return _src1.getValue(); }
            constexpr bool getM1()  const noexcept { return _flags & 0b10000; }
            constexpr bool getM2()  const noexcept { return _flags & 0b01000; }
            constexpr bool getM3()  const noexcept { return _flags & 0b00100; }
            constexpr bool getSF1() const noexcept { return _flags & 0b00010; }
            constexpr bool getSF2() const noexcept { return _flags & 0b00001; }
            /// @todo implement set
            EncodedInstruction encode() const noexcept override;
        private:
            Operand _srcDest;
            Operand _src2;
            Operand _src1;
            /**
             * Comprised of the following flags:
             * @code
             * bool _m1; // bit 4
             * bool _m2; // bit 3
             * bool _m3; // bit 2
             * bool _s2; // bit 1
             * bool _s1; // bit 0
             * @endcode
             */
            ByteOrdinal _flags;
            ByteOrdinal _bitpos;
    };
    class COBRFormatInstruction : public GenericFormatInstruction {
        public:
            using Base = GenericFormatInstruction;
        public:
            COBRFormatInstruction(const Instruction&);
            ~COBRFormatInstruction() override = default;
            constexpr auto getSrc1() const noexcept { return _source1; }
            constexpr auto getSrc2() const noexcept { return _source2; }
            constexpr auto getDisplacement() const noexcept { return _displacement; }
            constexpr bool getM1() const noexcept { return _flags & 0b100; }
            constexpr bool getT()  const noexcept { return _flags & 0b010; }
            constexpr bool getS2() const noexcept { return _flags & 0b001; }
            constexpr auto getBitPos() const noexcept { return _source1.getValue(); }
            EncodedInstruction encode() const noexcept override;
        private:
            Operand _source1;
            Operand _source2;
            Ordinal _displacement : 10;
            /// These are the flags found within _flags
            /// @code
            /// bool _m1; // bit 2
            /// bool _t;  // bit 1
            /// bool _s2; // bit 0
            /// @endcode
            ByteOrdinal _flags;
    };
    class CTRLFormatInstruction : public GenericFormatInstruction {
        public:
            using Base = GenericFormatInstruction;
        public:
            CTRLFormatInstruction(const Instruction&);
            ~CTRLFormatInstruction() override = default;
            constexpr auto getDisplacement() const noexcept { return _displacement; }
            constexpr auto getT() const noexcept { return _t; }
            EncodedInstruction encode() const noexcept override;
        private:
            Integer _displacement : 22;
            bool _t;
    };
    /**
     * Generic super type for MEM class instructions
     */
    class MEMFormatInstruction : public GenericFormatInstruction {
        public:
            using Base = GenericFormatInstruction;
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
                // MEMA Effective Address kinds
                Offset = 0b00'0000,
                Abase_Plus_Offset = 0b10'0000,
                // MEMB Effective Address kinds
                Abase = 0b010000,
                IP_Plus_Displacement_Plus_8 = 0b010100,
                Abase_Plus_Index_Times_2_Pow_Scale = 0b011100,
                Displacement = 0b110000,
                Abase_Plus_Displacement = 0b110100,
                Index_Times_2_Pow_Scale_Plus_Displacement = 0b111000,
                Abase_Plus_Index_Times_2_Pow_Scale_Plus_Displacement = 0b111100,
                Bad = 0xFF,
            };
        public:
            MEMFormatInstruction(const Instruction& inst);
            ~MEMFormatInstruction() override = default;
            constexpr auto getSrcDest() const noexcept { return _srcDest; }
            constexpr auto getAbase() const noexcept { return _abase; }
            constexpr auto getRawMode() const noexcept { return _mode; }
            constexpr auto getMode() const noexcept { 
                auto mode = static_cast<AddressingModes>(_mode);
                switch (mode) {
                    case AddressingModes::Offset:
                    case AddressingModes::Abase_Plus_Offset:
                    case AddressingModes::Abase:
                    case AddressingModes::IP_Plus_Displacement_Plus_8:
                    case AddressingModes::Abase_Plus_Index_Times_2_Pow_Scale:
                    case AddressingModes::Displacement:
                    case AddressingModes::Abase_Plus_Displacement:
                    case AddressingModes::Index_Times_2_Pow_Scale_Plus_Displacement:
                    case AddressingModes::Abase_Plus_Index_Times_2_Pow_Scale_Plus_Displacement:
                        return mode;
                    default:
                        // Disallow modes that are not explicitly defined by the enumeration.
                        return AddressingModes::Bad;
                }
            }
            constexpr auto isOffsetAddressingMode() const noexcept { return getMode() == AddressingModes::Offset; }
            EncodedInstruction encode() const noexcept override;
            constexpr auto getOffset() const noexcept { return _offset; }
            constexpr auto getScale() const noexcept { return _scale; }
            constexpr auto getIndex() const noexcept { return _index; }
            constexpr auto getDisplacement() const noexcept { return _displacement; }
        private:
            Operand _srcDest;
            ByteOrdinal _abase;
            /// @todo convert this to be part of the code structure instead of wasting space
            ByteOrdinal _mode;
            // MEMA Specific Fields 
            Ordinal _offset : 12;
            // MEMB Specific Fields
            ByteOrdinal _scale;
            ByteOrdinal _index;
            Ordinal _displacement;
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
#define X(name, code, kind) \
        class kind ## name ## Operation final { };
#define reg(name, code, __) X(name, code, REG)
#define mem(name, code, __) X(name, code, MEM)
#define cobr(name, code, __) X(name, code, COBR)
#define ctrl(name, code, __) X(name, code, CTRL)
#include "opcodes.def"
#undef X
#undef reg
#undef mem
#undef cobr
#undef ctrl
    using REGOpcodes = std::variant<std::monostate
#define X(name, code, kind) \
        , kind ## name ## Operation
#define reg(name, code, __) X(name, code, REG)
#define mem(name, code, __) // X(name, code, MEM)
#define cobr(name, code, __) // X(name, code, COBR)
#define ctrl(name, code, __) // X(name, code, CTRL)
#include "opcodes.def"
#undef X
#undef reg
#undef mem
#undef cobr
#undef ctrl
    >;
    using MEMOpcodes = std::variant<std::monostate
#define X(name, code, kind) \
        , kind ## name ## Operation
#define reg(name, code, __) //X(name, code, REG)
#define mem(name, code, __) X(name, code, MEM)
#define cobr(name, code, __) // X(name, code, COBR)
#define ctrl(name, code, __) // X(name, code, CTRL)
#include "opcodes.def"
#undef X
#undef reg
#undef mem
#undef cobr
#undef ctrl
    >;
    using COBROpcodes = std::variant<std::monostate
#define X(name, code, kind) \
        , kind ## name ## Operation
#define reg(name, code, __) //X(name, code, REG)
#define mem(name, code, __) //X(name, code, MEM)
#define cobr(name, code, __) X(name, code, COBR)
#define ctrl(name, code, __) // X(name, code, CTRL)
#include "opcodes.def"
#undef X
#undef reg
#undef mem
#undef cobr
#undef ctrl
    >;
    using CTRLOpcodes = std::variant<std::monostate
#define X(name, code, kind) \
        , kind ## name ## Operation
#define reg(name, code, __) //X(name, code, REG)
#define mem(name, code, __) //X(name, code, MEM)
#define cobr(name, code, __) // X(name, code, COBR)
#define ctrl(name, code, __) X(name, code, CTRL)
#include "opcodes.def"
#undef X
#undef reg
#undef mem
#undef cobr
#undef ctrl
    >;

} // end namespace i960
#endif // end I960_INSTRUCTION_H__
