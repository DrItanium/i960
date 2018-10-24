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
    void Core::cmpr(const Real& src1, const Real& src2, bool ordered) noexcept {
        if (src1.isNaN() || src2.isNaN()) {
            _ac.conditionCode = 0b000;
            if (ordered) {
                // TODO raise floating point invalid operation exception
            }
        } else {
            compare(src1.floating, src2.floating);
        }
    }
    void Core::cmprl(const LongReal& src1, const LongReal& src2, bool ordered) noexcept {
        if (src1.isNaN() || src2.isNaN()) {
            _ac.conditionCode = 0b000;
            if (ordered) {
                // TODO raise floating point invalid operation exception
            }
        } else {
            compare(src1.floating, src2.floating);
        }
    }
    void Core::cmpre(const ExtendedReal& src1, const ExtendedReal& src2, bool ordered) noexcept {
        if (src1.isNaN() || src2.isNaN()) {
            _ac.conditionCode = 0b000;
            if (ordered) {
                // TODO raise floating point invalid operation exception
            }
        } else {
            compare(src1.floating, src2.floating);
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
