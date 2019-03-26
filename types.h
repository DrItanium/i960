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
    enum class ProcessorSeries
    {
        Jx,
        Hx,
        Cx,
        Kx,
        Sx,
    };
    enum class CoreVoltageKind {
        V3_3,
        V5_0,
    };
    template<ProcessorSeries series>
    class CoreInformation final {
        public:
            constexpr CoreInformation(const char* str, 
                    Ordinal devId, 
                    CoreVoltageKind voltage, 
                    Ordinal icacheSize, 
                    Ordinal dcacheSize) noexcept :
                _str(str), 
                _devId(devId), 
                _voltage(voltage), 
                _icacheSize(icacheSize), 
                _dcacheSize(dcacheSize)
                 { 
                }
            constexpr auto getString() const noexcept { return _str; }
            constexpr auto getDeviceId() const noexcept { return _devId; }
            constexpr auto getVoltage() const noexcept { return _voltage; }
            constexpr auto getInstructionCacheSize() const noexcept { return _icacheSize; }
            constexpr auto getDataCacheSize() const noexcept { return _dcacheSize; }
            constexpr auto getVersion() const noexcept { return (_devId & 0xF000'0000) >> 28; }
            constexpr auto getProductType() const noexcept { return (_devId & 0x0FE0'0000) >> 21; }
            constexpr auto getGeneration() const noexcept { return (_devId & 0x001E'0000) >> 17; }
            constexpr auto getModel() const noexcept { return (_devId & 0x0001'F000) >> 12; }
            constexpr auto getManufacturer() const noexcept { return (_devId & 0x0000'0FFE) >> 1; }
            constexpr auto getSeries() const noexcept { return series; }
        private:
            const char* _str;
            Ordinal _devId;
            CoreVoltageKind _voltage;
            Ordinal _icacheSize;
            Ordinal _dcacheSize;
    };
    using JxCoreInformation = CoreInformation<ProcessorSeries::Jx>;
    constexpr JxCoreInformation cpu80L960JA("80L960JA", 0x0082'1013, CoreVoltageKind::V3_3, 2048u, 1024u);
    constexpr JxCoreInformation cpu80L960JF("80L960JF", 0x0082'0013, CoreVoltageKind::V3_3, 4096u, 2048u);
    constexpr JxCoreInformation cpu80960JD("80960JD", 0x0882'0013, CoreVoltageKind::V5_0, 4096u, 2048u);
    constexpr JxCoreInformation cpu80960JF("80960JF", 0x0882'0013, CoreVoltageKind::V5_0, 4096u, 2048u);
    static_assert(cpu80960JF.getGeneration() == 0b0001, "Bad generation check!");
    static_assert(cpu80960JF.getModel() == 0, "Bad model check!");
    static_assert(cpu80960JF.getProductType() == 0b1000100, "Bad product type check!");
    static_assert(cpu80960JF.getManufacturer() == 0b0000'0001'001, "Bad manufacturer check!");
} // end namespace i960
#endif // end I960_TYPES_H__
