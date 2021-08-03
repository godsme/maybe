//
// Created by Darwin Yuan on 2021/6/21.
//

#ifndef PLACEMENT_DECEC01733DA4EE6825B4A610EBE5D47
#define PLACEMENT_DECEC01733DA4EE6825B4A610EBE5D47

#include <l0-infra/base/detail/StorageTrait.h>
#include <utility>
#include <new>

template<typename T>
struct Val_Placement {
    constexpr Val_Placement() = default;
    ~Val_Placement() = default;
    template<typename ... ARGS>
    constexpr Val_Placement(ARGS&& ... args) : storage{std::forward<ARGS>(args)...} {}

    template<typename ... ARGS>
    auto Emplace(ARGS&& ... args) -> T* {
        if constexpr(!(sizeof...(ARGS) == 0 && std::is_trivially_default_constructible_v<T>)) {
            if constexpr (std::is_aggregate_v<T>) {
                return new (&storage) T{ std::forward<ARGS>(args)... };
            } else {
                return new (&storage) T( std::forward<ARGS>(args)... );
            }
        } else {
            return GetObj();
        }
    }

    auto operator=(Val_Placement const& rhs) -> Val_Placement& {
        Emplace(rhs.GetRef());
        return *this;
    }

    auto operator=(Val_Placement&& rhs) -> Val_Placement& {
        Emplace(std::move(rhs.GetRef()));
        return *this;
    }

    auto Destroy() -> void {
        if constexpr (!std::is_trivially_destructible_v<T>) {
            storage.object.~T();
        }
    }

    template<typename ... ARGS>
    auto Replace(ARGS&& ... args) -> T* {
        static_assert(!std::is_const_v<T>);
        Destroy();
        return Emplace(std::forward<ARGS>(args)...);
    }

    auto GetObj() const -> T const* {
        return &GetRef();
    }

    auto GetObj() -> T* {
        return &GetRef();
    }

    constexpr auto GetRef() const -> T const& {
        return storage.object;
    }

    auto GetRef() -> T& {
        return storage.object;
    }

    auto operator->() -> T* {
        return GetObj();
    }

    auto operator->() const -> T const* {
        return GetObj();
    }

    constexpr auto operator*() const -> T const& {
        return storage.object;
    }

    auto operator*() -> T& {
        return storage.object;
    }

private:
    typename detail::StorageTrait<T>::Type storage;
};

#endif //PLACEMENT_DECEC01733DA4EE6825B4A610EBE5D47
