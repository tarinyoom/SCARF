# Define the static library target
add_library(scarflib STATIC)

# Set the source files for the scarflib library
set(SRC_FILES
    alternator.hpp
    sph.cpp
    sph.hpp
)

# Add the source files to the scarflib target
target_sources(scarflib PRIVATE ${SRC_FILES})

# Set the include directories for scarflib
target_include_directories(scarflib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(common)
add_subdirectory(dispatch)
add_subdirectory(model)
add_subdirectory(renderer)

set(common_dir ${CMAKE_CURRENT_SOURCE_DIR}/common)

# Link the renderer to scarflib
target_link_libraries(scarflib PRIVATE dispatch model renderer common)
