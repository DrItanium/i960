#include "types.h"
#include "core.h"
#include "opcodes.h"
#include <map>
#include <functional>

namespace i960 {
	void Core::dispatch(const Instruction& inst) noexcept {
		if (auto desc = Opcode::getDescription(inst); desc.isUndefined()) {
			// TODO raise fault
			throw "illegal instruction";
		} else if (desc.hasZeroArguments()) {
			// these operations require no further decoding effort so short
			// circuit them :D
			switch (desc) {
				case Opcode::mark:     mark(); break;
				case Opcode::fmark:    fmark(); break;
				case Opcode::flushreg: flushreg(); break;
				case Opcode::syncf:    syncf(); break;
				case Opcode::ret:      ret(); break;
				case Opcode::inten:    inten(); break;
				case Opcode::intdis:   intdis(); break;
#define X(kind) \
				case Opcode:: fault ## kind : \
					fault ## kind () ; \
					break;
#include "conditional_kinds.def"
#undef X
				default:
											  throw "unimplemented instruction";
			}
		} else if (desc.isReg()) {
			auto reg = inst._reg;
			NormalRegister imm1;
			NormalRegister imm2;
			NormalRegister imm3;
			auto src1Ind = reg._source1;
			auto src2Ind = reg._source2;
			auto src3Ind = reg._src_dest;
			NormalRegister& src1 = reg.src1IsLiteral() ? imm1 : getRegister(src1Ind);
			if (reg.src1IsLiteral()) {
				imm1.set(reg.src1ToIntegerLiteral());
			}
			NormalRegister& src2 = reg.src2IsLiteral() ? imm2 : getRegister(src2Ind);
			if (reg.src2IsLiteral()) {
				imm2.set(reg.src2ToIntegerLiteral());
			}
			// TODO It is impossible for m3 to be set when srcDest is used as a dest, error out before hand
			NormalRegister& srcDest = reg.srcDestIsLiteral() ? imm3 : getRegister(src3Ind);
			switch(desc) {
#define GenericInvoke(kind, args) case Opcode:: kind : kind args ; break 
#define Standard3ArgOp(kind) GenericInvoke(kind, (src1, src2, srcDest))
#define Standard2ArgOp(kind) GenericInvoke(kind, (src1, srcDest))
#define Standard3ArgOpIO(kind) Standard3ArgOp(kind ## o); Standard3ArgOp(kind ## i)
#define Standard2ArgOpIO(kind) Standard2ArgOp(kind ## o); Standard2ArgOp(kind ## i)
#define Standard2SourceOp(kind) GenericInvoke(kind, (src1, src2))
				Standard3ArgOp(notbit);
				Standard3ArgOp(clrbit);
				Standard3ArgOp(notor);
				Standard3ArgOp(opand);
				Standard3ArgOp(andnot);
				Standard3ArgOp(setbit);
				Standard3ArgOp(notand);
				Standard3ArgOp(opxor);
				Standard3ArgOp(opor);
				Standard3ArgOp(nor);
				Standard3ArgOp(xnor);
				Standard2ArgOp(opnot);
				Standard3ArgOp(ornot);
				Standard3ArgOp(nand);
				Standard3ArgOp(alterbit);
				Standard3ArgOpIO(add);
				Standard3ArgOpIO(sub);
				Standard3ArgOp(shro);
				Standard3ArgOp(shrdi);
				Standard3ArgOp(shri);
				Standard3ArgOp(shlo);
				Standard3ArgOp(rotate);
				Standard3ArgOp(shli);
				Standard2ArgOpIO(cmp);
				Standard2ArgOpIO(concmp);
				Standard3ArgOpIO(cmpinc);
				Standard3ArgOpIO(cmpdec);
				Standard2SourceOp(scanbyte);
				Standard2ArgOp(chkbit);
				Standard3ArgOp(addc);
				Standard3ArgOp(subc);
				Standard2ArgOp(mov);
				Standard3ArgOp(atmod);
				Standard3ArgOp(atadd);
				GenericInvoke(movl, (src1Ind, src2Ind));
				GenericInvoke(movt, (src1Ind, src2Ind));
				GenericInvoke(movq, (src1Ind, src2Ind));
				Standard2ArgOp(spanbit);
				Standard2ArgOp(scanbit);
				Standard3ArgOp(modac);
				Standard3ArgOp(modify);
				Standard3ArgOp(extract);
				Standard3ArgOp(modtc);
				Standard3ArgOp(modpc);
				Standard3ArgOpIO(mul);
				Standard3ArgOpIO(rem);
				Standard3ArgOpIO(div);
				GenericInvoke(calls, (src1));
#define X(kind) \
				Standard3ArgOp(addo ## kind); \
				Standard3ArgOp(addi ## kind); \
				Standard3ArgOp(subo ## kind); \
				Standard3ArgOp(subi ## kind); \
				Standard3ArgOp(sel ## kind); 
#include "conditional_kinds.def"
#undef X
#warning "Emul not impl'd as it is a special form"
#warning "Ediv not impl'd as it is a special form"
#warning "Modi not impl'd as it is a special form"
#undef Standard3ArgOp
#undef Standard2ArgOp
#undef Standard2SourceOp
#undef GenericInvoke
				default:
#warning "generate illegal instruction fault"
				throw "illegal instruction!";
			}
		} else if (desc.isMem()) {
			auto mem = inst._mem;
			NormalRegister immediateStorage;
			auto srcDestIndex = mem.getSrcDestIndex();
			auto srcDest = getRegister(srcDestIndex);
			if (mem.isMemAFormat()) {
				auto ma = mem._mema;
				using E = std::decay_t<decltype(ma)>;
				auto offset = ma._offset;
				auto abase = getRegister(ma._abase);
				immediateStorage.set<Ordinal>(ma.isOffsetAddressingMode() ?  offset : offset + abase.get<Ordinal>());
			} else {
				auto mb = mem._memb;
				using K = std::decay_t<decltype(mb)>;
				using E = K::AddressingModes;
				auto index = mb._index;
				auto scale = mb.getScaleFactor();
				auto mode = mb.getAddressingMode();
				auto displacement = mb.has32bitDisplacement() ? getFullDisplacement() : 0;
				auto abase = getRegister(mb._abase);
				switch (mode) {
					case E::Abase:
						immediateStorage.move(abase);
						break;
					case E::IP_Plus_Displacement_Plus_8:
						immediateStorage.set<Ordinal>(_instructionPointer + displacement + 8);
						break;
					case E::Abase_Plus_Index_Times_2_Pow_Scale:
						immediateStorage.set<Ordinal>(abase.get<Ordinal>() + index * scale);
						break;
					case E::Displacement:
						immediateStorage.set(displacement);
						break;
					case E::Abase_Plus_Displacement:
						immediateStorage.set(displacement + abase.get<Ordinal>());
						break;
					case E::Index_Times_2_Pow_Scale_Plus_Displacement:
						immediateStorage.set(index * scale + displacement);
						break;
					case E::Abase_Plus_Index_Times_2_Pow_Scale_Plus_Displacement:
						immediateStorage.set(abase.get<Ordinal>() + index * scale + displacement);
						break;
					default:
#warning "Fault on illegal mode!"
						throw "Illegal mode!";
				}
			}
			// opcode does not differentiate between mema and memb, another bit
			// does, so the actual arguments differ, not the set of actions
			switch(desc) {
#define Y(kind, a, b) \
		case Opcode:: kind : \
				kind ( a , b ) ; \
				break;
#define Z(kind, a) \
		case Opcode:: kind : \
				kind ( a ) ; \
				break;
#define LDP(suffix) \
				Y(ld ## suffix , immediateStorage, srcDest); \
				Y(st ## suffix , srcDest, immediateStorage );
#define WLDP(suffix) \
				Y(ld ## suffix, immediateStorage, srcDestIndex ); \
				Y(st ## suffix, srcDestIndex, immediateStorage ); 
				LDP(ob);
				LDP(os);
				LDP(ib);
				LDP(is);
				WLDP(l);
				WLDP(t);
				WLDP(q);
				Y(ld, immediateStorage, srcDest);
				Y(st, srcDest, immediateStorage);
				Y(lda, immediateStorage, srcDest);
				Y(balx, immediateStorage, srcDest);
				Z(bx, immediateStorage);
				Z(callx, immediateStorage);
#undef WLDP
#undef LDP
#undef Y
#undef Z
				default:
#warning "generate illegal instruction fault"
					throw "illegal instruction!";
			}
		} else if (desc.isCtrl()) {
			auto ctrl = inst._ctrl;
			Integer displacement = ctrl._displacement;
			switch (desc.getOpcode()) {
#define Y(kind) \
				case Opcode:: kind : \
					kind ( displacement ) ; \
					break;
#define X(kind) Y(b ## kind) 
				Y(b)
				Y(call)
				Y(bal)
#include "conditional_kinds.def"
#undef X
#undef Y
				default:
					throw "Illegal Instruction";
#warning "Generate illegal instruction fault"
			}
		} else if (desc.isCobr()) {
			auto cobr = inst._cobr;
			NormalRegister immediateStorage;
			auto displacement = cobr._displacement;
			auto& src1 = cobr.src1IsLiteral() ? immediateStorage : getRegister(cobr._source1);
			if (cobr.src1IsLiteral()) {
				immediateStorage.set<ByteOrdinal>(cobr._source1);
			}
			auto& src2 = getRegister(cobr._source2);
			switch(desc) {
#define Y(kind) \
				case Opcode:: kind : \
					kind ( src1, src2, displacement ); \
				break
#define X(kind) Y( cmpob ## kind ) 
				Y(bbc);
				Y(bbs);
				X(e);
				X(ge);
				X(l);
				X(ne);
				X(le);
#undef X
#define X(kind) \
				Y(cmpib ## kind ); \
				case Opcode:: test ## kind : test ## kind (src1); break;
#include "conditional_kinds.def"
#undef X
#undef Y
				default:
#warning "generate illegal instruction fault"
											 throw "illegal instruction!";
			}
		} else {
			// this is another anomaly case where the 
			throw "unimplemented instruction";
		}
	}
    Integer Core::getFullDisplacement() noexcept {
        auto addr = _instructionPointer + 4;
        union {
            Ordinal _ord;
            Integer _int;
        } conv;
        conv._ord = load(addr);
        return conv._int;
    }

} // end namespace i960
