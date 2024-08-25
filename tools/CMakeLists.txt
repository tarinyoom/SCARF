# Define the executable target
add_executable(scarf)

# Set the source file for the scarf executable
set(TOOLS_FILES
    driver.cpp
)

# Add the source files to the scarf executable
target_sources(scarf PRIVATE ${TOOLS_FILES})

# Link the executable against the scarflib library
target_link_libraries(scarf PRIVATE scarflib)
