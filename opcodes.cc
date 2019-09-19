#include "types.h"
#include "opcodes.h"
#include <map>

namespace i960::Opcode {
	const Description& getDescription(Ordinal opcode) noexcept {
		static constexpr Description undefined (0xFFFF, Description::Class::Undefined, "undefined", Description::ArgumentLayout::Undefined);
		static std::map<Ordinal, Description> lookupTable = {
#define body(name, code, arg, kind) \
			{ code , Description(code, Description::Class:: kind, #name , Description::ArgumentLayout:: arg ) },
#define reg(name, code, arg)  body(name, code, arg, Reg)
#define cobr(name, code, arg)  body(name, code, arg, Cobr)
#define mem(name, code, arg)  body(name, code, arg, Mem)
#define ctrl(name, code, arg)  body(name, code, arg, Ctrl)
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
} // end namespace i960::Opcode
