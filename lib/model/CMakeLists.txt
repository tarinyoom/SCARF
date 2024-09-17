# Define the model static library target
add_library(model STATIC)

# Set the source files for the model library
set(SRC_FILES
    dynamics.cpp
	dynamics.hpp
    step.hpp
	step.cpp
	state.cpp
	state.hpp
)

# Add the source files to the model target
target_sources(model PRIVATE ${SRC_FILES})

# Set the include directories for model
target_include_directories(model PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} common_dir)

target_link_libraries(model PRIVATE common)
