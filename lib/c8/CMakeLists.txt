cmake_minimum_required(VERSION 3.10)

project(c8)

option(ENABLE_JIT "Enables the Just in Time compiler for this build." ON)

set(
    ${PROJECT_NAME}_COMPILE_FLAGS
    "-Wall" "-Wextra" "-Wpedantic" "-Werror" "-pedantic-errors"
)

set(
    ${PROJECT_NAME}_LIBRARIES
    util
)

if(ENABLE_JIT)
    set(
        ${PROJECT_NAME}_LIBRARIES
        ${${PROJECT_NAME}_LIBRARIES}
        lightning
    )
endif()

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
    set(
        ${PROJECT_NAME}_COMPILE_FLAGS
        ${${PROJECT_NAME}_COMPILE_FLAGS} "-fprofile-arcs" "-ftest-coverage"
    )
    set(
        ${PROJECT_NAME}_LIBRARIES
        ${${PROJECT_NAME}_LIBRARIES} gcov
    )
endif()

file(
    GLOB_RECURSE
    ${PROJECT_NAME}_SOURCES
    "src/*.c"
)

add_library(
    ${PROJECT_NAME}
    ${${PROJECT_NAME}_SOURCES}
)

set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 99)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
        inc
    PRIVATE
        inc/${PROJECT_NAME}
)

target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC
        ${${PROJECT_NAME}_LIBRARIES}
)

target_compile_options(
    ${PROJECT_NAME}
    PUBLIC
        ${${PROJECT_NAME}_COMPILE_FLAGS}
)

if(ENABLE_JIT)
    target_compile_definitions(${PROJECT_NAME} PRIVATE ENABLE_JIT=ON)
endif()