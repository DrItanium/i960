#ifndef I960_PMCON_REGISTER_H__
#define I960_PMCON_REGISTER_H__
#include "types.h"
namespace i960 {
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
}
#endif // end I960_PMCON_REGISTER_H__
