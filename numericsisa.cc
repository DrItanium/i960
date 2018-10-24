#include "types.h"
#include "core.h"
#include "operations.h"
#include "opcodes.h"
#include <limits>
#include <cmath>
#include <math.h>
#define __DEFAULT_THREE_ARGS__ Core::SourceRegister src1, Core::SourceRegister src2, Core::DestinationRegister dest
#define __DEFAULT_DOUBLE_WIDE_THREE_ARGS__ const DoubleRegister& src1, const DoubleRegister& src2, DoubleRegister& dest
#define __DEFAULT_TWO_ARGS__ Core::SourceRegister src, Core::DestinationRegister dest
#define __DEFAULT_DOUBLE_WIDE_TWO_ARGS__ const DoubleRegister& src, DoubleRegister& dest
namespace i960 {
    template<typename Src1, typename Src2, typename Dest, typename T>
    void add(const Src1& src1, const Src2& src2, Dest& dest) noexcept {
        dest.template set<T>(src2.template get<T>() + src1.template get<T>());
    }
    template<typename Src1, typename Src2, typename Dest, typename T>
    void sub(const Src1& src1, const Src2& src2, Dest& dest) noexcept {
        dest.template set<T>(src2.template get<T>() - src1.template get<T>());
    }
    template<typename Src1, typename Src2, typename Dest, typename T>
    void mul(const Src1& src1, const Src2& src2, Dest& dest) noexcept {
        dest.template set<T>(src2.template get<T>() * src1.template get<T>());
    }
    template<typename Src, typename Dest, typename T>
    void tan(const Src& src, Dest& dest) noexcept {
        dest.template set<T>(::tan(src.template get<T>()));
    }
    template<typename Src, typename Dest, typename T>
    void cos(const Src& src, Dest& dest) noexcept {
        dest.template set<T>(::cos(src.template get<T>()));
    }
    template<typename Src, typename Dest, typename T>
    void sin(const Src& src, Dest& dest) noexcept {
        dest.template set<T>(::sin(src.template get<T>()));
    }
    template<typename Src1, typename Src2, typename Dest, typename T>
    void atan(const Src1& src1, const Src2& src2, Dest& dest) noexcept {
        dest.template set<T>(::atan(src2.template get<T>() / src1.template get<T>()));
    }
    void Core::addr(__DEFAULT_THREE_ARGS__) noexcept { add<decltype(src1), decltype(src2), decltype(dest), RawReal>(src1, src2, dest); }
    void Core::addr(SourceRegister src1, SourceRegister src2, ExtendedDestinationRegister dest) noexcept { add<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::addr(SourceRegister src1, ExtendedSourceRegister src2, ExtendedDestinationRegister dest) noexcept { add<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::addr(SourceRegister src1, ExtendedSourceRegister src2, DestinationRegister dest) noexcept { add<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::addr(ExtendedSourceRegister src1, ExtendedSourceRegister src2, ExtendedDestinationRegister dest) noexcept { add<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::addr(ExtendedSourceRegister src1, ExtendedSourceRegister src2, DestinationRegister dest) noexcept { add<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::addr(ExtendedSourceRegister src1, SourceRegister src2, ExtendedDestinationRegister dest) noexcept { add<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::addr(ExtendedSourceRegister src1, SourceRegister src2, DestinationRegister dest) noexcept { add<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::addrl(__DEFAULT_DOUBLE_WIDE_THREE_ARGS__) noexcept { add<decltype(src1), decltype(src2), decltype(dest), RawLongReal>(src1, src2, dest); }
    void Core::addrl(LongSourceRegister src1, LongSourceRegister src2, ExtendedDestinationRegister dest) noexcept { add<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::addrl(LongSourceRegister src1, ExtendedSourceRegister src2, ExtendedDestinationRegister dest) noexcept { add<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::addrl(LongSourceRegister src1, ExtendedSourceRegister src2, LongDestinationRegister dest) noexcept { add<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::addrl(ExtendedSourceRegister src1, ExtendedSourceRegister src2, ExtendedDestinationRegister dest) noexcept { add<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::addrl(ExtendedSourceRegister src1, ExtendedSourceRegister src2, LongDestinationRegister dest) noexcept { add<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::addrl(ExtendedSourceRegister src1, LongSourceRegister src2, ExtendedDestinationRegister dest) noexcept { add<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::addrl(ExtendedSourceRegister src1, LongSourceRegister src2, LongDestinationRegister dest) noexcept { add<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::subr(__DEFAULT_THREE_ARGS__) noexcept { sub<decltype(src1), decltype(src2), decltype(dest), RawReal>(src1, src2, dest); }
    void Core::subr(SourceRegister src1, SourceRegister src2, ExtendedDestinationRegister dest) noexcept { sub<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::subr(SourceRegister src1, ExtendedSourceRegister src2, ExtendedDestinationRegister dest) noexcept { sub<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::subr(SourceRegister src1, ExtendedSourceRegister src2, DestinationRegister dest) noexcept { sub<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::subr(ExtendedSourceRegister src1, ExtendedSourceRegister src2, ExtendedDestinationRegister dest) noexcept { sub<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::subr(ExtendedSourceRegister src1, ExtendedSourceRegister src2, DestinationRegister dest) noexcept { sub<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::subr(ExtendedSourceRegister src1, SourceRegister src2, ExtendedDestinationRegister dest) noexcept { sub<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::subr(ExtendedSourceRegister src1, SourceRegister src2, DestinationRegister dest) noexcept { sub<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::subrl(__DEFAULT_DOUBLE_WIDE_THREE_ARGS__) noexcept { sub<decltype(src1), decltype(src2), decltype(dest), RawLongReal>(src1, src2, dest); }
    void Core::subrl(LongSourceRegister src1, LongSourceRegister src2, ExtendedDestinationRegister dest) noexcept { sub<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::subrl(LongSourceRegister src1, ExtendedSourceRegister src2, ExtendedDestinationRegister dest) noexcept { sub<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::subrl(LongSourceRegister src1, ExtendedSourceRegister src2, LongDestinationRegister dest) noexcept { sub<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::subrl(ExtendedSourceRegister src1, ExtendedSourceRegister src2, ExtendedDestinationRegister dest) noexcept { sub<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::subrl(ExtendedSourceRegister src1, ExtendedSourceRegister src2, LongDestinationRegister dest) noexcept { sub<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::subrl(ExtendedSourceRegister src1, LongSourceRegister src2, ExtendedDestinationRegister dest) noexcept { sub<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::subrl(ExtendedSourceRegister src1, LongSourceRegister src2, LongDestinationRegister dest) noexcept { sub<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::mulr(__DEFAULT_THREE_ARGS__) noexcept { mul<decltype(src1), decltype(src2), decltype(dest), RawReal>(src1, src2, dest); }
    void Core::mulr(SourceRegister src1, SourceRegister src2, ExtendedDestinationRegister dest) noexcept { mul<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::mulr(SourceRegister src1, ExtendedSourceRegister src2, ExtendedDestinationRegister dest) noexcept { mul<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::mulr(SourceRegister src1, ExtendedSourceRegister src2, DestinationRegister dest) noexcept { mul<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::mulr(ExtendedSourceRegister src1, ExtendedSourceRegister src2, ExtendedDestinationRegister dest) noexcept { mul<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::mulr(ExtendedSourceRegister src1, ExtendedSourceRegister src2, DestinationRegister dest) noexcept { mul<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::mulr(ExtendedSourceRegister src1, SourceRegister src2, ExtendedDestinationRegister dest) noexcept { mul<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::mulr(ExtendedSourceRegister src1, SourceRegister src2, DestinationRegister dest) noexcept { mul<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::mulrl(__DEFAULT_DOUBLE_WIDE_THREE_ARGS__) noexcept { mul<decltype(src1), decltype(src2), decltype(dest), RawLongReal>(src1, src2, dest); }
    void Core::mulrl(LongSourceRegister src1, LongSourceRegister src2, ExtendedDestinationRegister dest) noexcept { mul<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::mulrl(LongSourceRegister src1, ExtendedSourceRegister src2, ExtendedDestinationRegister dest) noexcept { mul<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::mulrl(LongSourceRegister src1, ExtendedSourceRegister src2, LongDestinationRegister dest) noexcept { mul<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::mulrl(ExtendedSourceRegister src1, ExtendedSourceRegister src2, ExtendedDestinationRegister dest) noexcept { mul<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::mulrl(ExtendedSourceRegister src1, ExtendedSourceRegister src2, LongDestinationRegister dest) noexcept { mul<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::mulrl(ExtendedSourceRegister src1, LongSourceRegister src2, ExtendedDestinationRegister dest) noexcept { mul<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::mulrl(ExtendedSourceRegister src1, LongSourceRegister src2, LongDestinationRegister dest) noexcept { mul<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::tanr(SourceRegister src, DestinationRegister dest) noexcept { tan<decltype(src), decltype(dest), RawReal>(src, dest); }
    void Core::tanr(SourceRegister src, ExtendedDestinationRegister dest) noexcept { tan<decltype(src), decltype(dest), RawReal>(src, dest); }
    void Core::tanr(ExtendedSourceRegister src, DestinationRegister dest) noexcept { tan<decltype(src), decltype(dest), RawExtendedReal>(src, dest); }
    void Core::tanr(ExtendedSourceRegister src, ExtendedDestinationRegister dest) noexcept { tan<decltype(src), decltype(dest), RawExtendedReal>(src, dest); }
    void Core::tanrl(LongSourceRegister src, LongDestinationRegister dest) noexcept { tan<decltype(src), decltype(dest), RawLongReal>(src, dest); }
    void Core::tanrl(LongSourceRegister src, ExtendedDestinationRegister dest) noexcept { tan<decltype(src), decltype(dest), RawLongReal>(src, dest); }
    void Core::tanrl(ExtendedSourceRegister src, LongDestinationRegister dest) noexcept { tan<decltype(src), decltype(dest), RawExtendedReal>(src, dest); }
    void Core::tanrl(ExtendedSourceRegister src, ExtendedDestinationRegister dest) noexcept { tan<decltype(src), decltype(dest), RawExtendedReal>(src, dest); }
    void Core::cosr(SourceRegister src, DestinationRegister dest) noexcept { cos<decltype(src), decltype(dest), RawReal>(src, dest); }
    void Core::cosr(SourceRegister src, ExtendedDestinationRegister dest) noexcept { cos<decltype(src), decltype(dest), RawReal>(src, dest); }
    void Core::cosr(ExtendedSourceRegister src, DestinationRegister dest) noexcept { cos<decltype(src), decltype(dest), RawExtendedReal>(src, dest); }
    void Core::cosr(ExtendedSourceRegister src, ExtendedDestinationRegister dest) noexcept { cos<decltype(src), decltype(dest), RawExtendedReal>(src, dest); }
    void Core::cosrl(LongSourceRegister src, LongDestinationRegister dest) noexcept { cos<decltype(src), decltype(dest), RawLongReal>(src, dest); }
    void Core::cosrl(LongSourceRegister src, ExtendedDestinationRegister dest) noexcept { cos<decltype(src), decltype(dest), RawLongReal>(src, dest); }
    void Core::cosrl(ExtendedSourceRegister src, LongDestinationRegister dest) noexcept { cos<decltype(src), decltype(dest), RawExtendedReal>(src, dest); }
    void Core::cosrl(ExtendedSourceRegister src, ExtendedDestinationRegister dest) noexcept { cos<decltype(src), decltype(dest), RawExtendedReal>(src, dest); }
    void Core::sinr(SourceRegister src, DestinationRegister dest) noexcept { sin<decltype(src), decltype(dest), RawReal>(src, dest); }
    void Core::sinr(SourceRegister src, ExtendedDestinationRegister dest) noexcept { sin<decltype(src), decltype(dest), RawReal>(src, dest); }
    void Core::sinr(ExtendedSourceRegister src, DestinationRegister dest) noexcept { sin<decltype(src), decltype(dest), RawExtendedReal>(src, dest); }
    void Core::sinr(ExtendedSourceRegister src, ExtendedDestinationRegister dest) noexcept { sin<decltype(src), decltype(dest), RawExtendedReal>(src, dest); }
    void Core::sinrl(LongSourceRegister src, LongDestinationRegister dest) noexcept { sin<decltype(src), decltype(dest), RawLongReal>(src, dest); }
    void Core::sinrl(LongSourceRegister src, ExtendedDestinationRegister dest) noexcept { sin<decltype(src), decltype(dest), RawLongReal>(src, dest); }
    void Core::sinrl(ExtendedSourceRegister src, LongDestinationRegister dest) noexcept { sin<decltype(src), decltype(dest), RawExtendedReal>(src, dest); }
    void Core::sinrl(ExtendedSourceRegister src, ExtendedDestinationRegister dest) noexcept { sin<decltype(src), decltype(dest), RawExtendedReal>(src, dest); }
    void Core::atanr(__DEFAULT_THREE_ARGS__) noexcept { atan<decltype(src1), decltype(src2), decltype(dest), RawReal>(src1, src2, dest); }
    void Core::atanr(SourceRegister src1, SourceRegister src2, ExtendedDestinationRegister dest) noexcept { atan<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::atanr(SourceRegister src1, ExtendedSourceRegister src2, ExtendedDestinationRegister dest) noexcept { atan<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::atanr(SourceRegister src1, ExtendedSourceRegister src2, DestinationRegister dest) noexcept { atan<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::atanr(ExtendedSourceRegister src1, ExtendedSourceRegister src2, ExtendedDestinationRegister dest) noexcept { atan<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::atanr(ExtendedSourceRegister src1, ExtendedSourceRegister src2, DestinationRegister dest) noexcept { atan<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::atanr(ExtendedSourceRegister src1, SourceRegister src2, ExtendedDestinationRegister dest) noexcept { atan<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::atanr(ExtendedSourceRegister src1, SourceRegister src2, DestinationRegister dest) noexcept { atan<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::atanrl(__DEFAULT_DOUBLE_WIDE_THREE_ARGS__) noexcept { atan<decltype(src1), decltype(src2), decltype(dest), RawLongReal>(src1, src2, dest); }
    void Core::atanrl(LongSourceRegister src1, LongSourceRegister src2, ExtendedDestinationRegister dest) noexcept { atan<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::atanrl(LongSourceRegister src1, ExtendedSourceRegister src2, ExtendedDestinationRegister dest) noexcept { atan<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::atanrl(LongSourceRegister src1, ExtendedSourceRegister src2, LongDestinationRegister dest) noexcept { atan<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::atanrl(ExtendedSourceRegister src1, ExtendedSourceRegister src2, ExtendedDestinationRegister dest) noexcept { atan<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::atanrl(ExtendedSourceRegister src1, ExtendedSourceRegister src2, LongDestinationRegister dest) noexcept { atan<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::atanrl(ExtendedSourceRegister src1, LongSourceRegister src2, ExtendedDestinationRegister dest) noexcept { atan<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::atanrl(ExtendedSourceRegister src1, LongSourceRegister src2, LongDestinationRegister dest) noexcept { atan<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::cmpr(const Real& src1, const Real& src2) noexcept {
        if (src1.isNaN() || src2.isNaN()) {
            _ac.conditionCode = 0b000;
        } else {
            compare(src1.floating, src2.floating);
        }
    }
    void Core::cmprl(const LongReal& src1, const LongReal& src2) noexcept {
        if (src1.isNaN() || src2.isNaN()) {
            _ac.conditionCode = 0b000;
        } else {
            compare(src1.floating, src2.floating);
        }
    }
    void Core::cmpre(const ExtendedReal& src1, const ExtendedReal& src2) noexcept {
        if (src1.isNaN() || src2.isNaN()) {
            _ac.conditionCode = 0b000;
        } else {
            compare(src1.floating, src2.floating);
        }
    }
    void Core::cmprl(Core::LongSourceRegister src1, Core::LongSourceRegister src2) noexcept { 
        auto r0 = src1.get<LongReal>();
        auto r1 = src2.get<LongReal>();
        if (r0.isNaN() || r1.isNaN()) {
            _ac.conditionCode = 0b000;
        } else {
            compare(r0.floating, r1.floating);
        }
    }
	void Core::divr(Core::SourceRegister src1, Core::SourceRegister src2, Core::DestinationRegister dest) noexcept {
		dest.set<RawReal>(src2.get<RawReal>() / src1.get<RawReal>());
	}
	void Core::divrl(Core::LongSourceRegister src1, Core::LongSourceRegister src2, Core::LongDestinationRegister dest) noexcept {
		dest.set<RawLongReal>(src2.get<RawLongReal>() / src1.get<RawLongReal>());
	}
    void Core::cmpor(Core::SourceRegister src1, Core::SourceRegister src2) noexcept {
        auto r0 = src1.get<Real>();
        auto r1 = src2.get<Real>();
        if (r0.isNaN() || r1.isNaN()) {
            // TODO raise floating invalid operation exception
            _ac.conditionCode = 0b000;
        } else {
            compare(r0.floating, r1.floating);
        }
    }
    void Core::cmporl(Core::LongSourceRegister src1, Core::LongSourceRegister src2) noexcept {
        auto r0 = src1.get<LongReal>();
        auto r1 = src2.get<LongReal>();
        if (r0.isNaN() || r1.isNaN()) {
            // TODO raise floating invalid operation exception
            _ac.conditionCode = 0b000;
        } else {
            compare(r0.floating, r1.floating);
        }
    }
    void Core::daddc(__DEFAULT_THREE_ARGS__) noexcept {
        auto s1 = src1.get<ByteOrdinal>() & 0b1111;
        auto s2 = src2.get<ByteOrdinal>() & 0b1111;
        ByteOrdinal comb = s2 + s1 + ByteOrdinal(_ac.getCarryValue());
        _ac.conditionCode = shouldSetCarryBit(comb) ? 0b010 : 0b000;
        auto upperBits = src2.get<Ordinal>() & (~0b1111);
        dest.set<Ordinal>(upperBits | static_cast<Ordinal>((comb & 0b1111)));
    }
    void Core::dsubc(__DEFAULT_THREE_ARGS__) noexcept {
        auto s1 = src1.get<ByteOrdinal>() & 0b1111;
        auto s2 = src2.get<ByteOrdinal>() & 0b1111;
        ByteOrdinal comb = s2 - s1 - 1 + ByteOrdinal(_ac.getCarryValue());
        _ac.conditionCode = shouldSetCarryBit(comb) ? 0b010 : 0b000;
        auto upperBits = src2.get<Ordinal>() & (~0b1111);
        dest.set<Ordinal>(upperBits | static_cast<Ordinal>((comb & 0b1111)));
    }
    void Core::dmovt(Core::SourceRegister src, Core::DestinationRegister dest) noexcept {
        dest.move(src);
        auto sval = src.get<ByteOrdinal>();
        _ac.conditionCode = ((sval >= 0b0011000) && (sval <= 0b00111001)) ? 0b000 : 0b010;
    }
	template<typename T, typename I>
	void performClassification(const T& src, ArithmeticControls& ac) noexcept {
		I val = src.template get<I>();
		auto s = (val.sign << 3) & 0b1000;
		if (val.isZero()) {
			ac.arithmeticStatusField = s | 0b000;
		} else if (val.isDenormal()) {
			ac.arithmeticStatusField = s | 0b001;
		} else if (val.isNormal()) {
			ac.arithmeticStatusField = s | 0b010;
		} else if (val.isInfinity()) {
			ac.arithmeticStatusField = s | 0b011;
		} else if (val.isQuietNaN()) {
			ac.arithmeticStatusField = s | 0b100;
		} else if (val.isSignalingNaN()) {
			ac.arithmeticStatusField = s | 0b101;
		} else if (val.isReservedEncoding()) {
			ac.arithmeticStatusField = s | 0b110;
		}
	}
    void Core::classr(Core::SourceRegister src) noexcept {
		performClassification<decltype(src), Real>(src, _ac);
    }
    void Core::classr(Core::ExtendedSourceRegister src) noexcept {
		performClassification<decltype(src), ExtendedReal>(src, _ac);
    }
    void Core::classrl(Core::LongSourceRegister src) noexcept {
		performClassification<decltype(src), LongReal>(src, _ac);
    }
    void Core::classrl(Core::ExtendedSourceRegister src) noexcept {
		performClassification<decltype(src), ExtendedReal>(src, _ac);
    }
#undef __DEFAULT_TWO_ARGS__
#undef __DEFAULT_DOUBLE_WIDE_TWO_ARGS__
#undef __DEFAULT_THREE_ARGS__
#undef __DEFAULT_DOUBLE_WIDE_THREE_ARGS__
} // end namespace i960
