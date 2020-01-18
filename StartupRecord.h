#ifndef I960_IBR_H__
#define I960_IBR_H__
#include "types.h"
#include <tuple>
namespace i960 {
    class MachineStartupRecord {
        public:
            MachineStartupRecord() = default;
            virtual Ordinal computeChecksum() const noexcept = 0;
            virtual bool checksumPassed() const noexcept {
                return computeChecksum() == 0;
            }
            constexpr auto getFirstInstructionAddress() const noexcept { return _firstInstruction; }
            void setFirstInstructionAddress(Ordinal value) noexcept { _firstInstruction = value; }
            constexpr auto getPrcbPointer() const noexcept { return _prcbPointer; }
            void setPrcbPointer(Ordinal value) noexcept { _prcbPointer = value; }
        private:
            Ordinal _firstInstruction;
            Ordinal _prcbPointer;
    };
    constexpr bool initializationBootRecordSupported(ProcessorSeries proc) noexcept {
        switch (proc) {
            case ProcessorSeries::Sx:
            case ProcessorSeries::Kx:
                return false;
            default:
                return true;
        }
    }

    template<ProcessorSeries proc>
    constexpr Ordinal InitialMemoryImageStartAddress = 0x0000'0000;
    template<> constexpr Ordinal InitialMemoryImageStartAddress<ProcessorSeries::Jx> = 0xFEFF'FF30;
    template<> constexpr Ordinal InitialMemoryImageStartAddress<ProcessorSeries::Hx> = 0xFEFF'FF30;
    template<> constexpr Ordinal InitialMemoryImageStartAddress<ProcessorSeries::Cx> = 0xFEFF'FF00;

    /// This structure only applies to CX, HX, and JX cpus
    class InitializationBootRecord : public MachineStartupRecord {
        public:
            Ordinal computeChecksum() const noexcept override {
                /// @todo implement
                return 0;
            }
        public:
            Ordinal busByte0, busByte1, busByte2, busByte3;
            Integer checkSum[6];
    };


    class KxSxStartupRecord : public MachineStartupRecord {
        public:
            Ordinal satPointer;
            Ordinal checkWord;
            Ordinal checkWords[4];
        public:
            Ordinal computeChecksum() const noexcept override {
                // The startup record uses the eight words as a checksum where words 3 and 5-8 
                // are specially chosen such that the one's complement of the sum of these
                // eight words plus 0xFFFF'FFFF equals 0
                auto baseSum = satPointer + getPrcbPointer() + checkWord + getFirstInstructionAddress();
                for (int i = 0; i < 4; ++i) {
                    baseSum += checkWords[i];
                }
                return (~baseSum) + 0xFFFF'FFFF;
            }
    };


    template<ProcessorSeries proc>
    struct StartupRecordType final {
        StartupRecordType() = delete;
        ~StartupRecordType() = delete;
        StartupRecordType(const StartupRecordType&) = delete;
        StartupRecordType(StartupRecordType&&) = delete;
        StartupRecordType& operator=(const StartupRecordType&) = delete;
        StartupRecordType& operator=(StartupRecordType&&) = delete;
        static_assert(initializationBootRecordSupported(proc), "IBR is not supported on the given CPU series nor is a replacement defined");
        using ValueType = InitializationBootRecord;
    };

    template<ProcessorSeries proc>
    using StartupRecord_t = typename StartupRecordType<proc>::ValueType;
    template<ProcessorSeries proc>
    constexpr auto StartupRecordAddress = InitialMemoryImageStartAddress<proc>;
    template<ProcessorSeries proc>
    constexpr auto StartupRecordEndAddress = StartupRecordAddress<proc> + sizeof(StartupRecord_t<proc>);

} // end namespace i960
#endif // end I960_IBR_H__
