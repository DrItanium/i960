#ifndef I960_INSTRUCTION_H__
#define I960_INSTRUCTION_H__
#include "types.h"
#include "Operand.h"
#include <variant>
namespace i960 {
    struct REGFormat {
        union {
            struct {
                Ordinal _source1 : 5;
                Ordinal _unused : 2;
                Ordinal _opcode2 : 4;
                Ordinal _m1 : 1;
                Ordinal _m2 : 1;
                Ordinal _m3 : 1;
                Ordinal _source2 : 5;
                Ordinal _src_dest : 5;
                Ordinal _opcode : 8;
            };
            Ordinal _raw;
        };
        explicit REGFormat(Ordinal value, Ordinal = 0) : _raw(value) { }
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
        constexpr auto getRawValue() const noexcept {
            return _raw;
        }
    };
    static_assert(sizeof(REGFormat) == 1_words, "RegFormat sizes is does not equal Ordinal's size!");
    struct COBRFormat {
        union {
            struct {
                Ordinal _unused : 2;
                Ordinal _displacement : 11;
                Ordinal _m1 : 1;
                Ordinal _source2 : 5; 
                Ordinal _source1 : 5;
                Ordinal _opcode : 8;
            };
            Ordinal _raw;
        };
        explicit COBRFormat(Ordinal value, Ordinal = 0) : _raw(value) { }
        constexpr auto getRawValue() const noexcept {
            return _raw;
        }
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
        union {
            struct {
                Ordinal _unused : 2;
                Ordinal _displacement : 22;
                Ordinal _opcode : 8;
            };
            Ordinal _raw;
        };
        explicit CTRLFormat(Ordinal value, Ordinal = 0) : _raw(value) { }
        void encodeDisplacement(Ordinal value) noexcept {
            _displacement = value;
        }
        constexpr auto decodeDisplacement() const noexcept {
            return _displacement;
        }
        constexpr auto getRawValue() const noexcept {
            return _raw;
        }
    };
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
        explicit MEMAFormat(Ordinal first, Ordinal = 0) : _raw(first) { }
        constexpr auto getRawValue() const noexcept { return _raw; }
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
        enum AddressingModes : uint8_t {
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
        explicit MEMBFormat(Ordinal first, Ordinal second = 0) : _raw(first), _second(second) { }
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
        constexpr auto getRawValue() const noexcept { return _raw; }
    };
    class Instruction {

        public:
            using TypeContainer = std::variant<REGFormat, COBRFormat, CTRLFormat, MEMAFormat, MEMBFormat>;
            static constexpr Ordinal getBaseOpcode(Ordinal value) noexcept {
                return (0xFF000000 & value) >> 24;
            }
            static constexpr bool isCompareAndBranchFormat(Ordinal value) noexcept {
                auto opcode = getBaseOpcode(value);
                return opcode >= 0x20 && opcode < 0x40;
            }
            static constexpr bool isMemFormat(Ordinal value) noexcept {
                return getBaseOpcode(value) >= 0x80;
            }
            static constexpr bool isRegFormat(Ordinal value) noexcept {
                // this is a little strange since the opcode is actually 12-bits
                // instead of 8 bits. Only use the 8bits anyway
                auto opcode = getBaseOpcode(value);
                return opcode >= 0x58 && opcode < 0x80;
            }
            static constexpr bool isControlFormat(Ordinal value) noexcept {
                return getBaseOpcode(value) < 0x20;
            }
            static TypeContainer deduceKind(Ordinal first, Ordinal second) {
                if (isMemFormat(first)) {
                    if (MEMAFormat tmp(first, second); tmp._unused == 0) {
                        return tmp;
                    } else {
                        return MEMBFormat(first, second);
                    }
                } else if (isRegFormat(first)) {
                    return REGFormat(first, second);
                } else if (isCompareAndBranchFormat(first)) {
                    return COBRFormat(first, second);
                } else if (isControlFormat(first)) {
                    return CTRLFormat(first, second);
                } else {
                    throw "Bad kind!";
                }
            }
        public:
            Instruction(Ordinal raw, Ordinal second = 0) : _storage(deduceKind(raw, second)) { }
            template<typename Visitor>
            constexpr decltype(auto) visit(Visitor&& v) {
                return std::visit(v, _storage);
            }
            template<typename Visitor>
            constexpr decltype(auto) visit(Visitor&& v) const {
                return std::visit(v, _storage);
            }
            constexpr bool isTwoOrdinalInstruction() const noexcept {
                return visit(overloaded{
                            [](auto&&) { return false; },
                            [](MEMBFormat&& value) { return value.has32bitDisplacement(); }
                            });
            }
            constexpr Ordinal getOpcode() const noexcept {
                return visit(overloaded{ 
                            [](REGFormat&& value) { return value.getOpcode(); },
                            [](auto&& value) { return getBaseOpcode(value.getRawValue()); }
                        });

            }
            constexpr auto getRawValue() const noexcept {
                return visit([](auto&& value) { return value.getRawValue(); });
            }
        private:
            TypeContainer _storage;

    };
    //static_assert(sizeof(Instruction) == 2_words, "Instruction must be 2 words wide!");

} // end namespace i960
#endif // end I960_INSTRUCTION_H__
