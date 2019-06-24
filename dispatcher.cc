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
	void Core::dispatch(const Instruction& inst) noexcept {
        std::visit([this, &inst](auto&& value) {
                using K = typename std::decay_t<decltype(value)>;
                if constexpr (!std::is_same_v<K, Opcode::UndefinedDescription>) {
                    inst.visit([this, &value](auto&& code) { 
                                if constexpr (std::is_same_v<typename K::TargetKind, std::decay_t<decltype(code)> >) {
                                    (this ->* value.Signature)(code); 
                                } else {
                                    throw "Illegal instruction combination!";
                                }
                            });
                } else {
                    throw "Undefined operation!";
                }
                },
                Opcode::determineTargetOpcode(inst));
//		auto selectRegister = [this](const Operand& operand, NormalRegister& imm) -> NormalRegister& {
//			if (operand.isLiteral()) {
//				imm.set(operand.getValue());
//				return imm;
//			} else {
//				return getRegister(operand);
//			}
//		};
//		if (auto desc = Opcode::getDescription(inst); desc.isUndefined()) {
//			generateFault(OperationFaultSubtype::InvalidOpcode);
//		} else if (desc.hasZeroArguments()) {
//			// these operations require no further decoding effort so short
//			// circuit them :D
//			switch (desc) {
//#define Y(kind) case Opcode:: kind :  kind () ; break;
//#define X(kind, __) Y(fault ## kind );
//				Y(mark);  Y(fmark); Y(flushreg); 
//				Y(syncf); Y(ret);   Y(inten);
//				Y(intdis);
//#include "conditional_kinds.def"
//#undef X
//#undef Y
//                default: 
//                    generateFault(OperationFaultSubtype::InvalidOpcode); 
//                    break;
//			}
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
//				Op2Arg(spanbit);  Op2Arg(scanbit);  Op2Arg(opnot);
//				Op2Arg(chkbit);   Op2Arg(cmpo);     Op2Arg(cmpi);    
//				Op2Arg(concmpo);  Op2Arg(concmpi);
//
//				Op3Arg(notbit);  Op3Arg(clrbit);   Op3Arg(notor);   Op3Arg(opand);
//				Op3Arg(andnot);  Op3Arg(setbit);   Op3Arg(notand);  Op3Arg(opxor);
//				Op3Arg(opor);    Op3Arg(nor);      Op3Arg(xnor);    Op3Arg(ornot);
//				Op3Arg(nand);    Op3Arg(alterbit); Op3Arg(shro);    Op3Arg(shrdi);
//				Op3Arg(shri);    Op3Arg(shlo);     Op3Arg(rotate);  Op3Arg(shli);
//				Op3Arg(addc);    Op3Arg(subc);     Op3Arg(atmod);   Op3Arg(atadd);
//				Op3Arg(modify);  Op3Arg(extract);  Op3Arg(modac);   Op3Arg(modtc);
//				Op3Arg(modpc);   Op3Arg(addo);     Op3Arg(addi);    Op3Arg(subo);
//				Op3Arg(subi);    Op3Arg(cmpinco);  Op3Arg(cmpinci); Op3Arg(cmpdeco);
//				Op3Arg(cmpdeci); Op3Arg(mulo);     Op3Arg(muli);    Op3Arg(remo);
//				Op3Arg(remi);    Op3Arg(divo);     Op3Arg(divi);    Op3Arg(dcctl);
//
//
//				Y(scanbyte, (src1, src2));
//                Y(cmpos, (src1, src2));
//                Y(cmpis, (src1, src2));
//                Y(cmpob, (src1, src2));
//                Y(cmpib, (src1, src2));
//                Y(bswap, (src1, src2));
//				Y(mov, (reg.decodeSrc1(), opSrcDest));
//				Y(movl, (reg.decodeSrc1(), opSrcDest));
//				Y(movt, (reg.decodeSrc1(), opSrcDest));
//				Y(movq, (reg.decodeSrc1(), opSrcDest));
//				Y(calls, (src1));
//				Y(eshro, (src1, opSrc2, srcDest));
//				Y(emul, (src1, src2, opSrcDest));
//				Y(ediv, (src1, opSrc2, opSrcDest));
//#define X(kind, __) \
//				Op3Arg(addo ## kind); \
//				Op3Arg(addi ## kind); \
//				Op3Arg(subo ## kind); \
//				Op3Arg(subi ## kind); \
//				Op3Arg(sel ## kind); 
//#include "conditional_kinds.def"
//#undef X
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
	}
} // end namespace i960
