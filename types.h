#ifndef I960_TYPES_H__
#define I960_TYPES_H__
#include <cstdint>
#include <memory>
namespace i960 {

    template<typename T>
    constexpr bool LegalConversion = false;
    using ByteOrdinal = std::uint8_t;
    using ShortOrdinal = std::uint16_t;
    using Ordinal = std::uint32_t;
    using LongOrdinal = std::uint64_t;
    using InstructionPointer = Ordinal;
    using ByteInteger = std::int8_t;
    using ShortInteger = std::int16_t;
    using Integer = std::int32_t;
    using LongInteger = std::int64_t;

    enum class ConditionCode : Ordinal {
        False = 0b000,
        Greater = 0b001,
        Equal = 0b010,
        Less = 0b100,
        Ordered = 0b111,
        NotEqual = 0b101,
        LessOrEqual = 0b110,
        GreaterOrEqual = 0b011,
        Unordered = False,
        NotOrdered = Unordered,
        True = Equal,
    };
    using TestTypes = ConditionCode;
	

    constexpr Ordinal widen(ByteOrdinal value) noexcept {
        return Ordinal(value);
    }
    constexpr Integer widen(ByteInteger value) noexcept {
        return Integer(value);
    }
    constexpr Ordinal computeNextFrameStart(Ordinal currentAddress) noexcept {
        // add 1 to the masked out value to make sure we don't overrun anything
        return (currentAddress & ~(Ordinal(0b11111))) + 1; // next 64 byte frame start
    }
    constexpr Ordinal computeStackFrameStart(Ordinal framePointerAddress) noexcept {
        return framePointerAddress + 64;
    }

	/**
	 * Physical Memory Region Control Register. Represents a 512 mbyte memory
	 * region to control
	 */
	enum class PMCONRegisterKind {
		Region0_1, 
		Region2_3, 
		Region4_5,
		Region6_7, 
		Region8_9, 
		Region10_11,
		Region12_13,
		Region14_15,
	};
	using PMCONRegisterRange_t = std::tuple<Ordinal, Ordinal>;
    template<PMCONRegisterKind kind>
    constexpr PMCONRegisterRange_t getRegisterRange() noexcept {
        if constexpr (kind == PMCONRegisterKind::Region0_1) {
            return std::make_tuple(0x0000'0000, 0x1FFF'FFFF);
        } else if constexpr (kind == PMCONRegisterKind::Region2_3) {
            return std::make_tuple(0x2000'0000, 0x3FFF'FFFF);
        } else if constexpr (kind == PMCONRegisterKind::Region4_5) {
            return std::make_tuple(0x4000'0000, 0x5FFF'FFFF);
        } else if constexpr (kind == PMCONRegisterKind::Region6_7) {
            return std::make_tuple(0x6000'0000, 0x7FFF'FFFF);
        } else if constexpr (kind == PMCONRegisterKind::Region8_9) {
            return std::make_tuple(0x8000'0000, 0x9FFF'FFFF);
        } else if constexpr (kind == PMCONRegisterKind::Region10_11) {
            return std::make_tuple(0xA000'0000, 0xBFFF'FFFF);
        } else if constexpr (kind == PMCONRegisterKind::Region12_13) {
            return std::make_tuple(0xC000'0000, 0xDFFF'FFFF);
        } else if constexpr (kind == PMCONRegisterKind::Region14_15) {
            return std::make_tuple(0xE000'0000, 0xFFFF'FFFF);
        } else {
            static_assert(LegalConversion<PMCONRegisterKind>, "Illegal pmcon register kind provided!");
        }
    }
    template<PMCONRegisterKind kind>
    constexpr PMCONRegisterRange_t PMCONRegisterRange = getRegisterRange<kind>();
	union PMCONRegister final {
		struct {
			Ordinal _unused0 : 22;
			Ordinal _busWidth : 2;
			Ordinal _unused1 : 8;
		};
		Ordinal _value;
		constexpr bool busWidthIs8bit() const noexcept { return _busWidth == 0b00; }
		constexpr bool busWidthIs16bit() const noexcept { return _busWidth == 0b01; }
		constexpr bool busWidthIs32bit() const noexcept { return _busWidth == 0b10; }
		constexpr bool busWidthIsUndefined() const noexcept { return _busWidth == 0b11; }
	} __attribute__((packed));
	union BCONRegister final {
		struct {
			Ordinal _configurationEntriesInControlTableValid : 1;
			Ordinal _internalRAMProtection : 1;
			Ordinal _supervisorInternalRAMProtection : 1;
		};
		Ordinal _value;
		constexpr bool pmconEntriesValid() const noexcept { return _configurationEntriesInControlTableValid != 0; }
		constexpr bool internalDataRAMProtectedFromUserModeWrites() const noexcept { return _internalRAMProtection != 0; }
		constexpr bool first64BytesProtectedFromSupervisorModeWrites() const noexcept { return _supervisorInternalRAMProtection != 0; }
	} __attribute__((packed));
	union LogicalMemoryTemplateStartingAddressRegister final {
		struct {
			Ordinal _byteOrder : 1;
			Ordinal _dataCacheEnable : 1;
			Ordinal _reserved : 10;
			/**
			 * Upper 20-bits for the starting address for a logical data
			 * template. The lower 12 bits are fixed at zero. The starting
			 * address is modulo 4 kbytes
			 */
			Ordinal _templateStartingAddress : 20;
		};
		Ordinal _value;
		constexpr bool littleEndianByteOrder() const noexcept { return _byteOrder == 0; }
		constexpr bool bigEndianByteOrder() const noexcept { return _byteOrder != 0; }
		constexpr bool dataCacheEnabled() const noexcept { return _dataCacheEnable != 0; }
		constexpr Ordinal getTemplateStartingAddress() const noexcept { return _templateStartingAddress; }
	} __attribute__((packed));
	union LogicalMemoryTemplateMaskRegister final {
		struct {
			Ordinal _logicalMemoryTemplateEnabled : 1;
			Ordinal _reserved : 11;
			Ordinal _templateAddressMask : 20;
		};
		Ordinal _value;
		constexpr bool logicalMemoryTemplateEnabled() const noexcept { return _logicalMemoryTemplateEnabled; }
		constexpr Ordinal getTemplateAddressMask() const noexcept { return _templateAddressMask; }
	} __attribute__((packed));
	union DefaultLogicalMemoryConfigurationRegister final {
		struct {
			Ordinal _byteOrder : 1;
			Ordinal _dataCacheEnable : 1;
		};
		Ordinal _value;
		constexpr bool littleEndianByteOrder() const noexcept { return _byteOrder == 0; }
		constexpr bool bigEndianByteOrder() const noexcept { return _byteOrder != 0; }
		constexpr bool dataCacheEnabled() const noexcept { return _dataCacheEnable != 0; }
	} __attribute__((packed));
    /**
     * Also known as the PRCB, it is a data structure in memory which the cpu uses to track
     * various states
     */
    struct ProcessorControlBlock {
        Ordinal reserved0 = 0;
        Ordinal processorControls;
        Ordinal reserved1 = 0;
        Ordinal currentProcessSS;
        Ordinal dispatchPortSS;
        Ordinal interruptTablePhysicalAddress;
        Ordinal interruptStackPointer;
        Ordinal reserved2 = 0;
        Ordinal region3SS;
        Ordinal systemProcedureTableSS;
        Ordinal faultTablePhysicalAddress;
        Ordinal reserved3 = 0;
        Ordinal multiprocessorPreemption[3];
        Ordinal reserved4 = 0;
        Ordinal idleTime[2];
        Ordinal systemErrorFault;
        Ordinal reserved5;
        Ordinal resumptionRecord[12];
        Ordinal systemErrorFaultRecord[11];
    } __attribute__((packed));

    // memory map
    enum class MemoryMap : Ordinal {
        NMIVector = 0x0000'0000,
        OptionalInterruptVectorsBegin = 0x0000'0004,
        OptionalInterruptVectorsEnd = 0x0000'003F,
        DataCacheUnreservedStart = 0x0000'0040,
        DataCacheUnreservedEnd = 0x0000'03FF,
        // internal data cache end
        // normal memory begin
        ExternalUnusedMemoryBegin = 0x0000'0400,
        ExternalUnusedMemoryEnd = 0xFEFF'FF2F,
        InitializationBootRecordBegin = 0xFEFF'FF30,
        InitializationBootRecordEnd = 0xFEFF'FF5F,
        ReservedMemoryBegin = 0xFEFF'FF60,
        ReservedMemoryEnd = 0xFEFF'FFFF,
        MemoryMappedRegisterSpaceBegin = 0xFF00'0000,
        MemoryMappedRegisterSpaceEnd = 0xFFFF'FFFF,
    };

	/**
	 * A block of 1024 bytes which is readable and writable but not
	 * executable. It is embedded within the processor itself and generates
	 * no external buss activity is generated when accessed.
	 */
	template<Ordinal numBytes>
	struct InternalDataRam {
		public:
			// first 64 bytes are reserved for optional interrupt vectors and the nmi vector
			constexpr static Ordinal MinimumSize = 4;
			constexpr static Ordinal TotalReservedBytes = 64;
			constexpr static Ordinal TotalByteCapacity = numBytes;
			static_assert(numBytes >= MinimumSize, "InternalDataRam must be at least 4 bytes in size");
			static_assert((numBytes & 1) == 0, "numBytes is not even!");
			static_assert(numBytes == ((numBytes >> 2) << 2), "numBytes is not a power of two value!");
			constexpr static Ordinal TotalUnreservedBytes  = TotalByteCapacity - TotalReservedBytes;
			constexpr static Ordinal TotalWordCapacity = TotalByteCapacity / sizeof(Ordinal);
			constexpr static Ordinal TotalReservedWords = TotalReservedBytes / sizeof(Ordinal);
			constexpr static Ordinal TotalUnreservedWords = TotalUnreservedBytes / sizeof(Ordinal);
			constexpr static Ordinal LegalMaskValue = TotalWordCapacity - 1;
			template<Ordinal baseAddress>
			constexpr static Ordinal LargestAddress = (baseAddress +  TotalByteCapacity) - 1;
			template<Ordinal baseAddress>
			constexpr static Ordinal SmallestAddress = baseAddress;
			
		public:
			InternalDataRam() = default;
			~InternalDataRam() = default;
			void reset() noexcept {
				// zero out memory
				for (auto i = 0u; i < TotalWordCapacity; ++i) {
					write(i, 0);
				}
			}
			void writeByte(Ordinal address, ByteOrdinal value) noexcept {
				auto localOrdinal = read(address);
				switch (address & 0b11) {
					case 0b00:
						localOrdinal = (localOrdinal & 0xFFFFFF00) | Ordinal(value);
						break;
					case 0b01:
						localOrdinal = (localOrdinal & 0xFFFF00FF) | (Ordinal(value) << 8);
						break;
					case 0b10:
						localOrdinal = (localOrdinal & 0xFF00FFFF) | (Ordinal(value) << 16);
						break;
					case 0b11:
						localOrdinal = (localOrdinal & 0x00FFFFFF) | (Ordinal(value) << 24);
						break;
					default:
						throw "Should never fire!";
				}
				write(address & (~0b11), localOrdinal);
			}
			void write(Ordinal address, Ordinal value) noexcept {
				auto actualAddress = (address >> 2) & LegalMaskValue;
				_words[actualAddress] = value;
			}
			ByteOrdinal readByte(Ordinal address) const noexcept {
				// extract out the ordinal that the byte is a part of
				auto closeValue = read(address);
				switch (address & 0b11) {
					case 0b00: 
						return ByteOrdinal(closeValue);
					case 0b01: 
						return ByteOrdinal(closeValue >> 8);
					case 0b10: 
						return ByteOrdinal(closeValue >> 16);
					case 0b11: 
						return ByteOrdinal(closeValue >> 24);
					default:
						throw "Should never be hit!";
				}
			}
			Ordinal read(Ordinal address) const noexcept {
				// the address needs to be fixed to be a multiple of four
				auto realAddress = (address >> 2) & LegalMaskValue;
				return _words[realAddress];
			}
			constexpr Ordinal totalByteCapacity() const noexcept { return TotalByteCapacity; }
		private:
			Ordinal _words[TotalUnreservedWords + TotalReservedWords];
	} __attribute__((packed));
	using JxCPUInternalDataRam = InternalDataRam<1024>;
	static_assert(sizeof(JxCPUInternalDataRam) == JxCPUInternalDataRam::TotalByteCapacity, "InternalDataRam is larger than its storage footprint!");


} // end namespace i960
#endif // end I960_TYPES_H__
