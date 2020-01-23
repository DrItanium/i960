#ifndef I960_PROCESS_CONTROLS_H__
#define I960_PROCESS_CONTROLS_H__
#include "types.h"
namespace i960 {
    class ProcessControls {
        public:
            static constexpr FlagFragment<Ordinal, 0x0000'0001> TraceEnableMask{};
            static constexpr FlagFragment<Ordinal, 0x0000'0002> ExecutionModeMask{};
            static constexpr FlagFragment<Ordinal, 0x0000'0200> ResumeMask{};
            static constexpr FlagFragment<Ordinal, 0x0000'0400> TraceFaultPendingMask{};
            static constexpr FlagFragment<Ordinal, 0x0000'2000> StateMask{};
            static constexpr SameWidthFragment<Ordinal, 0x001F'0000, 16> PriorityMask{};
        private:
            static constexpr Ordinal InternalPriorityMask = 0x1F;
        public:
            static constexpr Ordinal CoreArchitectureExtractMask = constructOrdinalMask(TraceEnableMask.getMask(), ExecutionModeMask.getMask(), ResumeMask.getMask(), TraceFaultPendingMask.getMask(), StateMask.getMask(), PriorityMask.getMask());
            static constexpr Ordinal CoreArchitectureReservedMask = ~CoreArchitectureExtractMask;
            static_assert(CoreArchitectureReservedMask == 0xFFE0'D9FC);
            static constexpr bool extractTraceEnable(Ordinal value) noexcept {
                return TraceEnableMask.decode(value);
            }
            static constexpr bool extractExecutionMode(Ordinal value) noexcept { 
                return ExecutionModeMask.decode(value);
            }
            static constexpr bool extractResume(Ordinal value) noexcept {
                return ResumeMask.decode(value);
            }
            static constexpr bool extractTraceFaultPending(Ordinal value) noexcept {
                return TraceFaultPendingMask.decode(value);
            }
            static constexpr bool extractState(Ordinal value) noexcept {
                return StateMask.decode(value);
            }
            static constexpr Ordinal extractPriority(Ordinal value) noexcept {
                return PriorityMask.decode(value);
            }
            static constexpr Ordinal create(bool traceEnable, bool executionMode, bool resume, bool traceFaultPending, bool state, Ordinal priority) noexcept {
                return ((TraceEnableMask.encode(traceEnable)) |
                       (ExecutionModeMask.encode(executionMode)) |
                       (ResumeMask.encode(resume)) |
                       (TraceFaultPendingMask.encode(traceFaultPending)) |
                       (StateMask.encode(state)) |
                       (PriorityMask.encode(priority))) & CoreArchitectureExtractMask;
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
            void setState(bool value) noexcept              { _state = value; }
            void enableTrace() noexcept                     { _traceEnable = true; }
            void disableTrace() noexcept                    { _traceEnable = false; }
            void setTraceMode(bool value) noexcept          { _traceEnable = value; }
            void enterSupervisorMode() noexcept             { _executionMode = true; }
            void enterUserMode() noexcept                   { _executionMode = false; }
            void interrupt() noexcept                       { _state = true; }
            void execute() noexcept                         { _state = false; }
            void setProcessPriority(Ordinal value) noexcept { _priority = (value & InternalPriorityMask); }
            void markTraceFaultPending() noexcept           { _traceFaultPending = true; }
            void markTraceFaultNotPending() noexcept        { _traceFaultPending = false; }
            void setRawValue(Ordinal value) noexcept;
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
