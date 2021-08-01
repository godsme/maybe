cmake_minimum_required(VERSION 3.20)
project(maybe)

set(CMAKE_CXX_STANDARD 17)

include_directories(
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/third-party"
)

set(MAYBE_SRC include/l0-infra/maybe/Maybe.h test/PlacementTest.cpp include/l0-infra/base/RefPlacement.h)

add_executable(maybe_test test/catch.cpp ${MAYBE_SRC})
