//
// Created by Darwin Yuan on 2021/8/1.
//
#include <l0-infra/base/Ref_Placement.h>
#include <catch.hpp>

SCENARIO("Placement Test") {
    Ref_Placement<int> int_ref;
    int a = 10;

    int_ref.Emplace(a);
    REQUIRE(*int_ref == a);

    REQUIRE(int_ref.GetObj() == &a);

    int b = 20;

    int_ref.Replace(b);

    REQUIRE(*int_ref == b);
    REQUIRE(int_ref.GetObj() == &b);
}
