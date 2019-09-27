#ifndef I960_PROCESS_CONTROLS_H__
#define I960_PROCESS_CONTROLS_H__
#include "types.h"
namespace i960 {
    class ProcessControls {
        public:
            constexpr ProcessControls(Ordinal raw = 0) : _value(raw) { }
            constexpr bool traceEnabled() const noexcept            { return _traceEnable != 0; }
            constexpr bool inUserMode() const noexcept              { return _executionMode == 0; }
            constexpr bool inSupervisorMode() const noexcept        { return _executionMode != 0; }
            constexpr bool traceFaultIsPending() const noexcept     { return _traceFaultPending != 0; }
            constexpr bool traceFaultIsNotPending() const noexcept  { return _traceFaultPending == 0; }
            constexpr bool isExecuting() const noexcept             { return _state == 0; }
            constexpr bool isInterrupted() const noexcept           { return _state != 0; }
            constexpr Ordinal getProcessPriority() const noexcept   { return _priority; }
            constexpr auto getRawValue() const noexcept { return _value; }
            void setState(bool value) noexcept { _state = value; }
            void enableTrace() noexcept { _traceEnable = true; }
            void disableTrace() noexcept { _traceEnable = false; }
            void enterSupervisorMode() noexcept { _executionMode = true; }
            void enterUserMode() noexcept { _executionMode = false; }
            void interrupt() noexcept { _state = true; }
            void execute() noexcept { _state = false; }
            void setProcessPriority(Ordinal value) noexcept { _priority = value; }
            void markTraceFaultPending() noexcept { _traceFaultPending = true; }
            void markTraceFaultNotPending() noexcept { _traceFaultPending = false; }
            void setRawValue(Ordinal value) noexcept { _value = value; }
        public:
            void clear() noexcept { _value = 0; }
        private:
            union {
                struct {
                    Ordinal _traceEnable : 1;
                    Ordinal _executionMode : 1;
                    Ordinal _unused0 : 7;
                    Ordinal _resume : 1;
                    Ordinal _traceFaultPending : 1;
                    Ordinal _unused1 : 2;
                    Ordinal _state : 1;
                    Ordinal _unused2 : 2;
                    Ordinal _priority : 5;
                    Ordinal _unused3 : 11;
                };
                Ordinal _value;
            };
    };
} // end namespace i960
#endif // end I960_PROCESS_CONTROLS_H__
