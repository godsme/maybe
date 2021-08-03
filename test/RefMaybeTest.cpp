//
// Created by Darwin Yuan on 2021/8/2.
//

#include <l0-infra/option/Option.h>
#include <catch.hpp>

namespace {
    int constructed = 0;
    struct Foo {
        Foo(int a, int b) : a{a}, b{b} {
            constructed++;
        }

        Foo(Foo&& rhs) : a{rhs.a}, b{rhs.b} {
            constructed++;
        }

        ~Foo() {
            constructed--;
        }

        int a;
        int b;

        auto operator==(Foo const& rhs) const -> bool {
            return a == rhs.a && b == rhs.b;
        }
    };

    static_assert(!std::is_copy_constructible_v<Foo>);
}

SCENARIO("Ref Option Test") {
    Option<Foo&> maybe;
    static_assert(sizeof(Option<Foo&>) == sizeof(void*));
    static_assert(std::is_trivially_destructible_v<Option<Foo&>>);
    static_assert(!std::is_trivially_copyable_v<Option<Foo&>>);
    REQUIRE(!maybe.Present());
    REQUIRE(maybe == std::nullopt);
    REQUIRE(maybe == Option<Foo&>{});
}

SCENARIO("Ref Nothing Test") {
    Option<Foo&> maybe = std::nullopt;
    REQUIRE(!maybe.Present());
    REQUIRE(maybe == std::nullopt);
}

SCENARIO("Ref cons by Nothing Test") {
    Option<Foo&> maybe{std::nullopt};
    REQUIRE(!maybe.Present());
    REQUIRE(maybe == std::nullopt);
}

SCENARIO("Ref cons by value Option Test") {
    Foo a{10, 20};
    Option<Foo&> maybe{a};
    REQUIRE(maybe.Present());
    REQUIRE(maybe != std::nullopt);
    REQUIRE(*maybe == Foo{10, 20});
}

SCENARIO("Ref Foo cons by another Option Test") {
    constructed = 0;
    {
        Foo a{10, 20};
        REQUIRE(constructed == 1);
        Option<Foo&> maybe1{a};
        Option<Foo&> maybe{std::move(maybe1)};

        REQUIRE(constructed == 1);

        REQUIRE(maybe.Present());
        REQUIRE(maybe != std::nullopt);
        REQUIRE(*maybe == Foo{10, 20});
        REQUIRE(maybe == maybe1);

        Foo defaultValue{20, 30};
        Foo& result = maybe.ValueOr(defaultValue);
        REQUIRE(result == Foo{10, 20});
    }

    REQUIRE(constructed == 0);
}

SCENARIO("Ref Foo copy assignment by another Option Test") {
    {
        Option<Foo&> maybe1;
        REQUIRE(constructed == 0);

        Foo a{10, 20};
        REQUIRE(constructed == 1);

        Option<Foo&> maybe{a};
        REQUIRE(maybe != maybe1);
        REQUIRE(constructed == 1);

        maybe = std::move(maybe1);
        REQUIRE(constructed == 1);

        REQUIRE(!maybe.Present());
        REQUIRE(maybe == std::nullopt);

        REQUIRE(maybe == maybe1);
    }

    REQUIRE(constructed == 0);
}