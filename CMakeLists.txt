cmake_minimum_required(VERSION 3.5)

add_subdirectory(lib)

project(EBox)

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