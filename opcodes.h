#ifndef I960_OPCODES_H__
#define I960_OPCODES_H__
#include "types.h"
namespace i960 {
    enum class Opcode : Ordinal {
#define o(name, code) name = code,
#define c(baseName, baseAddress) \
		o(baseName ## e, (baseAddress | 0x0a0)) \
		o(baseName ## g, (baseAddress | 0x090)) \
		o(baseName ## ge, (baseAddress | 0x0b0)) \
		o(baseName ## l, (baseAddress | 0x0c0)) \
		o(baseName ## le, (baseAddress | 0x0e0)) \
		o(baseName ## ne, (baseAddress | 0x0d0)) \
		o(baseName ## no, (baseAddress | 0x080)) \
		o(baseName ## o, (baseAddress | 0x0f0))
#include "opcodes.def"
#undef c
#undef o
    };
	constexpr Ordinal decode(Opcode code) noexcept {
		return static_cast<Ordinal>(code);
	}
} // end namespace i960
#endif // end I960_OPCODES_H__
