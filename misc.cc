#include "archlevel.h"
#include "types.h"

namespace i960 {
	void outputSupportMessage(std::ostream& os) {
		os << "Supported instruction sets:";
		if (coreArchitectureSupported()) {
			os << " core";
		}
		if (numericsArchitectureSupported()) {
			os << " numerics";
		}
		if (protectedArchitectureSupported()) {
			os << " protected";
		}
		if (extendedArchitectureSupported()) {
			os << " extended";
		}
	}
}
