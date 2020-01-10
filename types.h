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
        Unknown,
        Jx,
        Hx,
        Cx,
        Kx,
        Sx,
        Mx,
    };
    enum class CoreVoltageKind {
        V3_3,
        V5_0,
        Unknown,
    };
    /// Common base for a device id used in IEEE1149.1 JTAG interfacing
    class IEEE1149_1DeviceIdentification final {
        public:
            static constexpr uint16_t Manufacturer_Altera = 0b000'0110'1110;
            static constexpr uint16_t Manufacturer_Intel = 0b000'0000'1001;
        public:
            constexpr IEEE1149_1DeviceIdentification(uint8_t version, uint16_t partNum, uint16_t mfgr) : _version(version & 0b1111), _partNumber(partNum), _manufacturer(mfgr & 0x7FF) { }
            constexpr IEEE1149_1DeviceIdentification(uint32_t id) noexcept :
                IEEE1149_1DeviceIdentification(
                decode<uint32_t, uint8_t, 0xF000'0000, 28>(id),
                decode<uint32_t, uint16_t, 0x0FFF'F000, 12>(id),
                decode<uint32_t, uint16_t, 0xFFE, 1>(id)) { }
            constexpr auto getVersion() const noexcept { return _version; }
            constexpr auto getPartNumber() const noexcept { return _partNumber; }
            constexpr auto getManufacturer() const noexcept { return _manufacturer; }
            constexpr uint32_t getId() const noexcept {
                return encode<uint32_t, uint8_t, 0xF000'0000, 28>(
                        encode<uint32_t, uint16_t, 0x0FFF'F000, 12>(
                            static_cast<uint32_t>(getManufacturer() << 1),
                            getPartNumber()),
                        getVersion()) | 1; // make sure lsb is 1
            }
        private:
            uint8_t _version;
            uint16_t _partNumber;
            uint16_t _manufacturer;
    };
    /**
     * Contains common hardcoded methods related to device identification codes
     */
    template<bool hardwareAccessible>
    class BaseDeviceIdenficationCode {
        public:
            static constexpr uint8_t ProductType = 0b000'100; // i960
        public:
            constexpr auto getProductType() const noexcept { return ProductType; }
            constexpr auto getManufacturer() const noexcept { return IEEE1149_1DeviceIdentification::Manufacturer_Intel; }
            constexpr auto deviceIdCodeInHardware() const noexcept { return hardwareAccessible; }
    };
    class HardwareDeviceIdentificationCode final : public BaseDeviceIdenficationCode<true> {
        public:
            constexpr HardwareDeviceIdentificationCode(uint8_t version, bool vcc, uint8_t generation, uint8_t model) noexcept :
                _version(version & 0b11111),
                _vcc(vcc),
                _gen(generation & 0b1111),
                _model(model & 0b11111) { }
            constexpr HardwareDeviceIdentificationCode(Ordinal raw) noexcept : HardwareDeviceIdentificationCode(
                    i960::decode<Ordinal, uint8_t, 0xF000'0000, 28>(raw),
                    i960::decode<Ordinal, bool, (1 << 27), 27>(raw),
                    i960::decode<Ordinal, uint8_t, (0b1111 << 17), 17>(raw),
                    i960::decode<Ordinal, uint8_t, (0b11111 << 12), 12>(raw)) { }
            constexpr auto getVersion() const noexcept { return _version; }
            constexpr auto getGeneration() const noexcept { return _gen; }
            constexpr auto getModel() const noexcept { return _model; }
            constexpr Ordinal getDeviceId() const noexcept { 
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
            constexpr auto getSeries() const noexcept {
                switch (_gen & 0b1111) {
                    case 0b0001:
                        return ProcessorSeries::Jx;
                    case 0b0010:
                        return ProcessorSeries::Hx;
                    default:
                        return ProcessorSeries::Unknown;
                }
            }

            constexpr auto getCoreVoltage() const noexcept { 
                switch (getSeries()) {
                    case ProcessorSeries::Jx:
                        return _vcc ? CoreVoltageKind::V5_0 : CoreVoltageKind::V3_3; 
                    case ProcessorSeries::Hx:
                        return _vcc ? CoreVoltageKind::V3_3 : CoreVoltageKind::V5_0;
                    default:
                        return CoreVoltageKind::Unknown;
                }
            }
            constexpr auto is5VCore() const noexcept { return getCoreVoltage() == CoreVoltageKind::V5_0; }
            constexpr auto is3_3VCore() const noexcept { return getCoreVoltage() == CoreVoltageKind::V3_3; }

            constexpr auto unpackCacheInformation() const noexcept {
                switch(getSeries()) {
                    case ProcessorSeries::Jx:
                        // format is DDPCC
                        // DD -> Clock Multiplier
                        // P  -> Product Derivative
                        // CC -> cache size
                        //       11 -> 16K I-cache, 4-K dcache
                        //       10 -> Unknown
                        //       01 -> 2K I-cache,  1-K dcache
                        //       00 -> 4K I-cache,  2-K Dcache
                        switch (getModel() & 0b11) {
                            case 0b00: return std::make_tuple<Ordinal, Ordinal>(4096u, 2048u);
                            case 0b01: return std::make_tuple<Ordinal, Ordinal>(2048u, 1024u);
                            case 0b11: return std::make_tuple<Ordinal, Ordinal>(16384u, 4096u);
                        }
                        break;
                    case ProcessorSeries::Hx:
                        return std::make_tuple<Ordinal, Ordinal>(16384u, 8192u);
                    default:
                        break;
                }
                return std::make_tuple<Ordinal, Ordinal>(0, 0);
            }
            constexpr auto getInstructionCacheSize() const noexcept { return std::get<0>(unpackCacheInformation()); }
            constexpr auto getDataCacheSize() const noexcept { return std::get<1>(unpackCacheInformation()); }
        private:
            uint8_t _version;
            bool _vcc;
            uint8_t _gen;         // : 4
            uint8_t _model;       // : 5
    };
    static_assert(HardwareDeviceIdentificationCode(0x0082'B013).getDeviceId() == 0x0082'B013);
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
                _stepping(stepping & 0b1111) { }
            constexpr auto getInstructionCacheSize() const noexcept { return _icacheSize; }
            constexpr auto getDataCacheSize() const noexcept { return _dcacheSize; }
            constexpr auto getCoreVoltage() const noexcept { return _voltage; }
            constexpr auto is3_3VoltCore() const noexcept { return getCoreVoltage() == CoreVoltageKind::V3_3; }
            constexpr auto is5VoltCore() const noexcept { return getCoreVoltage() == CoreVoltageKind::V5_0; }
            constexpr auto getSeries() const noexcept { return _series; }
            constexpr auto getStepping() const noexcept { return _stepping; }
            constexpr auto getVersion() const noexcept { return getStepping(); }
            constexpr Ordinal getDeviceId() const noexcept { return 0; }
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
                    const char* str, 
                    DeviceCode code, 
                    Ordinal salign = 4) noexcept : _str(str), _code(code), _salign(salign) { }
            constexpr CoreInformation(
                    const char* str, 
                    ProcessorSeries series,
                    CoreVoltageKind voltage, 
                    Ordinal salign = 4, // found many instances where intel defaults to 4 unless stated otherwise
                    Ordinal icacheSize = 0, 
                    Ordinal dcacheSize = 0,
                    uint8_t stepping = 0
                    ) noexcept : CoreInformation(str, ConstructedDeviceIdentificationCode { series, voltage, icacheSize, dcacheSize, stepping }, salign) { }
            constexpr CoreInformation(const char* str, Ordinal deviceId, Ordinal salign = 4) noexcept : CoreInformation(str, HardwareDeviceIdentificationCode(deviceId), salign) { }
            constexpr auto getString() const noexcept               { return _str; }
            constexpr Ordinal getDeviceId() const noexcept          { return std::visit([](auto&& d) noexcept { return d.getDeviceId(); }, _code); }
            constexpr auto getVoltage() const noexcept              { return std::visit([](auto&& d) noexcept { return d.getCoreVoltage(); }, _code); }
            constexpr auto getInstructionCacheSize() const noexcept { return std::visit([](auto&& d) noexcept { return d.getInstructionCacheSize(); }, _code); }
            constexpr auto getDataCacheSize() const noexcept        { return std::visit([](auto&& d) noexcept { return d.getDataCacheSize(); }, _code); }
            constexpr auto getSeries() const noexcept               { return std::visit([](auto&& d) noexcept { return d.getSeries(); }, _code); }
            constexpr auto hasDeviceId() const noexcept             { return std::visit([](auto&& d) noexcept { return d.deviceIdCodeInHardware(); }, _code); }
            constexpr auto getManufacturer() const noexcept         { return std::visit([](auto&& d) noexcept { return d.getManufacturer(); }, _code); }
            constexpr auto getProductType() const noexcept          { return std::visit([](auto&& d) noexcept { return d.getProductType(); }, _code); }
            constexpr auto hasInstructionCache() const noexcept     { return getInstructionCacheSize() > 0; }
            constexpr auto hasDataCache() const noexcept            { return getDataCacheSize() > 0; }
            constexpr auto is5VoltCore() const noexcept             { return getVoltage() == CoreVoltageKind::V5_0; }
            constexpr auto is3_3VoltCore() const noexcept           { return getVoltage() == CoreVoltageKind::V3_3; }
            constexpr auto getVersion() const noexcept              { return std::visit([](auto&& d) noexcept { return d.getVersion(); }, _code); }
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
            const char* _str;
            DeviceCode _code;
            Ordinal _salign;
    };
    
    // for the Sx series, I cannot find Appendix F of the reference manual online so I'm assuming 4 for SALIGN for now
    constexpr CoreInformation cpu80960SA("80960SA", ProcessorSeries::Sx, CoreVoltageKind::V5_0, 4, 512u);
    constexpr CoreInformation cpu80960SB("80960SB", ProcessorSeries::Sx, CoreVoltageKind::V5_0, 4, 512u);
    constexpr CoreInformation cpu80L960JA("80L960JA", 0x0082'1013, 1);
    constexpr CoreInformation cpu80L960JF("80L960JF", 0x0082'0013, 1);
    constexpr CoreInformation cpu80960JF("80960JF", 0x0882'0013, 1);
    constexpr CoreInformation cpu80960JD("80960JD", 0x0882'0013, 1);
    constexpr CoreInformation cpu80960JT_A0("80960JT", 0x0082'B013, 1);
#define CoreWithHardwareDeviceId(title, version, vcc, gen, model, salign) \
    constexpr CoreInformation cpu ## title ( #title , HardwareDeviceIdentificationCode { version, vcc, gen, model }, salign)
    CoreWithHardwareDeviceId(80960HA_A0, 0b0000, true, 0b0010, 0, 1);
    CoreWithHardwareDeviceId(80960HA_A1, 0b0001, true, 0b0010, 0, 1);
    CoreWithHardwareDeviceId(80960HA_A2, 0b0001, true, 0b0010, 0, 1);
    CoreWithHardwareDeviceId(80960HA_B0, 0b0010, true, 0b0010, 0, 1);
    CoreWithHardwareDeviceId(80960HA_B2, 0b0010, true, 0b0010, 0, 1);
    CoreWithHardwareDeviceId(80960HA, 0b0010, true, 0b0010, 0, 1); // most recent 80960HA
#undef CoreWithHardwareDeviceId

    static_assert(cpu80960JT_A0.getDataCacheSize() == 4096u);
    static_assert(cpu80960JT_A0.getInstructionCacheSize() == 16384u);
    static_assert(cpu80L960JA.getDataCacheSize() == 1024u);
    static_assert(cpu80L960JA.getInstructionCacheSize() == 2048u);

    static_assert(cpu80L960JF.getDataCacheSize() == 2048u);
    static_assert(cpu80L960JF.getInstructionCacheSize() == 4096u);

    static_assert(cpu80960SA.hasInstructionCache());
    static_assert(!cpu80960SA.hasDataCache());
} // end namespace i960
#endif // end I960_TYPES_H__
