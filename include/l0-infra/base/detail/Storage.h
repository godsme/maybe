//
// Created by Darwin Yuan on 2021/6/21.
//

#ifndef OBJECT_ARRAY_STORAGETRAIT_H
#define OBJECT_ARRAY_STORAGETRAIT_H

#include <type_traits>
#include <utility>

namespace detail {
    template<typename T, bool = std::is_trivially_destructible_v<T>>
    struct Storage;

    template<typename T>
    struct Storage<T, true> {
        constexpr Storage() : __null_type__{} {}

        template<typename ... ARGS>
        constexpr Storage(std::in_place_t, ARGS&& ... args)
            : object(std::forward<ARGS>(args)...)
        {}

        auto Destroy() {}

        union {
            T object;
            char __null_type__;
        };
    };

    template<typename T>
    struct Storage<T, false> {
        constexpr Storage() : __null_type__{} {}

        template<typename ... ARGS>
        constexpr Storage(std::in_place_t, ARGS&& ... args)
            : object(std::forward<ARGS>(args)...)
        {}

        ~Storage() {}

        auto Destroy() {
            object.~T();
        }

        union {
            T object;
            char __null_type__;
        };
    };
}

#endif //OBJECT_ARRAY_STORAGETRAIT_H
