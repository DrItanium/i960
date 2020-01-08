#include "ProcessControls.h"

namespace i960 {

void
ProcessControls::clear() noexcept {
    _traceEnable = false;
    _executionMode = false;
    _resume = false;
    _traceFaultPending = false;
    _state = false;
    _priority = 0;
}

void
ProcessControls::setRawValue(Ordinal raw) noexcept {
    _traceEnable = extractTraceEnable(raw);
    _executionMode = extractExecutionMode(raw);
    _resume = extractResume(raw);
    _traceFaultPending = extractTraceFaultPending(raw);
    _state = extractState(raw);
    _priority = extractPriority(raw);
}

} // end namespace i960
