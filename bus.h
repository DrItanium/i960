#ifndef I960_BUS_H__
#define I960_BUS_H__
namespace i960 {
	/**
	 * The bus control unit is how the processor interfaces with the rest of
	 * the system.
	 */
	class BusControlUnit {
	};
	class Bus {
		public:
			enum class State {
				Idle,
				Address,
				Wait_Data,
				Recovery,
				Hold,
				Once,
			};
	};
} // end namespace i960
#endif // end I960_BUS_H__
