//
// Created by Darwin Yuan on 2021/8/1.
//

#ifndef PLACEMENT_C5A827A5E70E4D0396C1AC85D28DFC41
#define PLACEMENT_C5A827A5E70E4D0396C1AC85D28DFC41

template<typename T>
struct Ref_Placement {
    constexpr Ref_Placement() : pointer{0} {}
    constexpr explicit Ref_Placement(Ref_Placement const& rhs) : pointer{rhs.pointer} {}
    constexpr explicit Ref_Placement(T& v) : ref{v} {}

    explicit constexpr operator bool() const {
        return pointer != 0;
    }

    auto Emplace(T& v) -> T* {
        new (&ref) RefHolder{v};
        return &v;
    }

    auto operator=(Ref_Placement const& rhs) -> Ref_Placement& {
        Emplace(rhs.ref.ref);
        return *this;
    }

    auto Destroy() -> void {}

    auto Replace(T& v) -> T* {
        return Emplace(v);
    }

    constexpr auto GetObj() const -> T* {
        return &GetRef();
    }

    auto GetObj() -> T* {
        return &GetRef();
    }

    constexpr auto GetRef() const -> T& {
        return ref.ref;
    }

    auto GetRef() -> T& {
        return ref.ref;
    }

    auto operator->() -> T* {
        return GetObj();
    }

    constexpr auto operator->() const -> T* {
        return GetObj();
    }

    constexpr auto operator*() const -> T& {
        return GetRef();
    }

    auto operator*() -> T& {
        return GetRef();
    }

private:
    struct RefHolder {
        T& ref;
    };

    union {
        RefHolder ref;
        T* pointer;
    };
};

#endif //PLACEMENT_C5A827A5E70E4D0396C1AC85D28DFC41
