# Define the static library target
add_library(scarflib STATIC)

# Add source files to the scarflib target
target_sources(scarflib PRIVATE alternator.hpp sph.cpp sph.hpp)

# Set the include directories for scarflib
target_include_directories(scarflib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(common)
add_subdirectory(dispatch)
add_subdirectory(model)
add_subdirectory(parse)
add_subdirectory(render)

set(common_dir ${CMAKE_CURRENT_SOURCE_DIR}/common)

target_link_libraries(scarflib PRIVATE
    dispatch
	model
	parse
	render
	common)
