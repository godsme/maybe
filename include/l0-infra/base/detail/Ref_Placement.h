//
// Created by Darwin Yuan on 2021/8/1.
//

#ifndef PLACEMENT_C5A827A5E70E4D0396C1AC85D28DFC41
#define PLACEMENT_C5A827A5E70E4D0396C1AC85D28DFC41

#include <type_traits>

template<typename T>
struct Ref_Placement {
    constexpr Ref_Placement() : pointer{nullptr} {}

    template<typename U, typename = std::enable_if_t<std::is_convertible_v<U*, T*>>>
    constexpr explicit Ref_Placement(Ref_Placement<U> const& rhs)
        : pointer{static_cast<T*>(rhs.pointer)} {}

    template<typename U, typename = std::enable_if_t<std::is_convertible_v<U*, T*>>>
    constexpr explicit Ref_Placement(U& v) : pointer{std::addressof(static_cast<T&>(v))} {}

    explicit constexpr operator bool() const {
        return pointer != 0;
    }

    auto Emplace(T& v) -> T& {
        pointer = std::addressof(v);
        return *pointer;
    }

    template<typename U, typename = std::enable_if_t<std::is_convertible_v<U*, T*>>>
    auto operator=(Ref_Placement<U> const& rhs) -> Ref_Placement& {
        pointer = static_cast<T*>(rhs.pointer);
        return *this;
    }

    auto Destroy() -> void {}

    auto Replace(T& v) -> T& {
        return Emplace(v);
    }

    constexpr auto GetRef() const -> T const& {
        return *pointer;
    }

    auto GetRef() -> T& {
        return *pointer;
    }

    auto operator->() -> T* {
        return pointer;
    }

    constexpr auto operator->() const -> T const* {
        return pointer;
    }

    constexpr auto operator*() const -> T& {
        return GetRef();
    }

    auto operator*() -> T& {
        return GetRef();
    }

private:
    template<typename U>
    friend struct Ref_Placement;

private:
    T* pointer;
};

#endif //PLACEMENT_C5A827A5E70E4D0396C1AC85D28DFC41
