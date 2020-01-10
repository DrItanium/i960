#ifndef I960_TYPES_H__
#define I960_TYPES_H__
#include <cstdint>
#include <memory>
#include <variant>
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

    template<typename T, typename R, T mask, T shift = static_cast<T>(0)>
    constexpr R decode(T value) noexcept {
        using RK = std::decay_t<R>;
        using TK = std::decay_t<T>;
        if constexpr (std::is_same_v<RK, bool> && std::is_integral_v<TK>) {
            // exploit explicit boolean conversion to get rid of the shift if we know we're looking
            // at a conversion from an integral to a bool. In other cases, the explicit boolean conversion
            // could be dependent on the shift so we have to carryout the entire operation
            return value & mask;
        } else {
            return static_cast<R>((value & mask) >> shift);
        }
    }
    template<Ordinal boolMask>
    constexpr Ordinal encodeBool(bool value) noexcept {
        return value ? boolMask : 0;
    }
    template<typename T, typename R, T mask, T shift>
    constexpr T encode(T value, R input) noexcept {
        using K = std::decay_t<R>;
        if constexpr (auto result = value & (~mask) /* we always want to do this */; std::is_same_v<K, bool>) {
            // ignore shift in this case since the mask describes the bits in the correct positions
            if (input) {
                result |= mask;
            } 
            return result;
        } else {
            return result | ((static_cast<T>(input) << shift) & mask);
        }
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
    /**
     * Contains common hardcoded methods related to device identification codes
     */
    template<bool hardwareAccessible>
    class BaseDeviceIdenficationCode {
        public:
            static constexpr uint8_t ProductType = 0b000'100; // : 6
            static constexpr uint16_t Manufacturer = 0b0000'0001'001; // : 12
        public:

            constexpr auto getProductType() const noexcept { return ProductType; }
            constexpr auto getManufacturer() const noexcept { return Manufacturer; }
            constexpr auto deviceIdCodeInHardware() const noexcept { return hardwareAccessible; }
    };
    class HardwareDeviceIdentificationCode final : public BaseDeviceIdenficationCode<true> {
        public:
            constexpr HardwareDeviceIdentificationCode(uint8_t version, bool vcc, uint8_t generation, uint8_t model) noexcept :
                _version(version),
                _vcc(vcc),
                _gen(generation),
                _model(model) { }
            constexpr HardwareDeviceIdentificationCode(Ordinal raw) noexcept : HardwareDeviceIdentificationCode(
                    i960::decode<Ordinal, uint8_t, 0xF000'0000, 28>(raw),
                    i960::decode<Ordinal, bool, (1 << 27), 27>(raw),
                    i960::decode<Ordinal, uint8_t, (0b1111 << 17), 17>(raw),
                    i960::decode<Ordinal, uint8_t, (0b11111 << 12), 12>(raw)) { }
            constexpr auto getVersion() const noexcept { return _version; }
            constexpr auto getCoreVoltage() const noexcept { 
                if (_vcc) {
                    return CoreVoltageKind::V5_0;
                } else {
                    return CoreVoltageKind::V3_3;
                }
            }
            constexpr auto is5VCore() const noexcept { return _vcc; }
            constexpr auto is3_3VCore() const noexcept { return !_vcc; }
            constexpr auto getGeneration() const noexcept { return _gen; }
            constexpr auto getModel() const noexcept { return _model; }
            constexpr auto makeDeviceId() const noexcept { 
                constexpr Ordinal vccMask = 1 << 27;
                Ordinal start = static_cast<Ordinal>(_version) << 28;
                if (_vcc) {
                    start |= vccMask;
                }
                start |= (static_cast<Ordinal>(getProductType() & 0b111111) << 21);
                start |= (static_cast<Ordinal>(_gen & 0b1111) << 17);
                start |= (static_cast<Ordinal>(_model & 0b11111) << 12);
                start |= (static_cast<Ordinal>(getManufacturer()) << 1);
                return (start | 1); // lsb must be 1
            }
        private:
            uint8_t _version;
            bool _vcc;
            uint8_t _gen;         // : 4
            uint8_t _model;       // : 5
    };
    /**
     * If the cpu doesn't have a device id register built in then we need to
     * provide a consistent way to describe the features within through a hand maintained 
     * structure
     */
    class ConstructedDeviceIdentificationCode final : public BaseDeviceIdenficationCode<false> {
        public:
            constexpr ConstructedDeviceIdentificationCode(ProcessorSeries s, CoreVoltageKind v, Ordinal icache = 0, Ordinal dcache = 0, uint8_t stepping = 0) noexcept :
                _series(s),
                _icacheSize(icache),
                _dcacheSize(dcache),
                _voltage(v),
                _stepping(stepping) { }
            constexpr auto getICacheSize() const noexcept { return _icacheSize; }
            constexpr auto getDCacheSize() const noexcept { return _dcacheSize; }
            constexpr auto getCoreVoltage() const noexcept { return _voltage; }
            constexpr auto is3_3VoltCore() const noexcept { return _voltage == CoreVoltageKind::V3_3; }
            constexpr auto is5VoltCore() const noexcept { return _voltage == CoreVoltageKind::V5_0; }
            constexpr auto getSeries() const noexcept { return _series; }
            constexpr auto getStepping() const noexcept { return _stepping; }
        private:
            ProcessorSeries _series;
            Ordinal _icacheSize;
            Ordinal _dcacheSize;
            CoreVoltageKind _voltage;
            uint8_t _stepping;
    };
    using DeviceCode = std::variant<ConstructedDeviceIdentificationCode, HardwareDeviceIdentificationCode>;
    class CoreInformation final {
        public:
            constexpr CoreInformation(
                    ProcessorSeries series,
                    const char* str, 
                    CoreVoltageKind voltage, 
                    Ordinal salign = 4, // found many instances where intel defaults to 4 unless stated otherwise
                    Ordinal icacheSize = 0, 
                    Ordinal dcacheSize = 0,
                    Ordinal devId = 0) noexcept :
                _series(series),
                _str(str), 
                _devId(devId), 
                _voltage(voltage), 
                _icacheSize(icacheSize), 
                _dcacheSize(dcacheSize),
                _salign(salign) { }
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
            constexpr auto getSalign() const noexcept               { return _salign; }
            constexpr auto getStackFrameAlignment() const noexcept  { return (_salign * 16); }
            constexpr Ordinal getNumberOfIgnoredFramePointerBits() const noexcept { 
                // these are the constants that would be found by solving the equation
                // SALIGN*16 = 2^n
                // for n. Log2 is not constexpr in C++...
                switch (getStackFrameAlignment()) {
                    case 16:    return 4;
                    case 32:    return 5;
                    case 64:    return 6;
                    default:    return 0xFFFF'FFFF;
                }
            }
        private:
            ProcessorSeries _series;
            const char* _str;
            Ordinal _devId;
            CoreVoltageKind _voltage;
            Ordinal _icacheSize;
            Ordinal _dcacheSize;
            Ordinal _salign;
    };
    // for the Sx series, I cannot find Appendix F of the reference manual online so I'm assuming 4 for SALIGN for now
    constexpr CoreInformation cpu80960SA(ProcessorSeries::Sx , "80960SA", CoreVoltageKind::V5_0, 4, 512u);
    constexpr CoreInformation cpu80960SB(ProcessorSeries::Sx , "80960SB", CoreVoltageKind::V5_0, 4, 512u);
    constexpr CoreInformation cpu80L960JA(ProcessorSeries::Jx, "80L960JA", CoreVoltageKind::V3_3, 1, 2048u, 1024u, 0x0082'1013);
    constexpr CoreInformation cpu80L960JF(ProcessorSeries::Jx, "80L960JF", CoreVoltageKind::V3_3, 1, 4096u, 2048u, 0x0082'0013);
    constexpr CoreInformation cpu80960JD( ProcessorSeries::Jx, "80960JD", CoreVoltageKind::V5_0,  1, 4096u, 2048u, 0x0882'0013);
    constexpr CoreInformation cpu80960JF( ProcessorSeries::Jx, "80960JF", CoreVoltageKind::V5_0,  1, 4096u, 2048u, 0x0882'0013);
    constexpr CoreInformation cpu80960JT_A0 (ProcessorSeries::Jx, "80960JT", CoreVoltageKind::V3_3,  1, 16384, 4096u, 0x0082'B013);
    static_assert(HardwareDeviceIdentificationCode(0, false, 0b0001, 0b01011).makeDeviceId() == cpu80960JT_A0.getDeviceId());
    // sanity checks
    static_assert(cpu80960JF.getGeneration() == 0b0001, "Bad generation check!");
    static_assert(cpu80960JF.getModel() == 0, "Bad model check!");
    static_assert(cpu80960JF.getProductType() == 0b1000100, "Bad product type check!");
    static_assert(cpu80960JF.getManufacturer() == 0b0000'0001'001, "Bad manufacturer check!");
    static_assert(cpu80960JF.is5VoltCore(), "Bad voltage value!");
    static_assert(cpu80960JF.getSeries() == ProcessorSeries::Jx, "Bad processor series!");
} // end namespace i960
#endif // end I960_TYPES_H__
