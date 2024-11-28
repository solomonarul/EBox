cmake_minimum_required(VERSION 3.10)

add_subdirectory(lib)

project(EBox)

set(
    ${PROJECT_NAME}_COMPILE_FLAGS
    "-Wall" "-Wextra" "-Wpedantic" "-Werror" "-pedantic-errors"
)

file(
    GLOB_RECURSE
    ${PROJECT_NAME}_SOURCES
    "src/*.c"
)

add_executable(
    ${PROJECT_NAME}
    ${${PROJECT_NAME}_SOURCES}
)

set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 99)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
        inc/
)

target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC
        util
        bf
)

target_compile_options(
    ${PROJECT_NAME}
    PUBLIC
        ${${PROJECT_NAME}_COMPILE_FLAGS}
)

file(
    GLOB_RECURSE
    ${PROJECT_NAME}_BENCH_SOURCES
    "bench/*.c"
)

add_executable(
    ${PROJECT_NAME}_bench
    ${${PROJECT_NAME}_BENCH_SOURCES}
)

set_property(TARGET ${PROJECT_NAME}_bench PROPERTY C_STANDARD 99)

target_include_directories(
    ${PROJECT_NAME}_bench
    PUBLIC
        inc/
)

target_link_libraries(
    ${PROJECT_NAME}_bench
    PUBLIC
        util
        bf
)

target_compile_options(
    ${PROJECT_NAME}_bench
    PUBLIC
        ${${PROJECT_NAME}_COMPILE_FLAGS}
)

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
    file(
        GLOB_RECURSE
        ${PROJECT_NAME}_TEST_SOURCES
        "test/*.c"
    )

    add_executable(
        ${PROJECT_NAME}_tests
        ${${PROJECT_NAME}_TEST_SOURCES}
    )

    set_property(TARGET ${PROJECT_NAME}_tests PROPERTY C_STANDARD 99)

    target_include_directories(
        ${PROJECT_NAME}_tests
        PUBLIC
            inc/
    )

    target_link_libraries(
        ${PROJECT_NAME}_tests
        PUBLIC
            util
            bf
            gcov
    )

    target_compile_options(
        ${PROJECT_NAME}_tests
        PUBLIC
            ${${PROJECT_NAME}_COMPILE_FLAGS}
            "-fprofile-arcs" "-ftest-coverage"
    )
endif()