/**
 * @file
 * Interface to an i960Jx timer found on the CPU die
 */

#ifndef I960_TIMER_H__
#define I960_TIMER_H__
#include "types.h"

namespace i960 {
/**
 * A fully independent 32-bit timer found on the die of an i960jx processor
 */
class Timer {
    public:
        union ModeRegister {
            /** 
             * When reload is not set, then this bit is set when the TCR reaches zero;This bit is unpredictable when reload is set.
             */
            Ordinal tc : 1;
            /**
             * Controls the timer's run/stop status, when set then decrement count register every timer clock cycle (TCLOCK). 
             * TCLOCK is determined by the csel value.
             */
            Ordinal enable : 1;
            /**
             * Determines whether the timer runs continuously or in single-shot mode. 
             * If reload = 1, then allow the timer to run continuously.
             * If reload = 0, then timer runs until count register is zero
             */
            Ordinal reload : 1;
            /**
             * Determines whether user modes writes are permitted to the registers. 
             * Supervisor writes are allowed regardless of this bit's condition. It is
             * always legal to read the registers. If this bit is set, then a fault
             * will be generated on user mode write (TYPE.MISMATCH).
             */
            Ordinal sup : 1;
            /**
             * Select the timer clock rate
             */
            Ordinal csel : 2;
            /**
             * The entire register contents 
             */
            Ordinal raw;
        };
        using CountRegister = Ordinal;
        /**
         * User programs this value to contain the timer's reload count. 
         * Only used when auto-reload is active. Every time the count reaches zero
         * then this value is reloaded into the count register
         */
        using ReloadRegister = Ordinal;
    public:
        ModeRegister getMode() const noexcept;
        CountRegister getCount() const noexcept;
        ReloadRegister getReload() const noexcept;
        // TODO implement logic
    private:
        ModeRegister _mode;
        CountRegister _count;
        ReloadRegister _reload;

        

};
} // end namespace i960

#endif // end I960_TIMER_H__
