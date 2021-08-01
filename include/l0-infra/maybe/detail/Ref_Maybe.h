//
// Created by Darwin Yuan on 2021/8/2.
//

#ifndef MAYBE_07C6172EDA2C406B9355BE40E15AA134
#define MAYBE_07C6172EDA2C406B9355BE40E15AA134

#include <l0-infra/base/detail/Ref_Placement.h>
#include <optional>

namespace detail {
    template<typename T>
    struct Ref_Maybe {
        static_assert(!std::is_reference_v<T>);

        constexpr Ref_Maybe() {}
        constexpr Ref_Maybe(std::nullopt_t) {}
        constexpr Ref_Maybe(Ref_Maybe<T> const& rhs) : value{rhs.value} {}
        constexpr Ref_Maybe(T& ref) : value{ref} {}

        auto operator=(Ref_Maybe const& rhs) -> Ref_Maybe& {
            value = rhs.value;
            return *this;
        }

        constexpr auto Present() const -> bool {
            return (bool)value;
        }

        constexpr auto Value() const -> decltype(auto) {
            return value.GetRef();
        }

        auto Value() -> decltype(auto) {
            return value.GetRef();
        }

        auto Emplace(T& ref) -> auto {
            return value.Emplace(ref);
        }

        constexpr auto ValueOr(T& defaultValue) const -> T& {
            return Present() ?  Value() : defaultValue;
        }

        constexpr auto ValueOr(T&& defaultValue) const = delete;

    private:
        Ref_Placement<T> value;
    };
}

#endif //MAYBE_07C6172EDA2C406B9355BE40E15AA134
