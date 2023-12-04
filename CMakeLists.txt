cmake_minimum_required(VERSION 3.25)
project(EADS-lab-2)

option(FETCHCONTENT_QUIET "" OFF)
option(FETCHCONTENT_UPDATES_DISCONNECTED "" ON)
Include(FetchContent)

option(CATCH_CONFIG_ENABLE_ALL_STRINGMAKERS "" YES )

FetchContent_Declare(Catch2
        URL https://github.com/catchorg/Catch2/archive/refs/tags/v3.4.0.tar.gz
        )

FetchContent_MakeAvailable(Catch2)

set(CMAKE_CXX_STANDARD 17)

add_executable(EADS-lab-2 bi_ring_test.cpp bi_ring.h bi_ring_test.h )
target_link_libraries(EADS-lab-2 PRIVATE Catch2::Catch2WithMain)