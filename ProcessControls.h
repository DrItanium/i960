#ifndef I960_PROCESS_CONTROLS_H__
#define I960_PROCESS_CONTROLS_H__
#include "types.h"
namespace i960 {
    union ProcessControls {
        struct {
            Ordinal traceEnable : 1;
            Ordinal executionMode : 1;
            Ordinal unused0 : 7;
            Ordinal resume : 1;
            Ordinal traceFaultPending : 1;
            Ordinal unused1 : 2;
            Ordinal state : 1;
            Ordinal unused2 : 2;
            Ordinal priority : 5;
            Ordinal unused3 : 11;
        };
        Ordinal value;
        constexpr ProcessControls(Ordinal raw = 0) : value(raw) { }
		constexpr bool traceEnabled() const noexcept            { return traceEnable != 0; }
		constexpr bool inUserMode() const noexcept              { return executionMode == 0; }
		constexpr bool inSupervisorMode() const noexcept        { return executionMode != 0; }
		constexpr bool traceFaultIsPending() const noexcept     { return traceFaultPending != 0; }
		constexpr bool traceFaultIsNotPending() const noexcept  { return traceFaultPending == 0; }
		constexpr bool isExecuting() const noexcept             { return state == 0; }
		constexpr bool isInterrupted() const noexcept           { return state != 0; }
		constexpr Ordinal getProcessPriority() const noexcept   { return priority; }
		void clear() noexcept;
		void enterSupervisorMode() noexcept;
		void enterUserMode() noexcept;
    } __attribute__((packed));
} // end namespace i960
#endif // end I960_PROCESS_CONTROLS_H__
