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
    struct Maybe {
    protected:
        auto destroy() -> void {
            if(present) value.Destroy();
        }

    public:
        constexpr Maybe() : present{false} {}
        constexpr Maybe(Nothing) : Maybe() {}
        constexpr Maybe(Maybe<T> const& rhs) : present{rhs.present} {
            if(present) {
                value = rhs.value;
            }
        }

        constexpr Maybe(Maybe<T>&& rhs) : present{rhs.present} {
            if(present) {
                value = std::move(rhs.value);
            }
        }

        constexpr Maybe(Maybe<T>& rhs) : Maybe{const_cast<Maybe<T> const&>(rhs)} {}

        template<typename ARG, typename ... ARGS>
        constexpr Maybe(ARG&& arg, ARGS&& ... args) : present{true} {
            value.Emplace(std::forward<ARG>(arg), std::forward<ARGS>(args)...);
        }

        auto operator=(Maybe const& rhs) -> Maybe& {
            if constexpr(!std::is_trivially_destructible_v<T>) {
                destroy();
            }

            present = rhs.present;
            if(present) value = rhs.value;

            return *this;
        }

        auto operator=(Maybe&& rhs) -> Maybe& {
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

        constexpr auto operator*() const -> decltype(auto) {
            return value.GetRef();
        }

        template<typename C>
        friend constexpr auto operator==(Maybe<C> const& lhs, Nothing) -> bool {
            return !lhs.present;
        }

        template<typename C>
        friend constexpr auto operator==(Nothing, Maybe<C> const& rhs) -> bool {
            return !rhs.present;
        }

        template<typename C>
        friend constexpr auto operator!=(Maybe<C> const& lhs, Nothing) -> bool {
            return lhs.present;
        }

        template<typename C>
        friend constexpr auto operator!=(Nothing, Maybe<C> const& rhs) -> bool {
            return rhs.present;
        }

    protected:
        Placement<T> value;
        bool present;
    };
}

template<typename T
        , bool = std::is_trivially_destructible_v<T>
        , bool = std::is_copy_constructible_v<T>
        , bool = std::is_move_constructible_v<T>>
struct Maybe : detail::Maybe<T> {
    using Parent = detail::Maybe<T>;
    using Parent::Parent;

    Maybe(Maybe const&) = default;
    Maybe(Maybe&&) = default;

    auto operator=(Maybe const&) -> Maybe& = default;
    auto operator=(Maybe&&) -> Maybe& = default;
};

template<typename T>
struct Maybe<T, true, true, false> : detail::Maybe<T> {
    using Parent = detail::Maybe<T>;
    using Parent::Parent;

    Maybe(Maybe const&) = default;
    Maybe(Maybe&&) = delete;

    auto operator=(Maybe const&) -> Maybe& = default;
    auto operator=(Maybe&&) -> Maybe& = delete;
};

template<typename T>
struct Maybe<T, true, false, false> : detail::Maybe<T> {
    using Parent = detail::Maybe<T>;
    using Parent::Parent;

    Maybe(Maybe const&) = delete;
    Maybe(Maybe&&) = delete;

    auto operator=(Maybe const&) -> Maybe& = delete;
    auto operator=(Maybe&&) -> Maybe& = delete;
};

template<typename T>
struct Maybe<T, false, true, true> : detail::Maybe<T> {
    using Parent = detail::Maybe<T>;
    using Parent::Parent;

    Maybe(Maybe const&) = default;
    Maybe(Maybe&&) = default;

    auto operator=(Maybe const&) -> Maybe& = default;
    auto operator=(Maybe&&) -> Maybe& = default;

    ~Maybe() {
        Parent::destroy();
    }
};

template<typename T>
struct Maybe<T, false, true, false> : detail::Maybe<T> {
    using Parent = detail::Maybe<T>;
    using Parent::Parent;

    Maybe(Maybe const&) = default;
    Maybe(Maybe&&) = delete;

    auto operator=(Maybe const&) -> Maybe& = default;
    auto operator=(Maybe&&) -> Maybe& = delete;

    ~Maybe() {
        Parent::destroy();
    }
};

template<typename T>
struct Maybe<T, false, false, false> : detail::Maybe<T> {
    using Parent = detail::Maybe<T>;
    using Parent::Parent;

    Maybe(Maybe const&) = delete;
    Maybe(Maybe&&) = delete;

    auto operator=(Maybe const&) -> Maybe& = delete;
    auto operator=(Maybe&&) -> Maybe& = delete;

    ~Maybe() {
        Parent::destroy();
    }
};


#endif //MAYBE_5B22D47F4DD54C17BAE28C385C86B3FC
