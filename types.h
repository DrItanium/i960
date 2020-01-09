#ifndef I960_TYPES_H__
#define I960_TYPES_H__
#include <cstdint>
#include <memory>
namespace i960 {

    using ByteOrdinal = std::uint8_t;
    using ShortOrdinal = std::uint16_t;
    using Ordinal = std::uint32_t;
    constexpr auto getMostSignificantBit(Ordinal value) noexcept {
        return value & 0x8000'0000;
    }
    constexpr auto mostSignificantBitSet(Ordinal value) noexcept { 
        return getMostSignificantBit(value) != 0;
    }
    constexpr auto mostSignificantBitClear(Ordinal value) noexcept {
        return getMostSignificantBit(value) == 0;
    }
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

    template<Ordinal boolMask>
    constexpr Ordinal encodeBool(bool value) noexcept {
        return value ? boolMask : 0;
    }
    template<typename T>
    using EnableIfUnsigned = std::enable_if_t<std::is_unsigned_v<std::decay_t<T>>>;
    template<typename I, typename = EnableIfUnsigned<I>>
    union UnsignedToSigned {
        public:
            using U = std::make_unsigned_t<std::decay_t<I>>;
            using S = std::make_signed_t<std::decay_t<I>>;
        private:
            U ordinal;
            S integer;
        public:
            constexpr UnsignedToSigned(U value) noexcept : ordinal(value) { }
            constexpr auto getInteger() const noexcept { return integer; }
    };
    template<typename T>
    using EnableIfSigned = std::enable_if_t<std::is_signed_v<std::decay_t<T>>>;
    template<typename I, typename = EnableIfSigned<I>>
    union SignedToUnsigned{
        public:
            using U = std::make_unsigned_t<std::decay_t<I>>;
            using S = std::make_signed_t<std::decay_t<I>>;
        private:
            U ordinal;
            S integer;
        public:
            constexpr SignedToUnsigned(S value) noexcept : integer(value) { }
            constexpr auto getOrdinal() const noexcept { return ordinal; }
    };
    template<typename T, typename = EnableIfUnsigned<T>>
    constexpr auto toInteger(T value) noexcept {
        UnsignedToSigned<T> conv(value);
        return conv.getInteger();
    }
    template<typename T, typename = EnableIfSigned<T>>
    constexpr auto toOrdinal(T value) noexcept {
        SignedToUnsigned<T> conv(value);
        return conv.getOrdinal();
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
                    CoreVoltageKind voltage, 
                    Ordinal icacheSize = 0, 
                    Ordinal dcacheSize = 0,
                    Ordinal devId = 0) noexcept :
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
            constexpr auto hasDeviceId() const noexcept             { return _devId != 0; }
        private:
            ProcessorSeries _series;
            const char* _str;
            Ordinal _devId;
            CoreVoltageKind _voltage;
            Ordinal _icacheSize;
            Ordinal _dcacheSize;
    };
    constexpr CoreInformation cpu80960SA(ProcessorSeries::Sx , "80960SA", CoreVoltageKind::V5_0);
    constexpr CoreInformation cpu80960SB(ProcessorSeries::Sx , "80960SB", CoreVoltageKind::V5_0);
    constexpr CoreInformation cpu80L960JA(ProcessorSeries::Jx, "80L960JA", CoreVoltageKind::V3_3, 2048u, 1024u, 0x0082'1013);
    constexpr CoreInformation cpu80L960JF(ProcessorSeries::Jx, "80L960JF", CoreVoltageKind::V3_3, 4096u, 2048u, 0x0082'0013);
    constexpr CoreInformation cpu80960JD( ProcessorSeries::Jx, "80960JD", CoreVoltageKind::V5_0,  4096u, 2048u, 0x0882'0013);
    constexpr CoreInformation cpu80960JF( ProcessorSeries::Jx, "80960JF", CoreVoltageKind::V5_0,  4096u, 2048u, 0x0882'0013);
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

    template<typename ... Args>
    inline constexpr Ordinal constructOrdinalMask(Args&& ... masks) noexcept {
        return (... | masks);
    }

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
