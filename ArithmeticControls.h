#ifndef I960_ARITHMETIC_CONTROLS_H__
#define I960_ARITHMETIC_CONTROLS_H__

#include "types.h"
namespace i960 {
    /**
     * Arithmetic controls register, will eventually encompass all modes, core architecture only now.
     */
    class ArithmeticControls {
        public:
            constexpr ArithmeticControls(Ordinal rawValue) noexcept : 
                _conditionCode(extractConditionCode(rawValue)),
                _integerOverflowFlag(extractIntegerOverflowFlag(rawValue)),
                _integerOverflowMask(extractIntegerOverflowMask(rawValue)),
                _noImpreciseFaults(extractNoImpreciseFaults(rawValue)) { }
            constexpr ArithmeticControls() noexcept = default;
            ~ArithmeticControls() = default;
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

            constexpr auto getConditionCode(Ordinal mask) const noexcept {
                return _conditionCode & mask;
            }

            constexpr auto integerOverflowFlagSet() const noexcept { return _integerOverflowFlag; }
            constexpr auto maskIntegerOverflow() const noexcept { return _integerOverflowMask; }
            constexpr auto noImpreciseFaults() const noexcept { return _noImpreciseFaults; }
            void setConditionCode(ByteOrdinal value) noexcept { _conditionCode = value; }
            void setConditionCode(bool cond, Ordinal onTrue, Ordinal onFalse) noexcept {
                if (cond) {
                    setConditionCode(onTrue);
                } else {
                    setConditionCode(onFalse);
                }
            }
            void setIntegerOverflowFlag(bool value) noexcept { _integerOverflowFlag = value; }
            void setIntegerOverflowMask(bool value) noexcept { _integerOverflowMask = value; }
            void setNoImpreciseFaults(bool value) noexcept { _noImpreciseFaults = value; }
            void clearConditionCode() noexcept { _conditionCode = 0; }
            template<ByteOrdinal mask>
            constexpr auto conditionCodeIs() const noexcept { return _conditionCode == mask; }
            template<ByteOrdinal ... masks>
            constexpr auto conditionCodeIsOneOf() const noexcept { 
                return (conditionCodeIs<masks>() || ... );
            }
            template<ByteOrdinal mask>
            constexpr bool conditionCodeBitSet() const noexcept { return getConditionCode<mask>() != 0; }
            constexpr bool conditionCodeBitSet(ByteOrdinal mask) const noexcept { return getConditionCode(mask) != 0; }
            constexpr bool shouldCarryOut() const noexcept {
                // 0b01X where X is don't care
                return conditionCodeIsOneOf<0b010, 0b011>();
            }
            constexpr bool markedAsOverflow() const noexcept {
                // 0b0X1 where X is don't care
                return conditionCodeIsOneOf<0b001, 0b011>();
            }
            constexpr bool carrySet() const noexcept { return conditionCodeBitSet<0b010>(); }
            constexpr ByteOrdinal getCarryValue() const noexcept { return carrySet() ? 1 : 0; }
            constexpr Ordinal getRawValue() const noexcept { return create(_conditionCode, _integerOverflowFlag, _integerOverflowMask, _noImpreciseFaults); }
            void setRawValue(Ordinal value) noexcept {
                _conditionCode = extractConditionCode(value);
                _integerOverflowFlag = extractIntegerOverflowFlag(value);
                _integerOverflowMask = extractIntegerOverflowMask(value);
                _noImpreciseFaults = extractNoImpreciseFaults(value);
            }
        private:
            // numerics architecture arithmetic controls
            /**
             * The condition code computed by compare operations (3 bits)
             */
             ByteOrdinal _conditionCode = 0;

            /**
             * Arithmetic status flags (4 bits)
             */
            ByteOrdinal _arithmeticStatusFlags = 0;

            /**
             * Denotes an integer overflow happened
             */
            bool _integerOverflowFlag = false;
            /**
             * Inhibit the processor from invoking a fault handler
             * when integer overflow is detected.
             */
            bool _integerOverflowMask = false;
            /**
             * Disable faults generated by imprecise results being generated
             */
            bool _noImpreciseFaults = false;

            bool _floatingOverflowFlag = false;
            bool _floatingUnderflowFlag = false;
            bool _floatingInvalidOpFlag = false;
            bool _floatingZeroDivideFlag = false;
            bool _floatingInexactFlag = false;

            bool _floatingOverflowMask = false;
            bool _floatingUnderflowMask = false;
            bool _floatingInvalidOpMask = false;
            bool _floatingZeroDivideMask = false;
            bool _floatingInexactMask = false;
            bool _floatingNormalizingMode = false;
            ByteOrdinal _floatingRoundingControls = 0;
        public:
            enum Masks : Ordinal {
                ConditionCode                = 0b0000'0000'0000'0000'0000'0000'0000'0111,
                ArithmeticStatus             = 0b0000'0000'0000'0000'0000'0000'0111'1000,
                Reserved0                    = 0b0000'0000'0000'0000'0000'0000'1000'0000,
                IntegerOverflowFlag          = 0b0000'0000'0000'0000'0000'0001'0000'0000,
                Reserved1                    = 0b0000'0000'0000'0000'0000'1110'0000'0000,
                IntegerOverflowMask          = 0b0000'0000'0000'0000'0001'0000'0000'0000,
                Reserved2                    = 0b0000'0000'0000'0000'0110'0000'0000'0000,
                NoImpreciseFaults            = 0b0000'0000'0000'0000'1000'0000'0000'0000,
                FloatingOverflowFlag         = 0b0000'0000'0000'0001'0000'0000'0000'0000,
                FloatingUnderflowFlag        = 0b0000'0000'0000'0010'0000'0000'0000'0000,
                FloatingInvalidOpFlag        = 0b0000'0000'0000'0100'0000'0000'0000'0000,
                FloatingZeroDivideFlag       = 0b0000'0000'0000'1000'0000'0000'0000'0000,
                FloatingInexactFlag          = 0b0000'0000'0001'0000'0000'0000'0000'0000,
                Reserved3                    = 0b0000'0000'1110'0000'0000'0000'0000'0000,
                FloatingOverflowMask         = 0b0000'0001'0000'0000'0000'0000'0000'0000,
                FloatingUnderflowMask        = 0b0000'0010'0000'0000'0000'0000'0000'0000,
                FloatingInvalidOpMask        = 0b0000'0100'0000'0000'0000'0000'0000'0000,
                FloatingZeroDivideMask       = 0b0000'1000'0000'0000'0000'0000'0000'0000,
                FloatingInexactMask          = 0b0001'0000'0000'0000'0000'0000'0000'0000,
                FloatingPointNormalizingMode = 0b0010'0000'0000'0000'0000'0000'0000'0000,
                FloatingPointRoundingControl = 0b1100'0000'0000'0000'0000'0000'0000'0000,
            };
            using ConditionCodeEncoderDecoder = BitPattern<Ordinal, ByteOrdinal, Masks::ConditionCode, 0>;
            using ArithmeticStatusEncoderDecoder = BitPattern<Ordinal, ByteOrdinal, Masks::ArithmeticStatus, 3>;
            using IntegerOverflowFlagEncoderDecoder = FlagFragment<Ordinal, Masks::IntegerOverflowFlag>;
            using IntegerOverflowMaskEncoderDecoder = FlagFragment<Ordinal, Masks::IntegerOverflowMask>;
            using NoImpreciseFaultsEncoderDecoder = FlagFragment<Ordinal, Masks::NoImpreciseFaults>;
            using FloatingOverflowFlagEncoderDecoder = FlagFragment<Ordinal, Masks::FloatingOverflowFlag>;
            using FloatingUnderflowFlagEncoderDecoder = FlagFragment<Ordinal, Masks::FloatingUnderflowFlag>;
            using FloatingInvalidOpFlagEncoderDecoder = FlagFragment<Ordinal, Masks::FloatingInvalidOpFlag>;
            using FloatingZeroDivideFlagEncoderDecoder = FlagFragment<Ordinal, Masks::FloatingZeroDivideFlag>;
            using FloatingInexactFlagEncoderDecoder = FlagFragment<Ordinal, Masks::FloatingInexactFlag>;
            using FloatingOverflowMaskEncoderDecoder = FlagFragment<Ordinal, Masks::FloatingOverflowMask>;
            using FloatingUnderflowMaskEncoderDecoder = FlagFragment<Ordinal, Masks::FloatingUnderflowMask>;
            using FloatingInvalidOpMaskEncoderDecoder = FlagFragment<Ordinal, Masks::FloatingInvalidOpMask>;
            using FloatingZeroDivideMaskEncoderDecoder = FlagFragment<Ordinal, Masks::FloatingZeroDivideMask>;
            using FloatingInexactMaskEncoderDecoder = FlagFragment<Ordinal, Masks::FloatingInexactMask>;
            using FloatingPointNormalizingModeEncoderDecoder = FlagFragment<Ordinal, Masks::FloatingPointNormalizingMode>;
            using FloatingPointRoundingControlEncoderDecoder = BitPattern<Ordinal, ByteOrdinal, Masks::FloatingPointRoundingControl, 30>;
            static constexpr Ordinal CoreArchitectureExtractMask = constructOrdinalMask(
                    Masks::ConditionCode, 
                    Masks::IntegerOverflowFlag, 
                    Masks::IntegerOverflowMask, 
                    Masks::NoImpreciseFaults);
            using CoreArchitectureEncoderDecoder = BinaryEncoderDecoder<Ordinal, 
                  ConditionCodeEncoderDecoder, 
                  IntegerOverflowFlagEncoderDecoder, 
                  IntegerOverflowMaskEncoderDecoder, 
                  NoImpreciseFaultsEncoderDecoder>;
        public:
            static constexpr Ordinal create(ByteOrdinal conditionCode, bool integerOverflowFlag, bool integerOverflowMask, bool noImpreciseFaults) noexcept {
                return CoreArchitectureEncoderDecoder::encode(std::move(conditionCode), 
                        std::move(integerOverflowFlag), 
                        std::move(integerOverflowMask), 
                        std::move(noImpreciseFaults));
            }
            static constexpr Ordinal extractConditionCode(Ordinal raw) noexcept {
                return ConditionCodeEncoderDecoder::decodePattern(raw);
            }
            static constexpr bool extractIntegerOverflowFlag(Ordinal raw) noexcept {
                return IntegerOverflowFlagEncoderDecoder::decodePattern(raw);
            }
            static constexpr bool extractIntegerOverflowMask(Ordinal raw) noexcept {
                return IntegerOverflowMaskEncoderDecoder::decodePattern(raw);
            }
            static constexpr bool extractNoImpreciseFaults(Ordinal raw) noexcept {
                return NoImpreciseFaultsEncoderDecoder::decodePattern(raw);
            }
    };
    static_assert(ArithmeticControls::create(0b111, true, true, true) == ArithmeticControls::CoreArchitectureExtractMask, "create(CoreArchitecture) failed!");
    static_assert(ArithmeticControls::extractIntegerOverflowFlag(ArithmeticControls::Masks::IntegerOverflowFlag));
    static_assert(ArithmeticControls::extractIntegerOverflowMask(ArithmeticControls::Masks::IntegerOverflowMask));
    static_assert(ArithmeticControls::extractNoImpreciseFaults(ArithmeticControls::Masks::NoImpreciseFaults));
    static_assert(ArithmeticControls::extractConditionCode(0xF5) == 0x5);

} // end namespace i960
#endif // end I960_ARITHMETIC_CONTROLS_H__
