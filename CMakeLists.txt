cmake_minimum_required(VERSION 3.10)

add_subdirectory(lib)

project(EBox)

option(ENABLE_JIT "Enables the Just in Time compiler for this build." ON)

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
        c8
)

target_compile_options(
    ${PROJECT_NAME}
    PUBLIC
        ${${PROJECT_NAME}_COMPILE_FLAGS}
)
