//
// Created by Darwin Yuan on 2021/8/3.
//

#ifndef MAYBE_1FE4415056474623A46B184C7ADB11F1
#define MAYBE_1FE4415056474623A46B184C7ADB11F1

#include <limits>
#include <optional>

template<typename T>
struct Int_Maybe {
    static_assert(T::sEcReAt_InT_tAg);
    using IntType = typename T::IntType;

private:
    constexpr static auto GetEmpty() -> IntType {
        static_assert(T::LOWER_BOUNDARY > std::numeric_limits<IntType>::min() || T::UPPER_BOUNDARY < std::numeric_limits<IntType>::max());
        if constexpr(T::IS_SIGNED) {
            if constexpr(T::LOWER_BOUNDARY > std::numeric_limits<IntType>::min()) {
                return std::numeric_limits<IntType>::min();
            } else {
                return std::numeric_limits<IntType>::max();
            }
        } else {
            if constexpr(T::UPPER_BOUNDARY < std::numeric_limits<IntType>::max()) {
                return std::numeric_limits<IntType>::max();
            } else {
                return std::numeric_limits<IntType>::min();
            }
        }
    }

public:
    constexpr static IntType EMPTY = GetEmpty();

    constexpr Int_Maybe() {}
    constexpr Int_Maybe(std::nullopt_t) {}
    constexpr Int_Maybe(T const& value) : value{value} {}
    constexpr Int_Maybe(IntType const& value) : value{value} {}
    constexpr Int_Maybe(Int_Maybe const& rhs) : value{value} {}

    constexpr auto Present() const -> bool {
        return value.IsValid();
    }

    constexpr auto Value() const -> T {
        return value;
    }

    auto Value() -> T& {
        return value;
    }

    constexpr auto ValueOr(T const& defaultValue) const -> T {
        return Present() ? Value() : defaultValue;
    }

    auto Emplace(T const& v) -> auto {
        value = v;
    }

private:
    T value{EMPTY};
};

#endif //MAYBE_1FE4415056474623A46B184C7ADB11F1
