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
            struct {
                Ordinal _raw;
            };
        };
        explicit REGFormat(Ordinal value) : _raw(value) { }
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
    struct MemFormat {
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
            MEMBFormat(Ordinal first, Ordinal second = 0) : _raw(first), _second(second) { }
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
            return std::visit([](auto&& value) { return value.decodeSrcDest(); }, _storage);
        }
        constexpr Ordinal getOpcode() const noexcept {
            return std::visit([](auto&& value) { return value._opcode; }, _storage);
        }
        constexpr bool isMemAFormat() const noexcept {
            return std::holds_alternative<MEMAFormat>(_storage);
        }
        constexpr bool has32bitDisplacement() const noexcept {
            return std::visit([](auto&& value) {
                        if constexpr (std::is_same_v<std::decay_t<decltype(value)>, MEMBFormat>) {
                            return value.has32bitDisplacement(); 
                        } else {
                            return false;
                        }
                    }, _storage);
        }
        std::variant<MEMAFormat, MEMBFormat> _storage;
    };
    class Instruction {

        public:
            constexpr Instruction(Ordinal raw = 0, Ordinal second = 0) : _raw(raw), _second(second) {
                // now we go through and perform a decode operation
            }
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
                return std::visit([this](auto&& value) {
                            if constexpr (std::is_same_v<std::decay_t<decltype(value)>, MemFormat>) {
                                return value.has32bitDisplacement();
                            } else {
                                return false;
                            }
                        }, _storage);
            }
        private:
            std::variant<REGFormat, COBRFormat, CTRLFormat, MemFormat> _storage;
            Ordinal _raw;
            Ordinal _second;

    };
    //static_assert(sizeof(Instruction) == 2_words, "Instruction must be 2 words wide!");

} // end namespace i960
#endif // end I960_INSTRUCTION_H__
