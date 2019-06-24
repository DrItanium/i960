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
            using TargetKind = std::variant<MEMAFormat, MEMBFormat>; \
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
		auto selectRegister = [this](const Operand& operand, NormalRegister& imm) -> NormalRegister& { \
			if (operand.isLiteral()) { \
				imm.set(operand.getValue()); \
				return imm; \
			} else { \
				return getRegister(operand); \
			} \
		}; \
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
		auto selectRegister = [this](const Operand& operand, NormalRegister& imm) -> NormalRegister& { \
			if (operand.isLiteral()) { \
				imm.set(operand.getValue()); \
				return imm; \
			} else { \
				return getRegister(operand); \
			} \
		}; \
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
		auto selectRegister = [this](const Operand& operand, NormalRegister& imm) -> NormalRegister& { \
			if (operand.isLiteral()) { \
				imm.set(operand.getValue()); \
				return imm; \
			} else { \
				return getRegister(operand); \
			} \
		}; \
        auto opSrc2 = fmt.decodeSrc2(); \
        NormalRegister& src1 = selectRegister(fmt.decodeSrc1(), _temporary0); \
        NormalRegister& src2 = selectRegister(opSrc2, _temporary1); \
        op (src1, src2); \
    }
                X(scanbyte); X(cmpos); X(cmpis);
                X(cmpob);    X(cmpib); X(bswap);
#undef X
//		auto selectRegister = [this](const Operand& operand, NormalRegister& imm) -> NormalRegister& {
//			if (operand.isLiteral()) {
//				imm.set(operand.getValue());
//				return imm;
//			} else {
//				return getRegister(operand);
//			}
//		};
//		} else if (desc.isReg()) {
//			auto reg = inst._reg;
//			auto opSrc2 = reg.decodeSrc2();
//			auto opSrcDest = reg.decodeSrcDest();
//			NormalRegister& src1 = selectRegister(reg.decodeSrc1(), _temporary0);
//			NormalRegister& src2 = selectRegister(opSrc2, _temporary1);
//			// TODO It is impossible for m3 to be set when srcDest is used as a dest, error out before hand
//			NormalRegister& srcDest = selectRegister(opSrcDest, _temporary2);
//			switch(desc) {
//#define Y(kind, args) case Opcode:: kind : kind args ; break 
//#define Op3Arg(kind) Y(kind, (src1, src2, srcDest))
//#define Op2Arg(kind) Y(kind, (src1, srcDest))
//
//				Y(mov, (reg.decodeSrc1(), opSrcDest));
//				Y(movl, (reg.decodeSrc1(), opSrcDest));
//				Y(movt, (reg.decodeSrc1(), opSrcDest));
//				Y(movq, (reg.decodeSrc1(), opSrcDest));
//				Y(calls, (src1));
//				Y(eshro, (src1, opSrc2, srcDest));
//				Y(emul, (src1, src2, opSrcDest));
//				Y(ediv, (src1, opSrc2, opSrcDest));
//#undef Op3Arg
//#undef Op2Arg
//#undef Y
//				default: 
//                    generateFault(OperationFaultSubtype::InvalidOpcode); 
//                    break;
//			}
//		} else if (desc.isMem()) {
//			auto mem = inst._mem;
//            InstructionLength length = InstructionLength::Single;
//			if (mem.isMemAFormat()) {
//				auto ma = mem._mema;
//				_temporary0.set<Ordinal>(ma._offset + ma.isOffsetAddressingMode() ?  0 : getRegister(ma.decodeAbase()).get<Ordinal>());
//			} else {
//				auto mb = mem._memb;
//				using E = std::decay_t<decltype(mb)>::AddressingModes;
//				auto index = mb._index;
//				auto scale = mb.getScaleFactor();
//                auto displacement = 0u;
//                if (mb.has32bitDisplacement()) {
//                    displacement = mb.get32bitDisplacement();
//                    length = InstructionLength::Double;
//                }
//				switch (auto abase = getRegister(mb.decodeAbase()); mb.getAddressingMode()) {
//					case E::Abase:
//						_temporary0.move(abase);
//						break;
//					case E::IP_Plus_Displacement_Plus_8:
//						_temporary0.set<Ordinal>(_instructionPointer + displacement + 8);
//						break;
//					case E::Abase_Plus_Index_Times_2_Pow_Scale:
//						_temporary0.set<Ordinal>(abase.get<Ordinal>() + index * scale);
//						break;
//					case E::Displacement:
//						_temporary0.set(displacement);
//						break;
//					case E::Abase_Plus_Displacement:
//						_temporary0.set(displacement + abase.get<Ordinal>());
//						break;
//					case E::Index_Times_2_Pow_Scale_Plus_Displacement:
//						_temporary0.set(index * scale + displacement);
//						break;
//					case E::Abase_Plus_Index_Times_2_Pow_Scale_Plus_Displacement:
//						_temporary0.set(abase.get<Ordinal>() + index * scale + displacement);
//						break;
//					default: 
//						generateFault(OperationFaultSubtype::InvalidOpcode); 
//                        return;
//				}
//			}
//			// opcode does not differentiate between mema and memb, another bit
//			// does, so the actual arguments differ, not the set of actions
//			switch(desc) {
//                case Opcode::balx:
//                    balx(_temporary0, getRegister(mem.decodeSrcDest()), length);
//                    break;
//
//#define Y(kind, a, b) case Opcode:: kind : kind ( a , b ) ; break;
//#define YISSD(kind) Y(kind, _temporary0, getRegister(mem.decodeSrcDest()));
//#define YSDIS(kind) Y(kind, getRegister(mem.decodeSrcDest()), _temporary0);
//#define ZIS(kind) case Opcode:: kind : kind ( _temporary0 ) ; break;
//#define LDP(suffix) \
//				YISSD(ld ## suffix); \
//				YSDIS(st ## suffix);
//#define WLDP(suffix) \
//				Y(ld ## suffix, _temporary0, mem.decodeSrcDest()); \
//				Y(st ## suffix, mem.decodeSrcDest(), _temporary0 ); 
//				LDP(ob);     LDP(os);   LDP(ib); LDP(is);
//				WLDP(l);     WLDP(t);   WLDP(q); 
//				YSDIS(st);   YISSD(ld); YISSD(lda); 
//				ZIS(bx);   ZIS(callx);
//#undef ZIS
//#undef YISSD
//#undef YSDIS 
//#undef WLDP
//#undef LDP
//#undef Y
//				default: 
//					generateFault(OperationFaultSubtype::InvalidOpcode); 
//                    return;
//			}
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
//		} else {
//			generateFault(OperationFaultSubtype::InvalidOpcode); 
//		}
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
