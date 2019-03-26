#ifndef I960_IBR_H__
#define I960_IBR_H__
#include "types.h"
#include <tuple>
namespace i960 {
    constexpr bool initializationBootRecordSupported(ProcessorSeries proc) noexcept {
        switch (proc) {
            case ProcessorSeries::Sx:
            case ProcessorSeries::Kx:
                return false;
            default:
                return true;
        }
    }

    /// This structure only applies to CX, HX, and JX cpus
    struct InitializationBootRecord
    {
        Ordinal busByte0, busByte1, busByte2, busByte3;
        Ordinal firstInstruction;
        Ordinal prcbPtr;
        Integer checkSum[6];
    };

    static_assert(sizeof(InitializationBootRecord) == 48, "IBR is too large!");

    template<ProcessorSeries proc>
    constexpr Ordinal getIBRBegin() noexcept {
        static_assert(initializationBootRecordSupported(proc), "Provided processor series does not use an IBR");
        if constexpr (proc == ProcessorSeries::Cx) {
            return 0xFEFF'FF00;
        } else {
            return 0xFEFF'FF30;
        }
    }
    template<ProcessorSeries proc>
    constexpr auto getIBREnd() noexcept {
        static_assert(initializationBootRecordSupported(proc), "Provided processor series does not use an IBR");
        return getIBRBegin<proc>() + sizeof(InitializationBootRecord);
    }



    template<ProcessorSeries proc>
    constexpr auto getIBRAddressRange() noexcept {
        static_assert(initializationBootRecordSupported(proc), "Provided processor series does not use an IBR");
        return std::make_tuple(InitializationBootRecordBegin<proc>, getIBREnd<proc>());
    }



} // end namespace i960
#endif // end I960_IBR_H__
