

# Set minimum CMake version
cmake_minimum_required(VERSION 2.8.4)
##### Project Setup #####

# Set our output target
set(TARGET hextest)

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

# Add project sources
set(TEST_SOURCES
    ${PROJECT_SOURCE_DIR}/test/source/main.cpp
    ${PROJECT_SOURCE_DIR}/test/source/HexTest.cpp
)

##### Outputs #####

# Generate executable and link
add_executable(${TARGET} ${TEST_SOURCES})
target_link_libraries(${TARGET} ${OPTIONAL_LIBS} pthread gmock)

##### Testing #####
add_custom_target(tests COMMAND ${TARGET})