//
// Created by Darwin Yuan on 2021/8/3.
//
#include <l0-infra/option/Option.h>
#include <l0-infra/base/Int.h>
#include <sstream>
#include <catch.hpp>

namespace {
    using I8 = Int<-127, 127>;
    static_assert(sizeof(I8) == sizeof(int8_t));
}

SCENARIO("I8 Option Test") {
    Option<I8> maybe1;
    Option<I8> maybe{20};
    maybe = maybe1;
    REQUIRE(!maybe.Present());
    REQUIRE(maybe == std::nullopt);

    REQUIRE(maybe == maybe1);
}

SCENARIO("I8 cons by another Option Test") {
    Option<I8> maybe1{10};
    Option<I8> maybe{maybe1};
    REQUIRE(maybe.Present());
    REQUIRE(maybe != std::nullopt);
    auto value = *maybe;
    REQUIRE(*maybe == 10);
    REQUIRE(maybe == maybe1);

    auto&& f = [](auto&& elem) -> Option<std::string> {
        if(elem == 20) {
            return std::nullopt;
        }
        std::ostringstream ss;
        ss << (int)elem;
        return ss.str();
    };

    REQUIRE(maybe.Map(f) == Option{std::string("10")});

    *maybe = 20;
    REQUIRE(maybe.Value() == 20);

    REQUIRE(maybe.ValueOr(30) == 20);

    REQUIRE(maybe.Map(f) == std::nullopt);
}