#include "NormalRegister.h"

namespace i960 {

void NormalRegister::move(const NormalRegister& other) noexcept {
    set<Ordinal>(other.get<Ordinal>());
}
NormalRegister::~NormalRegister() {
    ordinal = 0;
}
void TraceControls::clear() noexcept {
    _instructionTraceMode = false;
    _branchTraceMode = false;
    _callTraceMode = false;
    _returnTraceMode = false;
    _prereturnTraceMode = false;
    _supervisorTraceMode = false;
    _markTraceMode = false;
    _instructionAddressBreakpoint0 = false;
    _instructionAddressBreakpoint1 = false;
    _dataAddressBreakpoint0 = false;
    _dataAddressBreakpoint1 = false;
}
void TraceControls::setRawValue(Ordinal raw) noexcept {
#define Base(extractorName, variable) \
        variable = extract ## extractorName( raw )
#define X(cap, lowcase) \
                Base(cap ## TraceMode, _ ## lowcase ## TraceMode)
#define Y(cap, lowcase) \
                Base(cap ## AddressBreakpoint0, _ ## lowcase ## AddressBreakpoint0); \
                Base(cap ## AddressBreakpoint1, _ ## lowcase ## AddressBreakpoint1)
            X(Instruction, instruction);
            X(Branch, branch);
            X(Call, call);
            X(Return, return);
            X(Prereturn, prereturn);
            X(Supervisor, supervisor);
            X(Mark, mark);
            Y(Instruction, instruction);
            Y(Data, data);
#undef X
#undef Y
#undef Base
}
} // end namespace i960
