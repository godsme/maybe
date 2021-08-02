//
// Created by Darwin Yuan on 2021/8/3.
//

#include <l0-infra/base/Int.h>
#include <catch.hpp>

SCENARIO("U8 Test") {
    using U8 = Int<-128, 127>;
    static_assert(std::is_same_v<typename U8::IntType, int8_t>);
    U8 a = 10;
    REQUIRE(a == 10);
    REQUIRE(a + 5 == 15);
}

SCENARIO("U8 Test 2") {
    using U8 = Int<-50, 127>;
    static_assert(std::is_same_v<typename U8::IntType, int8_t>);
    U8 a = -52;
    REQUIRE(a == -52);
    REQUIRE(!a.IsValid());
}