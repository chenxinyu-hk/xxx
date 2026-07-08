cmake_minimum_required(VERSION 3.10)

project(xxx C CXX)

set(CMAKE_C_STANDARD    11)
set(CMAKE_CXX_STANDARD  17)

option(ENABLE_BENCHMARK "Enable benchmark" OFF)

add_executable(main
    main.cpp
)

target_include_directories(main PRIVATE
    "${PROJECT_SOURCE_DIR}/include"
)

target_compile_options(main PRIVATE
    -Wall
    -Wextra
)

target_compile_definitions(main PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
)

set_target_properties(main PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build/bin"
)

if(ENABLE_BENCHMARK)

    set(BENCHMARK_ENABLE_TESTING        OFF CACHE BOOL "" FORCE)
    set(BENCHMARK_ENABLE_EXCEPTIONS     ON  CACHE BOOL "" FORCE)
    set(BENCHMARK_ENABLE_INSTALL        OFF CACHE BOOL "" FORCE)
    set(BENCHMARK_DOWNLOAD_DEPENDENCIES OFF CACHE BOOL "" FORCE)

    set(BENCHMARK_DIR "${PROJECT_SOURCE_DIR}/third_party/benchmark")

    if(EXISTS "${BENCHMARK_DIR}/CMakeLists.txt")

        message(STATUS "Using local benchmark: ${BENCHMARK_DIR}")

        add_subdirectory(
            ${BENCHMARK_DIR}
            ${CMAKE_BINARY_DIR}/benchmark-build
        )

    else()

        message(STATUS "Fetching benchmark from GitHub")

        include(FetchContent)

        FetchContent_Declare(
            benchmark
            GIT_REPOSITORY  https://github.com/google/benchmark.git
            GIT_TAG         v1.9.4
        )

        FetchContent_MakeAvailable(benchmark)

    endif()

    set(BM_SOURCES
        "${PROJECT_SOURCE_DIR}/benchmark/bm_xxx_array.cpp"
        "${PROJECT_SOURCE_DIR}/benchmark/bm_xxx_string.cpp"
    )

    add_executable(bm_main
        ${BM_SOURCES}
    )

    target_include_directories(bm_main PRIVATE
        "${PROJECT_SOURCE_DIR}/include"
    )

    target_link_libraries(bm_main PRIVATE
        benchmark::benchmark_main
    )

    target_compile_options(bm_main PRIVATE
        -Wall
        -Wextra
        -O3
    )

    target_compile_definitions(bm_main PRIVATE
        NDEBUG
    )

    set_target_properties(bm_main PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build/bin"
    )

endif()
