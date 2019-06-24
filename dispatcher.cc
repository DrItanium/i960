#include "types.h"
#include "core.h"
#include "opcodes.h"
#include <map>
#include <functional>
#include <sstream>
#include <string>

namespace i960 {

    template<typename ... Ts>
    using CoreSignature = void(Core::*)(Ts...);
    namespace Opcode {
        template<typename ... Ts>
        using GleanSignature = std::function<decltype(test(std::declval<Ts>()...))(Ts...)>;
        struct UndefinedDescription final {
            using TargetKind = std::nullptr_t;
		    static constexpr Description theDescription {0xFFFF'FFFF, Description::UndefinedClass(), "undefined", Description::UndefinedArgumentLayout()};
        };
#define reg(name, code, arg) \
        struct name ## Description final { \
            using TargetKind = REGFormat; \
            static constexpr i960::Opcode::Description theDescription { code , i960::Opcode::Description:: RegClass (), #name , i960::Opcode::Description:: arg ## ArgumentLayout () }; \
            static constexpr i960::CoreSignature<TargetKind const&> Signature = &Core:: name ; \
        }; 

#define cobr(name, code, arg) \
        struct name ## Description final { \
            using TargetKind = COBRFormat; \
            static constexpr i960::Opcode::Description theDescription { code , i960::Opcode::Description:: CobrClass (), #name , i960::Opcode::Description:: arg ## ArgumentLayout () }; \
            static constexpr i960::CoreSignature<TargetKind const&> Signature = &Core:: name ; \
        }; 
#define ctrl(name, code, arg) \
        struct name ## Description final { \
            using TargetKind = CTRLFormat; \
            static constexpr i960::Opcode::Description theDescription { code , i960::Opcode::Description:: CtrlClass (), #name , i960::Opcode::Description:: arg ## ArgumentLayout () }; \
            static constexpr i960::CoreSignature<TargetKind const&> Signature = &Core:: name; \
        }; 
#define mem(name, code, arg) \
        struct name ## Description final { \
            using TargetKind = Core::MemFormat; \
            static constexpr i960::Opcode::Description theDescription { code , i960::Opcode::Description:: MemClass (), #name , i960::Opcode::Description:: arg ## ArgumentLayout () }; \
            static constexpr i960::CoreSignature<TargetKind const&> Signature = &Core:: name ; \
        }; 
#include "opcodes.def"
#undef reg
#undef cobr
#undef mem
#undef ctrl
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
        TargetOpcode determineTargetOpcode(const Instruction& inst) noexcept {
            return determineTargetOpcode(inst.getOpcode());
        }
        constexpr TargetOpcode determineTargetOpcode(Ordinal opcode) noexcept {
            switch (opcode) {
#define body(name, code, arg, kind) case code : return name ## Description () ;
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
                default:
                    return UndefinedDescription();
            }
        }
    }
    void Core::mark(REGFormat const&) noexcept {
        mark();
    }
    void Core::fmark(REGFormat const&) noexcept {
        fmark();
    }
    void Core::flushreg(REGFormat const&) noexcept {
        flushreg();
    }
    void Core::syncf(REGFormat const&) noexcept {
        syncf();
    }
    void Core::ret(CTRLFormat const&) noexcept {
        ret();
    }
    void Core::inten(REGFormat const&) noexcept {
        inten();
    }
    void Core::intdis(REGFormat const&) noexcept {
        intdis();
    }
#define X(kind, __) \
    void Core:: fault ## kind (CTRLFormat const& ) noexcept { \
        fault ## kind () ; \
    }
#include "conditional_kinds.def"
#undef X
    // TODO It is impossible for m3 to be set when srcDest is used as a dest, error out before hand
#define Y(op) \
    void Core:: op ( REGFormat const& fmt) noexcept { \
        auto opSrc2 = fmt.decodeSrc2(); \
        auto opSrcDest = fmt.decodeSrcDest(); \
        NormalRegister& src1 = selectRegister(fmt.decodeSrc1(), _temporary0); \
        NormalRegister& src2 = selectRegister(opSrc2, _temporary1); \
        NormalRegister& srcDest = selectRegister(opSrcDest, _temporary2); \
        op (src1, src2, srcDest); \
    }
#define X(kind, __) \
				Y(addo ##  kind); \
				Y(addi ##  kind); \
				Y(subo ##  kind); \
				Y(subi ##  kind); \
				Y(sel ##  kind); 
#include "conditional_kinds.def"
#undef X
	Y(notbit);  Y(clrbit);   Y(notor);   Y(opand);
	Y(andnot);  Y(setbit);   Y(notand);  Y(opxor);
	Y(opor);    Y(nor);      Y(xnor);    Y(ornot);
	Y(nand);    Y(alterbit); Y(shro);    Y(shrdi);
	Y(shri);    Y(shlo);     Y(rotate);  Y(shli);
	Y(addc);    Y(subc);     Y(atmod);   Y(atadd);
	Y(modify);  Y(extract);  Y(modac);   Y(modtc);
	Y(modpc);   Y(addo);     Y(addi);    Y(subo);
	Y(subi);    Y(cmpinco);  Y(cmpinci); Y(cmpdeco);
	Y(cmpdeci); Y(mulo);     Y(muli);    Y(remo);
	Y(remi);    Y(divo);     Y(divi);    Y(dcctl);
#undef Y
#define Z(op) \
    void Core:: op (REGFormat const& fmt) noexcept { \
        auto opSrcDest = fmt.decodeSrcDest(); \
        NormalRegister& src1 = selectRegister(fmt.decodeSrc1(), _temporary0); \
        NormalRegister& srcDest = selectRegister(opSrcDest, _temporary2); \
        op (src1, srcDest); \
    }
				Z(spanbit);  Z(scanbit);  Z(opnot);
				Z(chkbit);   Z(cmpo);     Z(cmpi);    
				Z(concmpo);  Z(concmpi);
#undef Z
#define X(op) \
    void Core:: op ( REGFormat const& fmt) noexcept { \
        auto opSrc2 = fmt.decodeSrc2(); \
        NormalRegister& src1 = selectRegister(fmt.decodeSrc1(), _temporary0); \
        NormalRegister& src2 = selectRegister(opSrc2, _temporary1); \
        op (src1, src2); \
    }
                X(scanbyte); X(cmpos); X(cmpis);
                X(cmpob);    X(cmpib); X(bswap);
#undef X
    void Core::mov(REGFormat const& reg) noexcept { mov(reg.decodeSrc1(), reg.decodeSrcDest()); }
    void Core::movl(REGFormat const& reg) noexcept { movl(reg.decodeSrc1(), reg.decodeSrcDest()); }
    void Core::movt(REGFormat const& reg) noexcept { movt(reg.decodeSrc1(), reg.decodeSrcDest()); }
    void Core::movq(REGFormat const& reg) noexcept { movq(reg.decodeSrc1(), reg.decodeSrcDest()); }
    void Core::calls(REGFormat const& reg) noexcept {
        calls(selectRegister(reg.decodeSrc1(), _temporary0));
    }
    void Core::eshro(REGFormat const& reg) noexcept {
		NormalRegister& src1 = selectRegister(reg.decodeSrc1(), _temporary0);
		// TODO It is impossible for m3 to be set when srcDest is used as a dest, error out before hand
		NormalRegister& srcDest = selectRegister(reg.decodeSrcDest(), _temporary2);
        eshro(src1, reg.decodeSrc2(), srcDest);
    }
    void Core::ediv(REGFormat const& reg) noexcept {
		auto opSrc2 = reg.decodeSrc2();
		auto opSrcDest = reg.decodeSrcDest();
		NormalRegister& src1 = selectRegister(reg.decodeSrc1(), _temporary0);
		// TODO It is impossible for m3 to be set when srcDest is used as a dest, error out before hand
        ediv(src1, opSrc2, opSrcDest);
    }
    void Core::emul(REGFormat const& reg) noexcept {
		auto opSrc2 = reg.decodeSrc2();
		auto opSrcDest = reg.decodeSrcDest();
		NormalRegister& src1 = selectRegister(reg.decodeSrc1(), _temporary0);
		NormalRegister& src2 = selectRegister(opSrc2, _temporary1);
		// TODO It is impossible for m3 to be set when srcDest is used as a dest, error out before hand
        emul(src1, src2, opSrcDest);
    }
    NormalRegister& Core::selectRegister(const Operand& operand, NormalRegister& imm) {
        if (operand.isLiteral()) {
            imm.set(operand.getValue());
            return imm;
        } else {
            return getRegister(operand);
        }
    }
    void Core::prepareRegisters(MEMAFormat const& ma) noexcept {
        _temporary0.set<Ordinal>(ma._offset + ma.isOffsetAddressingMode() ? 0 : getRegister(ma.decodeAbase()).get<Ordinal>());
    }
    InstructionLength Core::prepareRegisters(MEMBFormat const& mb) noexcept {
        auto length = InstructionLength::Single;
        using E = std::decay_t<decltype(mb)>::AddressingModes;
        auto index = mb._index;
        auto scale = mb.getScaleFactor();
        auto displacement = 0u;
        if (mb.has32bitDisplacement()) {
            displacement = mb.get32bitDisplacement();
            length = InstructionLength::Double;
        }
        switch (auto abase = getRegister(mb.decodeAbase()); mb.getAddressingMode()) {
            case E::Abase:
                _temporary0.move(abase);
                break;
            case E::IP_Plus_Displacement_Plus_8:
                _temporary0.set<Ordinal>(_instructionPointer + displacement + 8);
                break;
            case E::Abase_Plus_Index_Times_2_Pow_Scale:
                _temporary0.set<Ordinal>(abase.get<Ordinal>() + index * scale);
                break;
            case E::Displacement:
                _temporary0.set(displacement);
                break;
            case E::Abase_Plus_Displacement:
                _temporary0.set(displacement + abase.get<Ordinal>());
                break;
            case E::Index_Times_2_Pow_Scale_Plus_Displacement:
                _temporary0.set(index * scale + displacement);
                break;
            case E::Abase_Plus_Index_Times_2_Pow_Scale_Plus_Displacement:
                _temporary0.set(abase.get<Ordinal>() + index * scale + displacement);
                break;
            default: 
                generateFault(OperationFaultSubtype::InvalidOpcode); 
                return length;
        }
        return length;
    }
    void Core::balx(MEMAFormat const& ma) noexcept {
        prepareRegisters(ma);
        balx<InstructionLength::Single>(_temporary0, getRegister(ma.decodeSrcDest()));
    }
    void Core::balx(MEMBFormat const& mb) noexcept {
        if (prepareRegisters(mb) == InstructionLength::Double) {
            balx<InstructionLength::Double>(_temporary0, getRegister(mb.decodeSrcDest()));
        } else {
            balx<InstructionLength::Single>(_temporary0, getRegister(mb.decodeSrcDest()));
        }
    }
    void Core::ldob(MEMAFormat const& ma) noexcept {
        prepareRegisters(ma);
        ldob(_temporary0, getRegister(ma.decodeSrcDest()));
    }
    void Core::ldob(MEMBFormat const& mb) noexcept {
        prepareRegisters(mb);
        ldob(_temporary0, getRegister(mb.decodeSrcDest()));
    }
    void Core::stob(MEMAFormat const& ma) noexcept {
        // single instruction length
        prepareRegisters(ma);
        stob(getRegister(ma.decodeSrcDest()), _temporary0);
    }
    void Core::stob(MEMBFormat const& mb) noexcept {
        prepareRegisters(mb);
        stob(getRegister(mb.decodeSrcDest()), _temporary0);
    }
    void Core::ldos(MEMAFormat const& ma) noexcept {
        prepareRegisters(ma);
        ldos(_temporary0, getRegister(ma.decodeSrcDest()));
    }
    void Core::ldos(MEMBFormat const& mb) noexcept {
        prepareRegisters(mb);
        ldos(_temporary0, getRegister(mb.decodeSrcDest()));
    }
    void Core::stos(MEMAFormat const& ma) noexcept {
        prepareRegisters(ma);
        stos(getRegister(ma.decodeSrcDest()), _temporary0);
    }
    void Core::stos(MEMBFormat const& mb) noexcept {
        prepareRegisters(mb);
        stos(getRegister(mb.decodeSrcDest()), _temporary0);
    }
    void Core::ldib(MEMAFormat const& ma) noexcept {
        prepareRegisters(ma);
        ldib(_temporary0, getRegister(ma.decodeSrcDest()));
    }
    void Core::ldib(MEMBFormat const& mb) noexcept {
        prepareRegisters(mb);
        ldib(_temporary0, getRegister(mb.decodeSrcDest()));
    }
    void Core::stib(MEMAFormat const& ma) noexcept {
        prepareRegisters(ma);
        stib(getRegister(ma.decodeSrcDest()), _temporary0);
    }
    void Core::stib(MEMBFormat const& mb) noexcept {
        prepareRegisters(mb);
        stib(getRegister(mb.decodeSrcDest()), _temporary0);
    }
    void Core::ldis(MEMAFormat const& ma) noexcept {
        prepareRegisters(ma);
        ldis(_temporary0, getRegister(ma.decodeSrcDest()));
    }
    void Core::ldis(MEMBFormat const& mb) noexcept {
        prepareRegisters(mb);
        ldis(_temporary0, getRegister(mb.decodeSrcDest()));
    }
    void Core::stis(MEMAFormat const& ma) noexcept {
        // single instruction length
        prepareRegisters(pa);
        stis(getRegister(ma.decodeSrcDest()), _temporary0);
    }
    void Core::stis(MEMBFormat const& mb) noexcept {
        // single instruction length
        prepareRegisters(mb);
        stis(getRegister(mb.decodeSrcDest()), _temporary0);
    }
    void Core::ldl(MEMAFormat const& ma) noexcept {
        prepareRegisters(ma);
        ldl(_temporary0, ma.decodeSrcDest());
    }
    void Core::stl(MEMAFormat const& ma) noexcept {
        prepareRegisters(ma);
        stl(ma.decodeSrcDest(), _temporary0);
    }
    void Core::ldl(MEMBFormat const& ma) noexcept {
        prepareRegisters(ma);
        ldl(_temporary0, ma.decodeSrcDest());
    }
    void Core::stl(MEMBFormat const& ma) noexcept {
        prepareRegisters(ma);
        stl(ma.decodeSrcDest(), _temporary0);
    }
    void Core::ldt(MEMAFormat const& ma) noexcept {
        prepareRegisters(ma);
        ldt(_temporary0, ma.decodeSrcDest());
    }
    void Core::stt(MEMAFormat const& ma) noexcept {
        prepareRegisters(ma);
        stt(ma.decodeSrcDest(), _temporary0);
    }
    void Core::ldt(MEMBFormat const& ma) noexcept {
        prepareRegisters(ma);
        ldt(_temporary0, ma.decodeSrcDest());
    }
    void Core::stt(MEMBFormat const& ma) noexcept {
        prepareRegisters(ma);
        stt(ma.decodeSrcDest(), _temporary0);
    }
    void Core::ldq(MEMAFormat const& ma) noexcept {
        prepareRegisters(ma);
        ldq(_temporary0, ma.decodeSrcDest());
    }
    void Core::stq(MEMAFormat const& ma) noexcept {
        prepareRegisters(ma);
        stq(ma.decodeSrcDest(), _temporary0);
    }
    void Core::ldq(MEMBFormat const& ma) noexcept {
        prepareRegisters(ma);
        ldq(_temporary0, ma.decodeSrcDest());
    }
    void Core::stq(MEMBFormat const& ma) noexcept {
        prepareRegisters(ma);
        stq(ma.decodeSrcDest(), _temporary0);
    }
    void Core::st(MEMAFormat const& ma) noexcept {
        prepareRegisters(ma);
        st(getRegister(ma.decodeSrcDest()), _temporary0);
    }
    void Core::st(MEMBFormat const& mb) noexcept {
        prepareRegisters(mb);
        st(getRegister(mb.decodeSrcDest()), _temporary0);
    }
    void Core::ld(MEMAFormat const& ma) noexcept {
        prepareRegisters(ma);
        ld(_temporary0, getRegister(ma.decodeSrcDest()));
    }
    void Core::ld(MEMBFormat const& mb) noexcept {
        prepareRegisters(mb);
        ld(_temporary0, getRegister( mb.decodeSrcDest()));
    }
    void Core::lda(MEMAFormat const& ma) noexcept {
        prepareRegisters(ma);
        lda(_temporary0, getRegister(ma.decodeSrcDest()));
    }
    void Core::lda(MEMBFormat const& mb) noexcept {
        prepareRegisters(mb);
        lda(_temporary0, getRegister( mb.decodeSrcDest()));
    }
    void Core::bx(MEMAFormat const& ma) noexcept {
        prepareRegisters(ma);
        bx(_temporary0);
    }
    void Core::bx(MEMBFormat const& ma) noexcept {
        prepareRegisters(ma);
        bx(_temporary0);
    }
    void Core::callx(MEMAFormat const& ma) noexcept {
        prepareRegisters(ma);
        callx(_temporary0);
    }
    void Core::callx(MEMBFormat const& ma) noexcept {
        prepareRegisters(ma);
        callx(_temporary0);
    }
//		} else if (desc.isCtrl()) {
//			switch (desc) {
//#define Y(kind) case Opcode:: kind : kind ( inst._ctrl.decodeDisplacement() ) ; break
//                Y(b);
//                Y(call);
//                Y(bal);
//#define X(kind, __) Y(b ## kind);
//#include "conditional_kinds.def"
//#undef X
//#undef Y
//				default: 
//					generateFault(OperationFaultSubtype::InvalidOpcode); 
//                    return;
//			}
//		} else if (desc.isCobr()) {
//			switch(desc) {
//#define Y(kind) case Opcode:: kind : kind ( selectRegister(inst._cobr.decodeSrc1(), _temporary0), getRegister(inst._cobr.decodeSrc2()), inst._cobr.decodeDisplacement()); break
//#define X(kind) Y( cmpob ## kind ) 
//				Y(bbc); Y(bbs); X(e);
//				X(ge);  X(l);   X(ne);
//				X(le);
//#undef X
//#define X(kind, __) \
//				Y(cmpib ## kind ); \
//				case Opcode:: test ## kind : test ## kind (selectRegister(inst._cobr.decodeSrc1(), _temporary0)); break;
//#include "conditional_kinds.def"
//#undef X
//#undef Y
//				default: 
//					generateFault(OperationFaultSubtype::InvalidOpcode); 
//                    return;
//			}
	void Core::dispatch(const Instruction& inst) noexcept {
        std::visit([this, &inst](auto&& value) {
                    using K = typename std::decay_t<decltype(value)>;
                    if constexpr (!std::is_same_v<K, Opcode::UndefinedDescription>) {
                        inst.visit([this, &value](auto&& code) { 
                                    if constexpr (std::is_same_v<typename K::TargetKind, std::decay_t<decltype(code)> >) {
                                        (this ->* value.Signature)(code); 
                                    } else {
                                        // bad bad bad things have happened!
                                        throw "Illegal instruction combination!";
                                    }
                                });
                    } else {
                        generateFault(OperationFaultSubtype::InvalidOpcode); 
                    }
                },
                Opcode::determineTargetOpcode(inst));
    }
} // end namespace i960
