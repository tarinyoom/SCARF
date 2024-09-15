# Define the renderer static library target
add_library(renderer STATIC)

# Set the source files for the renderer library
set(SRC_FILES
    color.hpp
	matrix.hpp
    render.cpp
    render.hpp
    rendering.tpp
    scene.hpp
)

# Add the source files to the renderer target
target_sources(renderer PRIVATE ${SRC_FILES})

# Set the include directories for renderer
target_include_directories(renderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} common_dir)

target_link_libraries(renderer PRIVATE common)
