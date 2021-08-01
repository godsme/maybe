//
// Created by Darwin Yuan on 2021/8/1.
//

#ifndef MAYBE_5B22D47F4DD54C17BAE28C385C86B3FC
#define MAYBE_5B22D47F4DD54C17BAE28C385C86B3FC

#include <l0-infra/base/Placement.h>

struct Nothing {};
constexpr Nothing nothing{};

namespace detail {
    template<typename T>
    struct MaybeBase {
        using ValueType = std::remove_reference_t<T>;
    protected:
        auto destroy() -> void {
            if(present) value.Destroy();
        }

    public:
        constexpr MaybeBase() : present{false} {}
        constexpr MaybeBase(Nothing) : MaybeBase() {}
        constexpr MaybeBase(MaybeBase<T> const& rhs) : present{rhs.present} {
            if(present) {
                value = rhs.value;
            }
        }

        constexpr MaybeBase(MaybeBase<T>&& rhs) : present{rhs.present} {
            if(present) {
                value = std::move(rhs.value);
            }
        }

        constexpr MaybeBase(MaybeBase<T>& rhs) : MaybeBase{const_cast<MaybeBase<T> const&>(rhs)} {}

        template<typename ARG, typename ... ARGS>
        constexpr MaybeBase(ARG&& arg, ARGS&& ... args) : present{true} {
            value.Emplace(std::forward<ARG>(arg), std::forward<ARGS>(args)...);
        }

        auto operator=(MaybeBase const& rhs) -> MaybeBase& {
            if constexpr(!std::is_trivially_destructible_v<T>) {
                destroy();
            }

            present = rhs.present;
            if(present) value = rhs.value;

            return *this;
        }

        auto operator=(MaybeBase&& rhs) -> MaybeBase& {
            if constexpr(!std::is_trivially_destructible_v<T>) {
                destroy();
            }

            present = rhs.present;
            if(present) value = std::move(rhs.value);

            return *this;
        }

        constexpr auto Present() const -> bool {
            return present;
        }

        constexpr auto Value() const -> decltype(auto) {
            return value.GetRef();
        }

        auto Value() -> decltype(auto) {
            return value.GetRef();
        }

        constexpr auto ValueOr(ValueType const& defaultValue) const& -> decltype(auto) {
            return present ?  Value() : defaultValue;
        }

        constexpr auto ValueOr(ValueType& defaultValue) const& -> decltype(auto) {
            return present ?  Value() : defaultValue;
        }

        constexpr auto ValueOr(ValueType & defaultValue) & -> decltype(auto) {
            return present ?  Value() : defaultValue;
        }

        constexpr auto ValueOr(ValueType const& defaultValue) & -> decltype(auto) {
            return present ?  Value() : defaultValue;
        }

        constexpr auto ValueOr(ValueType&& defaultValue) const& -> ValueType {
            static_assert(std::is_copy_assignable_v<ValueType>);
            static_assert(!std::is_reference_v<T>);
            return present ?  Value() : std::move(defaultValue);
        }

        constexpr auto ValueOr(ValueType&& defaultValue) & -> ValueType {
            static_assert(std::is_copy_assignable_v<ValueType>);
            static_assert(!std::is_reference_v<T>);
            return present ?  Value() : std::move(defaultValue);
        }

        constexpr auto ValueOr(ValueType&& defaultValue) && -> ValueType {
            static_assert(std::is_move_assignable_v<ValueType>);
            static_assert(!std::is_reference_v<T>);
            return present ?  std::move(Value()) : std::move(defaultValue);
        }

        constexpr auto operator*() const -> decltype(auto) {
            return Value();
        }

        auto operator*() -> decltype(auto) {
            return Value();
        }

        template<typename ... ARGS>
        auto Emplace(ARGS&& ... args) -> auto {
            if constexpr(!std::is_trivially_destructible_v<T>) {
                destroy();
            }
            present = true;
            return value.Emplace(std::forward<ARGS...>(args)...);
        }

        constexpr explicit operator bool() const {
            return present;
        }

    protected:
        Placement<T> value;
        bool present;
    };

    template<typename T, bool = std::is_trivially_destructible_v<T>>
    struct Maybe : MaybeBase<T> {
        using Parent = MaybeBase<T>;
        using Parent::Parent;
    };

    template<typename T>
    struct Maybe<T, false> : MaybeBase<T> {
        using Parent = MaybeBase<T>;
        using Parent::Parent;

        Maybe(Maybe const&) = default;
        Maybe(Maybe&&) = default;

        auto operator=(Maybe const&) -> Maybe& = default;
        auto operator=(Maybe&&) -> Maybe& = default;

        ~Maybe() {
            Parent::destroy();
        }
    };
}

template<typename T>
struct Maybe : detail::Maybe<T> {
    using Parent = detail::Maybe<T>;
    using Parent::Parent;

    using ValueType = typename Parent::ValueType;

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
        if(Parent::present) {
            return Maybe<result_t>{f(Parent::Value())}.Flatten();
        } else {
            return maybe_result_t{};
        }
    }

    friend constexpr auto operator==(Maybe const& lhs, Nothing) -> bool {
        return !lhs.present;
    }

    friend constexpr auto operator==(Maybe const& lhs, Maybe const& rhs) -> bool {
        if(lhs.present) {
            return rhs.present ? *lhs.value == *rhs.value : false;
        } else {
            return !rhs.present;
        }
    }

    friend constexpr auto operator!=(Maybe const& lhs, Maybe const& rhs) -> bool {
        return !operator==(lhs, rhs);
    }

    friend constexpr auto operator==(Nothing, Maybe const& rhs) -> bool {
        return !rhs.present;
    }

    friend constexpr auto operator!=(Maybe const& lhs, Nothing) -> bool {
        return lhs.present;
    }

    friend constexpr auto operator!=(Nothing, Maybe const& rhs) -> bool {
        return rhs.present;
    }
};

template<typename T>
Maybe(T&&) -> Maybe<T>;

#endif //MAYBE_5B22D47F4DD54C17BAE28C385C86B3FC
