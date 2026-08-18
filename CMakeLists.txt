cmake_minimum_required(VERSION 3.10)

project(xxx C CXX)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU"
        AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 8)
    add_compile_options(
        -fmacro-prefix-map=${PROJECT_SOURCE_DIR}/=
    )
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(
        -fmacro-prefix-map=${PROJECT_SOURCE_DIR}/=
    )
endif()

set(CMAKE_C_STANDARD    11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD  11)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)

add_library(xxx INTERFACE)

target_include_directories(xxx
    INTERFACE
        ${PROJECT_SOURCE_DIR}/include
)

option(XXX_BUILD_TESTS "Build tests" OFF)
option(XXX_BUILD_BENCHMARKS "Build benchmarks" OFF)

if (XXX_BUILD_TESTS)
    add_subdirectory(test)
endif()

if (XXX_BUILD_BENCHMARKS)
    add_subdirectory(benchmark)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_subdirectory(src)
endif()
