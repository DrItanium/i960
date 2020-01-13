#ifndef I960_OPCODES_H__
#define I960_OPCODES_H__
#include "types.h"
#include <variant>
namespace i960 {
    class Instruction;
}
namespace i960::Opcode {
        using EncodedOpcode = i960::Ordinal;
        /**
         * The opcode class as defined in i960, there are additions to make sure
         * that we don't run into strange edge cases; The values are encoded as they will show up in
         * the encoded 32-bit token
         */
        enum class Class : EncodedOpcode {
            // upper most 4 bits are consumed by this class field but only 5 are used, wasteful but who cares
            Unknown  = 0x00,
            Register = 0x10,
            COBR     = 0x20,
            Memory   = 0x30,
            Control  = 0x40,
        };
        /**
         * 4 bit code field which describes the instruction's argument layout
         */
        enum class Arguments : EncodedOpcode {
            // not set
            Undefined         = 0x00,
            // 0 args
            None              = 0x01,
            // 1 arg
            Disp              = 0x02,
            Mem               = 0x03,
            RegLit            = 0x04,
            Reg               = 0x05,
            // 2 args
            Mem_Reg           = 0x06,
            Reg_Mem           = 0x07,
            RegLit_Reg        = 0x08,
            RegLit_RegLit     = 0x09,
            // 3 args
            RegLit_RegLit_Reg = 0x0A,
            Reg_RegLit_Reg    = 0x0B,
            RegLit_Reg_Disp   = 0x0C,
        };

        template<typename T>
        constexpr EncodedOpcode FieldMask = 0xFFFF'FFFF;
        template<> constexpr EncodedOpcode FieldMask<Class>     = 0x0000'00F0;
        template<> constexpr EncodedOpcode FieldMask<Arguments> = 0x0000'000F;
        template<> constexpr EncodedOpcode FieldMask<OpcodeValue>  = 0xFFFF'0000;
        template<typename T>
        constexpr EncodedOpcode ShiftAmount = 0;
        template<> constexpr EncodedOpcode ShiftAmount<OpcodeValue> = 16;
        template<typename R>
        constexpr R extractField(EncodedOpcode raw) noexcept {
            static_assert(FieldMask<R> != 0xFFFF'FFFF, "FieldMask not specified for type");
            return i960::decode<decltype(raw), R, FieldMask<R>, ShiftAmount<R>>(raw);
        }
        constexpr Integer getArgumentCount(Arguments args) noexcept {
            switch(args) {
                case Arguments::None: 
                    return 0;
                case Arguments::Disp:
                case Arguments::Mem:
                case Arguments::RegLit:
                case Arguments::Reg:
                    return 1;
                case Arguments::Mem_Reg:
                case Arguments::Reg_Mem:
                case Arguments::RegLit_Reg:
                case Arguments::RegLit_RegLit:
                    return 2;
                case Arguments::RegLit_RegLit_Reg:
                case Arguments::Reg_RegLit_Reg:
                case Arguments::RegLit_Reg_Disp:
                    return 3;
                default:
                    return -1;
            }
        }
        class DecodedOpcode final {
            public:
                constexpr DecodedOpcode(Class cl, Arguments args, OpcodeValue opcode) noexcept : _class(cl), _arguments(args), _actualOpcode(opcode) { }
                constexpr DecodedOpcode(EncodedOpcode opcode) noexcept : DecodedOpcode(extractField<Class>(opcode), extractField<Arguments>(opcode), extractField<OpcodeValue>(opcode)) { }
                constexpr auto getEncodedOpcode() const noexcept {
                    return static_cast<EncodedOpcode>(_class) |
                           static_cast<EncodedOpcode>(_arguments) |
                           (static_cast<EncodedOpcode>(_actualOpcode) << ShiftAmount<decltype(_actualOpcode)>);
                }
                constexpr auto getClass() const noexcept { return _class; }
                constexpr auto getArguments() const noexcept { return _arguments; }
                constexpr auto getActualOpcode() const noexcept { return _actualOpcode; }
                constexpr auto getNumberOfArguments() const noexcept { return getArgumentCount(_arguments); }
                template<Class c>
                constexpr auto isOfClass() const noexcept {
                    return _class == c;
                }

                constexpr auto isUndefined() const noexcept { return isOfClass<Class::Unknown>(); }
                constexpr auto isUnknown() const noexcept { return isUndefined(); }
			    constexpr operator OpcodeValue() const noexcept { return _actualOpcode; }
            private:
                Class _class;
                Arguments _arguments;
                OpcodeValue _actualOpcode;
        };
        constexpr DecodedOpcode undefinedOpcode(Class::Unknown, Arguments::Undefined, 0xFFFF);
        class Description final {
            public:
                constexpr Description(const char* name, const DecodedOpcode& opcode) noexcept : _name(name), _opcode(opcode) { }
                constexpr const char* getName() const noexcept { return _name; }
                constexpr const char* getString() const noexcept { return getName(); }
                constexpr const DecodedOpcode& getOpcode() const noexcept { return _opcode; }
                constexpr auto isUndefined() const noexcept { return _opcode.isUndefined(); }
                constexpr auto isUnknown() const noexcept { return _opcode.isUnknown(); }
                template<Integer argumentCount> 
                constexpr auto hasNumberOfArguments() const noexcept { return _opcode.getNumberOfArguments() == argumentCount; }
                constexpr auto hasZeroArguments() const noexcept { return hasNumberOfArguments<0>(); }
                constexpr auto hasOneArgument() const noexcept { return hasNumberOfArguments<1>(); }
                constexpr auto hasTwoArguments() const noexcept { return hasNumberOfArguments<2>(); }
                constexpr auto hasThreeArguments() const noexcept { return hasNumberOfArguments<3>(); }
			    constexpr operator OpcodeValue() const noexcept { return _opcode; }
            private:
                const char* _name;
                const DecodedOpcode& _opcode;
        };
        constexpr Description undefinedOpcodeDescription("undefined/unknown", undefinedOpcode);
        template<OpcodeValue opcode>
        constexpr auto RetrieveDecodedOpcode = undefinedOpcode;
#define o(name, code, arg, kind) \
        constexpr DecodedOpcode decoded_ ## name (i960::constructOrdinalMask(Class:: kind, Arguments:: arg , static_cast<EncodedOpcode>(code) << 16)); \
        template<> constexpr auto RetrieveDecodedOpcode<code> = decoded_ ## name ; \
        static_assert(decoded_ ## name . getEncodedOpcode() == DecodedOpcode(Class:: kind, Arguments:: arg , code).getEncodedOpcode()); \
        constexpr Description name ( #name , decoded_ ## name );
#define reg(name, code, arg) o(name, code, arg, Register)
#define cobr(name, code, arg) o(name, code, arg, COBR) 
#define mem(name, code, arg) o(name, code, arg, Memory) 
#define ctrl(name, code, arg) o(name, code, arg, Control)
#include "opcodes.def"
#undef reg
#undef cobr
#undef mem
#undef ctrl
#undef o

        template<OpcodeValue opcode>
        constexpr auto IsControlFormat = RetrieveDecodedOpcode<opcode>.getClass() == Class::Control;
        template<OpcodeValue opcode>
        constexpr auto IsRegisterFormat = RetrieveDecodedOpcode<opcode>.getClass() == Class::Register;
        template<OpcodeValue opcode>
        constexpr auto IsCompareAndBranchFormat= RetrieveDecodedOpcode<opcode>.getClass() == Class::COBR;
        template<OpcodeValue opcode>
        constexpr auto IsMemoryFormat = RetrieveDecodedOpcode<opcode>.getClass() == Class::Memory;
        template<OpcodeValue opcode>
        constexpr auto NumberOfArguments = RetrieveDecodedOpcode<opcode>.getNumberOfArguments();

        constexpr const DecodedOpcode& decodeOpcode(OpcodeValue opcode) noexcept {
            switch(opcode) {
#define o(name, code, arg, kind) case code : return RetrieveDecodedOpcode<code>;
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
                default: return undefinedOpcode;
            }
        }
        constexpr auto getNumberOfArguments(OpcodeValue opcode) noexcept {
            switch(opcode) {
#define o(name, code, arg, kind) case code : return NumberOfArguments<code>;
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
                default: return NumberOfArguments<0xFFFF>;
            }
        }
        constexpr const Description& getDescription(i960::OpcodeValue opcode) noexcept {
            switch (opcode) {
#define o(name, code, arg, kind) case code : return name ;
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
                default: return undefinedOpcodeDescription; 
            }
        }
} // end namespace i960::Opcode
namespace i960::Operation {

#define X(name, code, kind) \
        struct name final { \
            static constexpr auto Opcode = static_cast<OpcodeValue>(code); \
            constexpr auto getOpcode() const noexcept { return Opcode; } \
            constexpr const i960::Opcode::DecodedOpcode& getDecodedForm() const noexcept { return i960::Opcode::decodeOpcode(Opcode); } \
        };
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
    using REG = std::variant<std::monostate
#define X(name, code, kind) \
        , name 
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
    using MEM = std::variant<std::monostate
#define X(name, code, kind) \
        , name 
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
    using COBR = std::variant<std::monostate
#define X(name, code, kind) \
        , name 
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
    using CTRL = std::variant<std::monostate
#define X(name, code, kind) \
        , name 
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

    struct CTRLClass final { };
    struct COBRClass final { };
    struct REGClass final { };
    struct MEMClass final { };
    constexpr CTRL translate(i960::OpcodeValue op, CTRLClass) noexcept {
        switch (op) {
#define X(name, code, kind) case code : return name () ; 
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
            default:
                return std::monostate();
        }
    }
    constexpr COBR translate(i960::OpcodeValue op, COBRClass) noexcept {
        switch (op) {
#define X(name, code, kind) case code : return name () ; 
#define reg(name, code, __) //X(name, code, REG)
#define mem(name, code, __) //X(name, code, MEM)
#define cobr(name, code, __)  X(name, code, COBR)
#define ctrl(name, code, __) // X(name, code, CTRL)
#include "opcodes.def"
#undef X
#undef reg
#undef mem
#undef cobr
#undef ctrl
            default:
                return std::monostate();
        }
    }
    constexpr REG translate(i960::OpcodeValue op, REGClass) noexcept {
        switch (op) {
#define X(name, code, kind) case code : return name () ; 
#define reg(name, code, __) X(name, code, REG)
#define mem(name, code, __) //X(name, code, MEM)
#define cobr(name, code, __)  //X(name, code, COBR)
#define ctrl(name, code, __) // X(name, code, CTRL)
#include "opcodes.def"
#undef X
#undef reg
#undef mem
#undef cobr
#undef ctrl
            default:
                return std::monostate();
        }
    }
    constexpr MEM translate(i960::OpcodeValue op, MEMClass) noexcept {
        switch (op) {
#define X(name, code, kind) case code : return name () ; 
#define reg(name, code, __) //X(name, code, REG)
#define mem(name, code, __) X(name, code, MEM)
#define cobr(name, code, __)  //X(name, code, COBR)
#define ctrl(name, code, __) // X(name, code, CTRL)
#include "opcodes.def"
#undef X
#undef reg
#undef mem
#undef cobr
#undef ctrl
            default:
                return std::monostate();
        }
    }
} // end namespace i960::Operation
#endif // end I960_OPCODES_H__
