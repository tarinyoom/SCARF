# Set the source file for the executable
set(TOOLS_FILES
    driver.cpp
)

# Add the source files to the scarf executable
target_sources(scarf PRIVATE ${TOOLS_FILES})

# Link the executable against the scarflib library
target_link_libraries(scarf PRIVATE scarflib)

# Include the src directory for the scarf executable
target_include_directories(scarf PRIVATE ${CMAKE_SOURCE_DIR}/src)
