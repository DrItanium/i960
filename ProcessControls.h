#ifndef I960_PROCESS_CONTROLS_H__
#define I960_PROCESS_CONTROLS_H__
#include "types.h"
namespace i960 {
    class ProcessControls {
        public:
            static constexpr Ordinal TraceEnableMask = 0x0000'0001;
            static constexpr Ordinal ExecutionModeMask = 0x0000'0002;
            static constexpr Ordinal ResumeMask = 0x0000'0200;
            static constexpr Ordinal TraceFaultPendingMask = 0x0000'0400;
            static constexpr Ordinal StateMask = 0x0000'2000;
            static constexpr Ordinal PriorityMask = 0x001F'0000;
            static constexpr Ordinal CoreArchitectureExtractMask = TraceEnableMask | ExecutionModeMask | ResumeMask | TraceFaultPendingMask | StateMask | PriorityMask;
            static constexpr Ordinal CoreArchitectureReservedMask = ~CoreArchitectureExtractMask;
            static_assert(CoreArchitectureReservedMask == 0xFFE0'D9FC);
            static constexpr bool extractTraceEnable(Ordinal value) noexcept {
                return value & TraceEnableMask;
            }
            static constexpr bool extractExecutionMode(Ordinal value) noexcept { 
                return value & ExecutionModeMask;
            }
            static constexpr bool extractResume(Ordinal value) noexcept {
                return value & ResumeMask;
            }
            static constexpr bool extractTraceFaultPending(Ordinal value) noexcept {
                return value & TraceFaultPendingMask;
            }
            static constexpr bool extractState(Ordinal value) noexcept {
                return value & StateMask;
            }
            static constexpr Ordinal extractPriority(Ordinal value) noexcept {
                return static_cast<Ordinal>((value & PriorityMask) >> 16);
            }
            static constexpr Ordinal create(bool traceEnable, bool executionMode, bool resume, bool traceFaultPending, bool state, Ordinal priority) noexcept {
                auto mte = static_cast<Ordinal>(traceEnable);
                auto mem = static_cast<Ordinal>(executionMode) << 1;
                auto mre = static_cast<Ordinal>(resume) << 9;
                auto mtfp = static_cast<Ordinal>(traceFaultPending) << 10;
                auto mst = static_cast<Ordinal>(state) << 13;
                auto mpri = priority << 16;
                return (mte | mem | mre | mtfp | mst | mpri) & CoreArchitectureExtractMask;
            }
        public:
            constexpr ProcessControls() noexcept = default;
            constexpr ProcessControls(Ordinal raw) : 
                _traceEnable(extractTraceEnable(raw)),
                _executionMode(extractExecutionMode(raw)),
                _resume(extractResume(raw)),
                _traceFaultPending(extractTraceFaultPending(raw)),
                _state(extractState(raw)),
                _priority(extractPriority(raw)) { }
            constexpr bool getTraceMode() const noexcept            { return _traceEnable; }
            constexpr bool traceEnabled() const noexcept            { return _traceEnable != 0; }
            constexpr bool inUserMode() const noexcept              { return _executionMode == 0; }
            constexpr bool inSupervisorMode() const noexcept        { return _executionMode != 0; }
            constexpr bool traceFaultIsPending() const noexcept     { return _traceFaultPending != 0; }
            constexpr bool traceFaultIsNotPending() const noexcept  { return _traceFaultPending == 0; }
            constexpr bool isExecuting() const noexcept             { return _state == 0; }
            constexpr bool isInterrupted() const noexcept           { return _state != 0; }
            constexpr Ordinal getProcessPriority() const noexcept   { return _priority; }
            constexpr auto getRawValue() const noexcept { return create(_traceEnable, _executionMode, _resume, _traceFaultPending, _state, _priority); }
            void setState(bool value) noexcept { _state = value; }
            void enableTrace() noexcept { _traceEnable = true; }
            void disableTrace() noexcept { _traceEnable = false; }
            void setTraceMode(bool value) noexcept { _traceEnable = value; }
            void enterSupervisorMode() noexcept { _executionMode = true; }
            void enterUserMode() noexcept { _executionMode = false; }
            void interrupt() noexcept { _state = true; }
            void execute() noexcept { _state = false; }
            void setProcessPriority(Ordinal value) noexcept { _priority = value; }
            void markTraceFaultPending() noexcept { _traceFaultPending = true; }
            void markTraceFaultNotPending() noexcept { _traceFaultPending = false; }
            //void setRawValue(Ordinal value) noexcept { _value = value; }
            void setRawValue(Ordinal value) noexcept;
        public:
            void clear() noexcept;
        private:
            bool _traceEnable = false;
            bool _executionMode = false;
            bool _resume = false;
            bool _traceFaultPending = false;
            bool _state = false;
            Ordinal _priority = 0;
    };
    static_assert(ProcessControls::create(true, true, true, true, true, 0b11111) == ProcessControls::CoreArchitectureExtractMask);
} // end namespace i960
#endif // end I960_PROCESS_CONTROLS_H__
