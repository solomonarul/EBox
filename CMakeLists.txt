cmake_minimum_required(VERSION 3.5)

add_subdirectory(lib)

project(EBox)

set(
    ${PROJECT_NAME}_COMPILE_FLAGS
    "-Wall" "-Wextra" "-Wpedantic" "-Werror"
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
    ${PROJECT_NAME}_TEST_SOURCES
    "test/*.c"
)

add_executable(
    ${PROJECT_NAME}_tests
    ${${PROJECT_NAME}_TEST_SOURCES}
)

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
)

target_compile_options(
    ${PROJECT_NAME}_tests
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