#ifndef I960_DOUBLE_REGISTER_H__
#define I960_DOUBLE_REGISTER_H__
#include "types.h"
#include "NormalRegister.h"
namespace i960 {
    class DoubleRegister final {
        public:
            DoubleRegister(NormalRegister& lower, NormalRegister& upper) : _lower(lower), _upper(upper) { }
            ~DoubleRegister() = default;
            template<typename T>
            T get() const noexcept {
                using K = std::decay_t<T>;
                if constexpr(std::is_same_v<K, LongOrdinal>) {
                    return LongOrdinal(_lower.ordinal) | (LongOrdinal(_upper.ordinal) << 32);
                } else {
                    static_assert(LegalConversion<K>, "Illegal type requested");
                }
            }
            template<typename T>
            void set(T value) noexcept {
                using K = std::decay_t<T>;
                if constexpr(std::is_same_v<K, LongOrdinal>) {
                    _lower.ordinal = static_cast<Ordinal>(value);
                    _upper.ordinal = static_cast<Ordinal>(value >> 32);
                } else {
                    static_assert(LegalConversion<K>, "Illegal type requested");
                }
            }
            void set(Ordinal lower, Ordinal upper) noexcept;
            Ordinal getLowerHalf() const noexcept { return _lower.get<Ordinal>(); }
            Ordinal getUpperHalf() const noexcept { return _upper.get<Ordinal>(); }

        private:
            NormalRegister& _lower;
            NormalRegister& _upper;
    };

} // end namespace i960
#endif // end I960_DOUBLE_REGISTER_H__
