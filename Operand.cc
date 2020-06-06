#include "Operand.h"
#include <iostream>

namespace i960 {

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
            }() << std::dec << static_cast<int>(op.getOffset());
        }
    }
    stream.flags(f);
    return stream;
}
} // end namespace i960
