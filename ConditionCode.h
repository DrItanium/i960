#ifndef I960_CONDITION_CODE_H__
#define I960_CONDITION_CODE_H__
#include "types.h"
namespace i960 {
    enum class ConditionCode : Ordinal {
        False = 0b000,
        Greater = 0b001,
        Equal = 0b010,
        Less = 0b100,
        Ordered = 0b111,
        NotEqual = 0b101,
        LessOrEqual = 0b110,
        GreaterOrEqual = 0b011,
        Unordered = False,
        NotOrdered = Unordered,
        True = Equal,
    };
    using TestTypes = ConditionCode;
} // end namespace I960_CONDITION_CODE_H__
#endif // end I960_CONDITION_CODE_H__
