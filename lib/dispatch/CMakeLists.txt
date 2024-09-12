# Define the dispatch static library target
add_library(dispatch STATIC)

# Find and link FFmpeg libraries
pkg_check_modules(FFMPEG REQUIRED IMPORTED_TARGET
    libavformat
    libavcodec
    libswscale
    libavutil
)

# Include FFmpeg headers and link libraries dispatch
target_include_directories(dispatch PRIVATE ${FFMPEG_INCLUDE_DIRS})
target_link_libraries(dispatch PRIVATE PkgConfig::FFMPEG)

# Add pkg-config flags
target_compile_options(dispatch PRIVATE ${FFMPEG_CFLAGS_OTHER})
target_link_options(dispatch PRIVATE ${FFMPEG_LDFLAGS_OTHER})

# Set the source files for the dispatch library
set(DISPATCH_SRC_FILES
    animation.hpp
	mov.cpp
	mov.hpp
)

# Add the source files to the dispatch target
target_sources(dispatch PRIVATE ${DISPATCH_SRC_FILES})

# Set the include directories for dispatch
target_include_directories(dispatch PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} common_dir)

target_link_libraries(dispatch PRIVATE common)
