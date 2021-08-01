cmake_minimum_required(VERSION 3.20)
project(maybe)

set(CMAKE_CXX_STANDARD 17)

include_directories(
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/third-party"
)

set(MAYBE_SRC include/l0-infra/maybe/Maybe.h test/Ref_PlacementTest.cpp
        include/l0-infra/base/detail/Ref_Placement.h
        include/l0-infra/base/detail/Val_Placement.h include/l0-infra/base/Placement.h include/l0-infra/base/Placement.h test/PlacementTest.cpp test/TrivalMaybeTest.cpp test/NonTrivialMaybeTest.cpp test/RefMaybeTest.cpp include/l0-infra/maybe/detail/Ref_Maybe.h include/l0-infra/maybe/detail/Val_Maybe.h)

add_executable(maybe_test test/catch.cpp ${MAYBE_SRC})
