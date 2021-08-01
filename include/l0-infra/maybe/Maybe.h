//
// Created by Darwin Yuan on 2021/8/1.
//

#ifndef MAYBE_5B22D47F4DD54C17BAE28C385C86B3FC
#define MAYBE_5B22D47F4DD54C17BAE28C385C86B3FC

#include <l0-infra/maybe/detail/Ref_Maybe.h>
#include <l0-infra/maybe/detail/Val_Maybe.h>

namespace detail {
    template<typename T>
    using Maybe = std::conditional_t<std::is_reference_v<T>, Ref_Maybe<std::remove_reference_t<T>>, Val_Maybe<T>>;
}

template<typename T>
struct Maybe : detail::Maybe<T> {
    using Parent = detail::Maybe<T>;
    using Parent::Parent;

    using ValueType = std::remove_reference_t<T>;

    constexpr auto operator*() const -> decltype(auto) {
        return Parent::Value();
    }

    auto operator*() -> decltype(auto) {
        return Parent::Value();
    }

    constexpr explicit operator bool() const {
        return Parent::Present();
    }

private:
    template<typename C>
    struct Unwrap {
        using Type = C;
    };

    template<typename C>
    struct Unwrap<Maybe<C>> {
        using Type = typename Unwrap<C>::Type;
    };

    template<typename C>
    using Unwrap_t = typename Unwrap<C>::Type;


    template<typename>
    struct IsMaybeType : std::false_type {};

    template<typename C>
    struct IsMaybeType<Maybe<C>> : std::true_type {};

public:
    constexpr auto Flatten() const -> Maybe<Unwrap_t<T>>  {
        if constexpr(IsMaybeType<T>::value) {
            if(!Parent::Present()) return {};
            return Parent::Value().Flatten();
        } else {
            return *this;
        }
    }

    template<typename F>
    constexpr auto Map(F&& f) const -> auto {
        using result_t = std::invoke_result_t<F, ValueType const&>;
        using maybe_result_t = Maybe<Unwrap_t<result_t>>;
        if(*this) {
            return Maybe<result_t>{f(Parent::Value())}.Flatten();
        } else {
            return maybe_result_t{};
        }
    }

    friend constexpr auto operator==(Maybe const& lhs, std::nullopt_t) -> bool {
        return !lhs;
    }

    friend constexpr auto operator==(Maybe const& lhs, Maybe const& rhs) -> bool {
        if(lhs) {
            return rhs ? *lhs == *rhs : false;
        } else {
            return !rhs;
        }
    }

    friend constexpr auto operator!=(Maybe const& lhs, Maybe const& rhs) -> bool {
        return !operator==(lhs, rhs);
    }

    friend constexpr auto operator==(std::nullopt_t, Maybe const& rhs) -> bool {
        return !rhs;
    }

    friend constexpr auto operator!=(Maybe const& lhs, std::nullopt_t) -> bool {
        return (bool)lhs;
    }

    friend constexpr auto operator!=(std::nullopt_t, Maybe const& rhs) -> bool {
        return (bool)rhs;
    }
};

template<typename T>
Maybe(T&&) -> Maybe<T>;

#endif //MAYBE_5B22D47F4DD54C17BAE28C385C86B3FC
