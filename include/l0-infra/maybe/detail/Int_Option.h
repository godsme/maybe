//
// Created by Darwin Yuan on 2021/8/3.
//

#ifndef MAYBE_ED27CD07A5DD4F89977DB024ECC1D0B8
#define MAYBE_ED27CD07A5DD4F89977DB024ECC1D0B8

#include <limits>
#include <optional>

template<typename T>
struct Int_Option {
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

    constexpr Int_Option() {}
    constexpr Int_Option(std::nullopt_t) {}
    constexpr Int_Option(T const& value) : value{value} {}
    constexpr Int_Option(IntType const& value) : value{value} {}
    constexpr Int_Option(Int_Option const& rhs) : value{rhs.value} {}

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

#endif //MAYBE_ED27CD07A5DD4F89977DB024ECC1D0B8
