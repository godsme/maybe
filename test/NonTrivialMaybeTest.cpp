//
// Created by Darwin Yuan on 2021/8/1.
//
#include <l0-infra/maybe/Maybe.h>
#include <catch.hpp>

namespace {
    int constructed = 0;
    struct Foo {
        Foo(int a, int b) : a{a}, b{b} {
            constructed++;
        }

        Foo(Foo const& rhs) : a{rhs.a}, b{rhs.b} {
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
}

SCENARIO("Foo Default Maybe Test") {
    Maybe<Foo> maybe;
    static_assert(!std::is_trivially_destructible_v<Maybe<Foo>>);
    static_assert(!std::is_trivially_copyable_v<Maybe<Foo>>);
    REQUIRE(!maybe.Present());
    REQUIRE(maybe == nothing);
}

SCENARIO("Foo Nothing Test") {
    Maybe<Foo> maybe = nothing;
    REQUIRE(!maybe.Present());
    REQUIRE(maybe == nothing);
}

SCENARIO("Foo cons by Nothing Test") {
    {
        Maybe<Foo> maybe{nothing};
        REQUIRE(!maybe.Present());
        REQUIRE(maybe == nothing);
    }

    REQUIRE(constructed == 0);
}

SCENARIO("Foo cons by value Maybe Test") {
    {
        Maybe<Foo> maybe{10, 20};
        REQUIRE(maybe.Present());
        REQUIRE(maybe != nothing);
        REQUIRE(*maybe == Foo{10, 20});
    }

    REQUIRE(constructed == 0);
}

SCENARIO("Foo cons by another Maybe Test") {
    REQUIRE(constructed == 0);
    {
        Maybe<Foo> maybe1{10, 20};
        Maybe<Foo> maybe{maybe1};

        REQUIRE(constructed == 2);

        REQUIRE(maybe.Present());
        REQUIRE(maybe != nothing);
        REQUIRE(*maybe == Foo{10, 20});
        REQUIRE(maybe == maybe1);
    }

    REQUIRE(constructed == 0);
}

SCENARIO("Foo copy assignment by another Maybe Test") {
    {
        Maybe<Foo> maybe1;
        REQUIRE(constructed == 0);
        Maybe<Foo> maybe{10, 20};
        REQUIRE(maybe != maybe1);
        REQUIRE(constructed == 1);
        maybe = maybe1;
        REQUIRE(constructed == 0);
        REQUIRE(!maybe.Present());
        REQUIRE(maybe == nothing);

        REQUIRE(maybe == maybe1);
    }

    REQUIRE(constructed == 0);
}
