#ifndef I960_OPCODES_H__
#define I960_OPCODES_H__
#include "types.h"
namespace i960::Opcode {
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
				RegLit_RegLit_Reg,
				Reg_RegLit_Reg,
				Disp,
				Mem_Reg,
				RegLit_Reg_Disp,
				RegLit_Reg,
				Mem,
				RegLit,
				RegLit_RegLit,
				Reg,
			};
			constexpr Description(Ordinal opcode, Class type, const char* str) noexcept : _opcode(opcode), _type(type), _str(str) { }
			constexpr auto getOpcode() const noexcept { return _opcode; }
			constexpr auto getType() const noexcept { return _type; }
			constexpr auto getString() const noexcept { return _str; }
#define X(cl) constexpr bool is ## cl () const noexcept { return isOfClass<Class:: cl > () ; }
			X(Reg);
			X(Cobr);
			X(Mem);
			X(Ctrl);
			X(Undefined);
#undef X
			constexpr operator Ordinal() const { return _opcode; }
			private:
				template<Class t>
				constexpr bool isOfClass() const noexcept { return _type == t; }
			private:
				Ordinal _opcode;
				Class _type;
				const char* _str;
		};
		constexpr Description unknown = Description(0xFFFF'FFFF, Description::Class::Undefined, "undefined");
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
