#include "types.h"
#include "core.h"
#include "opcodes.h"
#define __DEFAULT_THREE_ARGS__ Core::SourceRegister src1, Core::SourceRegister src2, Core::DestinationRegister dest
#define __DEFAULT_DOUBLE_WIDE_THREE_ARGS__ const DoubleRegister& src1, const DoubleRegister& src2, DoubleRegister& dest
#define __DEFAULT_TWO_ARGS__ Core::SourceRegister src, Core::DestinationRegister dest
#define __DEFAULT_DOUBLE_WIDE_TWO_ARGS__ const DoubleRegister& src, DoubleRegister& dest
namespace i960 {
	void Core::dispatch(const Instruction& inst) noexcept {
		if (inst.isRegFormat()) {
			dispatch(inst._reg);
		} else if (inst.isMemFormat()) {
			dispatch(inst._mem);
		} else if (inst.isControlFormat()) {
			dispatch(inst._ctrl);
		} else if (inst.isCompareAndBranchFormat()) {
			dispatch(inst._cobr);
		} else {
#warning "This throw statement should be a fault of some kind"
			throw "Illegal instruction";
		}
	}
	void Core::dispatch(const Instruction::CTRLFormat& i) noexcept {
		Integer displacement = i._displacement;
		switch (static_cast<Opcodes>(i._opcode)) {
			case Opcodes::B: 
				b(displacement); 
				break;
			case Opcodes::Call: 
				call(displacement); 
				break;
			case Opcodes::Ret:
				ret();
				break;
			case Opcodes::Bal:
				bal(displacement);
				break;
#define X(kind) \
			case Opcodes:: B ## kind : b ## kind (displacement) ; break; \
			case Opcodes:: Fault ## kind : fault ## kind () ; break;
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
		switch(static_cast<Opcodes>(i._opcode)) {
#define X(kind) case Opcodes:: Test ## kind : test ## kind (src1); break;
#include "conditional_kinds.def"
#undef X
			case Opcodes::Bbc:
				bbc(src1, src2, displacement);
				break;
			case Opcodes::Bbs:
				bbs(src1, src2, displacement);
#define X(kind) case Opcodes:: Cmpob ## kind : cmpob ## kind ( src1, src2, displacement ) ; break;
				X(g)
				X(e)
				X(ge)
				X(l)
				X(ne)
				X(le)
#undef X
#define X(kind) case Opcodes:: Cmpib ## kind : cmpib ## kind ( src1, src2, displacement ) ; break;
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
		switch(static_cast<Opcodes>(i._opcode)) {
			case Opcodes::Ldob:
				ldob(immediateStorage, srcDest);
				break;
			case Opcodes::Stob:
				stob(srcDest, immediateStorage);
				break;
			case Opcodes::Bx:
				bx(immediateStorage);
				break;
			case Opcodes::Balx:
				balx(immediateStorage, srcDest);
				break;
			case Opcodes::Callx:
				callx(immediateStorage);
				break;
			case Opcodes::Ldos:
				ldos(immediateStorage, srcDest);
				break;
			case Opcodes::Stos:
				stos(srcDest, immediateStorage);
				break;
			case Opcodes::Lda:
				stos(immediateStorage, srcDest);
				break;
			case Opcodes::Ld:
				ld(immediateStorage, srcDest);
				break;
			case Opcodes::St:
				st(srcDest, immediateStorage);
				break;
			case Opcodes::Ldl:
#warning "Fault should happen if the dest reg is non even!"
				ldl(immediateStorage, i._src_dest);
				break;
			case Opcodes::Stl:
#warning "Fault should happen if the src reg is non even!"
				stl(i._src_dest, immediateStorage);
				break;
			case Opcodes::Ldt:
#warning "Fault should happen if the dest reg is not divisible by four!"
				ldt(immediateStorage, i._src_dest);
				break;
			case Opcodes::Stt:
#warning "Fault should happen if the src reg is not divisible by four!"
				stt(i._src_dest, immediateStorage);
				break;
			case Opcodes::Ldq:
#warning "Fault should happen if the dest reg is not divisible by four!"
				ldq(immediateStorage, i._src_dest);
				break;
			case Opcodes::Stq:
#warning "Fault should happen if the src reg is not divisible by four!"
				stq(i._src_dest, immediateStorage);
				break;
			case Opcodes::Ldib:
				ldib(immediateStorage, srcDest);
				break;
			case Opcodes::Stib:
				stib(srcDest, immediateStorage);
				break;
			case Opcodes::Ldis:
				ldis(immediateStorage, srcDest);
				break;
			case Opcodes::Stis:
				stis(srcDest, immediateStorage);
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
		switch(static_cast<Opcodes>(i._opcode)) {
			case Opcodes::Ldob:
				ldob(immediateStorage, srcDest);
				break;
			case Opcodes::Stob:
				stob(srcDest, immediateStorage);
				break;
			case Opcodes::Bx:
				bx(immediateStorage);
				break;
			case Opcodes::Balx:
				balx(immediateStorage, srcDest);
				break;
			case Opcodes::Callx:
				callx(immediateStorage);
				break;
			case Opcodes::Ldos:
				ldos(immediateStorage, srcDest);
				break;
			case Opcodes::Stos:
				stos(srcDest, immediateStorage);
				break;
			case Opcodes::Lda:
				stos(immediateStorage, srcDest);
				break;
			case Opcodes::Ld:
				ld(immediateStorage, srcDest);
				break;
			case Opcodes::St:
				st(srcDest, immediateStorage);
				break;
			case Opcodes::Ldl:
#warning "Fault should happen if the dest reg is non even!"
				ldl(immediateStorage, i._src_dest);
				break;
			case Opcodes::Stl:
#warning "Fault should happen if the src reg is non even!"
				stl(i._src_dest, immediateStorage);
				break;
			case Opcodes::Ldt:
#warning "Fault should happen if the dest reg is not divisible by four!"
				ldt(immediateStorage, i._src_dest);
				break;
			case Opcodes::Stt:
#warning "Fault should happen if the src reg is not divisible by four!"
				stt(i._src_dest, immediateStorage);
				break;
			case Opcodes::Ldq:
#warning "Fault should happen if the dest reg is not divisible by four!"
				ldq(immediateStorage, i._src_dest);
				break;
			case Opcodes::Stq:
#warning "Fault should happen if the src reg is not divisible by four!"
				stq(i._src_dest, immediateStorage);
				break;
			case Opcodes::Ldib:
				ldib(immediateStorage, srcDest);
				break;
			case Opcodes::Stib:
				stib(srcDest, immediateStorage);
				break;
			case Opcodes::Ldis:
				ldis(immediateStorage, srcDest);
				break;
			case Opcodes::Stis:
				stis(srcDest, immediateStorage);
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
		if (i.isFloatingPoint()) {
			dispatchFP(i);
			return;
		}
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
		switch(static_cast<Opcodes>(i.getOpcode())) {
#define Standard3ArgOp(kind, fn) case Opcodes:: kind : fn ( src1, src2, srcDest ) ; break
#define Standard2ArgOp(kind, fn) case Opcodes:: kind : fn ( src1, srcDest ) ; break
#define Standard3ArgOpIO(kind, fn) Standard3ArgOp(kind ## o, fn ## o); Standard3ArgOp(kind ## i, fn ## i)
#define Standard2ArgOpIO(kind, fn) Standard2ArgOp(kind ## o, fn ## o); Standard2ArgOp(kind ## i, fn ## i)
#define Standard2SourceOp(kind, fn) case Opcodes:: kind : fn ( src1, src2) ; break
			Standard3ArgOp(Notbit, notbit);
            Standard3ArgOp(Clrbit, clrbit);
            Standard3ArgOp(Notor, notor);
			Standard3ArgOp(And, andOp);
			Standard3ArgOp(Andnot, andnot);
			Standard3ArgOp(Setbit, setbit);
			Standard3ArgOp(Notand, notand);
			Standard3ArgOp(Xor, xorOp);
			Standard3ArgOp(Or, orOp);
			Standard3ArgOp(Nor, nor);
			Standard3ArgOp(Xnor, xnor);
			Standard2ArgOp(Not, notOp);
			Standard3ArgOp(Ornot, ornot);
			Standard3ArgOp(Nand, nand);
			Standard3ArgOp(Alterbit, alterbit);
			Standard3ArgOpIO(Add, add);
			Standard3ArgOpIO(Sub, sub);
			Standard3ArgOp(Shro, shro);
			Standard3ArgOp(Shrdi, shrdi);
			Standard3ArgOp(Shri, shri);
			Standard3ArgOp(Shlo, shlo);
			Standard3ArgOp(Rotate, rotate);
			Standard3ArgOp(Shli, shli);
			Standard2ArgOpIO(Cmp, cmp);
			Standard2ArgOpIO(Concmp, concmp);
			Standard3ArgOpIO(Cmpinc, cmpinc);
			Standard3ArgOpIO(Cmpdec, cmpdec);
			Standard2SourceOp(Scanbyte, scanbyte);
			Standard2ArgOp(Chkbit, chkbit);
			Standard3ArgOp(Addc, addc);
			Standard3ArgOp(Subc, subc);
			Standard2ArgOp(Mov, mov);
			Standard3ArgOp(Atmod, atmod);
			Standard3ArgOp(Atadd, atadd);
            case Opcodes::Movl: 
                movl(i._source1, i._source2); 
                break;
            case Opcodes::Movt: 
                movt(i._source1, i._source2); 
                break;
            case Opcodes::Movq: 
                movq(i._source1, i._source2); 
                break;
#warning "synmovl, synmovt, and synmovq have not been implemented as they have special logic"
			Standard2ArgOp(Spanbit, spanbit);
			Standard2ArgOp(Scanbit, scanbit);
			Standard3ArgOp(Daddc, daddc);
			Standard3ArgOp(Dsubc, dsubc);
			Standard2ArgOp(Dmovt, dmovt);
			Standard3ArgOp(Modac, modac);
			Standard3ArgOp(Modify, modify);
			Standard3ArgOp(Extract, extract);
			Standard3ArgOp(Modtc, modtc);
			Standard3ArgOp(Modpc, modpc);
			case Opcodes::Calls: 
				calls(src1);
				break;
			case Opcodes::Mark:
				mark();
				break;
			case Opcodes::Fmark:
				fmark();
				break;
			case Opcodes::Flushreg:
				flushreg();
				break;
			case Opcodes::Syncf:
				syncf();
				break;
#warning "Emul not impl'd as it is a special form"
#warning "Ediv not impl'd as it is a special form"
			Standard3ArgOpIO(Mul, mul);
			Standard3ArgOpIO(Rem, rem);
			Standard3ArgOpIO(Div, div);
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
    using SourceRegisterSelector = std::variant<std::reference_wrapper<const Register>, std::reference_wrapper<const FloatingPointRegister>>;
    using DestinationRegisterSelector = std::variant<std::reference_wrapper<Register>, std::reference_wrapper<FloatingPointRegister>>;
    template<typename T>
    constexpr bool IsSourceSelector = std::is_same_v<std::decay_t<T>, SourceRegisterSelector>;
    template<typename T>
    constexpr bool IsDestinationSelector = std::is_same_v<std::decay_t<T>, DestinationRegisterSelector>;
    using LongSourceRegisterSelector = std::variant<std::reference_wrapper<const LongRegister>, std::reference_wrapper<const FloatingPointRegister>>;
    using LongDestinationRegisterSelector = std::variant<std::reference_wrapper<LongRegister>, std::reference_wrapper<FloatingPointRegister>>;
    template<typename T>
    constexpr bool IsLongSourceSelector = std::is_same_v<std::decay_t<T>, LongSourceRegisterSelector>;
    template<typename T>
    constexpr bool IsLongDestinationSelector = std::is_same_v<std::decay_t<T>, LongDestinationRegisterSelector>;
    void Core::dispatchFPLong(const Instruction::REGFormat& i) noexcept {
            auto opcode = static_cast<Opcodes>(i.getOpcode());
            NormalRegister imm1a, imm1b;
            NormalRegister imm2a, imm2b;
            LongRegister imm1(imm1a, imm1b);
            LongRegister imm2(imm2a, imm2b);
            std::optional<LongSourceRegisterSelector> src1;
            std::optional<LongSourceRegisterSelector> src2;
            std::optional<LongDestinationRegisterSelector> dest;
            if (i.m1Set()) {
                // if m1 is set then src1 is an extended register or float literal
                auto value = i._source1;
                switch (value) {
                    case 0b00000:
                    case 0b00001:
                    case 0b00010:
                    case 0b00011:
                        // TODO make TripleRegister capabile of taking in a floating point value
                        src1 = _floatingPointRegisters[value];
                        break;
                    case 0b10000:
                        imm1.set<RawLongReal>(+0.0);
                        src1 = imm1;
                        break;
                    case 0b10110:
                        imm1.set<RawLongReal>(+1.0);
                        src1 = imm1;
                        break;
                    default:
                        // TODO raise exception instead
                        throw "Illegal operand!";
                }
            } else {
                // src1 is a normal register
                LongRegister s1(getRegister(i._source1), getRegister(i._source1 + 1));
                src1 = s1;
            }
            if (i.m2Set()) {
                // if m2 is set then src2 is an extended register or float literal
                auto value = i._source2;
                switch (value) {
                    case 0b00000:
                    case 0b00001:
                    case 0b00010:
                    case 0b00011:
                        src2 = _floatingPointRegisters[value];
                        break;
                    case 0b10000:
                        imm2.set<RawLongReal>(+0.0);
                        src2 = imm2;
                        break;
                    case 0b10110:
                        imm2.set<RawLongReal>(+1.0);
                        src2 = imm2;
                        break;
                    default:
                        // TODO raise exception instead
                        throw "Illegal operand!";
                }
            } else {
                // src2 is a normal register
                LongRegister s2(getRegister(i._source2), getRegister(i._source2 + 1));
                src2 = s2;
            }
            if (i.m3Set()) {
                auto pos = i._src_dest;
                switch (pos) {
                    case 0b00000:
                    case 0b00001:
                    case 0b00010:
                    case 0b00011:
                        dest = _floatingPointRegisters[pos];
                        break;
                    default:
                        throw "Illegal bit pattern";
                }
                // if m3 is set then 
                //   src/dest is undefined
                //   src only is undefined
                //   dst only is an extended register
            } else {
                LongRegister dst(getRegister(i._src_dest), getRegister(i._src_dest + 1));
                dest = dst;
                // dest is a normal register
            }
            switch(opcode) {
#define X(title, op) \
                case Opcodes:: title : \
                                       optionalCheck(src1, src2, dest); \
                std::visit([this](auto&& src1, auto&& src2, auto&& dest) { \
                        op ( src1.get(), src2.get(), dest.get()); \
                        }, src1.value(), src2.value(), dest.value()); \
                break
#define Y(title, op) \
                case Opcodes:: title : \
                                       optionalCheck(src1, dest); \
                std::visit([this](auto&& src, auto&& dest) { \
                        op ( src.get(), dest.get()); \
                        }, src1.value(), dest.value()); \
                break
                X(Addrl, addrl);
                X(Subrl, subrl);
                X(Mulrl, mulrl);
                X(Divrl, divrl);
                X(Remrl, remrl);
                X(Atanrl, atanrl);
                Y(Cosrl, cosrl);
                Y(Sinrl, sinrl);
                Y(Tanrl, tanrl);
                X(Logeprl, logeprl);
                X(Logrl, logrl);
                Y(Movrl, movrl);
                Y(Roundrl, roundrl);
                Y(Logbnrl, logbnrl);
                Y(Exprl, exprl);
                case Opcodes::Scalerl:
                    optionalCheck(src1, src2, dest);
                    std::visit([this](auto&& src1, auto&& src2, auto&& dest) {
                                using K = std::decay_t<decltype(src1.get())>;
                                if constexpr (!std::is_same_v<K, LongRegister>) {
                                    // cannot have src1 be a floating point register
                                    throw "Illegal register specified!";
                                } else {
                                    scalerl(src1.get(), src2.get(), dest.get());
                                }
                            }, src1.value(), src2.value(), dest.value());
                    break;
#undef Y
#undef X 
                default:
                throw "illegal instruction";
            }
    }
	void Core::dispatchFP(const Instruction::REGFormat& i) noexcept {
        if (auto opcode = static_cast<Opcodes>(i.getOpcode()); isLongRealForm(opcode)) {
            dispatchFPLong(i);
        } else {
            NormalRegister imm1;
            NormalRegister imm2;
            std::optional<SourceRegisterSelector> src1;
            std::optional<SourceRegisterSelector> src2;
            std::optional<DestinationRegisterSelector> dest;

            // TODO implement
            if (i.m1Set()) {
                // if m1 is set then src1 is an extended register or float literal
                auto value = i._source1;
                switch (value) {
                    case 0b00000:
                    case 0b00001:
                    case 0b00010:
                    case 0b00011:
                        // TODO make TripleRegister capabile of taking in a floating point value
                        src1 = _floatingPointRegisters[value];
                        break;
                    case 0b10000:
                        imm1.set<RawReal>(+0.0f);
                        src1 = imm1;
                        break;
                    case 0b10110:
                        imm1.set<RawReal>(+1.0f);
                        src1 = imm1;
                        break;
                    default:
                        // TODO raise exception instead
                        throw "Illegal operand!";
                }
            } else {
                // src1 is a normal register
                src1 = getRegister(i._source1);
            }
            if (i.m2Set()) {
                // if m2 is set then src2 is an extended register or float literal
                auto value = i._source2;
                switch (value) {
                    case 0b00000:
                    case 0b00001:
                    case 0b00010:
                    case 0b00011:
                        src2 = _floatingPointRegisters[value];
                        break;
                    case 0b10000:
                        imm2.set<RawReal>(+0.0f);
                        src2 = imm2;
                        break;
                    case 0b10110:
                        imm2.set<RawReal>(+1.0f);
                        src2 = imm2;
                        break;
                    default:
                        // TODO raise exception instead
                        throw "Illegal operand!";
                }
            } else {
                // src2 is a normal register
                src2 = getRegister(i._source2);
            }
            if (i.m3Set()) {
                auto pos = i._src_dest;
                switch (pos) {
                    case 0b00000:
                    case 0b00001:
                    case 0b00010:
                    case 0b00011:
                        dest = _floatingPointRegisters[pos];
                        break;
                    default:
                        throw "Illegal bit pattern";
                }
                // if m3 is set then 
                //   src/dest is undefined
                //   src only is undefined
                //   dst only is an extended register
            } else {
                dest = getRegister(i._src_dest);
                // dest is a normal register
            }
            switch(opcode) {
#define X(title, op) \
                case Opcodes:: title : \
                                       optionalCheck(src1, src2, dest); \
                std::visit([this](auto&& src1, auto&& src2, auto&& dest) { \
                        op ( src1.get(), src2.get(), dest.get()); \
                        }, src1.value(), src2.value(), dest.value()); \
                break
#define Y(title, op) \
                case Opcodes:: title : \
                                       optionalCheck(src1, dest); \
                std::visit([this](auto&& src, auto&& dest) { \
                        op ( src.get(), dest.get()); \
                        }, src1.value(), dest.value()); \
                break
                X(Addr, addr);
                X(Subr, subr);
                X(Mulr, mulr);
                X(Divr, divr);
                X(Remr, remr);
                X(Atanr, atanr);
                Y(Cosr, cosr);
                Y(Sinr, sinr);
                Y(Tanr, tanr);
                X(Logepr, logepr);
                X(Logr, logr);
                Y(Movr, movr);
                Y(Roundr, roundr);
                Y(Logbnr, logbnr);
                Y(Expr, expr);
                case Opcodes::Scaler:
                    optionalCheck(src1, src2, dest);
                    std::visit([this](auto&& src1, auto&& src2, auto&& dest) {
                                using K = std::decay_t<decltype(src1.get())>;
                                if constexpr (!std::is_same_v<K, Register>) {
                                    // cannot have src1 be a floating point register
                                    throw "Illegal register specified!";
                                } else {
                                    scaler(src1.get(), src2.get(), dest.get());
                                }
                            }, src1.value(), src2.value(), dest.value());
                    break;
#undef Y
#undef X 
                default:
                throw "illegal instruction";
            }
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
