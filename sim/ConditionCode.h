#ifndef I960_CONDITION_CODE_H__
#define I960_CONDITION_CODE_H__
#include "types.h"
namespace i960 {
    /// A programmer visible three bit type code but an internal four bit type code to accomodate
    /// a no cond case
    enum class ConditionCode : Ordinal {
        False = 0b0000,
        Greater = 0b0001,
        Equal = 0b0010,
        Less = 0b0100,
        Ordered = 0b0111,
        NotEqual = 0b0101,
        LessOrEqual = 0b0110,
        GreaterOrEqual = 0b0011,
        Unordered = False,
        NotOrdered = Unordered,
        True = Equal,
        Unconditional = 0b1111, // internal concept to cleanup code
    };
    using TestTypes = ConditionCode;
} // end namespace I960_CONDITION_CODE_H__
#endif // end I960_CONDITION_CODE_H__
