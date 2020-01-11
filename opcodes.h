#ifndef I960_OPCODES_H__
#define I960_OPCODES_H__
#include "types.h"
#include <variant>
namespace i960 {
    class Instruction;
}
namespace i960::Opcode {
        using EncodedOpcode = i960::Ordinal;
        /**
         * The opcode class as defined in i960, there are additions to make sure
         * that we don't run into strange edge cases; The values are encoded as they will show up in
         * the encoded 32-bit token
         */
        enum class Class : EncodedOpcode {
            // upper most 4 bits are consumed by this class field but only 5 are used, wasteful but who cares
            Unknown  = 0x0000'0000,
            Register = 0x1000'0000,
            COBR     = 0x2000'0000,
            Memory   = 0x3000'0000,
            Control  = 0x4000'0000,
        };
        /**
         * 4 bit code field which describes the instruction's argument layout
         */
        enum class Arguments : EncodedOpcode {
            // not set
            Undefined         = 0x0000'0000,
            // 0 args
            None              = 0x0100'0000,
            // 1 arg
            Disp              = 0x0200'0000,
            Mem               = 0x0300'0000,
            RegLit            = 0x0400'0000,
            Reg               = 0x0500'0000,
            // 2 args
            Mem_Reg           = 0x0600'0000,
            Reg_Mem           = 0x0700'0000,
            RegLit_Reg        = 0x0800'0000,
            RegLit_RegLit     = 0x0900'0000,
            // 3 args
            RegLit_RegLit_Reg = 0x0A00'0000,
            Reg_RegLit_Reg    = 0x0B00'0000,
            RegLit_Reg_Disp   = 0x0C00'0000,
        };

        template<typename T>
        constexpr EncodedOpcode FieldMask = 0xFFFF'FFFF;
        template<> constexpr EncodedOpcode FieldMask<Class>     = 0xF000'0000;
        template<> constexpr EncodedOpcode FieldMask<Arguments> = 0x0F00'0000;
        template<> constexpr EncodedOpcode FieldMask<uint16_t>  = 0x0000'FFFF;
        template<typename R>
        constexpr R extractField(EncodedOpcode raw) noexcept {
            static_assert(FieldMask<R> != 0xFFFF'FFFF, "FieldMask not specified for type");
            return i960::decode<decltype(raw), R, FieldMask<R>, 0>(raw);
        }
        class DecodedOpcode final {
            public:
                constexpr DecodedOpcode(EncodedOpcode opcode) noexcept : 
                    _class(extractField<Class>(opcode)),
                    _arguments(extractField<Arguments>(opcode)),
                    _actualOpcode(extractField<uint16_t>(opcode)) { }
                constexpr auto getEncodedOpcode() const noexcept {
                    return static_cast<EncodedOpcode>(_class) |
                           static_cast<EncodedOpcode>(_arguments) |
                           static_cast<EncodedOpcode>(_actualOpcode);
                }
            private:
                Class _class;
                Arguments _arguments;
                uint16_t _actualOpcode;
        };


		enum Id : OpcodeValue {
#define o(name, code, arg, kind) \
	name = code,
#define reg(name, code, arg) o(name, code, arg, Reg)
#define cobr(name, code, arg) o(name, code, arg, Cobr) 
#define mem(name, code, arg) o(name, code, arg, Mem) 
#define ctrl(name, code, arg) o(name, code, arg, Ctrl)
#include "opcodes.def"
#undef reg
#undef cobr
#undef mem
#undef ctrl
#undef o
            Undefined = 0xFFFF,
		};
        template<Id id>
        constexpr auto IsMEMFormat = false;
        template<Id id>
        constexpr auto IsREGFormat = false;
        template<Id id>
        constexpr auto IsCTRLFormat = false;
        template<Id id>
        constexpr auto IsCOBRFormat = false;

#define o(name, code, arg, kind) \
        template<> \
        constexpr auto Is ## kind ## Format < Id :: name > = true; 
#define reg(name, code, arg) o(name, code, arg, REG)
#define cobr(name, code, arg) o(name, code, arg, COBR) 
#define mem(name, code, arg) o(name, code, arg, MEM) 
#define ctrl(name, code, arg) o(name, code, arg, CTRL)
#include "opcodes.def"
#undef reg
#undef cobr
#undef mem
#undef ctrl
#undef o


		struct Description final {
			enum class Class : Ordinal {
				Undefined,
				Reg,
				Cobr,
				Mem,
				Ctrl,
			};
			enum class ArgumentLayout {
				// not set
				Undefined,
				// 0 args
				None,
				// 1 arg
				Disp,
				Mem,
				RegLit,
				Reg,
				// 2 args
				Mem_Reg,
				Reg_Mem,
				RegLit_Reg,
				RegLit_RegLit,
				// 3 args
				RegLit_RegLit_Reg,
				Reg_RegLit_Reg,
				RegLit_Reg_Disp,
			};
			static constexpr Integer getArgumentCount(ArgumentLayout layout) noexcept {
				switch (layout) {
					case ArgumentLayout::None:
						return 0;
					case ArgumentLayout::Disp:
					case ArgumentLayout::Mem:
					case ArgumentLayout::Reg:
					case ArgumentLayout::RegLit:
						return 1;
					case ArgumentLayout::Mem_Reg:
					case ArgumentLayout::Reg_Mem:
					case ArgumentLayout::RegLit_Reg:
					case ArgumentLayout::RegLit_RegLit:
						return 2;
					case ArgumentLayout::RegLit_Reg_Disp:
					case ArgumentLayout::RegLit_RegLit_Reg:
					case ArgumentLayout::Reg_RegLit_Reg:
						return 3;
					default:
						return -1;
				}
			}

			constexpr Description(OpcodeValue opcode, Class type, const char* str, ArgumentLayout layout) noexcept : 
				_opcode(opcode), _type(type), _str(str), _argCount(getArgumentCount(layout)), _layout(layout) { }
            ~Description() = default;
			constexpr auto getOpcode() const noexcept { return _opcode; }
			constexpr auto getType() const noexcept { return _type; }
			constexpr auto getString() const noexcept { return _str; }
			constexpr auto getArgumentCount() const noexcept { return _argCount; }
			constexpr auto getArgumentLayout() const noexcept { return _layout; }
			constexpr auto hasZeroArguments() const noexcept { return _argCount == 0; }
			constexpr auto hasOneArgument() const noexcept { return _argCount == 1; }
			constexpr auto hasTwoArguments() const noexcept { return _argCount == 2; } 
			constexpr auto hasThreeArguments() const noexcept { return _argCount == 3; } 
			constexpr auto argumentCountUndefined() const noexcept { return _argCount == -1; }
#define X(cl) constexpr bool is ## cl () const noexcept { return isOfClass<Class:: cl > () ; }
			X(Reg);
			X(Cobr);
			X(Mem);
			X(Ctrl);
			X(Undefined);
#undef X
			constexpr operator OpcodeValue() const noexcept { return _opcode; }
			private:
				template<Class t>
				constexpr bool isOfClass() const noexcept { return _type == t; }
			private:
                OpcodeValue _opcode;
				Class _type;
				const char* _str;
				Integer _argCount;
				ArgumentLayout _layout;
		};


		const Description& getDescription(Ordinal opcode) noexcept;
		const Description& getDescription(const Instruction& inst) noexcept;
} // end namespace i960::Opcode
namespace i960::Operation {
    template<typename T>
    constexpr auto IsCTRLFormat = i960::Opcode::IsCTRLFormat<T::Opcode>;
    template<typename T>
    constexpr auto IsREGFormat = i960::Opcode::IsREGFormat<T::Opcode>;
    template<typename T>
    constexpr auto IsMEMFormat = i960::Opcode::IsMEMFormat<T::Opcode>;
    template<typename T>
    constexpr auto IsCOBRFormat = i960::Opcode::IsCOBRFormat<T::Opcode>;
#define X(name, code, kind) \
        struct name final { \
            static constexpr auto Opcode = static_cast<i960::Opcode::Id>(code); \
            constexpr auto getOpcode() noexcept { return Opcode; } \
        };
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
    using REG = std::variant<std::monostate
#define X(name, code, kind) \
        , name 
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
    using MEM = std::variant<std::monostate
#define X(name, code, kind) \
        , name 
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
    using COBR = std::variant<std::monostate
#define X(name, code, kind) \
        , name 
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
    using CTRL = std::variant<std::monostate
#define X(name, code, kind) \
        , name 
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
    struct CTRLClass final { };
    struct COBRClass final { };
    struct REGClass final { };
    struct MEMClass final { };
    constexpr CTRL translate(i960::OpcodeValue op, CTRLClass) noexcept {
        switch (op) {
#define X(name, code, kind) case code : return name () ; 
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
            default:
                return std::monostate();
        }
    }
    constexpr COBR translate(i960::OpcodeValue op, COBRClass) noexcept {
        switch (op) {
#define X(name, code, kind) case code : return name () ; 
#define reg(name, code, __) //X(name, code, REG)
#define mem(name, code, __) //X(name, code, MEM)
#define cobr(name, code, __)  X(name, code, COBR)
#define ctrl(name, code, __) // X(name, code, CTRL)
#include "opcodes.def"
#undef X
#undef reg
#undef mem
#undef cobr
#undef ctrl
            default:
                return std::monostate();
        }
    }
    constexpr REG translate(i960::OpcodeValue op, REGClass) noexcept {
        switch (op) {
#define X(name, code, kind) case code : return name () ; 
#define reg(name, code, __) X(name, code, REG)
#define mem(name, code, __) //X(name, code, MEM)
#define cobr(name, code, __)  //X(name, code, COBR)
#define ctrl(name, code, __) // X(name, code, CTRL)
#include "opcodes.def"
#undef X
#undef reg
#undef mem
#undef cobr
#undef ctrl
            default:
                return std::monostate();
        }
    }
    constexpr MEM translate(i960::OpcodeValue op, MEMClass) noexcept {
        switch (op) {
#define X(name, code, kind) case code : return name () ; 
#define reg(name, code, __) //X(name, code, REG)
#define mem(name, code, __) X(name, code, MEM)
#define cobr(name, code, __)  //X(name, code, COBR)
#define ctrl(name, code, __) // X(name, code, CTRL)
#include "opcodes.def"
#undef X
#undef reg
#undef mem
#undef cobr
#undef ctrl
            default:
                return std::monostate();
        }
    }
} // end namespace i960::Operation
#endif // end I960_OPCODES_H__
