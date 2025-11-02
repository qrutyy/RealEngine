cmake_minimum_required(VERSION 3.10)

project(RealEngine)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_testing()
option(BUILD_TESTS "Build test executables" OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
add_subdirectory(lib/SDL EXCLUDE_FROM_ALL)

file(GLOB TEST_SOURCES "tests/*.c")

file(GLOB APP_SOURCES
    "src/*.c"
    "lib/*.c"
)

list(FILTER APP_SOURCES EXCLUDE REGEX ".*main\\.c$")

add_library(${PROJECT_NAME}_lib ${APP_SOURCES})
target_link_libraries(${PROJECT_NAME}_lib PRIVATE SDL3::SDL3)
target_include_directories(${PROJECT_NAME}_lib PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib"
)
target_compile_options(${PROJECT_NAME}_lib PRIVATE -Wall -Wextra -Wpedantic)

add_executable(${PROJECT_NAME} src/main.c)
target_link_libraries(${PROJECT_NAME} PRIVATE 
    ${PROJECT_NAME}_lib
    SDL3::SDL3
)

if(BUILD_TESTS)
    message(STATUS "Building tests: ON")

    if(TEST_SOURCES)
        add_executable(${PROJECT_NAME}_tests 
            ${TEST_SOURCES}
            "lib/Unity/src/unity.c"
        )

        target_link_libraries(${PROJECT_NAME}_tests PRIVATE 
            ${PROJECT_NAME}_lib
            SDL3::SDL3
        )

        target_include_directories(${PROJECT_NAME}_tests PUBLIC
            "${CMAKE_CURRENT_SOURCE_DIR}/include"
            "${CMAKE_CURRENT_SOURCE_DIR}/lib"
            "${CMAKE_CURRENT_SOURCE_DIR}/lib/Unity/src"
        )
        target_compile_options(${PROJECT_NAME}_tests PRIVATE -Wall -Wextra -Wpedantic)
        add_test(NAME ${PROJECT_NAME}_tests COMMAND ${PROJECT_NAME}_tests)
    endif()
endif()