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
        auto Destroy() -> void {
            if constexpr(!std::is_trivially_destructible_v<T>) {
                if(present) {
                    value.Destroy();
                    present = false;
                }
            }
        }

        template<typename ... ARGS>
        auto Construct(ARGS&& ... args) -> decltype(auto) {
            present = true;
            return value.Emplace(std::forward<ARGS>(args)...);
        }

        template<typename THAT>
        auto ConstructFrom(THAT&& that) -> void {
            if(that.Present()) {
                Construct(std::forward<THAT>(that).Value());
            }
        }

    public:
        constexpr Val_Option() noexcept {}
        constexpr Val_Option(std::nullopt_t) noexcept {}
        constexpr Val_Option(Val_Option const& rhs) {
            ConstructFrom(rhs);
        }

        template<typename U, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
        constexpr Val_Option(Val_Option<U> const& rhs) {
            ConstructFrom(rhs);
        }

        template<typename U, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
        constexpr Val_Option(Val_Option<U>&& rhs) {
            ConstructFrom(std::move(rhs));
        }

        template<typename U, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
        constexpr Val_Option(Val_Option<U>& rhs) {
            ConstructFrom(rhs);
        }

        template<typename ... ARGS>
        constexpr Val_Option(ARGS&& ... args)
            : present{true}
            , value{std::in_place, std::forward<ARGS>(args)...} {
        }

        template<typename U, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
        auto operator=(Val_Option<U> const& rhs) -> Val_Option& {
            Destroy();
            ConstructFrom(rhs);
            return *this;
        }

        template<typename U, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
        auto operator=(Val_Option<U>&& rhs) -> Val_Option& {
            Destroy();
            ConstructFrom(std::move(rhs));
            return *this;
        }

        template<typename ... ARGS>
        auto Emplace(ARGS&& ... args) -> auto {
            Destroy();
            return Construct(std::forward<ARGS>(args)...);
        }

        constexpr auto Present() const -> bool {
            return present;
        }

        constexpr auto Value() const& -> decltype(auto) {
            return value.GetRef();
        }

        constexpr auto Value() const&& -> decltype(auto) {
            return std::move(value).GetRef();
        }

        auto Value() & -> decltype(auto) {
            return value.GetRef();
        }

        auto Value() && -> decltype(auto) {
            return std::move(value).GetRef();
        }

        constexpr auto ValueOr(T const& defaultValue) const& -> decltype(auto) {
            return present ? Value() : defaultValue;
        }

        constexpr auto ValueOr(T& defaultValue) const& -> decltype(auto) {
            return present ? Value() : defaultValue;
        }

        constexpr auto ValueOr(T & defaultValue) & -> decltype(auto) {
            return present ? Value() : defaultValue;
        }

        constexpr auto ValueOr(T const& defaultValue) & -> decltype(auto) {
            return present ? Value() : defaultValue;
        }

        constexpr auto ValueOr(T&& defaultValue) const& -> T {
            static_assert(std::is_copy_assignable_v<T>);
            return present ? Value() : std::move(defaultValue);
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

    protected:
        Val_Placement<T> value;
        bool present{false};
    };
}

namespace detail {
    template<typename T, bool = std::is_trivially_destructible_v<T>>
    struct Val_Option : base::Val_Option<T> {
        using Parent = base::Val_Option<T>;
        using Parent::Parent;
    };

    template<typename T>
    struct Val_Option<T, false> : base::Val_Option<T> {
        using Parent = base::Val_Option<T>;
        using Parent::Parent;

        Val_Option(Val_Option const&) = default;
        Val_Option(Val_Option&&) = default;

        auto operator=(Val_Option const&) -> Val_Option& = default;
        auto operator=(Val_Option&&) -> Val_Option& = default;

        ~Val_Option() {
            Parent::Destroy();
        }
    };
}

#endif //MAYBE_E975E67EA14440689D8586A383B22E5C
