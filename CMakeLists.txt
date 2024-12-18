cmake_minimum_required(VERSION 3.17)

project(SCARF CXX)

include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "PKG_CONFIG_PATH is set to: $ENV{PKG_CONFIG_PATH}")

#get_property(dirs DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)

find_package(PkgConfig REQUIRED)

add_subdirectory(lib)
add_subdirectory(tools)
add_subdirectory(tests)
