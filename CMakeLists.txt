cmake_minimum_required(VERSION 3.10)

project(xxx C CXX)

if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang"
        OR CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(
        -fmacro-prefix-map=${PROJECT_SOURCE_DIR}/=
    )
endif()

set(CMAKE_C_STANDARD    11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD  11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(xxx INTERFACE)

target_include_directories(xxx
    INTERFACE
        ${PROJECT_SOURCE_DIR}/include
)

option(XXX_BUILD_TESTS "Build tests" ON)
option(XXX_BUILD_BENCHMARKS "Build benchmarks" ON)

if (XXX_BUILD_TESTS)
    add_subdirectory(test)
endif()

if (XXX_BUILD_BENCHMARKS)
    add_subdirectory(benchmark)
endif()

add_subdirectory(src)
