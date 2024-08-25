pkg_check_modules(FFMPEG REQUIRED IMPORTED_TARGET
    libavformat
    libavcodec
    libswscale
    libavutil
)

# Include FFmpeg headers and link libraries globally
target_include_directories(scarf PRIVATE ${FFMPEG_INCLUDE_DIRS})
target_link_libraries(scarf PRIVATE PkgConfig::FFMPEG)

# Add pkg-config flags globally
target_compile_options(scarf PRIVATE ${FFMPEG_CFLAGS_OTHER})
target_link_options(scarf PRIVATE ${FFMPEG_LDFLAGS_OTHER})

# Define the executable and list of source files
target_sources(scarf PRIVATE
    animation.hpp
    array.tpp
    bbox.hpp
    color.hpp
    grid.hpp
    main.cpp
    mov.hpp
    pixel.hpp
    rendering.tpp
    sph.cpp
    sph.hpp
)
