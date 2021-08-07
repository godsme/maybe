//
// Created by Darwin Yuan on 2021/8/1.
//
#include <l0-infra/base/detail/Ref_Placement.h>
#include <catch.hpp>

SCENARIO("Ref Placement Test") {
    Ref_Placement<int> int_ref;
    int a = 10;

    int_ref.Emplace(a);
    REQUIRE(*int_ref == a);

    REQUIRE(int_ref.GetRef() == a);

    int b = 20;

    int_ref.Replace(b);

    REQUIRE(*int_ref == b);
    REQUIRE(int_ref.GetRef() == b);
}

namespace {
    struct Base {
        Base(int a) : a{a} {}
        int a;
        virtual ~Base() = default;
    };

    struct Derived : Base {
        Derived(int a, int b) : Base(a), b{b} {}
        int b;
    };
}

SCENARIO("Ref class Placement Test") {
    Derived derived{10, 20};
    Ref_Placement<Derived>  ref_derived{derived};

    REQUIRE(ref_derived);
    REQUIRE(ref_derived->a == 10);
    REQUIRE(ref_derived->b == 20);

    Ref_Placement<Base> ref_base{ref_derived};

    REQUIRE(ref_base);
    REQUIRE(ref_base->a == 10);

    ref_base = ref_derived;

    REQUIRE(ref_base);
    REQUIRE(ref_base->a == 10);
}
