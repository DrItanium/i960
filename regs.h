#ifndef I960_REGS_H__
#define I960_REGS_H__
#include "types.h"
namespace i960 {
    union ArithmeticControls {
        struct {
            Ordinal _conditionCode : 3;
            /**
             * Used to record results from the classify real (classr and classrl)
             * and remainder real (remr and remrl) instructions. 
             */
            Ordinal _arithmeticStatusField : 4;
            Ordinal _unknown : 1 ; // TODO figure out what this is
            /**
             * Denotes an integer overflow happened
             */
            Ordinal _integerOverflowFlag : 1;
            Ordinal _unknown2 : 3;
            /**
             * Inhibit the processor from invoking a fault handler
             * when integer overflow is detected.
             */
            Ordinal _integerOverflowMask : 1;
            Ordinal _unknown3 : 3;
            /**
             * Floating point overflow happened?
             */
            Ordinal _floatingOverflowFlag : 1;
            /**
             * Floating point underflow happened?
             */
            Ordinal _floatingUnderflowFlag : 1;
            /**
             * Floating point invalid operation happened?
             */
            Ordinal _floatingInvalidOperationFlag : 1;
            /**
             * Floating point divide by zero happened?
             */
            Ordinal _floatingZeroDivideFlag : 1;
            /**
             * Floating point rounding result was inexact?
             */
            Ordinal _floatingInexactFlag : 1;
            Ordinal _unknown4 : 3;
            /**
             * Don't fault on fp overflow?
             */
            Ordinal _floatingOverflowMask : 1;
            /**
             * Don't fault on fp underflow?
             */
            Ordinal _floatingUnderflowMask : 1;
            /**
             * Don't fault on fp invalid operation?
             */
            Ordinal _floatingInvalidOperationMask : 1;
            /**
             * Don't fault on fp div by zero?
             */
            Ordinal _floatingZeroDivideMask : 1;
            /**
             * Don't fault on fp round producing an inexact representation?
             */
            Ordinal _floatingInexactMask : 1;
            /**
             * Enable to produce normalized values, disable to produce unnormalized values (useful for software simulation).
             * Note that the processor will fault if it encounters denormalized fp values!
             */
            Ordinal _normalizingModeFlag : 1;
            /**
             * What kind of fp rounding should be performed?
             * Modes:
             *  0: round up (towards positive infinity)
             *  1: round down (towards negative infinity)
             *  2: Round toward zero (truncate)
             *  3: Round to nearest (even)
             */
            Ordinal _roundingControl : 2;
        };
        Ordinal _value;
    };
    using InstructionPointer = Ordinal;
    union ProcessControls {
        // TODO add breakdown
        Ordinal _value;
    };
    union TraceControls {
        // TODO add value breakdown
        Ordinal _value;
    };
    constexpr auto GlobalRegisterCount = 16;
    constexpr auto NumFloatingPointRegs = 4;
    constexpr auto LocalRegisterCount = 16;
    constexpr Ordinal LargestAddress = 0xFFFFFFFF;
    constexpr Ordinal LowestAddress = 0;
}

#endif
