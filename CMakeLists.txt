cmake_minimum_required(VERSION 3.17)

project(Loopie CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(PkgConfig REQUIRED)

message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "PKG_CONFIG_PATH is set to: $ENV{PKG_CONFIG_PATH}")

pkg_check_modules(FFMPEG REQUIRED IMPORTED_TARGET
    libavdevice
    libavfilter
    libavformat
    libavcodec
    libswresample
    libswscale
    libavutil
)

# Debug: Print FFmpeg package information
message(STATUS "FFmpeg include dirs: ${FFMPEG_INCLUDE_DIRS}")
message(STATUS "FFmpeg libraries: ${FFMPEG_LIBRARIES}")
message(STATUS "FFmpeg cflags: ${FFMPEG_CFLAGS_OTHER}")
message(STATUS "FFmpeg ldflags: ${FFMPEG_LDFLAGS_OTHER}")

add_executable(${PROJECT_NAME}
    src/pixel.hpp
    src/main.cpp
    src/mov.hpp
    src/cli.hpp
    src/cli.cpp
    src/animation.hpp
    src/black_box.cpp
    src/black_box.hpp
    src/three_body.cpp
    src/three_body.hpp
    src/vector.hpp
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

target_include_directories(${PROJECT_NAME} PRIVATE ${FFMPEG_INCLUDE_DIRS})

# Debug: Print the target include directories
get_target_property(target_include_dirs ${PROJECT_NAME} INCLUDE_DIRECTORIES)
# message(STATUS "Target include directories: ${target_include_dirs}")

# Link FFmpeg libraries
target_link_libraries(${PROJECT_NAME} PRIVATE PkgConfig::FFMPEG)

# Add pkg-config flags
target_compile_options(${PROJECT_NAME} PRIVATE ${FFMPEG_CFLAGS_OTHER})
target_link_options(${PROJECT_NAME} PRIVATE ${FFMPEG_LDFLAGS_OTHER})
