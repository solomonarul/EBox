cmake_minimum_required(VERSION 3.10)

project(bf)

set(
    ${PROJECT_NAME}_COMPILE_FLAGS
    "-Wall" "-Wextra" "-Wpedantic" "-Werror" "-pedantic-errors"
)

set(
    ${PROJECT_NAME}_LIBRARIES
    util
    lightning
)

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