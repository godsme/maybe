//
// Created by Darwin Yuan on 2021/8/1.
//
#include <l0-infra/maybe/Maybe.h>
#include <catch.hpp>
#include <optional>


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

SCENARIO("Foo Default Maybe Test") {
    Maybe<Foo> maybe;
    static_assert(!std::is_trivially_destructible_v<Maybe<Foo>>);
    static_assert(!std::is_trivially_copyable_v<Maybe<Foo>>);
    REQUIRE(!maybe.Present());
    REQUIRE(!maybe);
    REQUIRE(maybe == nothing);
    Foo defaultValue{10, 20};
    auto& result = maybe.ValueOr(defaultValue);
    REQUIRE(result == Foo{10, 20});
}

SCENARIO("Foo Nothing Test") {
    Maybe<Foo> maybe = nothing;
    REQUIRE(!maybe.Present());
    REQUIRE(!maybe);
    REQUIRE(maybe == nothing);
}

SCENARIO("Foo cons by Nothing Test") {
    {
        Maybe<Foo> maybe{nothing};
        REQUIRE(!maybe.Present());
        REQUIRE(!maybe);
        REQUIRE(maybe == nothing);
    }

    REQUIRE(constructed == 0);
}

SCENARIO("Foo cons by value Maybe Test") {
    {
        Maybe<Foo> maybe{10, 20};
        REQUIRE(maybe.Present());
        REQUIRE(maybe);
        REQUIRE(maybe != nothing);
        REQUIRE(*maybe == Foo{10, 20});
    }

    REQUIRE(constructed == 0);
}

SCENARIO("Foo cons by another Maybe Test") {
    constructed = 0;
    {
        Maybe<Foo> maybe1{10, 20};
        Maybe<Foo> maybe{std::move(maybe1)};

        REQUIRE(constructed == 2);

        REQUIRE(maybe.Present());
        REQUIRE(maybe != nothing);
        REQUIRE(*maybe == Foo{10, 20});
        REQUIRE(maybe == maybe1);

        Foo defaultValue{20, 30};
        Foo& result = maybe.ValueOr(defaultValue);
        REQUIRE(result == Foo{10, 20});
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

        maybe = std::move(maybe1);
        REQUIRE(constructed == 0);

        REQUIRE(!maybe.Present());
        REQUIRE(maybe == nothing);

        REQUIRE(maybe == maybe1);
    }

    REQUIRE(constructed == 0);
}
