cmake_minimum_required(VERSION 3.17)

project(SCARF CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "PKG_CONFIG_PATH is set to: $ENV{PKG_CONFIG_PATH}")

find_package(PkgConfig REQUIRED)

# Add executable target
add_executable(scarf)

add_subdirectory(src)
