//
// Created by Darwin Yuan on 2021/8/1.
//
#include <l0-infra/option/Option.h>
#include <catch.hpp>
#include <sstream>
#include <optional>

SCENARIO("Default Option Test") {
    constexpr Option<int> maybe{};
    static_assert(!maybe.Present());
    static_assert(maybe == std::nullopt);
    static_assert(maybe == Option<int>{});
    static_assert(std::is_trivially_destructible_v<Option<int>>);
    static_assert(!std::is_trivially_copyable_v<Option<int>>);
}

SCENARIO("Nothing Test") {
    Option<int> maybe = std::nullopt;
    REQUIRE(!maybe.Present());
    REQUIRE(maybe == std::nullopt);
}

SCENARIO("cons by Nothing Test") {
    Option<int> maybe{std::nullopt};
    REQUIRE(!maybe.Present());
    REQUIRE(maybe == std::nullopt);
}

SCENARIO("cons by value Option Test") {
    constexpr Option<int> maybe{10};
    static_assert((bool)maybe);
    static_assert(maybe.Present());
    static_assert(maybe != std::nullopt);
    static_assert(maybe == 10);
    REQUIRE(maybe.Present());
    REQUIRE(maybe != std::nullopt);
    REQUIRE(maybe == 10);
}

SCENARIO("cons by another Option Test") {
    constexpr Option<int> maybe1{10};
    Option<int> maybe{maybe1};
    REQUIRE(maybe.Present());
    REQUIRE(maybe != std::nullopt);
    REQUIRE(*maybe == 10);
    REQUIRE(maybe == maybe1);

    auto&& f = [](auto&& elem) -> Option<std::string> {
        if(elem == 20) {
            return std::nullopt;
        }
        std::ostringstream ss;
        ss << elem;
        return ss.str();
    };

    REQUIRE(maybe.Map(f) == Option{std::string("10")});

    *maybe = 20;
    REQUIRE(maybe.Value() == 20);

    REQUIRE(maybe.ValueOr(30) == 20);

    REQUIRE(maybe.Map(f) == std::nullopt);
}

SCENARIO("MapOr Option Test") {
    Option<int> maybe;
    auto&& f = [](auto&& elem) -> std::string {
        std::ostringstream ss;
        ss << elem;
        return ss.str();
    };

    REQUIRE(maybe.MapOr("20", f) == std::string("20"));
}

SCENARIO("copy assignment by another Option Test") {
    Option<int> maybe1;
    Option<int> maybe{20};
    maybe = maybe1;
    REQUIRE(!maybe.Present());
    REQUIRE(maybe == std::nullopt);

    REQUIRE(maybe == maybe1);
}

SCENARIO("String nothing ValueOr by another Option Test") {
    Option<std::string> maybe;
    auto result = maybe.ValueOr("abc");
    REQUIRE(result == std::string("abc"));
}

SCENARIO("String ValueOr by another Option Test") {
    Option<std::string> maybe{"123"};
    REQUIRE(maybe);
    auto result = maybe.ValueOr("abc");
    REQUIRE(result == std::string("123"));
    REQUIRE(*maybe == std::string("123"));
    REQUIRE(maybe == std::string("123"));

    maybe = std::nullopt;

    REQUIRE(!maybe);

    auto&& str = Option<std::string>{"123"}.Value();
    REQUIRE(str == std::string("123"));
    static_assert(std::is_same_v<std::string&&, decltype(Option<std::string>{"123"}.Value())>);
}
