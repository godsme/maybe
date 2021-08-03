//
// Created by Darwin Yuan on 2021/6/21.
//

#ifndef OBJECT_ARRAY_STORAGETRAIT_H
#define OBJECT_ARRAY_STORAGETRAIT_H

#include <type_traits>

namespace detail {
    struct UnionDummy {
        constexpr UnionDummy() = default;
    };

    template<typename T,
            bool = std::is_trivially_destructible_v<T>>
    struct StorageTrait {
        union Type {
            template<typename ... ARGS>
            constexpr Type(ARGS&& ... args) : object(std::forward<ARGS>(args)...) {}
            constexpr Type() : dummy() {}
            T object;
            UnionDummy dummy;
        };
    };

    template<typename T>
    struct StorageTrait<T, false> {
        union Type {
            template<typename ... ARGS>
            constexpr Type(ARGS&& ... args) : object(std::forward<ARGS>(args)...) {}
            constexpr Type() : dummy() {}
            Type(Type const&) = delete;
            ~Type() {}
            T object;
            UnionDummy dummy;
        };
    };
}

#endif //OBJECT_ARRAY_STORAGETRAIT_H
