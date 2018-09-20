#include "types.h"
#include "core.h"
#include "operations.h"
#include "opcodes.h"
#include <limits>
#include <cmath>
#define __DEFAULT_THREE_ARGS__ Core::SourceRegister src1, Core::SourceRegister src2, Core::DestinationRegister dest
#define __DEFAULT_DOUBLE_WIDE_THREE_ARGS__ const DoubleRegister& src1, const DoubleRegister& src2, DoubleRegister& dest
#define __DEFAULT_TWO_ARGS__ Core::SourceRegister src, Core::DestinationRegister dest
#define __DEFAULT_DOUBLE_WIDE_TWO_ARGS__ const DoubleRegister& src, DoubleRegister& dest
namespace i960 {

    void Core::addr(__DEFAULT_THREE_ARGS__) noexcept {
        dest.set(src2.get<RawReal>() + src1.get<RawReal>());
    }
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
    void Core::cmpr(Core::SourceRegister src1, Core::SourceRegister src2) noexcept { compare( src1.get<RawReal>(), src2.get<RawReal>()); }
    void Core::cmprl(Core::LongSourceRegister src1, Core::LongSourceRegister src2) noexcept { compare( src1.get<RawLongReal>(), src2.get<RawLongReal>()); }
	void Core::divr(Core::SourceRegister src1, Core::SourceRegister src2, Core::DestinationRegister dest) noexcept {
		dest.set<RawReal>(src2.get<RawReal>() / src1.get<RawReal>());
	}
	void Core::divrl(Core::LongSourceRegister src1, Core::LongSourceRegister src2, Core::LongDestinationRegister dest) noexcept {
		dest.set<RawLongReal>(src2.get<RawLongReal>() / src1.get<RawLongReal>());
	}
#undef __DEFAULT_TWO_ARGS__
#undef __DEFAULT_DOUBLE_WIDE_TWO_ARGS__
#undef __DEFAULT_THREE_ARGS__
#undef __DEFAULT_DOUBLE_WIDE_THREE_ARGS__
} // end namespace i960
