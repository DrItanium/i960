#ifndef I960_CORE_H__
#define I960_CORE_H__
#include "types.h"
namespace i960 {
    class Core {
        public:
            using SourceRegister = const NormalRegister&;
            using DestinationRegister = NormalRegister&;
            using LongSourceRegister = const DoubleRegister&;
            using LongDestinationRegister = DoubleRegister&;
            using RegisterWindow = NormalRegister[LocalRegisterCount];
            Core() = default;
            ~Core() = default;
            // TODO finish this once we have all the other sub components implemented behind the
            // scenes
        private:
            /** 
             * perform a call
             */
            void call(Integer displacement) noexcept;
            Ordinal load(Ordinal address) noexcept;
            void store(Ordinal address, Ordinal value) noexcept;

            void saveLocalRegisters() noexcept;
            void allocateNewLocalRegisterSet() noexcept;
            template<typename T>
            void setRegister(ByteOrdinal index, T value) noexcept {
                getRegister(index).set<T>(value);
            }
            void setRegister(ByteOrdinal index, SourceRegister other) noexcept;
            NormalRegister& getRegister(ByteOrdinal index) noexcept;
            PreviousFramePointer& getPFP() noexcept;
            Ordinal getStackPointerAddress() const noexcept;
            void setFramePointer(Ordinal value) noexcept;
            Ordinal getFramePointerAddress() const noexcept;
#define __DEFAULT_THREE_ARGS__ SourceRegister src1, SourceRegister src2, DestinationRegister dest
#define __DEFAULT_TWO_ARGS__ SourceRegister src, DestinationRegister dest
#define __DEFAULT_DOUBLE_WIDE_THREE_ARGS__ LongSourceRegister src1, LongSourceRegister src2, LongDestinationRegister dest
#define __DEFAULT_DOUBLE_WIDE_TWO_ARGS__ LongSourceRegister src, LongDestinationRegister dest
#define __GEN_DEFAULT_THREE_ARG_SIGS__(name) void name (__DEFAULT_THREE_ARGS__) noexcept
#define __TWO_SOURCE_REGS__ SourceRegister src, SourceRegister dest

            void callx(SourceRegister value) noexcept;
            void calls(SourceRegister value);
            __GEN_DEFAULT_THREE_ARG_SIGS__(addc);
            __GEN_DEFAULT_THREE_ARG_SIGS__(addo);
            __GEN_DEFAULT_THREE_ARG_SIGS__(addi);
            void addr(__DEFAULT_THREE_ARGS__) noexcept;
            void addrl(__DEFAULT_DOUBLE_WIDE_THREE_ARGS__) noexcept;
            void chkbit(SourceRegister pos, SourceRegister src) noexcept;
            __GEN_DEFAULT_THREE_ARG_SIGS__(alterbit);
            __GEN_DEFAULT_THREE_ARG_SIGS__(andOp);
            __GEN_DEFAULT_THREE_ARG_SIGS__(andnot);
            void atadd(__DEFAULT_THREE_ARGS__) noexcept; // TODO add other forms of atadd
            void atanr(__DEFAULT_THREE_ARGS__) noexcept;
            void atanrl(__DEFAULT_DOUBLE_WIDE_THREE_ARGS__) noexcept;
            void atmod(SourceRegister src, SourceRegister mask, DestinationRegister srcDest) noexcept; // TODO check out other forms of this instruction
            void b(Integer displacement) noexcept;
            void bx(SourceRegister targ) noexcept; // TODO check these two instructions out for more variants
            void bal(Integer displacement) noexcept;
            void balx(__DEFAULT_TWO_ARGS__) noexcept; // TODO check these two instructions out for more variants
            void bbc(SourceRegister pos, SourceRegister src, Integer targ) noexcept; 
            void bbs(SourceRegister pos, SourceRegister src, Integer targ) noexcept;
#define X(kind) void b ## kind (Integer) noexcept;
#include "conditional_kinds.def"
#undef X
            void classr(SourceRegister src) noexcept;
            void classrl(LongSourceRegister src) noexcept;
            void clrbit(SourceRegister pos, SourceRegister src, DestinationRegister dest) noexcept; // TODO look into the various forms further
            void cmpi(SourceRegister src1, SourceRegister src2) noexcept;
            void cmpo(SourceRegister src1, SourceRegister src2) noexcept;
            __GEN_DEFAULT_THREE_ARG_SIGS__(cmpdeci);
            __GEN_DEFAULT_THREE_ARG_SIGS__(cmpdeco);
            __GEN_DEFAULT_THREE_ARG_SIGS__(cmpinci);
            __GEN_DEFAULT_THREE_ARG_SIGS__(cmpinco);
            void cmpor(SourceRegister src1, SourceRegister src2) noexcept;
            void cmporl(LongSourceRegister src1, LongSourceRegister src2) noexcept;
            void cmpr(SourceRegister src1, SourceRegister src2) noexcept;
            void cmprl(LongSourceRegister src1, LongSourceRegister src2) noexcept;
            // compare and branch instructions
#define X(kind) void cmpib ## kind ( SourceRegister, SourceRegister, Integer) noexcept;
#include "conditional_kinds.def"
#undef X
#define X(kind) void cmp ## kind ( SourceRegister, SourceRegister, Integer) noexcept;
			X(obe)
			X(obne)
			X(obl)
			X(oble)
			X(obg)
			X(obge)
#undef X
            void concmpi(SourceRegister src1, SourceRegister src2) noexcept;
            void concmpo(SourceRegister src1, SourceRegister src2) noexcept;
            void cosr(__DEFAULT_TWO_ARGS__) noexcept;
            void cosrl(__DEFAULT_DOUBLE_WIDE_TWO_ARGS__) noexcept;
            void cpysre(__DEFAULT_THREE_ARGS__) noexcept; // TODO fix the signature of this function
            void cpyrsre(__DEFAULT_THREE_ARGS__) noexcept; // TODO fix the signature of this function
            void cvtilr(LongSourceRegister src, ExtendedReal& dest) noexcept;
            void cvtir(SourceRegister src, ExtendedReal& dest) noexcept;
            void cvtri(__DEFAULT_TWO_ARGS__) noexcept; // TODO fix this function as it deals with floating point registers
            void cvtril(SourceRegister src, LongDestinationRegister dest) noexcept; // TODO fix this function as it deals with floating point registers
            void cvtzri(__DEFAULT_TWO_ARGS__) noexcept; // TODO fix this function as it deals with floating point registers
            void cvtzril(SourceRegister src, LongDestinationRegister dest) noexcept; // TODO fix this function as it deals with floating point registers
            __GEN_DEFAULT_THREE_ARG_SIGS__(daddc);
            __GEN_DEFAULT_THREE_ARG_SIGS__(divo);
            __GEN_DEFAULT_THREE_ARG_SIGS__(divi);
            void divr(__DEFAULT_THREE_ARGS__) noexcept; // TODO divr and divrl do not support extended registers yet
            void divrl(__DEFAULT_DOUBLE_WIDE_THREE_ARGS__) noexcept;
            void dmovt(SourceRegister src, DestinationRegister dest) noexcept;
            __GEN_DEFAULT_THREE_ARG_SIGS__(dsubc);
            void ediv(SourceRegister src1, LongSourceRegister src2, DestinationRegister remainder, DestinationRegister quotient) noexcept;
            void emul(SourceRegister src1, SourceRegister src2, LongDestinationRegister dest) noexcept;
            void expr(__DEFAULT_TWO_ARGS__) noexcept;
            void exprl(LongSourceRegister src, LongDestinationRegister dest) noexcept;
            __GEN_DEFAULT_THREE_ARG_SIGS__(extract);
#define X(kind) void fault ## kind (Integer) noexcept;
#include "conditional_kinds.def"
#undef X
            void fill(SourceRegister dst, SourceRegister value, SourceRegister len) noexcept;
            void flushreg() noexcept;
            void fmark() noexcept;
            void ld(__DEFAULT_TWO_ARGS__) noexcept;
            void ldob(__DEFAULT_TWO_ARGS__) noexcept;
            void ldos(__DEFAULT_TWO_ARGS__) noexcept;
            void ldib(__DEFAULT_TWO_ARGS__) noexcept;
            void ldis(__DEFAULT_TWO_ARGS__) noexcept;
            void ldl(SourceRegister src, LongDestinationRegister dest) noexcept;
			void ldl(SourceRegister src, Ordinal srcDestIndex) noexcept {
				DoubleRegister reg(getRegister(srcDestIndex), getRegister(srcDestIndex + 1));
				ldl(src, reg);
			}
            void ldt(SourceRegister src, TripleRegister& dest) noexcept;
			void ldt(SourceRegister src, Ordinal srcDestIndex) noexcept {
				TripleRegister reg(getRegister(srcDestIndex), getRegister(srcDestIndex + 1), getRegister(srcDestIndex + 2));
				ldt(src, reg);
			}
            void ldq(SourceRegister src, QuadRegister& dest) noexcept;
			void ldq(SourceRegister src, Ordinal index) noexcept {
				QuadRegister reg(getRegister(index), getRegister(index + 1), getRegister(index + 2), getRegister(index + 3));
				ldq(src, reg);
			}
            void lda(__DEFAULT_TWO_ARGS__) noexcept;
            void logbnr(__DEFAULT_TWO_ARGS__) noexcept;
            void logbnrl(LongSourceRegister src, LongDestinationRegister dest) noexcept;
            void logepr(__DEFAULT_THREE_ARGS__) noexcept;
            void logeprl(__DEFAULT_DOUBLE_WIDE_THREE_ARGS__) noexcept;
            void logr(__DEFAULT_THREE_ARGS__) noexcept;
            void logrl(__DEFAULT_DOUBLE_WIDE_THREE_ARGS__) noexcept;
            void mark() noexcept;
            __GEN_DEFAULT_THREE_ARG_SIGS__(modac);
            __GEN_DEFAULT_THREE_ARG_SIGS__(modi);
            __GEN_DEFAULT_THREE_ARG_SIGS__(modify);
            __GEN_DEFAULT_THREE_ARG_SIGS__(modpc);
            __GEN_DEFAULT_THREE_ARG_SIGS__(modtc);
            void mov(__DEFAULT_TWO_ARGS__) noexcept;
            void movl(LongSourceRegister src, LongDestinationRegister dest) noexcept;
            void movt(const TripleRegister& src, TripleRegister& dest) noexcept;
            void movq(const QuadRegister& src, QuadRegister& dest) noexcept;
            void movl(ByteOrdinal src, ByteOrdinal dest) noexcept {
                DoubleRegister s(getRegister(src), getRegister(src + 1));
                DoubleRegister d(getRegister(dest), getRegister(dest + 1));
                movl(s, d);
            }
            void movt(ByteOrdinal src, ByteOrdinal dest) noexcept {
                TripleRegister s(getRegister(src), getRegister(src + 1), getRegister(src + 2));
                TripleRegister d(getRegister(dest), getRegister(dest + 1), getRegister(dest + 2));
                movt(s, d);
            }
            void movq(ByteOrdinal src, ByteOrdinal dest) noexcept {
                QuadRegister s(getRegister(src), getRegister(src + 1), getRegister(src + 2), getRegister(src + 3));
                QuadRegister d(getRegister(dest), getRegister(dest + 1), getRegister(dest + 2), getRegister(dest + 3));
                movq(s, d);
            }
            void movr(__DEFAULT_TWO_ARGS__) noexcept;
            void movrl(LongSourceRegister src, LongDestinationRegister dest) noexcept;
            void movre(const TripleRegister& src, TripleRegister& dest) noexcept;
            __GEN_DEFAULT_THREE_ARG_SIGS__(mulo);
            __GEN_DEFAULT_THREE_ARG_SIGS__(muli);
            void mulr(__DEFAULT_THREE_ARGS__) noexcept;
            void mulrl(__DEFAULT_DOUBLE_WIDE_THREE_ARGS__) noexcept;
            __GEN_DEFAULT_THREE_ARG_SIGS__(nand);
            __GEN_DEFAULT_THREE_ARG_SIGS__(nor);
            void notOp(__DEFAULT_TWO_ARGS__) noexcept;
            __GEN_DEFAULT_THREE_ARG_SIGS__(notand);
            __GEN_DEFAULT_THREE_ARG_SIGS__(notbit);
            __GEN_DEFAULT_THREE_ARG_SIGS__(notor);
            __GEN_DEFAULT_THREE_ARG_SIGS__(orOp);
            __GEN_DEFAULT_THREE_ARG_SIGS__(ornot);
            __GEN_DEFAULT_THREE_ARG_SIGS__(remo);
            __GEN_DEFAULT_THREE_ARG_SIGS__(remi);
            __GEN_DEFAULT_THREE_ARG_SIGS__(remr);
            void remrl(__DEFAULT_DOUBLE_WIDE_THREE_ARGS__) noexcept;
            void resumeprcs(SourceRegister src) noexcept;
            void ret() noexcept;
            void rotate(__DEFAULT_THREE_ARGS__) noexcept;
            void roundr(__DEFAULT_TWO_ARGS__) noexcept;
            void roundrl(LongSourceRegister src, LongDestinationRegister dest) noexcept;
            void scanbyte(SourceRegister src1, SourceRegister src2) noexcept;
            void scaler(__DEFAULT_THREE_ARGS__) noexcept;
            void scalerl(LongSourceRegister src1, LongSourceRegister src2, LongDestinationRegister dest) noexcept;
            void scanbit(__DEFAULT_TWO_ARGS__) noexcept;
            void setbit(__DEFAULT_THREE_ARGS__) noexcept;
            void shlo(__DEFAULT_THREE_ARGS__) noexcept;
            void shro(__DEFAULT_THREE_ARGS__) noexcept;
            void shli(__DEFAULT_THREE_ARGS__) noexcept;
            void shri(__DEFAULT_THREE_ARGS__) noexcept;
            void shrdi(__DEFAULT_THREE_ARGS__) noexcept;
            void sinr(__DEFAULT_TWO_ARGS__) noexcept; 
            void sinrl(__DEFAULT_DOUBLE_WIDE_TWO_ARGS__) noexcept; 
            void spanbit(__DEFAULT_TWO_ARGS__) noexcept;
            void sqrtr(__DEFAULT_TWO_ARGS__) noexcept;
            void sqrtrl(LongSourceRegister src, LongDestinationRegister dest) noexcept;
            void st(__TWO_SOURCE_REGS__) noexcept;
            void stob(__TWO_SOURCE_REGS__) noexcept;
            void stos(__TWO_SOURCE_REGS__) noexcept;
            void stib(__TWO_SOURCE_REGS__) noexcept;
            void stis(__TWO_SOURCE_REGS__) noexcept;
            void stl(LongSourceRegister src, SourceRegister dest) noexcept;
			void stl(Ordinal ind, SourceRegister dest) noexcept {
				DoubleRegister reg(getRegister(ind), getRegister(ind + 1));
				stl(reg, dest);
			}
            void stt(const TripleRegister& src, SourceRegister dest) noexcept;
            void stt(Ordinal ind, SourceRegister dest) noexcept {
				TripleRegister reg(getRegister(ind), getRegister(ind + 1), getRegister(ind + 2));
				stt(reg, dest);
			}

            void stq(const QuadRegister& src, SourceRegister dest) noexcept;
            void stq(Ordinal ind, SourceRegister dest) noexcept {
				QuadRegister reg(getRegister(ind), getRegister(ind + 1), getRegister(ind + 2), getRegister(ind + 3));
				stq(reg, dest);
			}
            __GEN_DEFAULT_THREE_ARG_SIGS__(subc); 
            __GEN_DEFAULT_THREE_ARG_SIGS__(subo);
            __GEN_DEFAULT_THREE_ARG_SIGS__(subi);
            void subr(__DEFAULT_THREE_ARGS__) noexcept;
            void subrl(LongSourceRegister src1, LongSourceRegister src2, LongDestinationRegister dest) noexcept;
            void syncf() noexcept;
            void synld(__DEFAULT_TWO_ARGS__) noexcept;
            void synmov(__DEFAULT_TWO_ARGS__) noexcept;
            void synmovl(__DEFAULT_TWO_ARGS__) noexcept;
            void synmovq(__DEFAULT_TWO_ARGS__) noexcept;
            void tanr(__DEFAULT_TWO_ARGS__) noexcept;
            void tanrl(LongSourceRegister src, LongDestinationRegister dest) noexcept;
#define X(kind) void test ## kind ( DestinationRegister) noexcept;
#include "conditional_kinds.def"
#undef X
#ifdef PROTECTED_ARCHITECTURE
            void cmpstr(SourceRegister src1, SourceRegister src2, SourceRegister len) noexcept;
            void condrec(SourceRegister src, DestinationRegister dest) noexcept;
            void condwait(SourceRegister src) noexcept;
            void inspacc(__DEFAULT_TWO_ARGS__) noexcept;
            void ldphy(__DEFAULT_TWO_ARGS__) noexcept;
            void ldtime(DestinationRegister dest) noexcept;
            void movqstr(SourceRegister dst, SourceRegister src, SourceRegister len) noexcept;
            void movstr(SourceRegister dst, SourceRegister src, SourceRegister len) noexcept;
            void receive(__DEFAULT_TWO_ARGS__) noexcept;
            void saveprcs() noexcept;
            void schedprcs(SourceRegister src) noexcept;
            void send(SourceRegister dest, SourceRegister src1, SourceRegister src2) noexcept;
            void sendserv(SourceRegister src) noexcept;
            void signal(SourceRegister src) noexcept;
            void wait(SourceRegister src) noexcept;
#endif
            __GEN_DEFAULT_THREE_ARG_SIGS__(xnor);
            __GEN_DEFAULT_THREE_ARG_SIGS__(xorOp);
#undef __TWO_SOURCE_REGS__
#undef __GEN_DEFAULT_THREE_ARG_SIGS__
#undef __DEFAULT_THREE_ARGS__
#undef __DEFAULT_DOUBLE_WIDE_THREE_ARGS__
#undef __DEFAULT_TWO_ARGS__
#undef __DEFAULT_DOUBLE_WIDE_TWO_ARGS__
            template<typename T>
            void compare(T src1, T src2) noexcept {
                if (src1 < src2) {
                    _ac._conditionCode = 0b100;
                } else if (src1 == src2) {
                    _ac._conditionCode = 0b010;
                } else {
                    _ac._conditionCode = 0b001;
                }
            }
            template<TestTypes t>
            void testGeneric(Core::DestinationRegister dest) noexcept {
                dest.set<Ordinal>((_ac._conditionCode & (Ordinal(t))) != 0 ? 1 : 0);
            }
            template<ConditionCode cc>
            void branchIfGeneric(Integer addr) noexcept {
                if constexpr (cc == ConditionCode::Unordered) {
                    if (_ac._conditionCode == 0) {
                        b(addr);
                    }
                } else {
                    if (((Ordinal(cc)) & _ac._conditionCode) != 0) {
                        b(addr);
                    }
                }
            }
		private:
			void dispatch(const Instruction::REGFormat& inst) noexcept;
			void dispatch(const Instruction::COBRFormat& inst) noexcept;
			void dispatch(const Instruction::CTRLFormat& inst) noexcept;
			void dispatch(const Instruction::MemFormat& inst) noexcept;
			void dispatch(const Instruction& decodedInstruction) noexcept;
			void dispatch(const Instruction::MemFormat::MEMAFormat& inst) noexcept;
			void dispatch(const Instruction::MemFormat::MEMBFormat& inst) noexcept;
			Integer getFullDisplacement() noexcept;
			void dispatchFP(const Instruction::REGFormat& inst) noexcept;
        private:
            RegisterWindow _globalRegisters;
            // The hardware implementations use register sets, however
            // to start with, we should just follow the logic as is and 
            // just save the contents of the registers to the stack the logic
            // is always sound to do it this way
            RegisterWindow _localRegisters;
            ArithmeticControls _ac;
            Ordinal _instructionPointer;
            ProcessControls _pc;
            TraceControls _tc;
            ExtendedReal _floatingPointRegisters[NumFloatingPointRegs];
            NormalRegister _internalRegisters[8]; // for internal conversion purposes to make decoding regular
    };

}
#endif // end I960_CORE_H__
