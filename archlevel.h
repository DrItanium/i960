#ifndef I960_ARCH_LEVEL_H__
#define I960_ARCH_LEVEL_H__

#ifdef EXTENDED_ARCHITECTURE
	#ifndef PROTECTED_ARCHITECTURE
		#define PROTECTED_ARCHITECTURE
	#endif // end PROTECTED_ARCHITECTURE
#endif  // end EXTENDED_ARCHITECTURE

#ifdef PROTECTED_ARCHITECTURE
    #ifndef NUMERICS_ARCHITECTURE
        // protected implies numerics
        #define NUMERICS_ARCHITECTURE
    #endif // end NUMERICS_ARCHITECTURE
#endif // end PROTECTED_ARCHITECTURE

namespace i960 {
constexpr bool CoreArchitectureSupported = true;

constexpr bool NumericsArchitectureSupported() noexcept {
#ifdef NUMERICS_ARCHITECTURE
	return true;
#else
	return false;
#endif // end NUMERICS_ARCHITECTURE
}
constexpr bool ProtectedArchitectureSupported() noexcept {
#ifdef PROTECTED_ARCHITECTURE
	return true;
#else
	return false;
#endif // end PROTECTED_ARCHITECTURE
}
constexpr bool ExtendedArchitectureSupported() noexcept {
#ifdef EXTENDED_ARCHITECTURE
	return true;
#else
	return false;
#endif // end EXTENDED_ARCHITECTURE
}

static_assert(!ExtendedArchitectureSupported(), "Extended Architecture is not well documented so it is not supported at this time!");
}
#endif // end I960_ARCH_LEVEL_H__
