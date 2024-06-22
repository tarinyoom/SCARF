cmake_minimum_required(VERSION 3.17)

project(Soundie CXX)

find_package(PkgConfig REQUIRED)

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
    src/main.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${FFMPEG_INCLUDE_DIRS})

# Debug: Print the target include directories
get_target_property(target_include_dirs ${PROJECT_NAME} INCLUDE_DIRECTORIES)
message(STATUS "Target include directories: ${target_include_dirs}")

# Link FFmpeg libraries
target_link_libraries(${PROJECT_NAME} PRIVATE PkgConfig::FFMPEG)

# Add pkg-config flags
target_compile_options(${PROJECT_NAME} PRIVATE ${FFMPEG_CFLAGS_OTHER})
target_link_options(${PROJECT_NAME} PRIVATE ${FFMPEG_LDFLAGS_OTHER})
