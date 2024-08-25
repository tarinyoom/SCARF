cmake_minimum_required(VERSION 3.17)

project(Loopie CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "PKG_CONFIG_PATH is set to: $ENV{PKG_CONFIG_PATH}")

find_package(PkgConfig REQUIRED)

pkg_check_modules(FFMPEG REQUIRED IMPORTED_TARGET
    libavformat
    libavcodec
    libswscale
    libavutil
)

# Add executable target
add_executable(${PROJECT_NAME})

# Include FFmpeg headers and link libraries globally
target_include_directories(${PROJECT_NAME} PRIVATE ${FFMPEG_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE PkgConfig::FFMPEG)

# Add pkg-config flags globally
target_compile_options(${PROJECT_NAME} PRIVATE ${FFMPEG_CFLAGS_OTHER})
target_link_options(${PROJECT_NAME} PRIVATE ${FFMPEG_LDFLAGS_OTHER})

add_subdirectory(src)
