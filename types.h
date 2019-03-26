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
        Jx, Hx, Cx,
    };
} // end namespace i960
#endif // end I960_TYPES_H__
