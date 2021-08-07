//
// Created by Darwin Yuan on 2021/8/1.
//

#include <l0-infra/base/Placement.h>
#include <catch.hpp>

SCENARIO("int ref Placement Test") {
    Placement<int&> int_ref;
    int a = 10;

    int_ref.Emplace(a);
    REQUIRE(*int_ref == a);

    REQUIRE(int_ref.GetRef() == a);

    int b = 20;

    int_ref.Replace(b);

    REQUIRE(*int_ref == b);
    REQUIRE(int_ref.GetRef() == b);
}

SCENARIO("int Placement Test") {
    Placement<int> placement;

    placement.Emplace(10);
    REQUIRE(*placement == 10);

    placement.Replace(20);
    REQUIRE(*placement == 20);
}

