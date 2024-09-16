#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <iostream>

#include "sph.hpp"

using namespace scarf;

TEST(scarf, end_to_end) {
  char arg1[] = "";
  char arg2[] = "examples/config.txt";
  char *args[] = {arg1, arg2};
  auto res = run(2, args);
  ASSERT_EQ(res, 0);

  auto file1 = "examples/generated.mov";
  auto file2 = "examples/reference.mov";

  auto size1 = std::filesystem::file_size(file1);
  auto size2 = std::filesystem::file_size(file2);

  // Assert that the sizes are equal
  ASSERT_EQ(size1, size2) << "Files are not the same size!";

  std::ifstream f1(file1, std::ios::binary);
  std::ifstream f2(file2, std::ios::binary);

  // Check if both files are open
  ASSERT_TRUE(f1.is_open()) << "Could not open " << file1 << std::endl;
  ASSERT_TRUE(f2.is_open()) << "Could not open " << file2 << std::endl;

  // Compare byte by byte
  char byte1 = 0, byte2 = 0;
  while (f1.get(byte1) && f2.get(byte2)) {
    ASSERT_EQ(byte1, byte2);
  }
}
