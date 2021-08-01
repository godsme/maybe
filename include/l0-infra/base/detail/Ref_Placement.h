//
// Created by Darwin Yuan on 2021/8/1.
//

#ifndef MAYBE_C5A827A5E70E4D0396C1AC85D28DFC41
#define MAYBE_C5A827A5E70E4D0396C1AC85D28DFC41

template<typename T>
struct Ref_Placement {
    Ref_Placement() : pointer{nullptr} {}
    ~Ref_Placement() = default;

    auto Emplace(T& v) -> T* {
        new (&ref) RefHolder{v};
        return &v;
    }

    auto Destroy() -> void {}

    auto Replace(T& v) -> T* {
        return Emplace(v);
    }

    auto GetObj() const -> T const* {
        return &GetRef();
    }

    auto GetObj() -> T* {
        return &GetRef();
    }

    auto GetRef() const -> T const& {
        return ref.ref;
    }

    auto GetRef() -> T& {
        return ref.ref;
    }

    auto operator->() -> T* {
        return GetObj();
    }

    auto operator->() const -> T const* {
        return GetObj();
    }

    auto operator*() const -> T const& {
        return GetRef();
    }

    auto operator*() -> T& {
        return GetRef();
    }

private:
    struct RefHolder{
        T& ref;
    };

    union {
        RefHolder ref;
        T* pointer;
    };
};

#endif //MAYBE_C5A827A5E70E4D0396C1AC85D28DFC41
