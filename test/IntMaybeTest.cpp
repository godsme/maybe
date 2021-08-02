//
// Created by Darwin Yuan on 2021/8/3.
//
#include <l0-infra/maybe/Maybe.h>
#include <l0-infra/base/Int.h>
#include <sstream>
#include <catch.hpp>

namespace {
    using U8 = Int<-127, 127>;
}

SCENARIO("U8 Maybe Test") {
    Maybe<U8> maybe1;
    Maybe<U8> maybe{20};
    maybe = maybe1;
    REQUIRE(!maybe.Present());
    REQUIRE(maybe == std::nullopt);

    REQUIRE(maybe == maybe1);
}

SCENARIO("U8 cons by another Maybe Test") {
    Maybe<U8> maybe1{10};
    Maybe<U8> maybe{maybe1};
    REQUIRE(maybe.Present());
    REQUIRE(maybe != std::nullopt);
    auto value = *maybe;
    REQUIRE(*maybe == 10);
    REQUIRE(maybe == maybe1);

    auto&& f = [](auto&& elem) -> Maybe<std::string> {
        if(elem == 20) {
            return std::nullopt;
        }
        std::ostringstream ss;
        ss << (int)elem;
        return ss.str();
    };

    REQUIRE(maybe.Map(f) == Maybe{std::string("10")});

    *maybe = 20;
    REQUIRE(maybe.Value() == 20);

    REQUIRE(maybe.ValueOr(30) == 20);

    REQUIRE(maybe.Map(f) == std::nullopt);
}