# Define the render static library target
add_library(render STATIC)

# Set the source files for the render library
set(SRC_FILES
    color.hpp
	matrix.hpp
    render.cpp
    render.hpp
    rendering.tpp
    scene.hpp
)

# Add the source files to the render target
target_sources(render PRIVATE ${SRC_FILES})

# Set the include directories for render
target_include_directories(render PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} common_dir)

target_link_libraries(render PRIVATE common)
