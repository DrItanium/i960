#include "types.h"
#include "core.h"
#include "opcodes.h"
#include <map>
#include <functional>

#define __DEFAULT_THREE_ARGS__ Core::SourceRegister src1, Core::SourceRegister src2, Core::DestinationRegister dest
#define __DEFAULT_DOUBLE_WIDE_THREE_ARGS__ const DoubleRegister& src1, const DoubleRegister& src2, DoubleRegister& dest
#define __DEFAULT_TWO_ARGS__ Core::SourceRegister src, Core::DestinationRegister dest
#define __DEFAULT_DOUBLE_WIDE_TWO_ARGS__ const DoubleRegister& src, DoubleRegister& dest
namespace i960 {
	// while slower than doing direct wiring, this design decouples names and
	// bodies from each other.
#define o(name, code, kind) \
	template<> \
	const decltype(auto) Core::CorrespondingFunction< Opcode:: name > = std::mem_fn( & Core:: name );
#define reg(name, code) o(name, code, Reg)
#define cobr(name, code) o(name, code, Cobr) 
#define mem(name, code) o(name, code, Mem) 
#define ctrl(name, code) o(name, code, Ctrl)
#include "opcodes.def"
#undef reg
#undef cobr
#undef mem
#undef ctrl
#undef o
	void Core::dispatch(const Instruction& inst) noexcept {
		if (auto desc = Opcode::getDescription(inst); desc.isUndefined()) {
		 	// TODO raise fault
			throw "illegal instruction";
		} else if (Opcode::hasZeroArguments(desc)) {
			// these operations require no further decoding effort so short
			// circuit them :D
			switch (desc) {
				case Opcode::mark:
					invokeOperation<Opcode::mark>();
					break;
				case Opcode::fmark:
					invokeOperation<Opcode::fmark>();
					break;
				case Opcode::flushreg:
					invokeOperation<Opcode::flushreg>();
					break;
				case Opcode::syncf:
					invokeOperation<Opcode::syncf>();
					break;
				case Opcode::ret:
					invokeOperation<Opcode::ret>();
					break;
#define X(kind) \
			case Opcode:: fault ## kind : invokeOperation<Opcode:: fault ## kind > () ; break;
#include "conditional_kinds.def"
#undef X
				default:
					throw "unimplemented instruction";
			}
			
		} else if (desc.isReg()) {
			dispatch(inst._reg);
		} else if (desc.isMem()) {
			dispatch(inst._mem);
		} else if (desc.isCtrl()) {
			dispatch(inst._ctrl);
		} else if (desc.isCobr()) {
			dispatch(inst._cobr);
		} else {
			// this is another anomaly case where the 
			throw "unimplemented instruction";
		}
	}
	void Core::dispatch(const Instruction::CTRLFormat& i) noexcept {
		Integer displacement = i._displacement;
		switch (i._opcode) {
			case Opcode::b: 
				invokeOperation<Opcode::b>(displacement);
				break;
			case Opcode::call: 
				invokeOperation<Opcode::call>(displacement);
				break;
			case Opcode::bal:
				invokeOperation<Opcode::bal>(displacement);
				break;
#define X(kind) \
			case Opcode:: b ## kind : invokeOperation< Opcode:: b ## kind > (displacement) ; break; 
#include "conditional_kinds.def"
#undef X
			default:
#warning "Generate illegal instruction fault"
									  throw "Illegal Instruction";
		}
	}
	void Core::dispatch(const Instruction::COBRFormat& i) noexcept {
		NormalRegister immediateStorage;
		auto displacement = i._displacement;
		NormalRegister& src1 = i.src1IsLiteral() ? immediateStorage : getRegister(i._source1);
		if (i.src1IsLiteral()) {
			immediateStorage.set<ByteOrdinal>(i._source1);
		}
		auto& src2 = getRegister(i._source2);
		switch(i._opcode) {
#define X(kind) case Opcode:: test ## kind : invokeOperation<Opcode:: test ## kind > (src1); break;
#include "conditional_kinds.def"
#undef X
			case Opcode::bbc:
				invokeOperation<Opcode::bbc>(src1, src2, displacement);
				break;
			case Opcode::bbs:
				invokeOperation<Opcode::bbs>(src1, src2, displacement);
				break;
#define X(kind) case Opcode:: cmpob ## kind : invokeOperation<Opcode:: cmpob ## kind > ( src1, src2, displacement ) ; break;
				X(e)
				X(ge)
				X(l)
				X(ne)
				X(le)
#undef X
#define X(kind) case Opcode:: cmpib ## kind : invokeOperation<Opcode:: cmpib ## kind > ( src1, src2, displacement ) ; break;
#include "conditional_kinds.def"
#undef X
			default:
#warning "generate illegal instruction fault"
				throw "illegal instruction!";
		}
	}
	void Core::dispatch(const Instruction::MemFormat& i) noexcept {
		NormalRegister immediateStorage;
		auto srcDestIndex = i.getSrcDestIndex();
		auto srcDest = getRegister(srcDestIndex);
		if (i.isMemAFormat()) {
			auto ma = i._mema;
			using E = std::decay_t<decltype(ma)>;
			auto offset = ma._offset;
			auto mode = ma._md == 0 ? E::AddressingModes::Offset : E::AddressingModes::Abase_Plus_Offset;
			auto abase = getRegister(ma._abase);
			immediateStorage.set<Ordinal>(mode == E::AddressingModes::Offset ? offset : (abase.get<Ordinal>() + offset));
		} else {
			auto mb = i._memb;
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
		switch(i.getOpcode()) {
			case Opcode::ldob:
				invokeOperation<Opcode::ldob>(immediateStorage, srcDest);
				break;
			case Opcode::stob:
				invokeOperation<Opcode::stob>(srcDest, immediateStorage);
				break;
			case Opcode::bx:
				invokeOperation<Opcode::bx>(immediateStorage);
				break;
			case Opcode::balx:
				invokeOperation<Opcode::balx>(immediateStorage, srcDest);
				break;
			case Opcode::callx:
				invokeOperation<Opcode::callx>(immediateStorage);
				break;
			case Opcode::ldos:
				invokeOperation<Opcode::ldos>(immediateStorage, srcDest);
				break;
			case Opcode::stos:
				invokeOperation<Opcode::stos>(srcDest, immediateStorage);
				break;
			case Opcode::lda:
				invokeOperation<Opcode::lda>(immediateStorage, srcDest);
				break;
			case Opcode::ld:
				invokeOperation<Opcode::ld>(immediateStorage, srcDest);
				break;
			case Opcode::st:
				invokeOperation<Opcode::st>(srcDest, immediateStorage);
				break;
			case Opcode::ldl:
				invokeOperation<Opcode::ldl>(immediateStorage, srcDestIndex);
				break;
			case Opcode::stl:
				invokeOperation<Opcode::stl>(srcDestIndex, immediateStorage);
				break;
			case Opcode::ldt:
				invokeOperation<Opcode::ldt>(immediateStorage, srcDestIndex);
				break;
			case Opcode::stt:
				invokeOperation<Opcode::stt>(srcDestIndex, immediateStorage);
				break;
			case Opcode::ldq:
				invokeOperation<Opcode::ldq>(immediateStorage, srcDestIndex);
				break;
			case Opcode::stq:
				invokeOperation<Opcode::stq>(srcDestIndex, immediateStorage);
				break;
			case Opcode::ldib:
				invokeOperation<Opcode::ldib>(immediateStorage, srcDest);
				break;
			case Opcode::stib:
				invokeOperation<Opcode::stib>(srcDest, immediateStorage);
				break;
			case Opcode::ldis:
				invokeOperation<Opcode::ldis>(immediateStorage, srcDest);
				break;
			case Opcode::stis:
				invokeOperation<Opcode::stis>(srcDest, immediateStorage);
				break;
			default:
#warning "generate illegal instruction fault"
				throw "illegal instruction!";
		}
	}
	void Core::dispatch(const Instruction::REGFormat& i) noexcept {
		NormalRegister imm1;
		NormalRegister imm2;
		NormalRegister imm3;
		NormalRegister& src1 = i.src1IsLiteral() ? imm1 : getRegister(i._source1);
		if (i.src1IsLiteral()) {
			imm1.set(i.src1ToIntegerLiteral());
		}
		NormalRegister& src2 = i.src2IsLiteral() ? imm2 : getRegister(i._source2);
		if (i.src2IsLiteral()) {
			imm2.set(i.src2ToIntegerLiteral());
		}
		NormalRegister& srcDest = i.srcDestIsLiteral() ? imm3 : getRegister(i._src_dest);
#warning "It is impossible for m3 to be set when srcDest is used as a dest, error out before hand"
		switch(i.getOpcode()) {
#define Standard3ArgOp(kind) case Opcode:: kind : invokeOperation<Opcode:: kind> ( src1, src2, srcDest ) ; break
#define Standard2ArgOp(kind) case Opcode:: kind : invokeOperation<Opcode:: kind> ( src1, srcDest ) ; break
#define Standard3ArgOpIO(kind) Standard3ArgOp(kind ## o); Standard3ArgOp(kind ## i)
#define Standard2ArgOpIO(kind) Standard2ArgOp(kind ## o); Standard2ArgOp(kind ## i)
#define Standard2SourceOp(kind) case Opcode:: kind : invokeOperation<Opcode:: kind > ( src1, src2) ; break
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
            case Opcode::movl: 
				invokeOperation<Opcode::movl>(i._source1, i._source2);
                break;
            case Opcode::movt: 
				invokeOperation<Opcode::movt>(i._source1, i._source2);
                break;
            case Opcode::movq: 
				invokeOperation<Opcode::movq>(i._source1, i._source2);
                break;
			Standard2ArgOp(spanbit);
			Standard2ArgOp(scanbit);
			Standard3ArgOp(modac);
			Standard3ArgOp(modify);
			Standard3ArgOp(extract);
			Standard3ArgOp(modtc);
			Standard3ArgOp(modpc);
			case Opcode::calls: 
				invokeOperation<Opcode::calls>(src1);
				break;
#warning "Emul not impl'd as it is a special form"
#warning "Ediv not impl'd as it is a special form"
			Standard3ArgOpIO(mul);
			Standard3ArgOpIO(rem);
			Standard3ArgOpIO(div);
#warning "Modi not impl'd as it is a special form"
#undef Standard3ArgOp
#undef Standard2ArgOp
#undef Standard2SourceOp
			default:
#warning "generate illegal instruction fault"
				throw "illegal instruction!";
		}
	} 
    template<typename T, typename K>
    void optionalCheck(const std::optional<T>& src1, const std::optional<K>& dest) {
            if (!src1.has_value()) {
                throw "Something really bad happened! No src";
            }
            if (!dest.has_value()) {
                throw "Something really bad happened! No dest";
            }
    }
    template<typename T, typename K>
    void optionalCheck(const std::optional<T>& src1, const std::optional<T>& src2, const std::optional<K>& dest) {
            if (!src1.has_value()) {
                throw "Something really bad happened! No src1";
            }
            if (!src2.has_value()) {
                throw "Something really bad happened! No src2";
            }
            if (!dest.has_value()) {
                throw "Something really bad happened! No dest";
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
#undef __DEFAULT_TWO_ARGS__
#undef __DEFAULT_DOUBLE_WIDE_TWO_ARGS__
#undef __DEFAULT_THREE_ARGS__
#undef __DEFAULT_DOUBLE_WIDE_THREE_ARGS__
