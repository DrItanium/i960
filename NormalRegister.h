#ifndef I960_NORMAL_REGISTER_H__
#define I960_NORMAL_REGISTER_H__

#include "types.h"
#include "ProcessControls.h"
namespace i960 {
    union PreviousFramePointer {
        struct {
            Ordinal returnCode : 3;
            Ordinal prereturnTrace : 1;
            Ordinal unused : 2; // 80960 ignores the lower six bits of this register
            Ordinal address : 26;
        };
        Ordinal value;
    } __attribute__((packed));
    class ProcedureEntry {
        public:
            static constexpr Ordinal TypeMask = 0b11;
            static constexpr auto AddressMask = ~TypeMask;
            constexpr ProcedureEntry(Ordinal _raw) noexcept : _type(_raw & TypeMask), _address(_raw & AddressMask) { }
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
    constexpr Ordinal getMostSignificantBit(Ordinal value) noexcept {
        return value & 0x8000'0000;
    }
    constexpr auto mostSignificantBitSet(Ordinal value) noexcept {
        return getMostSignificantBit(value) != 0;
    }
    constexpr auto mostSignificantBitClear(Ordinal value) noexcept {
        return getMostSignificantBit(value) == 0;
    }
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

	static_assert(sizeof(NormalRegister) == 1_words, "NormalRegister must be 32-bits wide!");

} // end namespace i960
#endif // end I960_NORMAL_REGISTER_H__
