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
		if (auto desc = Opcode::getDescription(inst); desc.isReg()) {
			dispatch(inst._reg);
		} else if (desc.isMem()) {
			dispatch(inst._mem);
		} else if (desc.isCtrl()) {
			dispatch(inst._ctrl);
		} else if (desc.isCobr()) {
			dispatch(inst._cobr);
		} else {
#warning "This throw statement should be a fault of some kind"
			throw "Illegal instruction";
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
			case Opcode::ret:
				invokeOperation<Opcode::ret>();
				break;
			case Opcode::bal:
				invokeOperation<Opcode::bal>(displacement);
				break;
#define X(kind) \
			case Opcode:: b ## kind : invokeOperation< Opcode:: b ## kind > (displacement) ; break; \
			case Opcode:: fault ## kind : invokeOperation<Opcode:: fault ## kind > () ; break;
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
		if (i.isMemAFormat()) {
			dispatch(i._mema);
		} else {
			dispatch(i._memb);
		}
	}
	void Core::dispatch(const Instruction::MemFormat::MEMAFormat& i) noexcept {
		using E = std::decay_t<decltype(i)>;
		NormalRegister immediateStorage;
		auto offset = i._offset;
		auto mode = i._md == 0 ? E::AddressingModes::Offset : E::AddressingModes::Abase_Plus_Offset;
		auto abase = getRegister(i._abase);
		immediateStorage.set<Ordinal>(mode == E::AddressingModes::Offset ? offset : (abase.get<Ordinal>() + offset));
		auto srcDest = getRegister(i._src_dest);
		switch(i._opcode) {
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
				invokeOperation<Opcode::ldl>(immediateStorage, i._src_dest);
				break;
			case Opcode::stl:
				invokeOperation<Opcode::stl>(i._src_dest, immediateStorage);
				break;
			case Opcode::ldt:
				invokeOperation<Opcode::ldt>(immediateStorage, i._src_dest);
				break;
			case Opcode::stt:
				invokeOperation<Opcode::stt>(i._src_dest, immediateStorage);
				break;
			case Opcode::ldq:
				invokeOperation<Opcode::ldq>(immediateStorage, i._src_dest);
				break;
			case Opcode::stq:
				invokeOperation<Opcode::stq>(i._src_dest, immediateStorage);
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
	void Core::dispatch(const Instruction::MemFormat::MEMBFormat& i) noexcept {
		NormalRegister immediateStorage;
		using K = std::decay_t<decltype(i)>;
		using E = K::AddressingModes;
		auto index = i._index;
		auto scale = i.getScaleFactor();
		auto mode = i.getAddressingMode();
		auto displacement = i.has32bitDisplacement() ? getFullDisplacement() : 0;
		auto srcDest = getRegister(i._src_dest);
		auto abase = getRegister(i._abase);
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
		switch(i._opcode) {
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
				invokeOperation<Opcode::ldl>(immediateStorage, i._src_dest);
				break;
			case Opcode::stl:
				invokeOperation<Opcode::stl>(i._src_dest, immediateStorage);
				break;
			case Opcode::ldt:
				invokeOperation<Opcode::ldt>(immediateStorage, i._src_dest);
				break;
			case Opcode::stt:
				invokeOperation<Opcode::stt>(i._src_dest, immediateStorage);
				break;
			case Opcode::ldq:
				invokeOperation<Opcode::ldq>(immediateStorage, i._src_dest);
				break;
			case Opcode::stq:
				invokeOperation<Opcode::stq>(i._src_dest, immediateStorage);
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
#define Standard3ArgOp(kind, fn) case Opcode:: kind : invokeOperation<Opcode:: kind> ( src1, src2, srcDest ) ; break
#define Standard2ArgOp(kind, fn) case Opcode:: kind : invokeOperation<Opcode:: kind> ( src1, srcDest ) ; break
#define Standard3ArgOpIO(kind, fn) Standard3ArgOp(kind ## o, fn ## o); Standard3ArgOp(kind ## i, fn ## i)
#define Standard2ArgOpIO(kind, fn) Standard2ArgOp(kind ## o, fn ## o); Standard2ArgOp(kind ## i, fn ## i)
#define Standard2SourceOp(kind, fn) case Opcode:: kind : invokeOperation<Opcode:: kind > ( src1, src2) ; break
			Standard3ArgOp(notbit, notbit);
            Standard3ArgOp(clrbit, clrbit);
            Standard3ArgOp(notor, notor);
			Standard3ArgOp(opand, opand);
			Standard3ArgOp(andnot, andnot);
			Standard3ArgOp(setbit, setbit);
			Standard3ArgOp(notand, notand);
			Standard3ArgOp(opxor, opxor);
			Standard3ArgOp(opor, opor);
			Standard3ArgOp(nor, nor);
			Standard3ArgOp(xnor, xnor);
			Standard2ArgOp(opnot, opnot);
			Standard3ArgOp(ornot, ornot);
			Standard3ArgOp(nand, nand);
			Standard3ArgOp(alterbit, alterbit);
			Standard3ArgOpIO(add, add);
			Standard3ArgOpIO(sub, sub);
			Standard3ArgOp(shro, shro);
			Standard3ArgOp(shrdi, shrdi);
			Standard3ArgOp(shri, shri);
			Standard3ArgOp(shlo, shlo);
			Standard3ArgOp(rotate, rotate);
			Standard3ArgOp(shli, shli);
			Standard2ArgOpIO(cmp, cmp);
			Standard2ArgOpIO(concmp, concmp);
			Standard3ArgOpIO(cmpinc, cmpinc);
			Standard3ArgOpIO(cmpdec, cmpdec);
			Standard2SourceOp(scanbyte, scanbyte);
			Standard2ArgOp(chkbit, chkbit);
			Standard3ArgOp(addc, addc);
			Standard3ArgOp(subc, subc);
			Standard2ArgOp(mov, mov);
			Standard3ArgOp(atmod, atmod);
			Standard3ArgOp(atadd, atadd);
            case Opcode::movl: 
				invokeOperation<Opcode::movl>(i._source1, i._source2);
                break;
            case Opcode::movt: 
				invokeOperation<Opcode::movt>(i._source1, i._source2);
                break;
            case Opcode::movq: 
				invokeOperation<Opcode::movq>(i._source1, i._source2);
                break;
			Standard2ArgOp(spanbit, spanbit);
			Standard2ArgOp(scanbit, scanbit);
			Standard3ArgOp(modac, modac);
			Standard3ArgOp(modify, modify);
			Standard3ArgOp(extract, extract);
			Standard3ArgOp(modtc, modtc);
			Standard3ArgOp(modpc, modpc);
			case Opcode::calls: 
				invokeOperation<Opcode::calls>(src1);
				break;
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
#warning "Emul not impl'd as it is a special form"
#warning "Ediv not impl'd as it is a special form"
			Standard3ArgOpIO(mul, mul);
			Standard3ArgOpIO(rem, rem);
			Standard3ArgOpIO(div, div);
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
