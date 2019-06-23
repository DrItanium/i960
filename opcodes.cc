#include "types.h"
#include "opcodes.h"
#include <map>

namespace i960::Opcode {
	const Description& getDescription(Ordinal opcode) noexcept {
		static constexpr Description undefined (0xFFFF'FFFF, Description::UndefinedClass(), "undefined", Description::UndefinedArgumentLayout());
		static std::map<Ordinal, Description> lookupTable = {
#define body(name, code, arg, kind) \
			{ code , Description(code, Description:: kind ## Class () , #name , Description:: arg ## ArgumentLayout () ) },
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
