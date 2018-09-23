#ifndef I960_MEMORY_INTERFACE_H__
#define I960_MEMORY_INTERFACE_H__
#include "types.h"
namespace i960 {
	/**
	 * Abstract interface to system memory
	 */
	class MemoryInterface {
		public:
			virtual void store(Ordinal address, Ordinal value, bool atomic) = 0;
			virtual Ordinal load(Ordinal address, bool atomic) const = 0;
	};
}
#endif // end I960_MEMORY_INTERFACE_H__

