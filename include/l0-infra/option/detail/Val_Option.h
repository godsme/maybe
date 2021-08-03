//
// Created by Darwin Yuan on 2021/8/2.
//

#ifndef MAYBE_E975E67EA14440689D8586A383B22E5C
#define MAYBE_E975E67EA14440689D8586A383B22E5C

#include <l0-infra/base/detail/Val_Placement.h>
#include <optional>

namespace detail::base {
    template<typename T>
    struct Val_Option {
        static_assert(!std::is_reference_v<T>);
    protected:
        auto destroy() -> void {
            if(present) value.Destroy();
        }

    public:
        constexpr Val_Option() : present{false} {}
        constexpr Val_Option(std::nullopt_t) : Val_Option() {}
        constexpr Val_Option(Val_Option<T> const& rhs) : present{rhs.present} {
            if(present) {
                value = rhs.value;
            }
        }

        constexpr Val_Option(Val_Option<T>&& rhs) : present{rhs.present} {
            if(present) {
                value = std::move(rhs.value);
            }
        }

        constexpr Val_Option(Val_Option<T>& rhs) : Val_Option{const_cast<Val_Option<T> const&>(rhs)} {}

        template<typename ARG, typename ... ARGS>
        constexpr Val_Option(ARG&& arg, ARGS&& ... args)
            : present{true}
            , value{std::forward<ARG>(arg), std::forward<ARGS>(args)...} {
        }

        auto operator=(Val_Option const& rhs) -> Val_Option& {
            if constexpr(!std::is_trivially_destructible_v<T>) {
                destroy();
            }

            present = rhs.present;
            if(present) value = rhs.value;

            return *this;
        }

        auto operator=(Val_Option&& rhs) -> Val_Option& {
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

        constexpr auto ValueOr(T const& defaultValue) const& -> decltype(auto) {
            return present ?  Value() : defaultValue;
        }

        constexpr auto ValueOr(T& defaultValue) const& -> decltype(auto) {
            return present ?  Value() : defaultValue;
        }

        constexpr auto ValueOr(T & defaultValue) & -> decltype(auto) {
            return present ?  Value() : defaultValue;
        }

        constexpr auto ValueOr(T const& defaultValue) & -> decltype(auto) {
            return present ?  Value() : defaultValue;
        }

        constexpr auto ValueOr(T&& defaultValue) const& -> T {
            static_assert(std::is_copy_assignable_v<T>);
            return present ?  Value() : std::move(defaultValue);
        }

        constexpr auto ValueOr(T&& defaultValue) & -> T {
            static_assert(std::is_copy_assignable_v<T>);
            if(present) {
                return Value();
            } else {
                return std::move(defaultValue);
            }
        }

        constexpr auto ValueOr(T&& defaultValue) && -> T {
            static_assert(std::is_move_assignable_v<T>);
            return present ?  std::move(Value()) : std::move(defaultValue);
        }

        template<typename ... ARGS>
        auto Emplace(ARGS&& ... args) -> auto {
            if constexpr(!std::is_trivially_destructible_v<T>) {
                destroy();
            }
            present = true;
            return value.Emplace(std::forward<ARGS...>(args)...);
        }

    protected:
        Val_Placement<T> value;
        bool present;
    };
}

namespace detail {
    template<typename T, bool = std::is_trivially_destructible_v<T>>
    struct Val_Maybe : base::Val_Option<T> {
        using Parent = base::Val_Option<T>;
        using Parent::Parent;
    };

    template<typename T>
    struct Val_Maybe<T, false> : base::Val_Option<T> {
        using Parent = base::Val_Option<T>;
        using Parent::Parent;

        Val_Maybe(Val_Maybe const&) = default;
        Val_Maybe(Val_Maybe&&) = default;

        auto operator=(Val_Maybe const&) -> Val_Maybe& = default;
        auto operator=(Val_Maybe&&) -> Val_Maybe& = default;

        ~Val_Maybe() {
            Parent::destroy();
        }
    };
}

#endif //MAYBE_E975E67EA14440689D8586A383B22E5C
