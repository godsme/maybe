//
// Created by Darwin Yuan on 2021/8/1.
//

#ifndef MAYBE_5B22D47F4DD54C17BAE28C385C86B3FC
#define MAYBE_5B22D47F4DD54C17BAE28C385C86B3FC

#include <l0-infra/maybe/detail/Ref_Maybe.h>
#include <l0-infra/maybe/detail/Val_Maybe.h>
#include <l0-infra/maybe/detail/Int_Maybe.h>

namespace detail {
    template<typename T, typename = void>
    struct IsIntType : std::false_type {};

    template<typename T>
    struct IsIntType<T, std::enable_if_t<T::sEcReAt_InT_tAg>> {
        using IntType = typename T::IntType;
        constexpr static bool value = T::LOWER_BOUNDARY > std::numeric_limits<IntType>::min() || T::UPPER_BOUNDARY < std::numeric_limits<IntType>::max();
    };

    template<typename T>
    using ValueMaybe = std::conditional_t<IsIntType<T>::value, Int_Maybe<T>, Val_Maybe<T>>;

    template<typename T>
    using Maybe = std::conditional_t<std::is_reference_v<T>, Ref_Maybe<std::remove_reference_t<T>>, ValueMaybe<T>>;
}

template<typename T>
struct Maybe : detail::Maybe<T> {
private:
    using Parent = detail::Maybe<T>;
    using ValueType = std::remove_reference_t<T>;

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
    using Parent::Parent;

    constexpr auto operator*() const -> decltype(auto) {
        return Parent::Value();
    }

    auto operator*() -> decltype(auto) {
        return Parent::Value();
    }

    constexpr explicit operator bool() const {
        return Parent::Present();
    }

    constexpr auto Flatten() const -> Maybe<Unwrap_t<T>>  {
        if constexpr(IsMaybeType<T>::value) {
            if(!(*this)) return {};
            return (*(*this)).Flatten();
        } else {
            return *this;
        }
    }

    template<typename F>
    constexpr auto Map(F&& f) const -> auto {
        using result_t = std::invoke_result_t<F, ValueType const&>;
        using maybe_result_t = Maybe<Unwrap_t<result_t>>;
        if(*this) {
            return Maybe<result_t>{f(*(*this))}.Flatten();
        } else {
            return maybe_result_t{};
        }
    }

    template<typename F, typename V, typename U = std::invoke_result_t<F, ValueType const&>, std::enable_if_t<std::is_convertible_v<V, U>, bool> = true>
    constexpr auto MapOr(V&& defaultValue, F&& f) const -> U {
        if(*this) {
            return f(*(*this));
        } else {
            return std::forward<V>(defaultValue);
        }
    }

    template<typename PRED>
    constexpr auto Filter(PRED&& pred) const -> Maybe {
        if(*this && pred(*(*this))) {
            return *this;
        } else {
            return std::nullopt;
        }
    }

    friend constexpr auto operator==(Maybe const& lhs, T const& rhs) -> bool {
        return lhs ? *lhs == rhs : false;
    }

    friend constexpr auto operator==(T const& lhs, Maybe const& rhs) -> bool {
        return rhs ? *rhs == lhs : false;
    }

    friend constexpr auto operator!=(Maybe const& lhs, T const& rhs) -> bool {
        return !operator==(lhs, rhs);
    }

    friend constexpr auto operator!=(T const& lhs, Maybe const& rhs) -> bool {
        return !operator==(lhs, rhs);
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
