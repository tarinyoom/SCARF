# Define the static library target
add_library(scarflib STATIC)

# Find and link FFmpeg libraries
pkg_check_modules(FFMPEG REQUIRED IMPORTED_TARGET
    libavformat
    libavcodec
    libswscale
    libavutil
)

# Include FFmpeg headers and link libraries scarflib
target_include_directories(scarflib PRIVATE ${FFMPEG_INCLUDE_DIRS})
target_link_libraries(scarflib PRIVATE PkgConfig::FFMPEG)

# Add pkg-config flags
target_compile_options(scarflib PRIVATE ${FFMPEG_CFLAGS_OTHER})
target_link_options(scarflib PRIVATE ${FFMPEG_LDFLAGS_OTHER})

# Set the source files for the scarflib library
set(SRC_FILES
    animation.hpp
    mov.cpp
    mov.hpp
    sph.cpp
    sph.hpp
)

# Add the source files to the scarflib target
target_sources(scarflib PRIVATE ${SRC_FILES})

# Set the include directories for scarflib
target_include_directories(scarflib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(common)
add_subdirectory(model)
add_subdirectory(renderer)

set(common_dir ${CMAKE_CURRENT_SOURCE_DIR}/common)

# Link the renderer to scarflib
target_link_libraries(scarflib PRIVATE model renderer common)
