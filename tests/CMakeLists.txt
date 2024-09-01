# Define the executable target for tests
add_executable(scarftest)

# Find and link GTest using pkg-config
pkg_check_modules(GTEST REQUIRED gtest_main gtest)

target_include_directories(scarftest PRIVATE ${GTEST_INCLUDE_DIRS})
target_link_libraries(scarftest PRIVATE ${GTEST_LIBRARY_DIRS}/gtest_main.lib ${GTEST_LIBRARY_DIRS}/gtest.lib)

# Link the executable against the scarflib library
target_link_libraries(scarftest PRIVATE scarflib)

# Set the source files for the scarftest executable
set(TOOLS_FILES
    test_model.cpp
)

# Add the source files to the scarftest executable
target_sources(scarftest PRIVATE ${TOOLS_FILES})

# Add the test to CTest
add_test(NAME scarftest COMMAND scarftest)
