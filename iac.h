#ifndef I960_IAC_H__
#define I960_IAC_H__
#include "types.h"

namespace i960::IAC {
	struct Message {
		union {
			Ordinal _word0;
			struct {
				Ordinal _field2 : 16;
				Ordinal _field1 : 8;
				Ordinal _MsgType : 8;
			};
		};
		Ordinal _field3;
		Ordinal _field4;
		Ordinal _field5;
	};
	static_assert(sizeof(Message) == 16, "Message is not of the correct size!");
	/**
	 * The address that a processor can use to send itself IAC messages
	 */
	constexpr Ordinal ProcessorLoopback = 0xFF000010;
	/**
	 * Start of IAC memory space
	 */
	constexpr Ordinal MessageSpaceStart = 0xFF000000;
	constexpr Ordinal MessageSpaceEnd = 0xFFFFFFF0;
	constexpr Ordinal ProcessorIdMask = 0b1111111111;
	constexpr Ordinal ProcessorPriorityMask = 0b11111;

	constexpr Ordinal computeProcessorAddress(Ordinal processorIdent, Ordinal priorityLevel) noexcept {
		// first mask the pieces
		static constexpr Ordinal interalBitIdentifier = 0b0011000000 << 4;
		// lowest four bits are always zero
		auto maskedIdent = (ProcessorIDMask & processorIdent) << 14;
		auto maskedPriority = internalBitIdentifier | ((ProcessorPriorityMask & priorityLevel) << 4);
		return MessageSpaceStart | maskedIdent | maskedPriority;
	}

	enum MessageType {
		/**
		 * Field1 is an interrupt vector. Generates or posts an interrupt.
		 */
		Interrupt,
		/**
		 * Tests for a pending interrupt
		 */
		TestPendingInterrupts,
		/**
		 * Field3 is an address. Stores two words beginning at this address;
		 * the values are the addresses of the two initialization data
		 * structures
		 */
		StoreSystemBase,
		/**
		 * Invalidates all entries in the instruction cache
		 */
		PurgeInstructionCache,

		/**
		 * Store into the special break point registers. Field3 and Field4 are
		 * the values stored into the breakpoint registers. Bit 0 of the values
		 * are ignored because instructions are word aligned. Bit 1 indicates
		 * whether the breakpoint is enabled (0) or disabled (1).
		 */
		SetBreakpointRegister,
		/**
		 * Puts the processor into the stopped state
		 */
		Freeze,
		/**
		 * Puts the processor at step 2 of the initialization sequence.
		 */
		ContinueInitialization,
		/**
		 * Field3, field4, and field5 contain, respectively, the
		 * addresses expected in memory locations 0, 4, and 12 during
		 * initialization. Puts the processor at step 4 of the initialization
		 * sequence, using these addresses instead of those from memory
		 */
		ReinitializeProcessor,
	};

	union InterruptControlRegister {
		Ordinal _value;
		/**
		 * The set of "pins" that the chip responds on
		 * Int3's vector index must be a higher priority 2 which must be higher
		 * than 1 which must be higher than 0.
		 */
		struct {
			ByteOrdinal _int0Vector;
			ByteOrdinal _int1Vector;
			ByteOrdinal _int2Vector;
			ByteOrdinal _int3Vector;
		};
	};
	/**
	 * Memory mapped location of the Interrupt control register. Can only be
	 * read to and written from using synmov and synld instructions
	 */
	constexpr Ordinal InterruptControlRegisterMappedAddress = 0xFF000004;
} // end namespace i960::IAC

#endif
