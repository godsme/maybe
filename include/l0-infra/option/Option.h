//
// Created by Darwin Yuan on 2021/8/1.
//

#ifndef MAYBE_8A0D32BE25FC4E61A29F2CA9376105E1
#define MAYBE_8A0D32BE25FC4E61A29F2CA9376105E1

#include <l0-infra/option/detail/Ref_Option.h>
#include <l0-infra/option/detail/Val_Option.h>
#include <l0-infra/option/detail/Int_Option.h>

namespace detail {
    template<typename T, typename = void>
    struct IsIntType : std::false_type {};

    template<typename T>
    struct IsIntType<T, std::enable_if_t<T::sEcReAt_InT_tAg>> {
        using IntType = typename T::IntType;
        constexpr static bool value = T::LOWER_BOUNDARY > std::numeric_limits<IntType>::min() || T::UPPER_BOUNDARY < std::numeric_limits<IntType>::max();
    };

    template<typename T>
    using ValueOption = std::conditional_t<IsIntType<T>::value, Int_Option<T>, Val_Option<T>>;

    template<typename T>
    using Option = std::conditional_t<std::is_reference_v<T>, Ref_Option<std::remove_reference_t<T>>, ValueOption<T>>;
}

template<typename T>
struct Option : detail::Option<T> {
private:
    using Parent = detail::Option<T>;
    using ValueType = std::remove_reference_t<T>;

    template<typename C>
    struct Unwrap {
        using Type = C;
    };

    template<typename C>
    struct Unwrap<Option<C>> {
        using Type = typename Unwrap<C>::Type;
    };

    template<typename C>
    using Unwrap_t = typename Unwrap<C>::Type;


    template<typename>
    struct IsOptionType : std::false_type {};

    template<typename C>
    struct IsOptionType<Option<C>> : std::true_type {};

public:
    using Parent::Parent;

    constexpr auto operator*() const -> decltype(auto) {
        return Parent::Value();
    }

    auto operator*() -> decltype(auto) {
        return Parent::Value();
    }

    auto operator->() -> auto* {
        return &Parent::Value();
    }

    constexpr explicit operator bool() const {
        return Parent::Present();
    }

    constexpr auto Flatten() const -> Option<Unwrap_t<T>>  {
        if constexpr(IsOptionType<T>::value) {
            if(!(*this)) return {};
            return (*(*this)).Flatten();
        } else {
            return *this;
        }
    }

    template<typename F>
    constexpr auto Map(F&& f) const -> auto {
        using result_t = std::invoke_result_t<F, ValueType const&>;
        using maybe_result_t = Option<Unwrap_t<result_t>>;
        if(*this) {
            return Option<result_t>{f(*(*this))}.Flatten();
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
    constexpr auto Filter(PRED&& pred) const -> Option {
        if(*this && pred(*(*this))) {
            return *this;
        } else {
            return std::nullopt;
        }
    }

    friend constexpr auto operator==(Option const& lhs, T const& rhs) -> bool {
        return lhs ? *lhs == rhs : false;
    }

    friend constexpr auto operator==(T const& lhs, Option const& rhs) -> bool {
        return rhs ? *rhs == lhs : false;
    }

    friend constexpr auto operator!=(Option const& lhs, T const& rhs) -> bool {
        return !operator==(lhs, rhs);
    }

    friend constexpr auto operator!=(T const& lhs, Option const& rhs) -> bool {
        return !operator==(lhs, rhs);
    }

    friend constexpr auto operator==(Option const& lhs, std::nullopt_t) -> bool {
        return !lhs;
    }

    friend constexpr auto operator==(Option const& lhs, Option const& rhs) -> bool {
        if(lhs) {
            return rhs ? *lhs == *rhs : false;
        } else {
            return !rhs;
        }
    }

    friend constexpr auto operator!=(Option const& lhs, Option const& rhs) -> bool {
        return !operator==(lhs, rhs);
    }

    friend constexpr auto operator==(std::nullopt_t, Option const& rhs) -> bool {
        return !rhs;
    }

    friend constexpr auto operator!=(Option const& lhs, std::nullopt_t) -> bool {
        return (bool)lhs;
    }

    friend constexpr auto operator!=(std::nullopt_t, Option const& rhs) -> bool {
        return (bool)rhs;
    }
};

template<typename T>
Option(T&&) -> Option<T>;

#endif //MAYBE_8A0D32BE25FC4E61A29F2CA9376105E1
