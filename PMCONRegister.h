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
            static_assert(false_v<PMCONRegisterKind>, "Illegal pmcon register kind provided!");
        }
    }
    template<PMCONRegisterKind kind>
    constexpr PMCONRegisterRange_t PMCONRegisterRange = getRegisterRange<kind>();
    class PMCONRegister final {
        public:
            static constexpr auto extractBusWidth(Ordinal value) noexcept {
                return i960::decode<Ordinal, uint8_t, 0xC0'0000, 22>(value); 
            }
            static constexpr auto encodeBusWidth(Ordinal base, uint8_t width) noexcept {
                return i960::encode<Ordinal, uint8_t, 0xC0'0000, 22>(base, width);
            }
            static constexpr auto encodeBusWidth(uint8_t width) noexcept {
                return encodeBusWidth(0, width);
            }
        public:
            constexpr PMCONRegister() noexcept = default;
            constexpr PMCONRegister(Ordinal value) : _busWidth(extractBusWidth(value)) { }
            constexpr bool busWidthIs8bit() const noexcept { return _busWidth == 0b00; }
            constexpr bool busWidthIs16bit() const noexcept { return _busWidth == 0b01; }
            constexpr bool busWidthIs32bit() const noexcept { return _busWidth == 0b10; }
            constexpr bool busWidthIsUndefined() const noexcept { return _busWidth == 0b11; }
            void setBusWidth(Ordinal v) noexcept { _busWidth = v; }
            constexpr auto getRawValue() const noexcept { return encodeBusWidth(_busWidth); }
        private:
            uint8_t _busWidth = 0;
    };
	class BCONRegister final {
        public:
            static constexpr bool decodeCTV(Ordinal value) noexcept {
                return i960::decode<Ordinal, bool, 0x1, 0>(value);
            }
            static constexpr bool decodeIRP(Ordinal value) noexcept {
                return i960::decode<Ordinal, bool, 0x2, 1>(value);
            }
            static constexpr bool decodeSIRP(Ordinal value) noexcept {
                return i960::decode<Ordinal, bool, 0x4, 2>(value);
            }
            static constexpr Ordinal encodeRawValue(bool ctv, bool irp, bool sirp) noexcept {
                return i960::encode<Ordinal, bool, 0x1, 0>(
                        i960::encode<Ordinal, bool, 0x2, 1>(
                            i960::encode<Ordinal, bool, 0x4, 2>(0, sirp), irp) , ctv);
            }
        public:
            constexpr BCONRegister() noexcept = default;
            constexpr BCONRegister(Ordinal raw) noexcept : 
                _configurationEntriesInControlTableValid(decodeCTV(raw)),
                _internalRAMProtection(decodeIRP(raw)),
                _supervisorInternalRAMProtection(decodeSIRP(raw)) { }
            constexpr Ordinal getRawValue() const noexcept { 
                return encodeRawValue(getCTV(), getIRP(), getSIRP());
            }
            constexpr bool pmconEntriesValid() const noexcept { return getCTV(); }
            constexpr bool internalDataRAMProtectedFromUserModeWrites() const noexcept { return getIRP(); }
            constexpr bool first64BytesProtectedFromSupervisorModeWrites() const noexcept { return getSIRP(); }
            constexpr bool getSIRP() const noexcept { return _supervisorInternalRAMProtection; }
            constexpr bool getIRP() const noexcept { return _internalRAMProtection; }
            constexpr bool getCTV() const noexcept { return _configurationEntriesInControlTableValid; }
            void setCTV(bool value) noexcept  { _configurationEntriesInControlTableValid = value; }
            void setIRP(bool value) noexcept  { _internalRAMProtection = value; }
            void setSIRP(bool value) noexcept { _supervisorInternalRAMProtection = value; }
        private:
			bool _configurationEntriesInControlTableValid = false;
			bool _internalRAMProtection = false;
			bool _supervisorInternalRAMProtection = false;
	};
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
