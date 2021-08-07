//
// Created by Darwin Yuan on 2021/8/1.
//

#ifndef PLACEMENT_C5A827A5E70E4D0396C1AC85D28DFC41
#define PLACEMENT_C5A827A5E70E4D0396C1AC85D28DFC41

template<typename T>
struct Ref_Placement {
    constexpr Ref_Placement() : pointer{nullptr} {}
    constexpr explicit Ref_Placement(Ref_Placement const& rhs) : pointer{rhs.pointer} {}
    constexpr explicit Ref_Placement(T& v) : pointer{&v} {}

    explicit constexpr operator bool() const {
        return pointer != 0;
    }

    auto Emplace(T& v) -> T& {
        pointer = &v;
        return *pointer;
    }

    auto operator=(Ref_Placement const& rhs) -> Ref_Placement& {
        pointer = rhs.pointer;
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
    T* pointer;
};

#endif //PLACEMENT_C5A827A5E70E4D0396C1AC85D28DFC41
