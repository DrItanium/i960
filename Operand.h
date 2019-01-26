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
			static constexpr Ordinal encodingMask = 0b111111; 
			static constexpr Ordinal typeMask = 0b100000;
			static constexpr Ordinal valueMask = 0b011111;
			static constexpr Ordinal typeInputMask = 0b1;
			static constexpr Ordinal typeShiftAmount = 5;
			constexpr Operand(Ordinal rawValue) : _raw(rawValue & encodingMask) { }
			constexpr Operand(Ordinal type, Ordinal value) : Operand(((type & typeInputMask) << typeShiftAmount) | (value & valueMask)) { }
			constexpr bool isLiteral() const noexcept { return (_raw & typeMask) != 0; }
			constexpr bool isRegister() const noexcept { return (_raw & typeMask) == 0; }
			constexpr Ordinal getValue() const noexcept { return (_raw & valueMask); }
            constexpr Ordinal getOffset() const noexcept { return (_raw & 0b1111); }
			constexpr operator ByteOrdinal() const noexcept { return ByteOrdinal(getValue()); }
			constexpr auto notDivisibleBy(ByteOrdinal value) const noexcept { return (((ByteOrdinal)getValue()) % value) != 0; }
            constexpr auto isGlobalRegister() const noexcept { return isRegister() && (getValue() >= 0b10000); }
            constexpr auto isLocalRegister() const noexcept { return isRegister() && (getValue() < 0b10000); }
			constexpr Operand next() const noexcept {
				return Operand((_raw & typeMask) != 0, getValue() + 1);
			}
            constexpr bool operator ==(const Operand& other) const noexcept {
                return ((this->_raw & typeMask) == (other._raw & typeMask)) &&
                       (getValue() == other.getValue());
            }
		private:
			Ordinal _raw;
	};
    std::ostream& operator <<(std::ostream& stream, const Operand& op);

	constexpr Operand operator"" _lit(unsigned long long n) {
		return Operand(((Operand::valueMask & Ordinal(n)) + Operand::typeMask));
	}
	constexpr Operand operator"" _gr(unsigned long long n) {
		return Operand((n & 0b1111) + 0b10000);
	}
	constexpr Operand operator"" _lr(unsigned long long n) {
		return Operand((n & 0b1111));
	}

    constexpr auto PFP = 0_lr;
    constexpr auto SP = 1_lr;
    constexpr auto RIP = 2_lr;
    constexpr auto FP = 15_gr;
} // end namespace i960
#endif // end I960_OPERAND_H__
