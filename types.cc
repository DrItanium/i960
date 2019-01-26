#include "types.h"
#include <iostream>

namespace i960 {

void ProcessControls::clear() noexcept {
    value = 0;
}
void ProcessControls::enterSupervisorMode() noexcept {
    executionMode = 1;
}
void ProcessControls::enterUserMode() noexcept {
    executionMode = 0;
}

void TraceControls::clear() noexcept {
    value = 0;
}

void NormalRegister::move(const NormalRegister& other) noexcept {
    set<Ordinal>(other.get<Ordinal>());
}

void ArithmeticControls::clear() noexcept {
    value = 0;
}


Ordinal ArithmeticControls::modify(Ordinal mask, Ordinal value) noexcept {
    if (mask == 0) {
        return value;
    } else {
        auto tmp = value;
        value = (value & mask) | (value & ~(mask));
        return tmp;
    }
}

NormalRegister::~NormalRegister() {
    ordinal = 0;
}


void DoubleRegister::set(Ordinal lower, Ordinal upper) noexcept {
    _lower.set<Ordinal>(lower);
    _upper.set<Ordinal>(upper);
}

void TripleRegister::set(Ordinal l, Ordinal m, Ordinal u) noexcept {
    _lower.set<Ordinal>(l);
    _mid.set<Ordinal>(m);
    _upper.set<Ordinal>(u);
}


void QuadRegister::set(Ordinal l, Ordinal m, Ordinal u, Ordinal h) noexcept {
    _lower.set(l);
    _mid.set(m);
    _upper.set(u);
    _highest.set(h);
}

ArithmeticControls::~ArithmeticControls() {
    value = 0;
}

std::ostream& operator<<(std::ostream& stream, const Operand& op) {
    std::ios_base::fmtflags f(stream.flags());
    if (op.isLiteral()) {
        stream << std::dec << op.getValue();
    } else {
        if (op == PFP) {
            stream << "pfp";
        } else if (op == SP) {
            stream << "sp";
        } else if (op == RIP) {
            stream << "rip";
        } else if (op == FP) {
            stream << "fp";
        } else {
            stream << [op]() {
                if (op.isGlobalRegister()) {
                    return 'g';
                } else {
                    return 'r';
                }
            }() << op.getOffset();
        }
    }
    stream.flags(f);
    return stream;
}
} // end namespace i960
