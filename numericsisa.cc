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
    void Core::addr(__DEFAULT_THREE_ARGS__) noexcept { add<decltype(src1), decltype(src2), decltype(dest), RawReal>(src1, src2, dest); }
    void Core::addr(SourceRegister src1, SourceRegister src2, ExtendedDestinationRegister dest) noexcept { add<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::addr(SourceRegister src1, ExtendedSourceRegister src2, ExtendedDestinationRegister dest) noexcept { add<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::addr(SourceRegister src1, ExtendedSourceRegister src2, DestinationRegister dest) noexcept { add<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::addr(ExtendedSourceRegister src1, ExtendedSourceRegister src2, ExtendedDestinationRegister dest) noexcept { add<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::addr(ExtendedSourceRegister src1, ExtendedSourceRegister src2, DestinationRegister dest) noexcept { add<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::addr(ExtendedSourceRegister src1, SourceRegister src2, ExtendedDestinationRegister dest) noexcept { add<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::addr(ExtendedSourceRegister src1, SourceRegister src2, DestinationRegister dest) noexcept { add<decltype(src1), decltype(src2), decltype(dest), RawExtendedReal>(src1, src2, dest); }
    void Core::addrl(__DEFAULT_DOUBLE_WIDE_THREE_ARGS__) noexcept {
        dest.set(src2.get<RawLongReal>() + src1.get<RawLongReal>());
    }
    void Core::subr(__DEFAULT_THREE_ARGS__) noexcept {
        dest.set(src2.get<RawReal>() - src1.get<RawReal>());
    }
    void Core::subrl(__DEFAULT_DOUBLE_WIDE_THREE_ARGS__) noexcept {
        dest.set(src2.get<RawLongReal>() - src1.get<RawLongReal>());
    }
    void Core::mulr(__DEFAULT_THREE_ARGS__) noexcept {
        dest.set(src2.get<RawReal>() * src1.get<RawReal>());
    }
    void Core::mulrl(__DEFAULT_DOUBLE_WIDE_THREE_ARGS__) noexcept {
        dest.set(src2.get<RawLongReal>() * src1.get<RawLongReal>());
    }
    void Core::xnor(__DEFAULT_THREE_ARGS__) noexcept {
        dest.set<Ordinal>(i960::xnor<Ordinal>(src1.get<Ordinal>(), src2.get<Ordinal>()));
    }
    void Core::xorOp(__DEFAULT_THREE_ARGS__) noexcept {
        dest.set<Ordinal>(i960::xorOp<Ordinal>(src1.get<Ordinal>(), src2.get<Ordinal>()));
    }
    void Core::tanr(__DEFAULT_TWO_ARGS__) noexcept {
        dest.set<RawReal>(::tan(src.get<RawReal>()));
    }
    void Core::tanrl(__DEFAULT_DOUBLE_WIDE_TWO_ARGS__) noexcept {
        dest.set<RawLongReal>(::tan(src.get<RawLongReal>()));
    }
    void Core::cosr(__DEFAULT_TWO_ARGS__) noexcept {
        dest.set<RawReal>(::cos(src.get<RawReal>()));
    }
    void Core::cosrl(__DEFAULT_DOUBLE_WIDE_TWO_ARGS__) noexcept {
        dest.set<RawLongReal>(::cos(src.get<RawLongReal>()));
    }
    void Core::sinr(__DEFAULT_TWO_ARGS__) noexcept {
        dest.set<RawReal>(::sin(src.get<RawReal>()));
    }
    void Core::sinrl(__DEFAULT_DOUBLE_WIDE_TWO_ARGS__) noexcept {
        dest.set<RawLongReal>(::sin(src.get<RawLongReal>()));
    }
    void Core::atanr(__DEFAULT_THREE_ARGS__) noexcept {
        dest.set<RawReal>(::atan(src2.get<RawReal>() / src1.get<RawReal>()));
    }
    void Core::atanrl(__DEFAULT_DOUBLE_WIDE_THREE_ARGS__) noexcept {
        dest.set<RawLongReal>(::atan(src2.get<RawLongReal>() / src1.get<RawLongReal>()));
    }
    void Core::cmpr(Core::SourceRegister src1, Core::SourceRegister src2) noexcept { 
        auto r0 = src1.get<Real>();
        auto r1 = src2.get<Real>();
        if (r0.isNaN() || r1.isNaN()) {
            _ac.conditionCode = 0b000;
        } else {
            compare(r0.floating, r1.floating);
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
