cmake_minimum_required(VERSION 3.20)
project(option)

set(CMAKE_CXX_STANDARD 17)

include_directories(
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/third-party"
)

set(OPTION_SRC include/l0-infra/option/Option.h test/Ref_PlacementTest.cpp
        include/l0-infra/base/detail/Ref_Placement.h
        include/l0-infra/base/detail/Val_Placement.h include/l0-infra/base/Placement.h include/l0-infra/base/Placement.h test/PlacementTest.cpp test/TrivalMaybeTest.cpp test/NonTrivialMaybeTest.cpp test/Ref_Option_Test.cpp include/l0-infra/option/detail/Ref_Option.h include/l0-infra/option/detail/Val_Option.h include/l0-infra/base/Int.h include/l0-infra/base/Enum.h test/IntTest.cpp include/l0-infra/option/detail/Int_Option.h test/IntMaybeTest.cpp test/std_optional_test.cpp)

add_executable(option_test test/catch.cpp ${OPTION_SRC})
