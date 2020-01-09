#ifndef I960_NORMAL_REGISTER_H__
#define I960_NORMAL_REGISTER_H__

#include "types.h"
#include "ProcessControls.h"
namespace i960 {
    class PreviousFramePointer {
        public:
            static constexpr Ordinal ReturnCodeMask = 0x0000'0007;
            static constexpr Ordinal PreReturnTraceMask = 0x0000'0008;
            static constexpr Ordinal AddressMask = 0xFFFF'FFC0; // aligned to 64-byte boundaries
            static constexpr Ordinal ExtractionMask = constructOrdinalMask(ReturnCodeMask, PreReturnTraceMask, AddressMask); 
            static constexpr Ordinal ReservedMask = ~ExtractionMask;
            constexpr PreviousFramePointer(Ordinal raw) noexcept : 
                _returnCode(ReturnCodeMask & raw), 
                _prereturnTrace(PreReturnTraceMask & raw),
                _address(AddressMask & raw) { }
            constexpr PreviousFramePointer() noexcept = default;
            constexpr auto getReturnCode() const noexcept { return _returnCode; }
            void setReturnCode(Ordinal value) noexcept { _returnCode = ReturnCodeMask & value; }
            constexpr auto getPrereturnTrace() const noexcept { return _prereturnTrace; }
            void setPrereturnTrace(bool value) noexcept { _prereturnTrace = value; }
            constexpr auto getAddress() const noexcept { return _address; }
            void setAddress(Ordinal value) noexcept { _address = AddressMask & value; }
            constexpr auto getRawValue() const noexcept {
                return (getReturnCode() | getAddress() | (getPrereturnTrace() ? PreReturnTraceMask : 0)) & ExtractionMask;
            }
            void setRawValue(Ordinal raw) noexcept {
                _returnCode = ReturnCodeMask & raw;
                _prereturnTrace = PreReturnTraceMask & raw;
                _address = AddressMask & raw;
            }
        private:
            Ordinal _returnCode = 0;
            bool _prereturnTrace = false;
            Ordinal _address = 0;
    };
    class ProcedureEntry {
        public:
            static constexpr Ordinal TypeMask = 0b11;
            static constexpr auto AddressMask = ~TypeMask;
            constexpr ProcedureEntry() noexcept = default;
            constexpr ProcedureEntry(Ordinal raw) noexcept : _type(raw & TypeMask), _address(raw & AddressMask) { }
            constexpr bool isLocal() const noexcept { return _type == 0; }
            constexpr bool isSupervisor() const noexcept { return _type == 0b10; }
            constexpr auto getAddress() const noexcept { return _address; }
            constexpr auto getRawValue() const noexcept { return (_type & TypeMask) | (_address & AddressMask); }
            void setType(Ordinal type) noexcept { _type = TypeMask & type; }
            void setAddress(Ordinal addr) noexcept { _address = (addr & _address); }
            constexpr auto getType() const noexcept { return _type; }
        private:
            Ordinal _type = 0;
            Ordinal _address = 0;

    };
    union TraceControls {
        struct {
            Ordinal unused0 : 1;
			// trace mode bits
            Ordinal instructionTraceMode : 1;
            Ordinal branchTraceMode : 1;
			Ordinal callTraceMode : 1;
            Ordinal returnTraceMode : 1;
            Ordinal prereturnTraceMode : 1;
            Ordinal supervisorTraceMode : 1;
            Ordinal markTraceMode : 1;
            Ordinal unused1 : 16;
			// hardware breakpoint event flags
			Ordinal instructionAddressBreakpoint0 : 1;
			Ordinal instructionAddressBreakpoint1 : 1;
			Ordinal dataAddressBreakpoint0 : 1;
			Ordinal dataAddressBreakpoint1 : 1;
            Ordinal unused2 : 4;
        };
        Ordinal value;
        constexpr TraceControls(Ordinal raw = 0) : value(raw) { }
		constexpr bool traceMarked() const noexcept { return markTraceMode != 0; }
        void clear() noexcept;
    } __attribute__((packed));
	static_assert(sizeof(TraceControls) == 1_words, "TraceControls must be the size of an ordinal!");
    union NormalRegister {
        public:
            constexpr NormalRegister(Ordinal value = 0) : ordinal(value) { }
            ~NormalRegister(); 

            PreviousFramePointer pfp;
            ProcedureEntry pe;
            ProcessControls pc;
            TraceControls te;
            Ordinal ordinal;
            Integer integer;
            ByteOrdinal byteOrd;
            ShortOrdinal shortOrd;
			ByteInteger byteInt;
			ShortInteger shortInt;

            template<typename T>
            constexpr T get() const noexcept {
                using K = std::decay_t<T>;
                if constexpr(std::is_same_v<K, Ordinal>) {
                    return ordinal;
                } else if constexpr(std::is_same_v<K, Integer>) {
                    return integer;
                } else if constexpr(std::is_same_v<K, ByteOrdinal>) {
                    return byteOrd;
				} else if constexpr(std::is_same_v<K, ByteInteger>) {
					return byteInt;
                } else if constexpr(std::is_same_v<K, ShortOrdinal>) {
                    return shortOrd;
				} else if constexpr(std::is_same_v<K, ShortInteger>) {
					return shortInt;
                } else if constexpr(std::is_same_v<K, PreviousFramePointer>) {
                    return pfp;
                } else if constexpr(std::is_same_v<K, LongOrdinal>) {
                    return static_cast<LongOrdinal>(ordinal);
                } else {
                    static_assert(false_v<K>, "Illegal type requested");
                }
            }
            template<typename T>
            void set(T value) noexcept {
                using K = std::decay_t<T>;
                if constexpr(std::is_same_v<K, Ordinal>) {
                    ordinal = value;
                } else if constexpr(std::is_same_v<K, Integer>) {
                    integer = value;
                } else if constexpr(std::is_same_v<K, ByteOrdinal>) {
                    byteOrd = value;
                } else if constexpr(std::is_same_v<K, ShortOrdinal>) {
                    shortOrd = value;
				} else if constexpr(std::is_same_v<K, ByteInteger>) {
					byteInt = value;
				} else if constexpr(std::is_same_v<K, ShortInteger>) {
					shortInt = value;
                } else if constexpr(std::is_same_v<K, PreviousFramePointer>) {
                    ordinal = value.value;
                } else {
                    static_assert(false_v<K>, "Illegal type requested");
                }
            }
            void move(const NormalRegister& other) noexcept;
			constexpr Ordinal mostSignificantBit() const noexcept { return getMostSignificantBit(ordinal); }
			constexpr bool mostSignificantBitSet() const noexcept     { return i960::mostSignificantBitSet(ordinal); }
			constexpr bool mostSignificantBitClear() const noexcept   { return i960::mostSignificantBitClear(ordinal); }
    };

} // end namespace i960
#endif // end I960_NORMAL_REGISTER_H__
