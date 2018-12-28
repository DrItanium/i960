#include "opcodes.h"
#include <map>

namespace i960::Opcode {
	constexpr Description undefined (0xFFFF'FFFF, Description::Class::Undefined, "undefined", Description::ArgumentLayout::Undefined);
#define o(name, code, arg, kind) \
	constexpr Description Desc_ ## name = Description(code, Description::Class:: kind, #name , Description::ArgumentLayout:: arg ) ;
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
	const Description& getDescription(const Instruction& inst) noexcept {
		return getDescription(inst.getOpcode());
	}
	const Description& getDescription(Ordinal opcode) noexcept {
		static std::map<Ordinal, const Description&> lookupTable = {
#define body(name, code) \
			{ code , i960::Opcode:: Desc_ ## name }, 
#define reg(name, code, __)  body(name, code)
#define cobr(name, code, __)  body(name, code)
#define mem(name, code, __)  body(name, code)
#define ctrl(name, code, __)  body(name, code)
#include "opcodes.def"
#undef reg
#undef cobr
#undef mem
#undef ctrl
#undef body
		};
		if (auto pos = lookupTable.find(opcode); pos != lookupTable.end()) {
			return pos->second;
		} else {
			return undefined;
		}
	}
} // end namespace i960
