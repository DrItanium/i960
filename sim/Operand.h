#ifndef I960_OPERAND_H__
#define I960_OPERAND_H__
#include "types.h"
namespace i960 {
	/**
	 * Describes a register or literal in a single type. Internally
	 * the operand is a 6 bit number where bits [0,4] represent the value
	 * with the uppermost bit (bit 5) denoting if it is a literal or register
	 * reference.
	 */
	struct Operand final {
		public:
			static constexpr Ordinal encodingMask = 0b1'11111; 
			static constexpr Ordinal typeMask = 0b100000;
			static constexpr Ordinal valueMask = 0b011111;
			static constexpr Ordinal typeInputMask = 0b1;
			static constexpr Ordinal typeShiftAmount = 5;
            static constexpr Ordinal RegisterClassMask = 0b1111;
            static constexpr Ordinal GlobalRegisterId = 0b10000;
            static constexpr Operand makeLiteral(Ordinal value) noexcept {
                return Operand(((Operand::valueMask & Ordinal(value)) + Operand::typeMask));
            }
            static constexpr Operand makeGlobalRegister(Ordinal field) noexcept {
                return {(field & RegisterClassMask) + GlobalRegisterId};
            }
            static constexpr Operand makeLocalRegister(Ordinal field) noexcept {
                return { field & RegisterClassMask };
            }
			constexpr Operand(Ordinal rawValue) noexcept : _raw(rawValue & encodingMask) { }
			constexpr Operand(Ordinal type, Ordinal value) noexcept : Operand(((type & typeInputMask) << typeShiftAmount) | (value & valueMask)) { }
			constexpr bool isLiteral() const noexcept { return (_raw & typeMask) != 0; }
			constexpr bool isRegister() const noexcept { return (_raw & typeMask) == 0; }
			constexpr ByteOrdinal getValue() const noexcept { return (_raw & valueMask); }
            constexpr ByteOrdinal getOffset() const noexcept { return (_raw & RegisterClassMask); }
			constexpr auto notDivisibleBy(ByteOrdinal value) const noexcept { return (((ByteOrdinal)getValue()) % value) != 0; }
            constexpr auto isGlobalRegister() const noexcept { return isRegister() && (getValue() >= GlobalRegisterId); }
            constexpr auto isLocalRegister() const noexcept { return isRegister() && (getValue() < GlobalRegisterId); }
			constexpr Operand next() const noexcept {
				return Operand((_raw & typeMask) != 0, getValue() + 1);
			}
            constexpr bool operator ==(const Operand& other) const noexcept {
                return _raw == other._raw;
            }
			constexpr operator ByteOrdinal() const noexcept { return ByteOrdinal(getValue()); }
		private:
			Ordinal _raw;
	};
    std::ostream& operator <<(std::ostream& stream, const Operand& op);

	constexpr Operand operator"" _lit(unsigned long long n) noexcept {
        return Operand::makeLiteral(n);
	}
	constexpr Operand operator"" _gr(unsigned long long n) noexcept {
        return Operand::makeGlobalRegister(n);
	}
	constexpr Operand operator"" _lr(unsigned long long n) noexcept {
        return Operand::makeLocalRegister(n);
	}
    constexpr auto PFP = 0_lr;
    constexpr auto SP = 1_lr;
    constexpr auto RIP = 2_lr;
    constexpr auto FP = 15_gr;
} // end namespace i960
#endif // end I960_OPERAND_H__
