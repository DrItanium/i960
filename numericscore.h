#ifndef I960_NUMERICS_CORE_H__
#define I960_NUMERICS_CORE_H__
#include "core.h"
namespace i960 {
    class NumericsCore : public Core {
        public:
			NumericsCore(MemoryInterface& mem);
			/**
			 * Invoked by the external RESET pin, initializes the core.
			 */
			void reset();
            // TODO finish this once we have all the other sub components implemented behind the
            // scenes
        private:
            // begin numerics architecture 
            // TODO add all of the different various combinations
            template<typename Src, typename Dest>
            void tanr(const Src& src, Dest& dest) noexcept {
                using K = typename TwoArgumentExtraction<decltype(src), decltype(dest)>::Type;
                dest.template set<K>(::tan(src.template get<K>()));
            }
            template<typename Src, typename Dest>
            void tanrl(const Src& src, Dest& dest) noexcept {
                using K = typename TwoLongArgumentExtraction<decltype(src), decltype(dest)>::Type;
                dest.template set<K>(::tan(src.template get<K>()));
            }
            template<typename Src, typename Dest>
            void sinr(const Src& src, Dest& dest) noexcept {
                using K = typename TwoArgumentExtraction<decltype(src), decltype(dest)>::Type;
                dest.template set<K>(::sin(src.template get<K>()));
            }
            template<typename Src, typename Dest>
            void sinrl(const Src& src, Dest& dest) noexcept {
                using K = typename TwoLongArgumentExtraction<decltype(src), decltype(dest)>::Type;
                dest.template set<K>(::sin(src.template get<K>()));
            }
            template<typename Src, typename Dest>
            void cosr(const Src& src, Dest& dest) noexcept {
                using K = typename TwoArgumentExtraction<decltype(src), decltype(dest)>::Type;
                dest.template set<K>(::cos(src.template get<K>()));
            }
            template<typename Src, typename Dest>
            void cosrl(const Src& src, Dest& dest) noexcept {
                using K = typename TwoLongArgumentExtraction<decltype(src), decltype(dest)>::Type;
                dest.template set<K>(::cos(src.template get<K>()));
            }
            template<typename Src1, typename Src2, typename Dest>
            void atanr(const Src1& src1, const Src2& src2, Dest& dest) noexcept {
                using K = typename ThreeArgumentExtraction<decltype(src1), decltype(src2), decltype(dest)>::Type;
                dest.template set<K>(::atan(src2.template get<K>() / src1.template get<K>()));
            }
            template<typename Src1, typename Src2, typename Dest>
            void atanrl(const Src1& src1, const Src2& src2, Dest& dest) noexcept {
                using K = typename ThreeLongArgumentExtraction<decltype(src1), decltype(src2), decltype(dest)>::Type;
                dest.template set<K>(::atan(src2.template get<K>() / src1.template get<K>()));
            }
            template<typename Src1, typename Src2, typename Dest>
            void addr(const Src1& src1, const Src2& src2, Dest& dest) noexcept {
                using K = typename ThreeArgumentExtraction<decltype(src1), decltype(src2), decltype(dest)>::Type;
                dest.template set<K>(src2.template get<K>() + src1.template get<K>());
            }
            template<typename Src1, typename Src2, typename Dest>
            void addrl(const Src1& src1, const Src2& src2, Dest& dest) noexcept {
                using K = typename ThreeLongArgumentExtraction<decltype(src1), decltype(src2), decltype(dest)>::Type;
                dest.template set<K>(src2.template get<K>() + src1.template get<K>());
            }
            template<typename Src1, typename Src2, typename Dest>
            void divr(const Src1& src1, const Src2& src2, Dest& dest) noexcept {
                using K = typename ThreeArgumentExtraction<decltype(src1), decltype(src2), decltype(dest)>::Type;
                dest.template set<K>(src2.template get<K>() / src1.template get<K>());
            }
            template<typename Src1, typename Src2, typename Dest>
            void divrl(const Src1& src1, const Src2& src2, Dest& dest) noexcept {
                using K = typename ThreeLongArgumentExtraction<decltype(src1), decltype(src2), decltype(dest)>::Type;
                dest.template set<K>(src2.template get<K>() / src1.template get<K>());
            }
            template<typename Src1, typename Src2, typename Dest>
            void remr(const Src1& src1, const Src2& src2, Dest& dest) noexcept {
                using K = typename ThreeArgumentExtraction<decltype(src1), decltype(src2), decltype(dest)>::Type;
                auto s1 = src1.template get<K>();
                auto s2 = src2.template get<K>();
                // TODO truncate s1 / s2 towards zero
                dest.template set<K>(s2 - ((s1 / s2) * s1));
            }
            template<typename Src1, typename Src2, typename Dest>
            void remrl(const Src1& src1, const Src2& src2, Dest& dest) noexcept {
                using K = typename ThreeLongArgumentExtraction<decltype(src1), decltype(src2), decltype(dest)>::Type;
                auto s1 = src1.template get<K>();
                auto s2 = src2.template get<K>();
                // TODO truncate s1 / s2 towards zero
                dest.template set<K>(s2 - ((s1 / s2) * s1));
            }
            template<typename Src1, typename Src2, typename Dest>
            void subr(const Src1& src1, const Src2& src2, Dest& dest) noexcept {
                using K = typename ThreeArgumentExtraction<decltype(src1), decltype(src2), decltype(dest)>::Type;
                dest.template set<K>(src2.template get<K>() - src1.template get<K>());
            }
            template<typename Src1, typename Src2, typename Dest>
            void subrl(const Src1& src1, const Src2& src2, Dest& dest) noexcept {
                using K = typename ThreeLongArgumentExtraction<decltype(src1), decltype(src2), decltype(dest)>::Type;
                dest.template set<K>(src2.template get<K>() - src1.template get<K>());
            }
            template<typename Src1, typename Src2, typename Dest>
            void mulr(const Src1& src1, const Src2& src2, Dest& dest) noexcept {
                using K = typename ThreeArgumentExtraction<decltype(src1), decltype(src2), decltype(dest)>::Type;
                dest.template set<K>(src2.template get<K>() * src1.template get<K>());
            }
            template<typename Src1, typename Src2, typename Dest>
            void mulrl(const Src1& src1, const Src2& src2, Dest& dest) noexcept {
                using K = typename ThreeLongArgumentExtraction<decltype(src1), decltype(src2), decltype(dest)>::Type;
                dest.template set<K>(src2.template get<K>() * src1.template get<K>());
            }
            template<typename Src1, typename Src2>
            void cmpr(const Src1& src1, const Src2& src2) noexcept {
                using K = typename TwoSourceArgumentExtraction<decltype(src1), decltype(src2)>::Type;
                cmpr(src1.template get<K>(), src2.template get<K>());
            }
            template<typename Src1, typename Src2>
            void cmprl(const Src1& src1, const Src2& src2) noexcept {
                using K = typename TwoLongSourceArgumentExtraction<decltype(src1), decltype(src2)>::Type;
                cmpr(src1.template get<K>(), src2.template get<K>());
            }
            template<typename Src1, typename Src2>
            void cmpor(const Src1& src1, const Src2& src2) noexcept {
                using K = typename TwoSourceArgumentExtraction<decltype(src1), decltype(src2)>::Type;
                cmpr(src1.template get<K>(), src2.template get<K>(), true);
            }
            template<typename Src1, typename Src2>
            void cmporl(const Src1& src1, const Src2& src2) noexcept {
                using K = typename TwoLongSourceArgumentExtraction<decltype(src1), decltype(src2)>::Type;
                cmpr(src1.template get<K>(), src2.template get<K>(), true);
            }
            template<typename Src1, typename Src2, typename Dest>
            void logr(const Src1& src1, const Src2& src2, Dest& dest) noexcept {
                using K = typename ThreeArgumentExtraction<decltype(src1), decltype(src2), decltype(dest)>::Type;
                // TODO implement body for logr
                //dest.template set<K>(src2.template get<K>() * src1.template get<K>());
            }
            template<typename Src1, typename Src2, typename Dest>
            void logrl(const Src1& src1, const Src2& src2, Dest& dest) noexcept {
                using K = typename ThreeLongArgumentExtraction<decltype(src1), decltype(src2), decltype(dest)>::Type;
                // TODO implement body for logrl
                //dest.template set<K>(src2.template get<K>() * src1.template get<K>());
            }
            template<typename Src1, typename Src2, typename Dest>
            void logepr(const Src1& src1, const Src2& src2, Dest& dest) noexcept {
                using K = typename ThreeArgumentExtraction<decltype(src1), decltype(src2), decltype(dest)>::Type;
                // TODO implement body for logepr
                //dest.template set<K>(src2.template get<K>() * src1.template get<K>());
            }
            template<typename Src1, typename Src2, typename Dest>
            void logeprl(const Src1& src1, const Src2& src2, Dest& dest) noexcept {
                using K = typename ThreeLongArgumentExtraction<decltype(src1), decltype(src2), decltype(dest)>::Type;
                // TODO implement body for logeprl
                //dest.template set<K>(src2.template get<K>() * src1.template get<K>());
            }
            template<typename Src, typename Dest>
            void expr(const Src& src, Dest& dest) noexcept {
                using K = typename TwoArgumentExtraction<decltype(src), decltype(dest)>::Type;
                // TODO implement the expr operation
            }
            template<typename Src, typename Dest>
            void exprl(const Src& src, Dest& dest) noexcept {
                using K = typename TwoLongArgumentExtraction<decltype(src), decltype(dest)>::Type;
                // TODO implement the exprl operation
            }
            template<typename Src, typename Dest>
            void logbnr(const Src& src, Dest& dest) noexcept {
                using K = typename TwoArgumentExtraction<decltype(src), decltype(dest)>::Type;
                // TODO implement the logbnr operation
            }
            template<typename Src, typename Dest>
            void logbnrl(const Src& src, Dest& dest) noexcept {
                using K = typename TwoLongArgumentExtraction<decltype(src), decltype(dest)>::Type;
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
                using K = typename TwoArgumentExtraction<decltype(src), decltype(dest)>::Type;
                dest.template set<K>(::sqrt(src.template get<K>()));
                // TODO implement the sqrtr operation
            }
            template<typename Src, typename Dest>
            void sqrtrl(const Src& src, Dest& dest) noexcept {
                using K = typename TwoLongArgumentExtraction<decltype(src), decltype(dest)>::Type;
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
            template<typename Src, typename Dest>
            void roundr(const Src& src, Dest& dest) noexcept {
                using K = typename TwoArgumentExtraction<decltype(src), decltype(dest)>::Type;
                // TODO implement the roundr operation
            }
            template<typename Src, typename Dest>
            void roundrl(const Src& src, Dest& dest) noexcept {
                using K = typename TwoLongArgumentExtraction<decltype(src), decltype(dest)>::Type;
                // TODO implement the roundrl operation
            }
            void cvtilr(LongSourceRegister src, ExtendedReal& dest) noexcept;
            void cvtir(SourceRegister src, ExtendedReal& dest) noexcept;
            void cvtri(__DEFAULT_TWO_ARGS__) noexcept; // TODO fix this function as it deals with floating point registers
            void cvtril(SourceRegister src, LongDestinationRegister dest) noexcept; // TODO fix this function as it deals with floating point registers
            void cvtzri(__DEFAULT_TWO_ARGS__) noexcept; // TODO fix this function as it deals with floating point registers
            void cvtzril(SourceRegister src, LongDestinationRegister dest) noexcept; // TODO fix this function as it deals with floating point registers
            __GEN_DEFAULT_THREE_ARG_SIGS__(daddc);
            void dmovt(SourceRegister src, DestinationRegister dest) noexcept;
            __GEN_DEFAULT_THREE_ARG_SIGS__(dsubc);
            template<typename Src, typename Dest>
            void movr(const Src& src, Dest& dest) noexcept {
                using K = typename TwoArgumentExtraction<decltype(src), decltype(dest)>::Type;
                dest.template set<K>(src.template get<K>());
            }
            template<typename Src, typename Dest>
            void movrl(const Src& src, Dest& dest) noexcept {
                using K = typename TwoLongArgumentExtraction<decltype(src), decltype(dest)>::Type;
                dest.template set<K>(src.template get<K>());
            }
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
		private:
			void dispatchFP(const Instruction::REGFormat& inst) noexcept;
            void dispatchFPLong(const Instruction::REGFormat& inst) noexcept;
        private:
            FloatingPointRegister _floatingPointRegisters[NumFloatingPointRegs];
    };
}
#endif // end I960_NUMERICS_CORE_H__ 

