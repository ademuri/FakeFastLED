cmake_minimum_required(VERSION 2.8.8)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/lib8tion)

file(GLOB LIBRARY_SOURCES "*.cpp" "lib8tion/*.cpp")
add_library(FakeFastLED ${LIBRARY_SOURCES})
