#include "opcodes.h"
#include <map>

namespace i960::Opcode {
	const Description& getDescription(const Instruction& inst) noexcept {
		return getDescription(inst.getOpcode());
	}
	const Description& getDescription(Ordinal opcode) noexcept {
		static std::map<Ordinal, const Description&> lookupTable = {
#define body(name, code) \
			{ code , i960::Opcode:: name }, 
#define reg(name, code)  body(name, code)
#define cobr(name, code)  body(name, code)
#define mem(name, code)  body(name, code)
#define ctrl(name, code)  body(name, code)
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
