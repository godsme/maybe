//
// Created by Darwin Yuan on 2021/6/21.
//

#ifndef PLACEMENT_DECEC01733DA4EE6825B4A610EBE5D47
#define PLACEMENT_DECEC01733DA4EE6825B4A610EBE5D47

#include <l0-infra/base/detail/Storage.h>
#include <utility>
#include <new>

template<typename T>
struct Val_Placement : private detail::Storage<T> {
    using Parent = detail::Storage<T>;

    using Parent::Parent;

    using Parent::Destroy;

    template<typename ... ARGS>
    auto Emplace(ARGS&& ... args) & -> T& {
        if constexpr(!(sizeof...(ARGS) == 0 && std::is_trivially_default_constructible_v<T>)) {
            if constexpr (std::is_aggregate_v<T>) {
                return *(new (std::addressof(Parent::object)) T{ std::forward<ARGS>(args)... });
            } else {
                return *(new (std::addressof(Parent::object)) T( std::forward<ARGS>(args)... ));
            }
        } else {
            return GetRef();
        }
    }

    template<typename ... ARGS>
    auto Emplace(ARGS&& ... args) && -> T&& {
        return std::move(Emplace(std::forward<ARGS>(args)...));
    }

    template<typename ... ARGS>
    auto Replace(ARGS&& ... args) & -> T& {
        static_assert(!std::is_const_v<T>);
        Destroy();
        return Emplace(std::forward<ARGS>(args)...);
    }

    template<typename ... ARGS>
    auto Replace(ARGS&& ... args) && -> T&& {
        return std::move(Replace(std::forward<ARGS>(args)...));
    }

    constexpr auto GetRef() const& -> T const& {
        return Parent::object;
    }

    auto GetRef() & -> T& {
        return Parent::object;
    }

    constexpr auto GetRef() const&& -> T const&& {
        return std::move(Parent::object);
    }

    auto GetRef() && -> T&& {
        return std::move(Parent::object);
    }

    constexpr auto operator*() const& -> T const& {
        return GetRef();
    }

    constexpr auto operator*() const&& -> T const&& {
        return GetRef();
    }

    auto operator*() & -> T& {
        return GetRef();
    }

    auto operator*() && -> T&& {
        return GetRef();
    }

    constexpr auto operator->() const -> T const* {
        return &GetRef();
    }

    auto operator->() -> T* {
        return &GetRef();
    }
};

#endif //PLACEMENT_DECEC01733DA4EE6825B4A610EBE5D47
