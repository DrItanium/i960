#include "ProcessControls.h"

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

} // end namespace i960
