# Define the parser static library target
add_library(parser STATIC)

# Set the source files for the parser library
set(SRC_FILES
    parse.cpp
    parse.hpp
)

# Add the source files to the parser target
target_sources(parser PRIVATE ${SRC_FILES})

# Set the include directories for parser
target_include_directories(parser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} common_dir)

target_link_libraries(parser PRIVATE common)
