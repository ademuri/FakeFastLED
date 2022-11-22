cmake_minimum_required(VERSION 3.11)

project(fake-fast-led CXX)

# Generate the file needed for YCM integration
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra")

file(GLOB LIBRARY_SOURCES "src/*.cpp" "src/lib8tion/*.cpp")
add_library(fake-fast-led ${LIBRARY_SOURCES})

# Define headers for this library. PUBLIC headers are used for
# compiling the library, and will be added to consumers' build
# paths.
target_include_directories(fake-fast-led PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:.>
    private lib8tion)
