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

    using HalfInteger = ShortInteger;
    using HalfOrdinal = ShortOrdinal;
    using OpcodeValue = HalfOrdinal;
	template<typename T>
	constexpr auto byteCount(size_t count) noexcept {
		return count * sizeof(std::decay_t<T>);
	}
    constexpr auto operator"" _words(unsigned long long count) noexcept {
		return byteCount<Ordinal>(count);
    }
	constexpr auto operator"" _dwords(unsigned long long count) noexcept {
		return byteCount<LongOrdinal>(count);
	}

    enum class ProcessorSeries {
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
    class CoreInformation final {
        public:
            constexpr CoreInformation(
                    ProcessorSeries series,
                    const char* str, 
                    Ordinal devId, 
                    CoreVoltageKind voltage, 
                    Ordinal icacheSize = 0, 
                    Ordinal dcacheSize = 0) noexcept :
                _series(series),
                _str(str), 
                _devId(devId), 
                _voltage(voltage), 
                _icacheSize(icacheSize), 
                _dcacheSize(dcacheSize) { }
            constexpr auto getString() const noexcept               { return _str; }
            constexpr auto getDeviceId() const noexcept             { return _devId; }
            constexpr auto getVoltage() const noexcept              { return _voltage; }
            constexpr auto getInstructionCacheSize() const noexcept { return _icacheSize; }
            constexpr auto getDataCacheSize() const noexcept        { return _dcacheSize; }
            constexpr auto getVersion() const noexcept              { return (_devId & 0xF000'0000) >> 28; }
            constexpr auto getProductType() const noexcept          { return (_devId & 0x0FE0'0000) >> 21; }
            constexpr auto getGeneration() const noexcept           { return (_devId & 0x001E'0000) >> 17; }
            constexpr auto getModel() const noexcept                { return (_devId & 0x0001'F000) >> 12; }
            constexpr auto getManufacturer() const noexcept         { return (_devId & 0x0000'0FFE) >> 1; }
            constexpr auto getSeries() const noexcept               { return _series; }
            constexpr auto hasInstructionCache() const noexcept     { return _icacheSize != 0; }
            constexpr auto hasDataCache() const noexcept            { return _dcacheSize != 0; }
            constexpr auto is5VoltCore() const noexcept             { return _voltage == CoreVoltageKind::V5_0; }
            constexpr auto is3_3VoltCore() const noexcept           { return _voltage == CoreVoltageKind::V3_3; }
        private:
            ProcessorSeries _series;
            const char* _str;
            Ordinal _devId;
            CoreVoltageKind _voltage;
            Ordinal _icacheSize;
            Ordinal _dcacheSize;
    };
    constexpr CoreInformation cpu80L960JA(ProcessorSeries::Jx, "80L960JA", 0x0082'1013, CoreVoltageKind::V3_3, 2048u, 1024u);
    constexpr CoreInformation cpu80L960JF(ProcessorSeries::Jx, "80L960JF", 0x0082'0013, CoreVoltageKind::V3_3, 4096u, 2048u);
    constexpr CoreInformation cpu80960JD(ProcessorSeries::Jx, "80960JD", 0x0882'0013, CoreVoltageKind::V5_0, 4096u, 2048u);
    constexpr CoreInformation cpu80960JF(ProcessorSeries::Jx, "80960JF", 0x0882'0013, CoreVoltageKind::V5_0, 4096u, 2048u);
    // sanity checks
    static_assert(cpu80960JF.getGeneration() == 0b0001, "Bad generation check!");
    static_assert(cpu80960JF.getModel() == 0, "Bad model check!");
    static_assert(cpu80960JF.getProductType() == 0b1000100, "Bad product type check!");
    static_assert(cpu80960JF.getManufacturer() == 0b0000'0001'001, "Bad manufacturer check!");
    static_assert(cpu80960JF.is5VoltCore(), "Bad voltage value!");
    static_assert(cpu80960JF.getSeries() == ProcessorSeries::Jx, "Bad processor series!");
    template<typename T, typename R, T mask, T shift = static_cast<T>(0)>
    constexpr R decode(T value) noexcept {
        return static_cast<R>((value & mask) >> shift);
    }
    template<typename T, typename R, T mask, T shift>
    constexpr T encode(T value, R input) noexcept {
        return ((value & (~mask)) | ((static_cast<T>(input) << shift) & mask));
    }
    class Flags8 final {
        public:
            explicit constexpr Flags8(ByteOrdinal value ) noexcept : _storage(value) { }
            constexpr Flags8() noexcept : Flags8(0) { }
            constexpr auto getBackingStore() const noexcept { return _storage; }
            template<Ordinal index>
            constexpr bool get() const noexcept {
                static_assert(index < 8, "Illegal flag position");
                return _storage &  (1 << index);
            }
            template<Ordinal index>
            constexpr void clear() noexcept {
                static_assert(index < 8, "Illegal flag position");
                constexpr auto mask = 1 << index;
                _storage &= ~(mask);
            }
            template<Ordinal index>
            constexpr void set() noexcept {
                static_assert(index < 8, "Illegal flag position");
                constexpr auto mask = 1 << index;
                _storage |= mask;
            }
            template<Ordinal index>
            constexpr void set(bool value) noexcept {
                if (value) {
                    set<index>();
                } else {
                    clear<index>();
                }
            }
        private:
            ByteOrdinal _storage;
    };
    // false_v taken from https://quuxplusone.github.io/blog/2018/04/02/false-v/
    template<typename...>
        inline constexpr bool false_v = false;

    /**
     *
     * Allow a separate lambda to be defined for each specific std::visit case.
     * Found on the internet in many places
     * @tparam Ts the functions that make up the dispatch logic
     */
    template<typename ... Ts> 
    struct overloaded : Ts... 
    {
        using Ts::operator()...;
    };

    template<typename ... Ts>
    overloaded(Ts...) -> overloaded<Ts...>;
} // end namespace i960
#endif // end I960_TYPES_H__
