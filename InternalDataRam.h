#ifndef I960_INTERNAL_DATA_RAM_H__
#define I960_INTERNAL_DATA_RAM_H__
#include "types.h"
namespace i960 {
	/**
	 * A block of 1024 bytes which is readable and writable but not
	 * executable. It is embedded within the processor itself and generates
	 * no external buss activity is generated when accessed.
	 */
	template<Ordinal numBytes>
	struct InternalDataRam {
		public:
			// first 64 bytes are reserved for optional interrupt vectors and the nmi vector
			constexpr static Ordinal MinimumSize = 1_words;
			constexpr static Ordinal TotalReservedBytes = 64;
			constexpr static Ordinal TotalByteCapacity = numBytes;
			static_assert(numBytes >= MinimumSize, "InternalDataRam must be at least 4 bytes in size");
			static_assert((numBytes & 1) == 0, "numBytes is not even!");
			static_assert(numBytes == ((numBytes >> 2) << 2), "numBytes is not a power of two value!");
			constexpr static Ordinal TotalUnreservedBytes  = TotalByteCapacity - TotalReservedBytes;
			constexpr static Ordinal TotalWordCapacity = TotalByteCapacity / sizeof(Ordinal);
			constexpr static Ordinal TotalReservedWords = TotalReservedBytes / sizeof(Ordinal);
			constexpr static Ordinal TotalUnreservedWords = TotalUnreservedBytes / sizeof(Ordinal);
			constexpr static Ordinal LegalMaskValue = TotalWordCapacity - 1;
			template<Ordinal baseAddress>
			constexpr static Ordinal LargestAddress = (baseAddress +  TotalByteCapacity) - 1;
			template<Ordinal baseAddress>
			constexpr static Ordinal SmallestAddress = baseAddress;
			
		public:
			InternalDataRam() = default;
			~InternalDataRam() = default;
			void reset() noexcept {
				// zero out memory
				for (auto i = 0u; i < TotalWordCapacity; ++i) {
					write(i, 0);
				}
			}
			void writeByte(Ordinal address, ByteOrdinal value) noexcept {
				auto localOrdinal = read(address);
				switch (address & 0b11) {
					case 0b00:
						localOrdinal = (localOrdinal & 0xFFFFFF00) | Ordinal(value);
						break;
					case 0b01:
						localOrdinal = (localOrdinal & 0xFFFF00FF) | (Ordinal(value) << 8);
						break;
					case 0b10:
						localOrdinal = (localOrdinal & 0xFF00FFFF) | (Ordinal(value) << 16);
						break;
					case 0b11:
						localOrdinal = (localOrdinal & 0x00FFFFFF) | (Ordinal(value) << 24);
						break;
					default:
						throw "Should never fire!";
				}
				write(address & (~0b11), localOrdinal);
			}
			void write(Ordinal address, Ordinal value) noexcept {
				auto actualAddress = (address >> 2) & LegalMaskValue;
				_words[actualAddress] = value;
			}
			ByteOrdinal readByte(Ordinal address) const noexcept {
				// extract out the ordinal that the byte is a part of
				auto closeValue = read(address);
				switch (address & 0b11) {
					case 0b00: 
						return ByteOrdinal(closeValue);
					case 0b01: 
						return ByteOrdinal(closeValue >> 8);
					case 0b10: 
						return ByteOrdinal(closeValue >> 16);
					case 0b11: 
						return ByteOrdinal(closeValue >> 24);
					default:
						throw "Should never be hit!";
				}
			}
			Ordinal read(Ordinal address) const noexcept {
				// the address needs to be fixed to be a multiple of four
				auto realAddress = (address >> 2) & LegalMaskValue;
				return _words[realAddress];
			}
            LongOrdinal readDouble(Ordinal address) const noexcept {
                return (LongOrdinal(read(address+1)) << 32) | LongOrdinal(read(address));
            }
			constexpr Ordinal totalByteCapacity() const noexcept { return TotalByteCapacity; }
		private:
			Ordinal _words[TotalUnreservedWords + TotalReservedWords];
	} __attribute__((packed));
	using JxCPUInternalDataRam = InternalDataRam<1024>;
	static_assert(sizeof(JxCPUInternalDataRam) == JxCPUInternalDataRam::TotalByteCapacity, "InternalDataRam is larger than its storage footprint!");
} // end namespace i960
#endif // end I960_INTERNAL_DATA_RAM_H__
