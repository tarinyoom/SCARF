# Define the common static library target
add_library(common STATIC)

# Set the source files for the common library
set(COMMON_SRC_FILES
    bbox.hpp
    dummy.cpp
    vector.hpp
)

# Add the source files to the common target
target_sources(common PRIVATE ${COMMON_SRC_FILES})

# Set the include directories for common
target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
