#include "types.h"
#include "core.h"
#include "opcodes.h"
#include <map>
#include <functional>
#include <sstream>
#include <string>

namespace i960 {
	void throwUnimplementedDescription(const Opcode::Description& desc) {
		// TODO generate illegal instruction fault
		std::stringstream message;
		message << "unimplemented operation: " << desc.getString();
		auto str = message.str();
		throw str;
	}
	void Core::dispatch(const Instruction& inst) noexcept {
		if (auto desc = Opcode::getDescription(inst); desc.isUndefined()) {
			// TODO raise fault
			throw "illegal instruction";
		} else if (desc.hasZeroArguments()) {
			// these operations require no further decoding effort so short
			// circuit them :D
			switch (desc) {
#define Y(kind) case Opcode:: kind :  kind () ; break;
				Y(mark);  Y(fmark); Y(flushreg); 
				Y(syncf); Y(ret);   Y(inten);
				Y(intdis);
#define X(kind, __) Y(fault ## kind );
#include "conditional_kinds.def"
#undef X
#undef Y
				default: throwUnimplementedDescription(desc);
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
			if (reg.srcDestIsLiteral()) {
				imm3.set(reg.srcDestToIntegerLiteral());
			}
			switch(desc) {
#define Y(kind, args) case Opcode:: kind : kind args ; break 
#define Op3Arg(kind) Y(kind, (src1, src2, srcDest))
#define Op2Arg(kind) Y(kind, (src1, srcDest))
				Op2Arg(spanbit); Op2Arg(scanbit);  Op2Arg(opnot);
				Op2Arg(chkbit);  Op2Arg(mov);      Op2Arg(cmpo);
				Op2Arg(cmpi);    Op2Arg(concmpo);  Op2Arg(concmpi);

				Op3Arg(notbit);  Op3Arg(clrbit);   Op3Arg(notor);   Op3Arg(opand);
				Op3Arg(andnot);  Op3Arg(setbit);   Op3Arg(notand);  Op3Arg(opxor);
				Op3Arg(opor);    Op3Arg(nor);      Op3Arg(xnor);    Op3Arg(ornot);
				Op3Arg(nand);    Op3Arg(alterbit); Op3Arg(shro);    Op3Arg(shrdi);
				Op3Arg(shri);    Op3Arg(shlo);     Op3Arg(rotate);  Op3Arg(shli);
				Op3Arg(addc);    Op3Arg(subc);     Op3Arg(atmod);   Op3Arg(atadd);
				Op3Arg(modify);  Op3Arg(extract);  Op3Arg(modac);   Op3Arg(modtc);
				Op3Arg(modpc);   Op3Arg(addo);     Op3Arg(addi);    Op3Arg(subo);
				Op3Arg(subi);    Op3Arg(cmpinco);  Op3Arg(cmpinci); Op3Arg(cmpdeco);
				Op3Arg(cmpdeci); Op3Arg(mulo);     Op3Arg(muli);    Op3Arg(remo);
				Op3Arg(remi);    Op3Arg(divo);     Op3Arg(divi);    Op3Arg(dcctl);


				Y(scanbyte, (src1, src2));
				Y(movl, (src1Ind, src2Ind));
				Y(movt, (src1Ind, src2Ind));
				Y(movq, (src1Ind, src2Ind));
				Y(calls, (src1));
				Y(eshro, (src1, src2Ind, srcDest));
				Y(emul, (src1, src2, src3Ind));
				Y(ediv, (src1, src2Ind, src3Ind));
#define X(kind, __) \
				Op3Arg(addo ## kind); \
				Op3Arg(addi ## kind); \
				Op3Arg(subo ## kind); \
				Op3Arg(subi ## kind); \
				Op3Arg(sel ## kind); 
#include "conditional_kinds.def"
#undef X
#undef Op3Arg
#undef Op2Arg
#undef Y
				case Opcode::bswap:
					bswap(src1, src2);
					break;
				default: 
					throwUnimplementedDescription(desc);
			}
		} else if (desc.isMem()) {
			auto mem = inst._mem;
			NormalRegister immediateStorage;
			auto srcDestIndex = mem.getSrcDestIndex();
			auto srcDest = getRegister(srcDestIndex);
			if (mem.isMemAFormat()) {
				auto ma = mem._mema;
				auto offset = ma._offset;
				auto abase = getRegister(ma._abase);
				immediateStorage.set<Ordinal>(ma.isOffsetAddressingMode() ?  offset : offset + abase.get<Ordinal>());
			} else {
				auto getFullDisplacement = [this]() {
					auto addr = _instructionPointer + 4;
					union {
						Ordinal _ord;
						Integer _int;
					} conv;
					conv._ord = load(addr);
					return conv._int;
				};
				auto mb = mem._memb;
				using K = std::decay_t<decltype(mb)>;
				using E = K::AddressingModes;
				auto index = mb._index;
				auto scale = mb.getScaleFactor();
				auto displacement = mb.has32bitDisplacement() ? getFullDisplacement() : 0;
				auto abase = getRegister(mb._abase);
				switch (mb.getAddressingMode()) {
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
#define Y(kind, a, b) case Opcode:: kind : kind ( a , b ) ; break;
#define YISSD(kind) Y(kind, immediateStorage, srcDest);
#define YSDIS(kind) Y(kind, srcDest, immediateStorage);
#define ZIS(kind) case Opcode:: kind : kind ( immediateStorage ) ; break;
#define LDP(suffix) \
				YISSD(ld ## suffix); \
				YSDIS(st ## suffix);
#define WLDP(suffix) \
				Y(ld ## suffix, immediateStorage, srcDestIndex ); \
				Y(st ## suffix, srcDestIndex, immediateStorage ); 
				LDP(ob);     LDP(os);   LDP(ib); LDP(is);
				WLDP(l);     WLDP(t);   WLDP(q); 
				YSDIS(st);   YISSD(ld); YISSD(lda); 
				YISSD(balx); ZIS(bx);   ZIS(callx);
#undef ZIS
#undef YISSD
#undef YSDIS 
#undef WLDP
#undef LDP
#undef Y
				default: throwUnimplementedDescription(desc);
			}
		} else if (desc.isCtrl()) {
			auto ctrl = inst._ctrl;
			Integer displacement = ctrl._displacement;
			switch (desc) {
#define Y(kind) \
				case Opcode:: kind : \
									 kind ( displacement ) ; \
				break;
				Y(b)
					Y(call)
					Y(bal)
#define X(kind, __) Y(b ## kind) 
#include "conditional_kinds.def"
#undef X
#undef Y
				default: throwUnimplementedDescription(desc);
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
#define Y(kind) case Opcode:: kind : kind ( src1, src2, displacement ); break
#define X(kind) Y( cmpob ## kind ) 
				Y(bbc); Y(bbs); X(e);
				X(ge);  X(l);   X(ne);
				X(le);
#undef X
#define X(kind, __) \
				Y(cmpib ## kind ); \
				case Opcode:: test ## kind : test ## kind (src1); break;
#include "conditional_kinds.def"
#undef X
#undef Y
				default: throwUnimplementedDescription(desc);
			}
		} else {
			throwUnimplementedDescription(desc);
		}
	}

} // end namespace i960
