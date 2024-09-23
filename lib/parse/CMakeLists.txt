# Define the parse static library target
add_library(parse STATIC)

# Set the source files for the parse library
set(SRC_FILES
    parse.cpp
    parse.hpp
)

# Add the source files to the parse target
target_sources(parse PRIVATE ${SRC_FILES})

# Set the include directories for parse
target_include_directories(parse PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} common_dir)

target_link_libraries(parse PRIVATE common)
