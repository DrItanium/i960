#ifndef I960_ARCH_LEVEL_H__
#define I960_ARCH_LEVEL_H__
#include <ostream>
#ifdef EXTENDED_ARCHITECTURE
#	ifndef PROTECTED_ARCHITECTURE
#		define PROTECTED_ARCHITECTURE
#	endif // end PROTECTED_ARCHITECTURE
#endif  // end EXTENDED_ARCHITECTURE
#ifdef PROTECTED_ARCHITECTURE
#	ifndef NUMERICS_ARCHITECTURE
		// protected implies numerics
#		define NUMERICS_ARCHITECTURE
#	endif // end NUMERICS_ARCHITECTURE
#endif // end PROTECTED_ARCHITECTURE

namespace i960 {
	constexpr bool coreArchitectureSupported() noexcept {
		return true;
	}

	constexpr bool numericsArchitectureSupported() noexcept {
#ifdef NUMERICS_ARCHITECTURE
		return true;
#else
		return false;
#endif // end NUMERICS_ARCHITECTURE
	}
	constexpr bool protectedArchitectureSupported() noexcept {
#ifdef PROTECTED_ARCHITECTURE
		return true;
#else
		return false;
#endif // end PROTECTED_ARCHITECTURE
	}
	constexpr bool extendedArchitectureSupported() noexcept {
#ifdef EXTENDED_ARCHITECTURE
		return true;
#else
		return false;
#endif // end EXTENDED_ARCHITECTURE
	}
	void outputSupportMessage(std::ostream& os);
	static_assert(!extendedArchitectureSupported(), "Extended Architecture is not well documented so it is not supported at this time!");
}
#endif // end I960_ARCH_LEVEL_H__
