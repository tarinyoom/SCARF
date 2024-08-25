pkg_check_modules(FFMPEG REQUIRED IMPORTED_TARGET
    libavformat
    libavcodec
    libswscale
    libavutil
)

# Include FFmpeg headers and link libraries globally
target_include_directories(scarflib PRIVATE ${FFMPEG_INCLUDE_DIRS})
target_link_libraries(scarflib PRIVATE PkgConfig::FFMPEG)

# Add pkg-config flags globally
target_compile_options(scarflib PRIVATE ${FFMPEG_CFLAGS_OTHER})
target_link_options(scarflib PRIVATE ${FFMPEG_LDFLAGS_OTHER})

# Set the source files for the library
set(SRC_FILES
    animation.hpp
    array.tpp
    bbox.hpp
    color.hpp
    grid.hpp
    mov.cpp
    mov.hpp
    pixel.hpp
    rendering.tpp
    sph.cpp
    sph.hpp
)

# Add the source files to the scarflib target
target_sources(scarflib PRIVATE ${SRC_FILES})

# Include directories for the library
target_include_directories(scarflib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
