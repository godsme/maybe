//
// Created by Darwin Yuan on 2021/8/2.
//

#ifndef MAYBE_ABCD9ABF21A34875BA01686B71801245
#define MAYBE_ABCD9ABF21A34875BA01686B71801245

#include <type_traits>
#include <limits>
#include <algorithm>

namespace int_type::detail {
    template<typename INT_TYPE>
    constexpr auto Between(long long value) -> bool {
        return value >= std::numeric_limits<INT_TYPE>::min() && value <= std::numeric_limits<INT_TYPE>::max();
    }

    template<long long LOWER_BOUNDARY, long long UPPER_BOUNDARY>
    constexpr auto DeduceSignedIntType() -> auto {
        if constexpr(Between<int8_t>(LOWER_BOUNDARY) && Between<int8_t>(UPPER_BOUNDARY)) {
            return int8_t{};
        } else if constexpr(Between<int16_t>(LOWER_BOUNDARY) && Between<int16_t>(UPPER_BOUNDARY)) {
            return int16_t{};
        } else if constexpr(Between<int32_t>(LOWER_BOUNDARY) && Between<int32_t>(UPPER_BOUNDARY)) {
            return int32_t{};
        } else {
            return int64_t{};
        }
    }

    template<long long UPPER_BOUNDARY>
    constexpr auto DeduceUnsignedIntType() -> auto {
        static_assert(UPPER_BOUNDARY <= std::numeric_limits<uint64_t>::max());
        if constexpr(UPPER_BOUNDARY <= std::numeric_limits<uint8_t>::max()) {
            return uint8_t{};
        } else if constexpr(UPPER_BOUNDARY <= std::numeric_limits<uint16_t>::max()) {
            return uint16_t{};
        } else if constexpr(UPPER_BOUNDARY <= std::numeric_limits<uint32_t>::max()) {
            return uint32_t{};
        } else {
            return uint64_t{};
        }
    }

    template<long long LOWER_BOUNDARY, long long UPPER_BOUNDARY>
    constexpr auto DeduceIntType() -> auto {
        if constexpr(LOWER_BOUNDARY < 0) {
            return DeduceSignedIntType<LOWER_BOUNDARY, UPPER_BOUNDARY>();
        } else {
            return DeduceUnsignedIntType<UPPER_BOUNDARY>();
        }
    }
}

template<long long LOWER_BOUNDARY_, long long UPPER_BOUNDARY_>
struct Int {
    constexpr static bool sEcReAt_InT_tAg = true;
    static_assert(UPPER_BOUNDARY_ > LOWER_BOUNDARY_);

    constexpr static bool IS_SIGNED = LOWER_BOUNDARY_ < 0;
    using IntType = decltype(int_type::detail::DeduceIntType<LOWER_BOUNDARY_, UPPER_BOUNDARY_>());
    constexpr static IntType LOWER_BOUNDARY = LOWER_BOUNDARY_;
    constexpr static IntType UPPER_BOUNDARY = UPPER_BOUNDARY_;

    constexpr Int(IntType value) : value{value} {}

    constexpr operator IntType() const {
        return value;
    }

    constexpr auto IsValid() const -> bool {
        if constexpr(LOWER_BOUNDARY != 0) {
            return value >= LOWER_BOUNDARY && value <= UPPER_BOUNDARY;
        } else {
            return value <= UPPER_BOUNDARY;
        }
    }

protected:
    IntType value;
};

#endif //MAYBE_ABCD9ABF21A34875BA01686B71801245
