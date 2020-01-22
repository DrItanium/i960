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
            Undefined         = 0x0,
            // 0 args
            None              = 0x1,
            // 1 arg
            Disp              = 0x2,
            Mem               = 0x3,
            RegLit            = 0x4,
            Reg               = 0x5,
            // 2 args
            Mem_Reg           = 0x6,
            Reg_Mem           = 0x7,
            RegLit_Reg        = 0x8,
            RegLit_RegLit     = 0x9,
            // 3 args
            RegLit_RegLit_Reg = 0xA,
            Reg_RegLit_Reg    = 0xB,
            RegLit_Reg_Disp   = 0xC,
            Count,
        };
        static_assert(static_cast<uint8_t>(Arguments::Count) <= 0x10, "Too many argument styles defined!");

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
        template<Arguments k>
        constexpr auto ArgumentCount = getArgumentCount(k);
        template<Arguments k>
        constexpr auto HasArgumentCount = ArgumentCount<k> != -1;

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
                constexpr bool operator==(const DecodedOpcode& other) noexcept {
                    return (other._actualOpcode == _actualOpcode) &&
                           (other._arguments == _arguments) &&
                           (other._class == _class);
                }
                constexpr auto lowestThreeBitsOfMajorOpcode() const noexcept {
                    return (getActualOpcode() & 0b111'0000) >> 4;
                }
            private:
                Class _class;
                Arguments _arguments;
                OpcodeValue _actualOpcode;
        };
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
                constexpr bool operator==(const Description& other) noexcept {
                    return other._opcode == _opcode;
                }
            private:
                const char* _name;
                const DecodedOpcode& _opcode;
        };

        constexpr DecodedOpcode undefinedOpcode(Class::Unknown, Arguments::Undefined, 0xFFFF);
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
namespace i960 {
// Setup knowledge base about operations
    template<typename T>
    constexpr auto IsTestOperation = false;
#define X(op) template<> constexpr auto IsTestOperation<Operation:: test ## op > = true
    X(g);
    X(e);
    X(ge);
    X(l);
    X(ne);
    X(le);
    X(o);
    X(no);
#undef X
    template<typename T>
    constexpr auto IsCompareOrdinalAndBranchOperation = false;
#define X(op) template<> constexpr auto IsCompareOrdinalAndBranchOperation<Operation:: cmpob ## op > = true
    X(g);
    X(e);
    X(ge);
    X(l);
    X(ne);
    X(le);
#undef X
    template<typename T>
    constexpr auto IsCompareIntegerAndBranchOperation = false;

#define X(op) template<> constexpr auto IsCompareIntegerAndBranchOperation<Operation:: cmpib ## op > = true
    X(g);
    X(e);
    X(ge);
    X(l);
    X(ne);
    X(le);
    X(o);
    X(no);
#undef X

    template<typename T>
    constexpr auto IsSelectOperation = false;
#define X(op) template<> constexpr auto IsSelectOperation< Operation:: sel ## op > = true
    X(no);
    X(g);
    X(e);
    X(ge);
    X(l);
    X(ne);
    X(le);
    X(o);
#undef X
    template<typename T>
    constexpr auto IsConditionalBranchOperation = false;
#define X(op) template<> constexpr auto IsConditionalBranchOperation < Operation:: b ## op > = true
    X(g);
    X(e);
    X(ge);
    X(l);
    X(ne);
    X(le);
    X(o);
    X(no);
#undef X
    template<typename T>
    constexpr auto IsFaultOperation = false;
#define X(op) template<> constexpr auto IsFaultOperation < Operation:: fault  ## op > = true
    X(g);
    X(e);
    X(ge);
    X(l);
    X(ne);
    X(le);
    X(o);
    X(no);
#undef X
    template<typename T>
    constexpr auto IsConditionalAddOrdinalOperation = false;
    template<typename T>
    constexpr auto IsConditionalAddIntegerOperation = false;
    template<typename T>
    constexpr auto IsConditionalSubtractOrdinalOperation = false;
    template<typename T>
    constexpr auto IsConditionalSubtractIntegerOperation = false;
#define X(op) \
    template<> constexpr auto IsConditionalAddOrdinalOperation < Operation:: addo ## op > = true; \
    template<> constexpr auto IsConditionalAddIntegerOperation < Operation:: addi ## op > = true; \
    template<> constexpr auto IsConditionalSubtractIntegerOperation < Operation:: subi ## op > = true; \
    template<> constexpr auto IsConditionalSubtractOrdinalOperation < Operation:: subo ## op > = true
    X(no);
    X(g);
    X(e);
    X(ge);
    X(l);
    X(ne);
    X(le);
    X(o);
#undef X
    template<typename T>
    constexpr auto IsConditionalSubtractOperation = IsConditionalSubtractIntegerOperation<std::decay_t<T>> || IsConditionalSubtractOrdinalOperation<std::decay_t<T>>;
    template<typename T>
    constexpr auto IsConditionalAddOperation = IsConditionalAddIntegerOperation<std::decay_t<T>> || IsConditionalAddOrdinalOperation<std::decay_t<T>>;
    template<typename T, typename ... Kinds>
    constexpr auto IsInCollection = (std::is_same_v<std::decay_t<T>, std::decay_t<Kinds>> || ...);
    template<typename T>
    constexpr auto IsOrdinalOperation = IsConditionalAddOrdinalOperation<T> ||
                                        IsConditionalSubtractOrdinalOperation<T> ||
                                        IsCompareOrdinalAndBranchOperation<T> ||
                                        IsInCollection<T, Operation::addo,
                                                          Operation::cmpob,
                                                          Operation::cmpos,
                                                          Operation::cmpo,
                                                          Operation::ediv,
                                                          Operation::emul,
                                                          Operation::eshro,
                                                          Operation::cmpdeco,
                                                          Operation::cmpinco,
                                                          Operation::concmpo,
                                                          Operation::shlo,
                                                          Operation::shro,
                                                          Operation::subo,
                                                          Operation::mulo,
                                                          Operation::divo,
                                                          Operation::remo, 
                                                          Operation::addc /* really isn't ordinal or integer kinds but this is the closest */>;
    template<typename T>
    constexpr auto IsIntegerOperation = IsConditionalAddIntegerOperation<T> ||
                                        IsConditionalSubtractIntegerOperation<T> ||
                                        IsCompareIntegerAndBranchOperation<T> ||
                                        IsInCollection<T, Operation::addi,
                                                          Operation::cmpib,
                                                          Operation::cmpis,
                                                          Operation::cmpi,
                                                          Operation::cmpdeci,
                                                          Operation::cmpinci,
                                                          Operation::concmpi,
                                                          Operation::shli,
                                                          Operation::shri,
                                                          Operation::subi,
                                                          Operation::muli,
                                                          Operation::divi,
                                                          Operation::remi,
                                                          Operation::modi>;
    template<typename T>
    constexpr auto IsUnconditionalAddOperation = IsInCollection<T, Operation::addo, 
                                                      Operation::addi>;
    template<typename T>
    constexpr auto IsUnconditionalSubtractOperation = IsInCollection<T, Operation::subo, 
                                                           Operation::subi>;

    template<typename T>
    constexpr auto IsAddWithCarryOperation = IsInCollection<T, Operation::addc>;
    template<typename T>
    constexpr auto IsAddOperation = IsUnconditionalAddOperation<T> || IsConditionalAddOperation<T> || IsAddWithCarryOperation<T>;
    template<typename T>
    constexpr auto IsSubtractOperation = IsUnconditionalSubtractOperation<T> || IsConditionalSubtractOperation<T>;
    template<typename T>
    constexpr auto IsMultiplyOperation  = IsInCollection<T, Operation::mulo, 
                                                            Operation::muli,
                                                            Operation::emul>;
    template<typename T>
    constexpr auto IsDivideOperation    = IsInCollection<T, Operation::divo, 
                                                           Operation::divi,
                                                           Operation::ediv>;

    template<typename T>
    constexpr auto IsRemainderOperation = IsInCollection<T, Operation::remo, 
                                                            Operation::remi>;

    template<typename T>
    constexpr auto IsModuloOperation = IsInCollection<T, Operation::modi>;

    template<typename T>
    constexpr auto CheckForDivideByZero = IsDivideOperation<T> || IsRemainderOperation<T>;
    template<typename T>
    constexpr auto CheckForOverflow = IsInCollection<T, Operation::muli, Operation::addi, Operation::subi, Operation::divi> ||
                                       IsConditionalSubtractIntegerOperation<T> ||
                                       IsConditionalAddIntegerOperation<T>;
    template<typename T>
    constexpr auto IsPureConditionalCompare = IsInCollection<T, Operation::concmpi, Operation::concmpo>;
    template<typename T>
    constexpr auto IsCompareOperation = IsInCollection<T, Operation::cmpi, 
                                                          Operation::cmpo,
                                                          Operation::cmpib,
                                                          Operation::cmpob,
                                                          Operation::cmpis,
                                                          Operation::cmpos,
                                                          Operation::cmpinci,
                                                          Operation::cmpinco,
                                                          Operation::cmpdeci,
                                                          Operation::cmpdeco> || IsPureConditionalCompare<T>;
    template<typename T>
    constexpr auto InvertSrc1 = IsInCollection<T, Operation::andnot, 
                                                  Operation::nor, 
                                                  Operation::nand,
                                                  Operation::ornot,
                                                  Operation::clrbit>;
    template<typename T>
    constexpr auto InvertSrc2 = IsInCollection<T, Operation::notand, 
                                                  Operation::nand, 
                                                  Operation::nor,
                                                  Operation::notor>;

    template<typename T>
    constexpr auto OneShiftLeftSrc1 = IsInCollection<T, Operation::setbit,
                                                        Operation::notbit,
                                                        Operation::clrbit>;
    template<typename T>
    constexpr auto IsAndOperation = IsInCollection<T, Operation::opand, 
                                                      Operation::andnot, 
                                                      Operation::notand, 
                                                      Operation::nor, /* dst = ~s2 & ~s1 */
                                                      Operation::clrbit>;

    template<typename T>
    constexpr auto IsOrOperation = IsInCollection<T, Operation::opor, 
                                                     Operation::ornot, 
                                                     Operation::notor, 
                                                     Operation::nand, /* dst = ~s2 | ~s1 */
                                                     Operation::setbit >;
    template<typename T>
    constexpr auto IsXorOperation = IsInCollection<T, Operation::xnor, 
                                                      Operation::opxor,
                                                      Operation::notbit>;
    template<typename T>
    constexpr auto IsNotOperation = IsInCollection<T, Operation::opnot>;

    template<typename T>
    constexpr auto IsUnaryOperation = IsInCollection<T, Operation::opnot>;

    template<typename T>
    constexpr auto IsBitManipulationOperation = IsAndOperation<T> || IsOrOperation<T> || IsXorOperation<T> || IsNotOperation<T> ||
                                                IsInCollection<T, Operation::alterbit>;
    template<typename T>
    constexpr auto IsByteCompareOperation = IsInCollection<T, Operation::cmpob, Operation::cmpib>;
    template<typename T>
    constexpr auto IsShortCompareOperation = IsInCollection<T, Operation::cmpos, Operation::cmpis>;
    template<typename T>
    constexpr auto IsCompareAndIncrementOperation = IsInCollection<T, Operation::cmpinci, Operation::cmpinco>;
    template<typename T>
    constexpr auto IsCompareAndDecrementOperation = IsInCollection<T, Operation::cmpdeci, Operation::cmpdeco>;
    template<typename T>
    constexpr auto IsShiftLeftOperation = IsInCollection<T, Operation::shli, Operation::shlo>;
    template<typename T>
    constexpr auto IsShiftRightOperation = IsInCollection<T, Operation::shri, Operation::shro>;
    template<typename T>
    constexpr auto IsShiftOperation = IsShiftLeftOperation<T> || IsShiftRightOperation<T>;
    template<typename T>
    constexpr auto IsCheckBitAndBranchIfOperation = IsInCollection<T, Operation::bbc, Operation::bbs>;
    template<typename T>
    constexpr auto IsCOBRFormat = Opcode::IsCompareAndBranchFormat<T::Opcode>;
    template<typename T>
    constexpr auto IsCTRLFormat = Opcode::IsControlFormat<T::Opcode>;
    template<typename T>
    constexpr auto IsREGFormat = Opcode::IsRegisterFormat<T::Opcode>;
    template<typename T>
    constexpr auto IsMEMFormat = Opcode::IsMemoryFormat<T::Opcode>;
} // end namespace i960
#endif // end I960_OPCODES_H__
