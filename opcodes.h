#ifndef I960_OPCODES_H__
#define I960_OPCODES_H__
#include "types.h"
namespace i960::Opcode {
		constexpr bool hasZeroArguments(Ordinal opcode) noexcept;
		struct Description final {
			enum class Class : Ordinal {
				Undefined,
				Reg,
				Cobr,
				Mem,
				Ctrl,
			};
			enum class ArgumentLayout {
				None,
				// 1 arg
				Disp,
				Mem,
				RegLit,
				Reg,
				// 2 args
				Mem_Reg,
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

			constexpr Description(Ordinal opcode, Class type, const char* str) noexcept : _opcode(opcode), _type(type), _str(str) { }
			constexpr auto getOpcode() const noexcept { return _opcode; }
			constexpr auto getType() const noexcept { return _type; }
			constexpr auto getString() const noexcept { return _str; }
			constexpr bool hasZeroArguments() const noexcept { return i960::Opcode::hasZeroArguments(_opcode); }
#define X(cl) constexpr bool is ## cl () const noexcept { return isOfClass<Class:: cl > () ; }
			X(Reg);
			X(Cobr);
			X(Mem);
			X(Ctrl);
			X(Undefined);
#undef X
			constexpr operator Ordinal() const noexcept { return _opcode; }
			private:
				template<Class t>
				constexpr bool isOfClass() const noexcept { return _type == t; }
			private:
				Ordinal _opcode;
				Class _type;
				const char* _str;
		};
		constexpr Description undefined = Description(0xFFFF'FFFF, Description::Class::Undefined, "undefined");
#define o(name, code, kind) \
	constexpr Description name = Description(code, Description::Class:: kind, #name ) ;
#define reg(name, code) o(name, code, Reg)
#define cobr(name, code) o(name, code, Cobr) 
#define mem(name, code) o(name, code, Mem) 
#define ctrl(name, code) o(name, code, Ctrl)
#include "opcodes.def"
#undef reg
#undef cobr
#undef mem
#undef ctrl
#undef o

		const Description& getDescription(const Instruction& inst) noexcept;
		const Description& getDescription(Ordinal opcode) noexcept;
		constexpr bool hasZeroArguments(Ordinal opcode) noexcept {
			switch (opcode) {
				case Opcode::fmark:
				case Opcode::mark:
				case Opcode::ret:
				case Opcode::faulte:
				case Opcode::faultg:
				case Opcode::faultge:
				case Opcode::faultl:
				case Opcode::faultle:
				case Opcode::faultne:
				case Opcode::faultno:
				case Opcode::faulto:
				case Opcode::flushreg:
				case Opcode::syncf:
				case Opcode::inten:
				case Opcode::intdis:
					return true;
				default:
					return false;
			}
		}
} // end namespace i960::Opcode
#endif // end I960_OPCODES_H__
