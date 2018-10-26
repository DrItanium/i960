#ifndef I960_CORE_H__
#define I960_CORE_H__
#define __DEFAULT_THREE_ARGS__ SourceRegister src1, SourceRegister src2, DestinationRegister dest
#define __DEFAULT_TWO_ARGS__ SourceRegister src, DestinationRegister dest
#define __DEFAULT_DOUBLE_WIDE_THREE_ARGS__ LongSourceRegister src1, LongSourceRegister src2, LongDestinationRegister dest
#define __DEFAULT_DOUBLE_WIDE_TWO_ARGS__ LongSourceRegister src, LongDestinationRegister dest
#define __GEN_DEFAULT_THREE_ARG_SIGS__(name) void name (__DEFAULT_THREE_ARGS__) noexcept
#define __TWO_SOURCE_REGS__ SourceRegister src, SourceRegister dest
#include "types.h"
#include "memiface.h"
#include <cmath>
#include <math.h>
#include <variant>
#include <functional>
namespace i960 {
    using Register = NormalRegister;
    using LongRegister = DoubleRegister;
    using SourceRegister = const Register&;
    using DestinationRegister = Register&;
    using LongSourceRegister = const LongRegister&;
    using LongDestinationRegister = LongRegister&;
    
    // ExtendedRegisters are used for move to and from floating point
    using ExtendedRegister = TripleRegister;
    using ExtendedSourceRegister = const ExtendedRegister&; 
    using ExtendedDestinationRegister = ExtendedRegister&;


    using FloatingPointRegister = ExtendedReal;
    using FloatingPointSourceRegister = const FloatingPointRegister&;
    using FloatingPointDestinationRegister = FloatingPointRegister&;
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

    using RegisterWindow = NormalRegister[LocalRegisterCount];
    template<typename Src1, typename Src2, typename Dest>
    struct ThreeArgumentExtraction final {
        static_assert(std::is_same_v<Src1, SourceRegister> || std::is_same_v<Src1, FloatingPointSourceRegister>, "Illegal source register kind!"); 
        static_assert(std::is_same_v<Src2, SourceRegister> || std::is_same_v<Src2, FloatingPointSourceRegister>, "Illegal source register kind!"); 
        static_assert(std::is_same_v<Dest, DestinationRegister> || std::is_same_v<Dest, FloatingPointDestinationRegister>, "Illegal destination register kind!");
        using Type = RawExtendedReal;
        private:
            ThreeArgumentExtraction() = delete;
            ~ThreeArgumentExtraction() = delete;
            ThreeArgumentExtraction(const ThreeArgumentExtraction&) = delete;
            ThreeArgumentExtraction(ThreeArgumentExtraction&&) = delete;
    };
    template<>
    struct ThreeArgumentExtraction <SourceRegister, SourceRegister, DestinationRegister> final {
        using Type = RawReal;
        private:
            ThreeArgumentExtraction() = delete;
            ~ThreeArgumentExtraction() = delete;
            ThreeArgumentExtraction(const ThreeArgumentExtraction&) = delete;
            ThreeArgumentExtraction(ThreeArgumentExtraction&&) = delete;
    };
    template<typename Src1, typename Src2, typename Dest>
    struct ThreeLongArgumentExtraction final {
        static_assert(std::is_same_v<Src1, LongSourceRegister> || std::is_same_v<Src1, FloatingPointSourceRegister>, "Illegal source register kind!"); 
        static_assert(std::is_same_v<Src2, LongSourceRegister> || std::is_same_v<Src2, FloatingPointSourceRegister>, "Illegal source register kind!"); 
        static_assert(std::is_same_v<Dest, LongDestinationRegister> || std::is_same_v<Dest, FloatingPointDestinationRegister>, "Illegal destination register kind!");
        using Type = RawExtendedReal;
        private:
            ThreeLongArgumentExtraction() = delete;
            ~ThreeLongArgumentExtraction() = delete;
            ThreeLongArgumentExtraction(const ThreeLongArgumentExtraction&) = delete;
            ThreeLongArgumentExtraction(ThreeLongArgumentExtraction&&) = delete;
    };
    template<>
    struct ThreeLongArgumentExtraction <LongSourceRegister, LongSourceRegister, LongDestinationRegister> final {
        using Type = RawLongReal;
        private:
            ThreeLongArgumentExtraction() = delete;
            ~ThreeLongArgumentExtraction() = delete;
            ThreeLongArgumentExtraction(const ThreeLongArgumentExtraction&) = delete;
            ThreeLongArgumentExtraction(ThreeLongArgumentExtraction&&) = delete;
    };
    template<typename Src1, typename Dest>
    struct TwoArgumentExtraction final {
        static_assert(std::is_same_v<Src1, SourceRegister> || std::is_same_v<Src1, FloatingPointSourceRegister>, "Illegal source register kind!"); 
        static_assert(std::is_same_v<Dest, DestinationRegister> || std::is_same_v<Dest, FloatingPointDestinationRegister>, "Illegal destination register kind!");
        using Type = RawExtendedReal;
        private:
            TwoArgumentExtraction() = delete;
            ~TwoArgumentExtraction() = delete;
            TwoArgumentExtraction(const TwoArgumentExtraction&) = delete;
            TwoArgumentExtraction(TwoArgumentExtraction&&) = delete;
    };
    template<>
    struct TwoArgumentExtraction <SourceRegister, DestinationRegister> final {
        using Type = RawReal;
        private:
            TwoArgumentExtraction() = delete;
            ~TwoArgumentExtraction() = delete;
            TwoArgumentExtraction(const TwoArgumentExtraction&) = delete;
            TwoArgumentExtraction(TwoArgumentExtraction&&) = delete;
    };
    template<typename Src1, typename Dest>
    struct TwoLongArgumentExtraction final {
        static_assert(std::is_same_v<Src1, LongSourceRegister> || std::is_same_v<Src1, FloatingPointSourceRegister>, "Illegal source register kind!"); 
        static_assert(std::is_same_v<Dest, LongDestinationRegister> || std::is_same_v<Dest, FloatingPointDestinationRegister>, "Illegal destination register kind!");
        using Type = RawExtendedReal;
        private:
            TwoLongArgumentExtraction() = delete;
            ~TwoLongArgumentExtraction() = delete;
            TwoLongArgumentExtraction(const TwoLongArgumentExtraction&) = delete;
            TwoLongArgumentExtraction(TwoLongArgumentExtraction&&) = delete;
    };
    template<>
    struct TwoLongArgumentExtraction <LongSourceRegister, LongDestinationRegister> final {
        using Type = RawLongReal;
        private:
            TwoLongArgumentExtraction() = delete;
            ~TwoLongArgumentExtraction() = delete;
            TwoLongArgumentExtraction(const TwoLongArgumentExtraction&) = delete;
            TwoLongArgumentExtraction(TwoLongArgumentExtraction&&) = delete;
    };
    template<typename Src1, typename Src2>
    struct TwoSourceArgumentExtraction final {
        static_assert(std::is_same_v<Src1, SourceRegister> || std::is_same_v<Src1, FloatingPointSourceRegister>, "Illegal source register kind!"); 
        static_assert(std::is_same_v<Src2, SourceRegister> || std::is_same_v<Src2, FloatingPointSourceRegister>, "Illegal source register kind!");
        using Type = ExtendedReal;
        private:
            TwoSourceArgumentExtraction() = delete;
            ~TwoSourceArgumentExtraction() = delete;
            TwoSourceArgumentExtraction(const TwoSourceArgumentExtraction&) = delete;
            TwoSourceArgumentExtraction(TwoSourceArgumentExtraction&&) = delete;
    };
    template<>
    struct TwoSourceArgumentExtraction <SourceRegister, SourceRegister> final {
        using Type = Real;
        private:
            TwoSourceArgumentExtraction() = delete;
            ~TwoSourceArgumentExtraction() = delete;
            TwoSourceArgumentExtraction(const TwoSourceArgumentExtraction&) = delete;
            TwoSourceArgumentExtraction(TwoSourceArgumentExtraction&&) = delete;
    };
    template<typename Src1, typename Src2>
    struct TwoLongSourceArgumentExtraction final {
        static_assert(std::is_same_v<Src1, LongSourceRegister> || std::is_same_v<Src1, FloatingPointSourceRegister>, "Illegal source register kind!"); 
        static_assert(std::is_same_v<Src2, LongSourceRegister> || std::is_same_v<Src2, FloatingPointSourceRegister>, "Illegal source register kind!");
        using Type = ExtendedReal;
        private:
            TwoLongSourceArgumentExtraction() = delete;
            ~TwoLongSourceArgumentExtraction() = delete;
            TwoLongSourceArgumentExtraction(const TwoLongSourceArgumentExtraction&) = delete;
            TwoLongSourceArgumentExtraction(TwoLongSourceArgumentExtraction&&) = delete;
    };
    template<>
    struct TwoLongSourceArgumentExtraction <LongSourceRegister, LongSourceRegister> final {
        using Type = LongReal;
        private:
            TwoLongSourceArgumentExtraction() = delete;
            ~TwoLongSourceArgumentExtraction() = delete;
            TwoLongSourceArgumentExtraction(const TwoLongSourceArgumentExtraction&) = delete;
            TwoLongSourceArgumentExtraction(TwoLongSourceArgumentExtraction&&) = delete;
    };
    class Core {
        public:
			Core(MemoryInterface& mem);
			/**
			 * Invoked by the external RESET pin, initializes the core.
			 */
			void reset();
            // TODO finish this once we have all the other sub components implemented behind the
            // scenes
        private:
            /** 
             * perform a call
             */
            void call(Integer displacement) noexcept;
            Ordinal load(Ordinal address, bool atomic = false) noexcept;
            void store(Ordinal address, Ordinal value, bool atomic = false) noexcept;

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
            // begin core architecture
            void callx(SourceRegister value) noexcept;
            void calls(SourceRegister value);
            __GEN_DEFAULT_THREE_ARG_SIGS__(addc);
            __GEN_DEFAULT_THREE_ARG_SIGS__(addo);
            __GEN_DEFAULT_THREE_ARG_SIGS__(addi);
            void chkbit(SourceRegister pos, SourceRegister src) noexcept;
            __GEN_DEFAULT_THREE_ARG_SIGS__(alterbit);
            __GEN_DEFAULT_THREE_ARG_SIGS__(andOp);
            __GEN_DEFAULT_THREE_ARG_SIGS__(andnot);
            void atadd(__DEFAULT_THREE_ARGS__) noexcept; // TODO add other forms of atadd
            void atmod(SourceRegister src, SourceRegister mask, DestinationRegister srcDest) noexcept; // TODO check out other forms of this instruction
            void b(Integer displacement) noexcept;
            void bx(SourceRegister targ) noexcept; // TODO check these two instructions out for more variants
            void bal(Integer displacement) noexcept;
            void balx(__DEFAULT_TWO_ARGS__) noexcept; // TODO check these two instructions out for more variants
            void bbc(SourceRegister pos, SourceRegister src, Integer targ) noexcept; 
            void bbs(SourceRegister pos, SourceRegister src, Integer targ) noexcept;
            // compare and branch instructions as well
            // faults too
#define X(kind) \
            void b ## kind (Integer) noexcept; \
            void cmpib ## kind ( SourceRegister, SourceRegister, Integer) noexcept; \
            void fault ## kind () noexcept; \
            void test ## kind ( DestinationRegister) noexcept;
#define Y(kind) void cmp ## kind ( SourceRegister, SourceRegister, Integer) noexcept;
#include "conditional_kinds.def"
			Y(obe)
			Y(obne)
			Y(obl)
			Y(oble)
			Y(obg)
			Y(obge)
#undef X
#undef Y
            void clrbit(SourceRegister pos, SourceRegister src, DestinationRegister dest) noexcept; // TODO look into the various forms further
            void cmpi(SourceRegister src1, SourceRegister src2) noexcept;
            void cmpo(SourceRegister src1, SourceRegister src2) noexcept;
            __GEN_DEFAULT_THREE_ARG_SIGS__(cmpdeci);
            __GEN_DEFAULT_THREE_ARG_SIGS__(cmpdeco);
            __GEN_DEFAULT_THREE_ARG_SIGS__(cmpinci);
            __GEN_DEFAULT_THREE_ARG_SIGS__(cmpinco);
            void concmpi(SourceRegister src1, SourceRegister src2) noexcept;
            void concmpo(SourceRegister src1, SourceRegister src2) noexcept;
            __GEN_DEFAULT_THREE_ARG_SIGS__(divo);
            __GEN_DEFAULT_THREE_ARG_SIGS__(divi);
            void ediv(SourceRegister src1, LongSourceRegister src2, DestinationRegister remainder, DestinationRegister quotient) noexcept;
            void emul(SourceRegister src1, SourceRegister src2, LongDestinationRegister dest) noexcept;
            __GEN_DEFAULT_THREE_ARG_SIGS__(extract);
            void fill(SourceRegister dst, SourceRegister value, SourceRegister len) noexcept;
            void flushreg() noexcept;
            void fmark() noexcept;
            void ld(__DEFAULT_TWO_ARGS__) noexcept;
            void ldob(__DEFAULT_TWO_ARGS__) noexcept;
            void ldos(__DEFAULT_TWO_ARGS__) noexcept;
            void ldib(__DEFAULT_TWO_ARGS__) noexcept;
            void ldis(__DEFAULT_TWO_ARGS__) noexcept;
            void ldl(SourceRegister src, LongDestinationRegister dest) noexcept;
			inline void ldl(SourceRegister src, Ordinal srcDestIndex) noexcept {
				DoubleRegister reg(getRegister(srcDestIndex), getRegister(srcDestIndex + 1));
				ldl(src, reg);
			}
            void ldt(SourceRegister src, TripleRegister& dest) noexcept;
			inline void ldt(SourceRegister src, Ordinal srcDestIndex) noexcept {
				TripleRegister reg(getRegister(srcDestIndex), getRegister(srcDestIndex + 1), getRegister(srcDestIndex + 2));
				ldt(src, reg);
			}
            void ldq(SourceRegister src, QuadRegister& dest) noexcept;
			inline void ldq(SourceRegister src, Ordinal index) noexcept {
				QuadRegister reg(getRegister(index), getRegister(index + 1), getRegister(index + 2), getRegister(index + 3));
				ldq(src, reg);
			}
            void lda(__DEFAULT_TWO_ARGS__) noexcept;
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
            inline void movl(ByteOrdinal src, ByteOrdinal dest) noexcept {
                DoubleRegister s(getRegister(src), getRegister(src + 1));
                DoubleRegister d(getRegister(dest), getRegister(dest + 1));
                movl(s, d);
            }
            inline void movt(ByteOrdinal src, ByteOrdinal dest) noexcept {
                TripleRegister s(getRegister(src), getRegister(src + 1), getRegister(src + 2));
                TripleRegister d(getRegister(dest), getRegister(dest + 1), getRegister(dest + 2));
                movt(s, d);
            }
            inline void movq(ByteOrdinal src, ByteOrdinal dest) noexcept {
                QuadRegister s(getRegister(src), getRegister(src + 1), getRegister(src + 2), getRegister(src + 3));
                QuadRegister d(getRegister(dest), getRegister(dest + 1), getRegister(dest + 2), getRegister(dest + 3));
                movq(s, d);
            }
            __GEN_DEFAULT_THREE_ARG_SIGS__(mulo);
            __GEN_DEFAULT_THREE_ARG_SIGS__(muli);
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
            void resumeprcs(SourceRegister src) noexcept;
            void ret() noexcept;
            void rotate(__DEFAULT_THREE_ARGS__) noexcept;
            void scanbyte(SourceRegister src1, SourceRegister src2) noexcept;
            void scanbit(__DEFAULT_TWO_ARGS__) noexcept;
            void setbit(__DEFAULT_THREE_ARGS__) noexcept;
            void shlo(__DEFAULT_THREE_ARGS__) noexcept;
            void shro(__DEFAULT_THREE_ARGS__) noexcept;
            void shli(__DEFAULT_THREE_ARGS__) noexcept;
            void shri(__DEFAULT_THREE_ARGS__) noexcept;
            void shrdi(__DEFAULT_THREE_ARGS__) noexcept;
            void spanbit(__DEFAULT_TWO_ARGS__) noexcept;
            void st(__TWO_SOURCE_REGS__) noexcept;
            void stob(__TWO_SOURCE_REGS__) noexcept;
            void stos(__TWO_SOURCE_REGS__) noexcept;
            void stib(__TWO_SOURCE_REGS__) noexcept;
            void stis(__TWO_SOURCE_REGS__) noexcept;
            void stl(LongSourceRegister src, SourceRegister dest) noexcept;
			inline void stl(Ordinal ind, SourceRegister dest) noexcept {
				DoubleRegister reg(getRegister(ind), getRegister(ind + 1));
				stl(reg, dest);
			}
            void stt(const TripleRegister& src, SourceRegister dest) noexcept;
            inline void stt(Ordinal ind, SourceRegister dest) noexcept {
				TripleRegister reg(getRegister(ind), getRegister(ind + 1), getRegister(ind + 2));
				stt(reg, dest);
			}

            void stq(const QuadRegister& src, SourceRegister dest) noexcept;
            inline void stq(Ordinal ind, SourceRegister dest) noexcept {
				QuadRegister reg(getRegister(ind), getRegister(ind + 1), getRegister(ind + 2), getRegister(ind + 3));
				stq(reg, dest);
			}
            __GEN_DEFAULT_THREE_ARG_SIGS__(subc); 
            __GEN_DEFAULT_THREE_ARG_SIGS__(subo);
            __GEN_DEFAULT_THREE_ARG_SIGS__(subi);
            void syncf() noexcept;
            __GEN_DEFAULT_THREE_ARG_SIGS__(xnor);
            __GEN_DEFAULT_THREE_ARG_SIGS__(xorOp);
            // end core architecture
            // begin numerics architecture 
            // TODO add all of the different various combinations
            template<typename Src, typename Dest>
            void tanr(const Src& src, Dest& dest) noexcept {
                if constexpr (IsSourceSelector<Src>) {
                    if constexpr (IsDestinationSelector<Dest>) {
                        std::visit([this](auto&& src, auto&& dest) { tanr(src.get(), dest.get()); }, src, dest);
                    } else {
                        std::visit([this, &dest](auto&& src) { tanr(src.get(), dest); }, src);
                    }
                } else if constexpr (IsDestinationSelector<Dest>) {
                    std::visit([this, &src](auto&& dest) { tanr(src, dest.get()); }, dest);
                } else {
                    using K = typename TwoArgumentExtraction<decltype(src), decltype(dest)>::Type;
                    dest.template set<K>(::tan(src.template get<K>()));
                }
            }
            template<typename Src, typename Dest>
            void tanrl(const Src& src, Dest& dest) noexcept {
                using K = typename TwoLongArgumentExtraction<decltype(src), decltype(dest)>::Type;
                dest.template set<K>(::tan(src.template get<K>()));
            }
            template<typename Src, typename Dest>
            void sinr(const Src& src, Dest& dest) noexcept {
                using K = typename TwoArgumentExtraction<Src, Dest>::Type;
                dest.template set<K>(::sin(src.template get<K>()));
            }
            template<typename Src, typename Dest>
            void sinrl(const Src& src, Dest& dest) noexcept {
                using K = typename TwoLongArgumentExtraction<Src, Dest>::Type;
                dest.template set<K>(::sin(src.template get<K>()));
            }
            template<typename Src, typename Dest>
            void cosr(const Src& src, Dest& dest) noexcept {
                using K = typename TwoArgumentExtraction<Src, Dest>::Type;
                dest.template set<K>(::cos(src.template get<K>()));
            }
            template<typename Src, typename Dest>
            void cosrl(const Src& src, Dest& dest) noexcept {
                using K = typename TwoLongArgumentExtraction<Src, Dest>::Type;
                dest.template set<K>(::cos(src.template get<K>()));
            }
            template<typename Src1, typename Src2, typename Dest>
            void atanr(const Src1& src1, const Src2& src2, Dest& dest) noexcept {
                using K = typename ThreeArgumentExtraction<Src1, Src2, Dest>::Type;
                dest.template set<K>(::atan(src2.template get<K>() / src1.template get<K>()));
            }
            template<typename Src1, typename Src2, typename Dest>
            void atanrl(const Src1& src1, const Src2& src2, Dest& dest) noexcept {
                using K = typename ThreeLongArgumentExtraction<Src1, Src2, Dest>::Type;
                dest.template set<K>(::atan(src2.template get<K>() / src1.template get<K>()));
            }
            template<typename Src1, typename Src2, typename Dest>
            void addr(const Src1& src1, const Src2& src2, Dest& dest) noexcept {
                using K = typename ThreeArgumentExtraction<Src1, Src2, Dest>::Type;
                dest.template set<K>(src2.template get<K>() + src1.template get<K>());
            }
            template<typename Src1, typename Src2, typename Dest>
            void addrl(const Src1& src1, const Src2& src2, Dest& dest) noexcept {
                using K = typename ThreeLongArgumentExtraction<Src1, Src2, Dest>::Type;
                dest.template set<K>(src2.template get<K>() + src1.template get<K>());
            }
            template<typename Src1, typename Src2, typename Dest>
            void divr(const Src1& src1, const Src2& src2, Dest& dest) noexcept {
                using K = typename ThreeArgumentExtraction<Src1, Src2, Dest>::Type;
                dest.template set<K>(src2.template get<K>() / src1.template get<K>());
            }
            template<typename Src1, typename Src2, typename Dest>
            void divrl(const Src1& src1, const Src2& src2, Dest& dest) noexcept {
                using K = typename ThreeLongArgumentExtraction<Src1, Src2, Dest>::Type;
                dest.template set<K>(src2.template get<K>() / src1.template get<K>());
            }
            template<typename Src1, typename Src2, typename Dest>
            void remr(const Src1& src1, const Src2& src2, Dest& dest) noexcept {
                using K = typename ThreeArgumentExtraction<Src1, Src2, Dest>::Type;
                auto s1 = src1.template get<K>();
                auto s2 = src2.template get<K>();
                // TODO truncate s1 / s2 towards zero
                dest.template set<K>(s2 - ((s1 / s2) * s1));
            }
            template<typename Src1, typename Src2, typename Dest>
            void remrl(const Src1& src1, const Src2& src2, Dest& dest) noexcept {
                using K = typename ThreeLongArgumentExtraction<Src1, Src2, Dest>::Type;
                auto s1 = src1.template get<K>();
                auto s2 = src2.template get<K>();
                // TODO truncate s1 / s2 towards zero
                dest.template set<K>(s2 - ((s1 / s2) * s1));
            }
            template<typename Src1, typename Src2, typename Dest>
            void subr(const Src1& src1, const Src2& src2, Dest& dest) noexcept {
                using K = typename ThreeArgumentExtraction<Src1, Src2, Dest>::Type;
                dest.template set<K>(src2.template get<K>() - src1.template get<K>());
            }
            template<typename Src1, typename Src2, typename Dest>
            void subrl(const Src1& src1, const Src2& src2, Dest& dest) noexcept {
                using K = typename ThreeLongArgumentExtraction<Src1, Src2, Dest>::Type;
                dest.template set<K>(src2.template get<K>() - src1.template get<K>());
            }
            template<typename Src1, typename Src2, typename Dest>
            void mulr(const Src1& src1, const Src2& src2, Dest& dest) noexcept {
                using K = typename ThreeArgumentExtraction<Src1, Src2, Dest>::Type;
                dest.template set<K>(src2.template get<K>() * src1.template get<K>());
            }
            template<typename Src1, typename Src2, typename Dest>
            void mulrl(const Src1& src1, const Src2& src2, Dest& dest) noexcept {
                using K = typename ThreeLongArgumentExtraction<Src1, Src2, Dest>::Type;
                dest.template set<K>(src2.template get<K>() * src1.template get<K>());
            }
            template<typename Src1, typename Src2>
            void cmpr(const Src1& src1, const Src2& src2) noexcept {
                using K = typename TwoSourceArgumentExtraction<Src1, Src2>::Type;
                cmpr(src1.template get<K>(), src2.template get<K>());
            }
            template<typename Src1, typename Src2>
            void cmprl(const Src1& src1, const Src2& src2) noexcept {
                using K = typename TwoLongSourceArgumentExtraction<Src1, Src2>::Type;
                cmpr(src1.template get<K>(), src2.template get<K>());
            }
            template<typename Src1, typename Src2>
            void cmpor(const Src1& src1, const Src2& src2) noexcept {
                using K = typename TwoSourceArgumentExtraction<Src1, Src2>::Type;
                cmpr(src1.template get<K>(), src2.template get<K>(), true);
            }
            template<typename Src1, typename Src2>
            void cmporl(const Src1& src1, const Src2& src2) noexcept {
                using K = typename TwoLongSourceArgumentExtraction<Src1, Src2>::Type;
                cmpr(src1.template get<K>(), src2.template get<K>(), true);
            }
            template<typename Src1, typename Src2, typename Dest>
            void logr(const Src1& src1, const Src2& src2, Dest& dest) noexcept {
                using K = typename ThreeArgumentExtraction<Src1, Src2, Dest>::Type;
                // TODO implement body for logr
                //dest.template set<K>(src2.template get<K>() * src1.template get<K>());
            }
            template<typename Src1, typename Src2, typename Dest>
            void logrl(const Src1& src1, const Src2& src2, Dest& dest) noexcept {
                using K = typename ThreeLongArgumentExtraction<Src1, Src2, Dest>::Type;
                // TODO implement body for logrl
                //dest.template set<K>(src2.template get<K>() * src1.template get<K>());
            }
            template<typename Src1, typename Src2, typename Dest>
            void logepr(const Src1& src1, const Src2& src2, Dest& dest) noexcept {
                using K = typename ThreeArgumentExtraction<Src1, Src2, Dest>::Type;
                // TODO implement body for logepr
                //dest.template set<K>(src2.template get<K>() * src1.template get<K>());
            }
            template<typename Src1, typename Src2, typename Dest>
            void logeprl(const Src1& src1, const Src2& src2, Dest& dest) noexcept {
                using K = typename ThreeLongArgumentExtraction<Src1, Src2, Dest>::Type;
                // TODO implement body for logeprl
                //dest.template set<K>(src2.template get<K>() * src1.template get<K>());
            }
            template<typename Src, typename Dest>
            void expr(const Src& src, Dest& dest) noexcept {
                using K = typename TwoArgumentExtraction<Src, Dest>::Type;
                // TODO implement the expr operation
            }
            template<typename Src, typename Dest>
            void exprl(const Src& src, Dest& dest) noexcept {
                using K = typename TwoLongArgumentExtraction<Src, Dest>::Type;
                // TODO implement the exprl operation
            }
            template<typename Src, typename Dest>
            void logbnr(const Src& src, Dest& dest) noexcept {
                using K = typename TwoArgumentExtraction<Src, Dest>::Type;
                // TODO implement the logbnr operation
            }
            template<typename Src, typename Dest>
            void logbnrl(const Src& src, Dest& dest) noexcept {
                using K = typename TwoLongArgumentExtraction<Src, Dest>::Type;
                // TODO implement the logbnrl operation
            }
            template<typename Src1, typename Src2, typename Dest>
            void scaler(const Src1& src1, const Src2& src2, Dest& dest) noexcept {
                static_assert(std::is_same_v<decltype(src1), SourceRegister>, "Illegal src1 type");
                static_assert(std::is_same_v<decltype(src2), SourceRegister> ||
                              std::is_same_v<decltype(src2), FloatingPointSourceRegister> , "Illegal src2 type");
                static_assert(std::is_same_v<decltype(dest), DestinationRegister> ||
                              std::is_same_v<decltype(dest), FloatingPointDestinationRegister> , "Illegal destination type");
                if constexpr (std::is_same_v<decltype(src1), SourceRegister> &&
                              std::is_same_v<decltype(src2), SourceRegister> &&
                              std::is_same_v<decltype(dest), DestinationRegister>) {
                    // TODO implement scaler for reals
                } else {
                    // TODO implement scaler for extended reals
                }
            }
            template<typename Src1, typename Src2, typename Dest>
            void scalerl(const Src1& src1, const Src2& src2, Dest& dest) noexcept {
                static_assert(std::is_same_v<decltype(src1), LongSourceRegister>, "Illegal src1 type");
                static_assert(std::is_same_v<decltype(src2), LongSourceRegister> ||
                              std::is_same_v<decltype(src2), FloatingPointSourceRegister> , "Illegal src2 type");
                static_assert(std::is_same_v<decltype(dest), LongDestinationRegister> ||
                              std::is_same_v<decltype(dest), FloatingPointDestinationRegister> , "Illegal destination type");
                if constexpr (std::is_same_v<decltype(src1), LongSourceRegister> &&
                              std::is_same_v<decltype(src2), LongSourceRegister> &&
                              std::is_same_v<decltype(dest), LongDestinationRegister>) {
                    // TODO implement scalerl for reals
                } else {
                    // TODO implement scalerl for extended reals
                }
            }
            template<typename Src, typename Dest>
            void sqrtr(const Src& src, Dest& dest) noexcept {
                using K = typename TwoArgumentExtraction<Src, Dest>::Type;
                dest.template set<K>(::sqrt(src.template get<K>()));
                // TODO implement the sqrtr operation
            }
            template<typename Src, typename Dest>
            void sqrtrl(const Src& src, Dest& dest) noexcept {
                using K = typename TwoLongArgumentExtraction<Src, Dest>::Type;
                // TODO implement the sqrtrl operation
                dest.template set<K>(::sqrt(src.template get<K>()));
            }
            void cmpr(const Real& src1, const Real& src2, bool ordered = false) noexcept;
            void cmpr(const LongReal& src1, const LongReal& src2, bool ordered = false) noexcept;
            void cmpr(const ExtendedReal& src1, const ExtendedReal& src2, bool ordered = false) noexcept;
            void classr(SourceRegister src) noexcept;
			void classr(FloatingPointSourceRegister src) noexcept;
            void classrl(LongSourceRegister src) noexcept;
			void classrl(FloatingPointSourceRegister src) noexcept;
            void cpysre(__DEFAULT_THREE_ARGS__) noexcept; // TODO fix the signature of this function
            void cpyrsre(__DEFAULT_THREE_ARGS__) noexcept; // TODO fix the signature of this function
            void roundr(__DEFAULT_TWO_ARGS__) noexcept;
            void roundrl(LongSourceRegister src, LongDestinationRegister dest) noexcept;
            void cvtilr(LongSourceRegister src, ExtendedReal& dest) noexcept;
            void cvtir(SourceRegister src, ExtendedReal& dest) noexcept;
            void cvtri(__DEFAULT_TWO_ARGS__) noexcept; // TODO fix this function as it deals with floating point registers
            void cvtril(SourceRegister src, LongDestinationRegister dest) noexcept; // TODO fix this function as it deals with floating point registers
            void cvtzri(__DEFAULT_TWO_ARGS__) noexcept; // TODO fix this function as it deals with floating point registers
            void cvtzril(SourceRegister src, LongDestinationRegister dest) noexcept; // TODO fix this function as it deals with floating point registers
            __GEN_DEFAULT_THREE_ARG_SIGS__(daddc);
            void dmovt(SourceRegister src, DestinationRegister dest) noexcept;
            __GEN_DEFAULT_THREE_ARG_SIGS__(dsubc);
            void movr(__DEFAULT_TWO_ARGS__) noexcept;
            void movrl(LongSourceRegister src, LongDestinationRegister dest) noexcept;
            void movre(const TripleRegister& src, TripleRegister& dest) noexcept;
            void synld(__DEFAULT_TWO_ARGS__) noexcept;
            void synmov(__DEFAULT_TWO_ARGS__) noexcept;
            void synmovl(__DEFAULT_TWO_ARGS__) noexcept;
            void synmovq(__DEFAULT_TWO_ARGS__) noexcept;
            // end numerics architecture 
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
            template<typename T>
            void compare(T src1, T src2) noexcept {
                if (src1 < src2) {
                    _ac.conditionCode = 0b100;
                } else if (src1 == src2) {
                    _ac.conditionCode = 0b010;
                } else {
                    _ac.conditionCode = 0b001;
                }
            }
            template<TestTypes t>
            void testGeneric(DestinationRegister dest) noexcept {
                dest.set<Ordinal>((_ac.conditionCode & (Ordinal(t))) != 0 ? 1 : 0);
            }
            template<ConditionCode cc>
            bool conditionCodeIs() const noexcept {
                return (_ac.conditionCode & static_cast<Ordinal>(cc)) != 0;
            }
            template<ConditionCode cc>
            void branchIfGeneric(Integer addr) noexcept {
                if constexpr (cc == ConditionCode::Unordered) {
                    if (_ac.conditionCode == 0) {
                        b(addr);
                    }
                } else {
                    if (((Ordinal(cc)) & _ac.conditionCode) != 0) {
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
            FloatingPointRegister _floatingPointRegisters[NumFloatingPointRegs];
			MemoryInterface& _mem;
			Ordinal _initialWords[8];
			Ordinal _prcbAddress;
			Ordinal _systemProcedureTableAddress;
    };

}
#undef __TWO_SOURCE_REGS__
#undef __GEN_DEFAULT_THREE_ARG_SIGS__
#undef __DEFAULT_THREE_ARGS__
#undef __DEFAULT_DOUBLE_WIDE_THREE_ARGS__
#undef __DEFAULT_TWO_ARGS__
#undef __DEFAULT_DOUBLE_WIDE_TWO_ARGS__
#endif // end I960_CORE_H__
