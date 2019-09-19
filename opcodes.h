#ifndef I960_OPCODES_H__
#define I960_OPCODES_H__
#include "types.h"
#include "Instruction.h"
namespace i960::Opcode {
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
		struct Description final {
			enum class Class : Ordinal {
				Undefined,
				Reg,
				Cobr,
				Mem,
				Ctrl,
			};
			enum class ArgumentLayout {
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
				// not set
				Undefined,
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
		inline const Description& getDescription(const Instruction& inst) noexcept {
            return getDescription(inst.getOpcode());
        }
#define X(name, code, kind) \
        struct kind ## name ## Operation final { }; 
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
} // end namespace i960::Opcode
#endif // end I960_OPCODES_H__
