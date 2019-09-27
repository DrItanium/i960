#ifndef I960_ARITHMETIC_CONTROLS_H__
#define I960_ARITHMETIC_CONTROLS_H__

#include "types.h"
namespace i960 {
    /**
     * Arithmetic controls register, will eventually encompass all modes, core architecture only now.
     */
    class ArithmeticControls {
        public:
            constexpr ArithmeticControls(Ordinal rawValue = 0) noexcept : _rawValue(rawValue) { }
            ~ArithmeticControls() = default;
            constexpr auto getRawValue() const noexcept { return _rawValue; }
            constexpr auto getConditionCode() const noexcept { return _conditionCode; }
            template<Ordinal mask = 0b111>
            constexpr auto getConditionCode() const noexcept { 
                if constexpr (mask >= 0b111) {
                    return _conditionCode;
                } else if constexpr (mask == 0) {
                    return 0;
                } else {
                    return _conditionCode & mask;
                }
            }

            constexpr auto integerOverflowFlagSet() const noexcept { return _integerOverflowFlag; }
            constexpr auto maskIntegerOverflow() const noexcept { return _integerOverflowMask; }
            constexpr auto noImpreciseFaults() const noexcept { return _noImpreciseFaults; }
            void setRawValue(Ordinal value) noexcept { _rawValue = value; }
            void setConditionCode(Ordinal value) noexcept { _conditionCode = value; }
            void setIntegerOverflowFlag(bool value) noexcept { _integerOverflowFlag = value; }
            void setIntegerOverflowMask(bool value) noexcept { _integerOverflowMask = value; }
            void setNoImpreciseFaults(bool value) noexcept { _noImpreciseFaults = value; }
            void clearConditionCode() noexcept { _conditionCode = 0; }
            template<Ordinal mask>
            constexpr auto conditionCodeIs() const noexcept { return _conditionCode == mask; }
            template<Ordinal mask>
            constexpr bool conditionCodeBitSet() const noexcept { return getConditionCode<mask>() != 0; }
            constexpr bool shouldCarryOut() const noexcept {
                // 0b01X where X is don't care
                return _conditionCode == 0b010 || _conditionCode == 0b011;
            }
            constexpr bool markedAsOverflow() const noexcept {
                // 0b0X1 where X is don't care
                return _conditionCode == 0b001 || _conditionCode == 0b011;
            }
            constexpr bool carrySet() const noexcept { return conditionCodeBitSet<0b010>(); }
            constexpr Ordinal getCarryValue() const noexcept { return carrySet() ? 1 : 0; }
        private:
            union {
                struct {
                    // Core architecture AC
                    Ordinal _conditionCode : 3;
                    Ordinal reserved0 : 5;
                    /**
                     * Denotes an integer overflow happened
                     */
                    Ordinal _integerOverflowFlag : 1;
                    /**
                     * Reserved, bind to zero always
                     */
                    Ordinal reserved1 : 3;
                    /**
                     * Inhibit the processor from invoking a fault handler
                     * when integer overflow is detected.
                     */
                    Ordinal _integerOverflowMask : 1;
                    /**
                     * Reserved, always bind to zero
                     */
                    Ordinal reserved2 : 2;
                    /**
                     * Disable faults generated by imprecise results being generated
                     */
                    Ordinal _noImpreciseFaults : 1;
                    Ordinal reserved3 : 16;
                };
                Ordinal _rawValue;
            };
    };
    static_assert(sizeof(ArithmeticControls) == sizeof(Ordinal), "ArithmeticControls must be the same size as an ordinal");

} // end namespace i960
#endif // end I960_ARITHMETIC_CONTROLS_H__
