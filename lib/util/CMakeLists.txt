cmake_minimum_required(VERSION 3.5)

project(util)

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