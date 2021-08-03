//
// Created by Darwin Yuan on 2021/8/3.
//
#include <optional>
#include <catch.hpp>

SCENARIO("optional test") {
    std::optional<int> option{10};
    std::optional<int> option2{option};
    auto result = *option2;
    REQUIRE(result == 10);
}
