#ifndef I960_OPCODES_H__
#define I960_OPCODES_H__
#include "types.h"
#include "Instruction.h"
#include <variant>
namespace i960::Opcode {
		struct Description final {
#define DefClass(name) \
            struct name ## Class final { }
            DefClass(Undefined);
            DefClass(Reg);
            DefClass(Cobr);
            DefClass(Mem);
            DefClass(Ctrl);
#undef DefClass
            using Class = std::variant<
                UndefinedClass,
                RegClass,
                CobrClass,
                MemClass,
                CtrlClass>;
#define DefClass(name) \
            struct name ## ArgumentLayout final { };
            // zero arguments
            DefClass(None);
            // 1 arg
            DefClass(Disp);
            DefClass(Mem);
            DefClass(RegLit);
            DefClass(Reg);
            // 2 args
            DefClass(Mem_Reg);
            DefClass(Reg_Mem);
            DefClass(RegLit_Reg);
            DefClass(RegLit_RegLit);
            // 3 args
            DefClass(RegLit_RegLit_Reg);
            DefClass(Reg_RegLit_Reg);
            DefClass(RegLit_Reg_Disp);
            // not set
            DefClass(Undefined);
#undef DefClass
            using ArgumentLayout = std::variant<
                // not set
                UndefinedArgumentLayout
#define DefClass(name) \
                , name ## ArgumentLayout
            // zero arguments
            DefClass(None)
            // 1 arg
            DefClass(Disp)
            DefClass(Mem)
            DefClass(RegLit)
            DefClass(Reg)
            // 2 args
            DefClass(Mem_Reg)
            DefClass(Reg_Mem)
            DefClass(RegLit_Reg)
            DefClass(RegLit_RegLit)
            // 3 args
            DefClass(RegLit_RegLit_Reg)
            DefClass(Reg_RegLit_Reg)
            DefClass(RegLit_Reg_Disp)
#undef DefClass
            >;
			static constexpr Integer getArgumentCount(ArgumentLayout layout) noexcept {
                return std::visit(overloaded {
                            [](auto&&) { return -1; },
#define X(name, count) [](name ## ArgumentLayout &&) { return count ; }
                            X(None, 0),
                            X(Disp, 1),
                            X(Mem, 1),
                            X(RegLit, 1),
                            X(Reg, 1),
                            X(Mem_Reg, 2),
                            X(Reg_Mem, 2),
                            X(RegLit_Reg, 2),
                            X(RegLit_RegLit, 2),
                            X(RegLit_RegLit_Reg, 3),
                            X(Reg_RegLit_Reg, 3),
                            X(RegLit_Reg_Disp, 3),
#undef X
                        }, layout);
			}

			constexpr Description(Ordinal opcode, Class type, const char* str, ArgumentLayout layout) noexcept : 
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
#define X(cl) constexpr auto is ## cl () const noexcept { \
    return std::visit([](auto&& value) { return std::is_same_v<std::decay_t<decltype(value)>, cl ## Class >; }, _type); \
}
			X(Reg);
			X(Cobr);
			X(Mem);
			X(Ctrl);
			X(Undefined);
#undef X
			constexpr operator Ordinal() const noexcept { return _opcode; }
			private:
				Ordinal _opcode;
				Class _type;
				const char* _str;
				Integer _argCount;
				ArgumentLayout _layout;
		};
        struct UndefinedDescription final {
		    static constexpr Description theDescription {0xFFFF'FFFF, Description::UndefinedClass(), "undefined", Description::UndefinedArgumentLayout()};
        };
#define o(name, code, arg, kind) \
        struct name ## Description final { \
            static constexpr Description theDescription { code , Description:: kind ## Class (), #name , Description:: arg ## ArgumentLayout () }; \
        }; 
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
        using TargetOpcode = std::variant<
        UndefinedDescription
#define o(name, code, arg, kind) \
            , name ## Description 
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
        >;
		inline const Description& getDescription(const Instruction& inst) noexcept {
            return getDescription(inst.getOpcode());
        }
		const Description& getDescription(Ordinal opcode) noexcept;
} // end namespace i960::Opcode
#endif // end I960_OPCODES_H__
