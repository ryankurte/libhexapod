# Hexapod build file
# https://github.com/ryankurte/libhexapod
# Copyright 2017 Ryan Kurte

# Set minimum CMake version
cmake_minimum_required(VERSION 2.8.4)
##### Project Setup #####

# Set our output target
set(TARGET hex)

# Configure project and languages
project(${TARGET} C CXX ASM)

set(CMAKE_C_FLAGS "-std=gnu11 -g")
set(CMAKE_CXX_FLAGS "-std=gnu++11 -g")

# Set build
set(CMAKE_BUILD_TYPE DEBUG)

##### Modules #####

include(${PROJECT_SOURCE_DIR}/lib/hexapod.cmake)

##### Files #####

# Add project headers
include_directories(${PROJECT_SOURCE_DIR}/test/include)
include_directories(${PROJECT_SOURCE_DIR}/util/include)

# Add project sources
set(TEST_SOURCES
    ${PROJECT_SOURCE_DIR}/test/source/main.cpp
    ${PROJECT_SOURCE_DIR}/test/source/HexTest.cpp
)

set(UTIL_SOURCES
    ${PROJECT_SOURCE_DIR}/util/source/main.cpp
    ${PROJECT_SOURCE_DIR}/util/source/csvfile.c
    ${PROJECT_SOURCE_DIR}/util/source/util.c
)

##### Outputs #####

add_definitions(-Wall -Wpedantic)

# Generate tests and link
add_executable(${TARGET}-test ${TEST_SOURCES})
target_link_libraries(${TARGET}-test ${OPTIONAL_LIBS} pthread gmock)

add_executable(${TARGET}-util ${UTIL_SOURCES})
target_link_libraries(${TARGET}-util ${OPTIONAL_LIBS} pthread gmock)

##### Testing #####
add_custom_target(tests COMMAND ${TARGET}-test)
