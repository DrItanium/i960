#ifndef I960_ARCH_LEVEL_H__
#define I960_ARCH_LEVEL_H__
#include <ostream>
#ifdef PROTECTED_ARCHITECTURE
#error "Protected architecture not yet supported!"
#endif

namespace i960 {
	/**
	 * The level of support inside the simulator, not the current core!
	 */
	constexpr bool coreArchitectureSupported = true;
	constexpr bool numericsArchitectureSupported = true;
	constexpr bool protectedArchitectureSupported = false;
	constexpr bool extendedArchitectureSupported = false;
	static_assert(!protectedArchitectureSupported, "Protected architecture is documented but super complex, not currently supported by the simulator!");
	static_assert(!extendedArchitectureSupported, "Extended Architecture is not well documented so it is not supported at this time!");
}
#endif // end I960_ARCH_LEVEL_H__
