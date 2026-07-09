cmake_minimum_required(VERSION 3.10)

project(xxx C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_library(xxx INTERFACE)

target_include_directories(xxx
    INTERFACE
        ${PROJECT_SOURCE_DIR}/include
)

option(XXX_BUILD_TESTS "Build tests" OFF)
option(XXX_BUILD_BENCHMARKS "Build benchmarks" OFF)
option(XXX_BUILD_EXAMPLES "Build examples" ON)

if (XXX_BUILD_TESTS)
    add_subdirectory(test)
endif()

if (XXX_BUILD_BENCHMARKS)
    add_subdirectory(benchmark)
endif()

if (XXX_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
