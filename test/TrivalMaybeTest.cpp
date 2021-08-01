//
// Created by Darwin Yuan on 2021/8/1.
//
#include <l0-infra/maybe/Maybe.h>
#include <catch.hpp>

SCENARIO("Default Maybe Test") {
    Maybe<int> maybe;
    static_assert(std::is_trivially_destructible_v<Maybe<int>>);
    static_assert(!std::is_trivially_copyable_v<Maybe<int>>);
    REQUIRE(!maybe.Present());
    REQUIRE(maybe == nothing);
}

SCENARIO("Nothing Test") {
    Maybe<int> maybe = nothing;
    REQUIRE(!maybe.Present());
    REQUIRE(maybe == nothing);
}

SCENARIO("cons by Nothing Test") {
    Maybe<int> maybe{nothing};
    REQUIRE(!maybe.Present());
    REQUIRE(maybe == nothing);
}

SCENARIO("cons by value Maybe Test") {
    Maybe<int> maybe{10};
    REQUIRE(maybe.Present());
    REQUIRE(maybe != nothing);
    REQUIRE(*maybe == 10);
}

SCENARIO("cons by another Maybe Test") {
    Maybe<int> maybe1{10};
    Maybe<int> maybe{maybe1};
    REQUIRE(maybe.Present());
    REQUIRE(maybe != nothing);
    REQUIRE(*maybe == 10);
}

SCENARIO("copy assignment by another Maybe Test") {
    Maybe<int> maybe1;
    Maybe<int> maybe{20};
    maybe = maybe1;
    REQUIRE(!maybe.Present());
    REQUIRE(maybe == nothing);
}
