# Define the renderer static library target
add_library(renderer STATIC)

# Set the source files for the renderer library
set(RENDERER_SRC_FILES
    array.tpp
    color.hpp
    grid.hpp
	matrix.hpp
    pixel.hpp
    render.cpp
    render.hpp
    rendering.tpp
    scene.hpp
)

# Add the source files to the renderer target
target_sources(renderer PRIVATE ${RENDERER_SRC_FILES})

# Set the include directories for renderer
target_include_directories(renderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
