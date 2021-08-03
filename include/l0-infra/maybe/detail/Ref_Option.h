//
// Created by Darwin Yuan on 2021/8/2.
//

#ifndef MAYBE_DE58EF5F44BE45C0AACA977FB4197456
#define MAYBE_DE58EF5F44BE45C0AACA977FB4197456

#include <l0-infra/base/detail/Ref_Placement.h>
#include <optional>

namespace detail {
    template<typename T>
    struct Ref_Option {
        static_assert(!std::is_reference_v<T>);

        constexpr Ref_Option() {}
        constexpr Ref_Option(std::nullopt_t) {}
        constexpr Ref_Option(Ref_Option<T> const& rhs) : value{rhs.value} {}
        constexpr Ref_Option(T& ref) : value{ref} {}

        auto operator=(Ref_Option const& rhs) -> Ref_Option& {
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

#endif //MAYBE_DE58EF5F44BE45C0AACA977FB4197456
