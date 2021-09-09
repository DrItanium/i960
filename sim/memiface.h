#ifndef I960_MEMORY_INTERFACE_H__
#define I960_MEMORY_INTERFACE_H__
#include "types.h"
namespace i960 {
	/**
	 * Abstract interface to system memory
	 */
	class MemoryInterface {
		public:
            virtual ~MemoryInterface() = default;
			virtual void store(Ordinal address, Ordinal value, bool atomic) = 0;
            virtual void storeDouble(Ordinal address, LongOrdinal value, bool atomic) {
                // default implementation
                store(address, static_cast<Ordinal>(value), atomic);
                store(address+1, static_cast<Ordinal>(value >> 32), atomic);
            }

			virtual Ordinal load(Ordinal address, bool atomic) const = 0;
            virtual LongOrdinal loadDouble(Ordinal address, bool atomic) const {
                // default implementation
                auto lower = static_cast<LongOrdinal>(load(address, atomic));
                auto upper = static_cast<LongOrdinal>(load(address+1, atomic)) << 32;
                return lower | upper;
            }

	};
}
#endif // end I960_MEMORY_INTERFACE_H__

