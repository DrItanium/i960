#ifndef I960_TYPES_H__
#define I960_TYPES_H__
#include <cstdint>
#include <memory>
#include <variant>
namespace i960 {

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
    template<typename T>
    using ShiftMaskPair = std::tuple<T, T>;
    template<typename T, typename R, ShiftMaskPair<T> description>
    constexpr R decode(T value) noexcept {
        return decode<T, R, std::get<0>(description), std::get<1>(description)>(value);
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
    template<typename T, typename R, ShiftMaskPair<T> description>
    constexpr T encode(T value, R input) noexcept {
        return encode<T, R, std::get<0>(description), std::get<1>(description)>(value, input);
    }

    template<typename T, typename R, T mask, T shift = static_cast<T>(0)>
    class BitPattern final {
        public:
            using DataType = T;
            using SliceType = R;
            using InteractPair = std::tuple<DataType, DataType>;
            using TupleSliceType = std::tuple<SliceType>;
        public:
            static constexpr auto Mask = mask;
            static constexpr auto Shift = shift;
        public:
            constexpr BitPattern() = default;
            ~BitPattern() = default;
            constexpr InteractPair getDescription() const noexcept { return _description; }
            constexpr auto getMask() const noexcept { return std::get<0>(_description); }
            constexpr auto getShift() const noexcept { return std::get<1>(_description); }
            constexpr auto decode(DataType input) const noexcept {
                return decodePattern(input);
            }
            constexpr auto encode(DataType value, SliceType input) const noexcept {
                return encodePattern(value, input);
            }
            constexpr auto encode(SliceType input) const noexcept {
                return encode(static_cast<DataType>(0), input);
            }
            constexpr auto patternClear(DataType input) const noexcept {
                return decode(input) == 0;
            }
            constexpr auto patternSet(DataType input) const noexcept {
                return decode(input) != 0;
            }
            constexpr auto patternSetTo(DataType input, SliceType value) const noexcept {
                return decode(input) == value;
            }
            static constexpr auto decodePattern(DataType input) noexcept {
                return i960::decode<DataType, SliceType, _description>(input);
            }
            static constexpr auto encodePattern(DataType value, SliceType input) noexcept {
                return i960::encode<DataType, SliceType, _description>(value, input);
            }
            static constexpr auto encodePattern(SliceType input) noexcept {
                return encodePattern(static_cast<DataType>(0), input);
            }

        private:
            static constexpr InteractPair _description { mask, shift };
    };
    template<typename T, typename R, T mask, T shift = static_cast<T>(0)>
    using BitFragment = BitPattern<T, R, mask, shift>;
    template<typename T, T mask, T shift = 0>
    using SameWidthFragment = BitFragment<T, T, mask, shift>;

    template<typename T, T mask>
    using FlagFragment = BitFragment<T, bool, mask>;


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

    using MostSignificantBitOfOrdinal = SameWidthFragment<Ordinal, 0x8000'0000>;
    constexpr MostSignificantBitOfOrdinal MostSignificantBitPattern;
    template<typename T, std::enable_if_t<std::is_integral_v<std::decay_t<T>>, int> = 0>
    using LowestTwoBitsPattern = SameWidthFragment<T, static_cast<T>(0b11)>;
    template<typename T, std::enable_if_t<std::is_integral_v<std::decay_t<T>>, int> = 0>
    using LowestBitPattern = SameWidthFragment<T, static_cast<T>(0b1)>;
    template<typename T, std::enable_if_t<std::is_integral_v<std::decay_t<T>>, int> = 0>
    using InverseOfLowestTwoBitsPattern = SameWidthFragment<T, ~static_cast<T>(0b11)>;

    template<typename R, Ordinal mask, Ordinal shift = 0>
    using OrdinalBitPattern = BitPattern<Ordinal, R, mask, shift>;
    template<uint8_t index>
    using OrdinalFlagPattern = OrdinalBitPattern<bool, 1u << (index & 0b11111)>; // shift does not matter

    template<uint8_t index>
    using ByteOrdinalFlagPattern = BitPattern<ByteOrdinal, bool, 1u << (index & 0b111), (index & 0b111)>;

    using LowestOrdinalBitPattern = OrdinalFlagPattern<0>;
    using MostSignificantOrdinalBitPattern = OrdinalFlagPattern<31>;
    static_assert(LowestOrdinalBitPattern::decodePattern(0x1));
    static_assert(!LowestOrdinalBitPattern::decodePattern(0x2));
    static_assert(MostSignificantOrdinalBitPattern::decodePattern(0x8000'0000));
    static_assert(!MostSignificantOrdinalBitPattern::decodePattern(0x0800'0000));


    using HighestOrdinalQuadrant = OrdinalBitPattern<ByteOrdinal, 0xFF00'0000, 24>;
    using HigherOrdinalQuadrant =  OrdinalBitPattern<ByteOrdinal, 0x00FF'0000, 16>;
    using LowerOrdinalQuadrant = OrdinalBitPattern<ByteOrdinal, 0x0000'FF00, 8>;
    using LowestOrdinalQuadrant = OrdinalBitPattern<ByteOrdinal, 0x0000'00FF>;
    using UpperOrdinalHalf = OrdinalBitPattern<HalfOrdinal, 0xFFFF'0000, 16>;
    using LowerOrdinalHalf = OrdinalBitPattern<HalfOrdinal, 0x0000'FFFF>;


    using OrdinalQuadrants = std::tuple<ByteOrdinal, ByteOrdinal, ByteOrdinal, ByteOrdinal>;
    using OrdinalHalves = std::tuple<HalfOrdinal, HalfOrdinal>;


    template<typename T, typename ... Patterns>
    class BinaryEncoderDecoder final {
        public:
            using BinaryType = T;
            using UnpackedBinary = std::tuple<typename Patterns::SliceType...>;
            using DataType = T;
            using SliceType = UnpackedBinary;
            static constexpr auto NumberOfPatterns = std::tuple_size_v<UnpackedBinary>;
            static_assert(NumberOfPatterns > 0, "Must have at least one pattern");
            static_assert((std::is_same_v<typename Patterns::DataType, BinaryType> && ...), "All patterns must operate on the provided binary type");
            /// @todo figure out how to bind the return of tuple_cat from SliceTypes
        public:
            static constexpr UnpackedBinary decode(BinaryType input) noexcept {
                return std::make_tuple<typename Patterns::SliceType...>(Patterns::decodePattern(input)...);
            }
            static constexpr BinaryType encode(typename Patterns::SliceType&& ... values) noexcept {
                return (Patterns::encodePattern(values) | ...);
            }
            static constexpr BinaryType encode(BinaryType value, typename Patterns::SliceType&& ... inputs) noexcept {
                return (Patterns::encodePattern(value, inputs) | ... );
            }
            static constexpr BinaryType encode(BinaryType value, UnpackedBinary&& tup) noexcept {
                return std::apply([value](typename Patterns::SliceType&& ... args) noexcept { return encode(value, args...); }, tup);
            }
            static constexpr BinaryType encode(UnpackedBinary&& tup) noexcept {
                return std::apply([](typename Patterns::SliceType&& ... args) noexcept { return encode(args...); }, tup);
            }
            static constexpr auto decodePattern(BinaryType input) noexcept { return decode(input); }
            static constexpr BinaryType encodePattern(UnpackedBinary&& tup) noexcept { return encode(tup); }
            static constexpr BinaryType encodePattern(BinaryType value, UnpackedBinary&& tup) noexcept { return encode(value, tup); }
            static constexpr BinaryType encodePattern(typename Patterns::SliceType&& ... values) noexcept { return encode(values...); }
            static constexpr BinaryType encodePattern(BinaryType value, typename Patterns::SliceType&& ... tup) noexcept { return encode(value, tup...); }
    };
    using OrdinalHalvesEncoderDecoder = BinaryEncoderDecoder<Ordinal, LowerOrdinalHalf, UpperOrdinalHalf>;
    using OrdinalQuadrantsEncoderDecoder = BinaryEncoderDecoder<Ordinal, LowestOrdinalQuadrant, LowerOrdinalQuadrant, HigherOrdinalQuadrant, HighestOrdinalQuadrant>;
    static constexpr Ordinal TestValue = 0xFDED'ABCD;
    static_assert(std::get<1>(OrdinalHalvesEncoderDecoder::decode(TestValue)) == 0xFDED);
    static_assert(OrdinalHalvesEncoderDecoder::encode(0xABCD, 0xFDED) == TestValue);

    constexpr OrdinalQuadrants getQuadrants(Ordinal input) noexcept {
        return OrdinalQuadrantsEncoderDecoder::decode(input);
    }
    static_assert(std::get<3>(getQuadrants(0xFDEDABCD)) == 0xFD);
    static_assert(std::get<2>(getQuadrants(0xFDEDABCD)) == 0xED);
    static_assert(std::get<1>(getQuadrants(0xFDEDABCD)) == 0xAB);
    static_assert(std::get<0>(getQuadrants(0xFDEDABCD)) == 0xCD);
    constexpr OrdinalHalves getHalves(Ordinal input) noexcept {
        return OrdinalHalvesEncoderDecoder::decode(input);
    }

    using OrdinalHalvesAndQuadsEncoderDecoder = 
        BinaryEncoderDecoder<Ordinal, OrdinalHalvesEncoderDecoder, OrdinalQuadrantsEncoderDecoder>;
    constexpr auto getQuadrantsAndHalves(Ordinal input) noexcept {
        return OrdinalHalvesAndQuadsEncoderDecoder::decode(input);
    }

    static_assert(OrdinalHalvesAndQuadsEncoderDecoder::NumberOfPatterns == 2);
    static_assert(std::get<3>(std::get<1>(getQuadrantsAndHalves(0xFDEDABCD))) == 0xFD);
    static_assert(std::get<2>(std::get<1>(getQuadrantsAndHalves(0xFDEDABCD))) == 0xED);
    static_assert(std::get<1>(std::get<1>(getQuadrantsAndHalves(0xFDEDABCD))) == 0xAB);
    static_assert(std::get<0>(std::get<1>(getQuadrantsAndHalves(0xFDEDABCD))) == 0xCD);
    static_assert(std::get<1>(std::get<0>(getQuadrantsAndHalves(0xFDEDABCD))) == 0xFDED);
    static_assert(std::get<0>(std::get<0>(getQuadrantsAndHalves(0xFDEDABCD))) == 0xABCD);

    static_assert(std::get<1>(getHalves(0xFDEDABCD)) == 0xFDED);
    static_assert(std::get<0>(getHalves(0xFDEDABCD)) == 0xABCD);

    constexpr auto getMostSignificantBit(Ordinal value) noexcept {
        return MostSignificantBitPattern.decode(value);
    }
    constexpr auto mostSignificantBitSet(Ordinal value) noexcept { 
        return MostSignificantBitPattern.patternSet(value);
    }
    constexpr auto mostSignificantBitClear(Ordinal value) noexcept {
        return MostSignificantBitPattern.patternClear(value);
    }
    template<typename T>
    constexpr T getLowestTwoBits(T address) noexcept {
        return LowestTwoBitsPattern<T>::decodePattern(address);
    }
    template<typename T>
    constexpr T getLowestBit(T address) noexcept {
        return LowestBitPattern<T>::decodePattern(address);
    }
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

    template<Ordinal mask, Ordinal shift = 0>
    using OrdinalToByteOrdinalField = BitFragment<Ordinal, ByteOrdinal, mask, shift>;


    // false_v taken from https://quuxplusone.github.io/blog/2018/04/02/false-v/
    template<typename...>
    inline constexpr bool false_v = false;

    template<typename ... Args>
    inline constexpr Ordinal constructOrdinalMask(Args&& ... masks) noexcept {
        return (... | static_cast<Ordinal>(masks));
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
            using VersionPattern = BitPattern<uint32_t, uint8_t, 0xF000'0000, 28>;
            using PartNumberPattern = BitPattern<uint32_t, uint16_t, 0x0FFF'F000, 12>;
            using ManufacturerPattern = BitPattern<uint32_t, uint16_t, 0xFFE, 1>;
            static constexpr BitFragment<uint32_t, uint8_t, 0xF000'0000, 28> VersionMask {};
            static constexpr BitFragment<uint32_t, uint16_t, 0x0FFF'F000, 12> PartNumMask {};
            static constexpr BitFragment<uint32_t, uint16_t, 0xFFE, 1> ManufacturerMask { };
            using ManufacturerMaskEncoder = BinaryEncoderDecoder<uint32_t, VersionPattern, PartNumberPattern, ManufacturerPattern>;
        public:
            constexpr IEEE1149_1DeviceIdentification(uint8_t version, uint16_t partNum, uint16_t mfgr) : _version(version & 0b1111), _partNumber(partNum), _manufacturer(mfgr & 0x7FF) { }
            constexpr IEEE1149_1DeviceIdentification(typename ManufacturerMaskEncoder::UnpackedBinary&& info) : IEEE1149_1DeviceIdentification(std::get<0>(info), std::get<1>(info), std::get<2>(info)) { }
            constexpr IEEE1149_1DeviceIdentification(uint32_t id) noexcept : IEEE1149_1DeviceIdentification(ManufacturerMaskEncoder::decode(id)) { }
            constexpr auto getVersion() const noexcept { return _version; }
            constexpr auto getPartNumber() const noexcept { return _partNumber; }
            constexpr auto getManufacturer() const noexcept { return _manufacturer; }
            constexpr uint32_t getId() const noexcept {
                return ManufacturerMaskEncoder::encode(getVersion(), getPartNumber(), getManufacturer()) | 1; // make sure lsb is 1
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
            using VersionDecoder = BitFragment<Ordinal, uint8_t, 0xF000'0000, 28 >;
            using VCCDecoder = FlagFragment<Ordinal, 1<<27>;
            using GenerationDecoder = BitFragment<Ordinal, uint8_t, (0b1111 << 17), 17>;
            using ModelDecoder = BitFragment<Ordinal, uint8_t, (0b11111 << 12), 12>;
        public:
            constexpr HardwareDeviceIdentificationCode(uint8_t version, bool vcc, uint8_t generation, uint8_t model) noexcept :
                _version(version & 0b11111),
                _vcc(vcc),
                _gen(generation & 0b1111),
                _model(model & 0b11111) { }
            constexpr HardwareDeviceIdentificationCode(Ordinal raw) noexcept : HardwareDeviceIdentificationCode(
                    VersionDecoder::decodePattern(raw),
                    VCCDecoder::decodePattern(raw),
                    GenerationDecoder::decodePattern(raw),
                    ModelDecoder::decodePattern(raw)) { }
            constexpr auto getVersion() const noexcept { return _version; }
            constexpr auto getGeneration() const noexcept { return _gen; }
            constexpr auto getModel() const noexcept { return _model; }
            constexpr Ordinal getDeviceId() const noexcept { 
                Ordinal start = VersionDecoder::encodePattern(_version);
                start |= VCCDecoder::encodePattern(_vcc);
                start |= (static_cast<Ordinal>(getProductType() & 0b111111) << 21);
                start |= GenerationDecoder::encodePattern(_gen);
                start |= ModelDecoder::encodePattern(_model);
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
#define Def80960HxCore(title, model) \
    CoreWithHardwareDeviceId(title ## _A0, 0b0000, true, 0b0010, model, 1); \
    CoreWithHardwareDeviceId(title ## _A1, 0b0001, true, 0b0010, model, 1); \
    CoreWithHardwareDeviceId(title ## _A2, 0b0001, true, 0b0010, model, 1); \
    CoreWithHardwareDeviceId(title ## _B0, 0b0010, true, 0b0010, model, 1); \
    CoreWithHardwareDeviceId(title ## _B2, 0b0010, true, 0b0010, model, 1)
    Def80960HxCore(80960HA, 0b00000);
    Def80960HxCore(80960HD, 0b00001);
    Def80960HxCore(80960HT, 0b00010);
#undef Def80960HxCore
    constexpr auto cpu80960HA(cpu80960HA_B2);
    constexpr auto cpu80960HD(cpu80960HD_B2);
    constexpr auto cpu80960HT(cpu80960HT_B2);
#undef CoreWithHardwareDeviceId

    static_assert(cpu80960JT_A0.getDataCacheSize() == 4096u);
    static_assert(cpu80960JT_A0.getInstructionCacheSize() == 16384u);
    static_assert(cpu80L960JA.getDataCacheSize() == 1024u);
    static_assert(cpu80L960JA.getInstructionCacheSize() == 2048u);

    static_assert(cpu80L960JF.getDataCacheSize() == 2048u);
    static_assert(cpu80L960JF.getInstructionCacheSize() == 4096u);

    static_assert(cpu80960SA.hasInstructionCache());
    static_assert(!cpu80960SA.hasDataCache());
    static_assert(cpu80960HA.getSeries() == ProcessorSeries::Hx);
    static_assert(cpu80960HT.getSeries() == ProcessorSeries::Hx);
    static_assert(cpu80960HD.getSeries() == ProcessorSeries::Hx);
    static_assert(cpu80960SA.getSeries() == ProcessorSeries::Sx);
    static_assert(cpu80960SB.getSeries() == ProcessorSeries::Sx);
} // end namespace i960
#endif // end I960_TYPES_H__
