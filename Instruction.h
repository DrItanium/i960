#ifndef I960_INSTRUCTION_H__
#define I960_INSTRUCTION_H__
#include "types.h"
#include "Operand.h"
#include <variant>
namespace i960 {
    using RawEncodedInstruction = Ordinal;
    using RawDoubleEncodedInstruction = LongOrdinal;
    using EncodedInstruction = std::variant<RawEncodedInstruction, RawDoubleEncodedInstruction>;
    class DecodedInstruction {
        public:
            /**
             * Construct a 64-bit opcode, the second RawEncodedInstruction may not
             * be used depending on the instruction.
             * @param first The first 32-bits
             * @param second The second 32-bits (not used in all cases)
             */
            constexpr DecodedInstruction(RawEncodedInstruction first, RawEncodedInstruction second) : _enc(first), _second(second) { }
            /**
             * Construct a 32-bit only opcode as though it is a 64-bit opcode
             * (upper 32-bits are set to zero)
             * @param first the lower 32-bits
             */
            constexpr explicit DecodedInstruction(RawEncodedInstruction first) : DecodedInstruction(first, 0) { }
            constexpr explicit DecodedInstruction(RawDoubleEncodedInstruction value) : DecodedInstruction(RawEncodedInstruction(value), RawEncodedInstruction(value >> 32)) { }
            ~DecodedInstruction() = default;
            constexpr auto getLowerHalf() const noexcept { return _enc; }
            constexpr auto getUpperHalf() const noexcept { return _second; }
            /**
             * Extract the 8-bit opcode as a 16-bit opcode to make it common to
             * REG format instructions; The lower 4 bits will be zero unless it
             * is a reg format instruction; The upper most four bits will also 
             * be zero
             * @return the 16-bit opcode
             */
            constexpr HalfOrdinal getStandardOpcode() const noexcept {
                return ((_enc >> 20) & 0x0FF0);
            }
            constexpr HalfOrdinal getExtendedOpcode() const noexcept {
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
            constexpr auto isMemFormat() const noexcept {
                return (getStandardOpcode() >= 0x800);
            }
            constexpr Ordinal getOpcode() const noexcept {
                if (isREGFormat()) {
                    return getExtendedOpcode();
                } else {
                    return getStandardOpcode();
                }
            }
        private:
            RawEncodedInstruction _enc;
            RawEncodedInstruction _second;

    };
    class GenericFormatInstruction {
        public:
            GenericFormatInstruction(const DecodedInstruction& inst) : _opcode(inst.getOpcode()) { }
            virtual ~GenericFormatInstruction() = default;
            constexpr auto getOpcode() const noexcept { return _opcode; }
            inline void setOpcode(Ordinal opcode) noexcept { _opcode = opcode; }
            virtual EncodedInstruction encode() const noexcept = 0;
        private:
            Ordinal _opcode;
    };
    class REGFormatInstruction : public GenericFormatInstruction {
        public:
            using Base = GenericFormatInstruction;
        public:
            REGFormatInstruction(const DecodedInstruction& inst);
            ~REGFormatInstruction() override = default;
            constexpr auto getSrc1() const noexcept { return _src1; }
            void setSrc1(Operand src1) noexcept { _src1 = src1; }
            constexpr auto getSrc2() const noexcept { return _src2; }
            void setSrc2(Operand src2) noexcept { _src2 = src2; }
            constexpr auto getSrcDest() const noexcept { return _srcDest; }
            void setSrcDest(Operand src3) noexcept { _srcDest = src3; }
            constexpr auto getM1() const noexcept { return _m1; }
            constexpr auto getM2() const noexcept { return _m2; }
            constexpr auto getM3() const noexcept { return _m3; }
            void setM1(bool value) noexcept { _m1 = value; }
            void setM2(bool value) noexcept { _m2 = value; }
            void setM3(bool value) noexcept { _m3 = value; }
            constexpr auto getSF1() const noexcept { return _sf1; }
            constexpr auto getSF2() const noexcept { return _sf2; }
            void setSF1(bool value) noexcept { _sf1 = value; }
            void setSF2(bool value) noexcept { _sf2 = value; }
            EncodedInstruction encode() const noexcept override;
        private:
            Operand _src1;
            Operand _src2;
            Operand _srcDest;
            bool _m1, _m2, _m3;
            bool _sf1, _sf2;
    };
    class COBRFormatInstruction : public GenericFormatInstruction {
        public:
            using Base = GenericFormatInstruction;
        public:
            COBRFormatInstruction(const DecodedInstruction&);
            ~COBRFormatInstruction() override = default;
            constexpr auto getSource1() const noexcept { return _source1; }
            constexpr auto getSource2() const noexcept { return _source2; }
            constexpr auto getM1() const noexcept { return _m1; }
            constexpr auto getDisplacement() const noexcept { return _displacement; }
            constexpr auto getT() const noexcept { return _t; }
            constexpr auto getS2() const noexcept { return _s2; }
            /// @todo implement setters and encode operations
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
            CTRLFormatInstruction(const DecodedInstruction&);
            ~CTRLFormatInstruction() override = default;
            constexpr auto getDisplacement() const noexcept { return _displacement; }
            constexpr auto getT() const noexcept { return _t; }
            EncodedInstruction encode() const noexcept override;
        private:
            Ordinal _displacement;
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
            MEMFormatInstruction(const DecodedInstruction& inst);
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
            EncodedInstruction encode() const noexcept override;
            constexpr auto getOffset() const noexcept { return _offset; }
            constexpr auto getScale() const noexcept { return _scale; }
            constexpr auto getIndex() const noexcept { return _index; }
            constexpr auto getDisplacement() const noexcept { return _displacement; }
        private:
            Operand _srcDest;
            ByteOrdinal _abase;
            ByteOrdinal _mode;
            // MEMA Specific Fields 
            Ordinal _offset : 12;
            // MEMB Specific Fields
            ByteOrdinal _scale;
            ByteOrdinal _index;
            Ordinal _displacement;
    };

    union Instruction {
        struct REGFormat {
            Ordinal _source1 : 5;
            Ordinal _unused : 2;
            Ordinal _opcode2 : 4;
            Ordinal _m1 : 1;
            Ordinal _m2 : 1;
            Ordinal _m3 : 1;
            Ordinal _source2 : 5;
            Ordinal _src_dest : 5;
            Ordinal _opcode : 8;
            constexpr Ordinal getOpcode() const noexcept {
                return (_opcode << 4) | _opcode2;
            }
			void encodeSrc1(const Operand& operand) noexcept {
				_source1 = operand.getValue();
				_m1 = operand.isLiteral() ? 1 : 0;
			}
			constexpr auto decodeSrc1() const noexcept {
				return Operand(_m1, _source1);
			}
			void encodeSrc2(const Operand& operand) noexcept {
				_source2 = operand.getValue();
				_m2 = operand.isLiteral() ? 1 : 0;
			}
			constexpr auto decodeSrc2() const noexcept {
				return Operand(_m2, _source2);
			}
			void encodeSrcDest(const Operand& operand) noexcept {
				_src_dest = operand.getValue();
				_m3 = operand.isLiteral() ? 1 : 0;
			}
			constexpr auto decodeSrcDest() const noexcept {
				return Operand(_m3, _src_dest);
			}
        };
		static_assert(sizeof(REGFormat) == 1_words, "RegFormat sizes is does not equal Ordinal's size!");
        struct COBRFormat {
			Ordinal _unused : 2;
            Ordinal _displacement : 11;
            Ordinal _m1 : 1;
            Ordinal _source2 : 5; 
            Ordinal _source1 : 5;
            Ordinal _opcode : 8;
            constexpr auto src1IsLiteral() const noexcept { return _m1 != 0; }
			void encodeSrc1(const Operand& operand) noexcept {
				_source1 = operand.getValue();
				_m1 = operand.isLiteral() ? 1 : 0;
			}
			constexpr auto decodeSrc1() const noexcept {
				return Operand(_m1, _source1); 
			}
			void encodeSrc2(const Operand& operand) noexcept {
				// regardless if you give me a literal or a register, use the
				// value as is.
				_source2 = operand.getValue();
			}
			constexpr auto decodeSrc2() const noexcept {
				return Operand(0, _source2);
			}
			constexpr auto decodeDisplacement() const noexcept {
				return _displacement;
			}
        };
        struct CTRLFormat {
			Ordinal _unused : 2;
            Ordinal _displacement : 22;
            Ordinal _opcode : 8;
			void encodeDisplacement(Ordinal value) noexcept {
				_displacement = value;
			}
			constexpr auto decodeDisplacement() const noexcept {
				return _displacement;
			}
        };
        union MemFormat {
            struct MEMAFormat {
                enum AddressingModes {
                    Offset = 0,
                    Abase_Plus_Offset = 1,
                };
                union {
                    struct {
                        Ordinal _offset : 12;
                        Ordinal _unused : 1;
                        Ordinal _md : 1;
                        Ordinal _abase : 5;
                        Ordinal _src_dest : 5;
                        Ordinal _opcode : 8;
                    };
                    Ordinal _raw;
                };
                constexpr AddressingModes getAddressingMode() const noexcept {
                    return static_cast<AddressingModes>(_md);
                }
				constexpr bool isOffsetAddressingMode() const noexcept {
					return getAddressingMode() == AddressingModes::Offset;
				}
				constexpr auto decodeSrcDest() const noexcept {
					return Operand(0, _src_dest);
				}
				constexpr auto decodeAbase() const noexcept {
					return Operand(0, _abase);
				}
            };
            struct MEMBFormat {
                enum AddressingModes {
                    Abase = 0b0100,
                    IP_Plus_Displacement_Plus_8 = 0b0101,
                    Reserved = 0b0110,
                    Abase_Plus_Index_Times_2_Pow_Scale = 0b0111,
                    Displacement = 0b1100,
                    Abase_Plus_Displacement = 0b1101,
                    Index_Times_2_Pow_Scale_Plus_Displacement = 0b1110,
                    Abase_Plus_Index_Times_2_Pow_Scale_Plus_Displacement = 0b1111,
                };
                union {
                    struct {
                        Ordinal _index : 5;
                        Ordinal _unused : 2;
                        Ordinal _scale : 3;
                        Ordinal _mode : 4;
                        Ordinal _abase : 5;
                        Ordinal _src_dest : 5;
                        Ordinal _opcode : 8;
                    };
                    struct {
                        Ordinal _raw; // for decoding purposes
                        Ordinal _second;
                    };
                };
                constexpr AddressingModes getAddressingMode() const noexcept {
                    return static_cast<AddressingModes>(_mode);
                }
                constexpr bool has32bitDisplacement() const noexcept {
                    switch (getAddressingMode()) {
                        case AddressingModes::Abase:
                        case AddressingModes::Abase_Plus_Index_Times_2_Pow_Scale:
                        case AddressingModes::Reserved:
                            return false;
                        default:
                            return true;
                    }
                }
                ByteOrdinal getScaleFactor() const noexcept {
                    switch (_scale) {
                        case 0b000: 
                            return 1;
                        case 0b001:
                            return 2;
                        case 0b010:
                            return 4;
                        case 0b011:
                            return 8;
                        case 0b100: 
                            return 16;
                        default:
                            // TODO raise an invalid opcode fault instead
                            return 0; 
                    }
                }
				constexpr auto decodeSrcDest() const noexcept { return Operand(0, _src_dest); }
				constexpr auto decodeAbase() const noexcept   { return Operand(0, _abase); }
                constexpr auto get32bitDisplacement() const noexcept { return _second; }
            };
			constexpr auto decodeSrcDest() const noexcept {
				if (isMemAFormat()) {
					return _mema.decodeSrcDest();
				} else {
					return _memb.decodeSrcDest();
				}
			}
			constexpr Ordinal getOpcode() const noexcept {
				if (isMemAFormat()) {
					return _mema._opcode;
				} else {
					return _memb._opcode;
				}
			}
            constexpr bool isMemAFormat() const noexcept {
                return _mema._unused == 0;
            }
            MEMAFormat _mema;
            MEMBFormat _memb;
        };

        static_assert(sizeof(MemFormat) == 2_words, "MemFormat must be 2 words wide!");

        constexpr Instruction(Ordinal raw = 0, Ordinal second = 0) : _raw(raw), _second(second) { }
        constexpr Ordinal getBaseOpcode() const noexcept {
            return (0xFF000000 & _raw) >> 24;
        }
        constexpr Ordinal getOpcode() const noexcept {
            if (isRegFormat()) {
                return _reg.getOpcode();
            } else {
                return getBaseOpcode();
            }
        }
        constexpr bool isControlFormat() const noexcept {
            return getBaseOpcode() < 0x20;
        }
        constexpr bool isCompareAndBranchFormat() const noexcept {
            auto opcode = getBaseOpcode();
            return opcode >= 0x20 && opcode < 0x40;
        }
        constexpr bool isMemFormat() const noexcept {
            return getBaseOpcode() >= 0x80;
        }
        constexpr bool isRegFormat() const noexcept {
            // this is a little strange since the opcode is actually 12-bits
            // instead of 8 bits. Only use the 8bits anyway
            auto opcode = getBaseOpcode();
            return opcode >= 0x58 && opcode < 0x80;
        }
        constexpr bool isTwoOrdinalInstruction() const noexcept {
            return isMemFormat() && !_mem.isMemAFormat() && _mem._memb.has32bitDisplacement();
        }
        REGFormat _reg;
        COBRFormat _cobr;
        CTRLFormat _ctrl;
        MemFormat _mem;
        struct {
            Ordinal _raw;
            Ordinal _second;
        };

    } __attribute__((packed));
    static_assert(sizeof(Instruction) == 2_words, "Instruction must be 2 words wide!");

} // end namespace i960
#endif // end I960_INSTRUCTION_H__
