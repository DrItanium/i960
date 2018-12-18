#ifndef I960_OPCODES_H__
#define I960_OPCODES_H__
#include "types.h"
namespace i960 {
	namespace Opcode {
		struct Description final {
			enum class Class {
				Reg,
				Cobr,
				Mem,
				Ctrl,
			};
			constexpr Description(Ordinal opcode, Class type) noexcept : _opcode(opcode), _type(type) { }
			constexpr auto getOpcode() const noexcept { return _opcode; }
			constexpr auto getType() const noexcept { return _type; }
#define X(cl) constexpr bool is ## cl () const noexcept { return isOfClass<Class:: cl > () ; }
			X(Reg);
			X(Cobr);
			X(Mem);
			X(Ctrl);
#undef X
			constexpr operator Ordinal() const { return _opcode; }
			private:
				template<Class t>
				constexpr bool isOfClass() const noexcept { return _type == t; }
			private:
				Ordinal _opcode;
				Class _type;
		};

		
#define o(name, code, kind) \
	constexpr Description name = Description(code, Description::Class:: kind ) ;
#define reg(name, code) o(name, code, Reg)
#define cobr(name, code) o(name, code, Cobr) 
#define mem(name, code) o(name, code, Mem) 
#define ctrl(name, code) o(name, code, Ctrl)

#define creg(baseName, baseAddress) \
		reg(baseName ## e, (baseAddress | 0x0a0)) \
		reg(baseName ## g, (baseAddress | 0x090)) \
		reg(baseName ## ge, (baseAddress | 0x0b0)) \
		reg(baseName ## l, (baseAddress | 0x0c0)) \
		reg(baseName ## le, (baseAddress | 0x0e0)) \
		reg(baseName ## ne, (baseAddress | 0x0d0)) \
		reg(baseName ## no, (baseAddress | 0x080)) \
		reg(baseName ## o, (baseAddress | 0x0f0))
#include "opcodes.def"
#undef creg
#undef reg
#undef cobr
#undef mem
#undef ctrl
#undef o

    }
} // end namespace i960
#endif // end I960_OPCODES_H__
