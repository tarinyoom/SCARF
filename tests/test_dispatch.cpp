#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>

#include "dispatch/make_video_writer.hpp"

using namespace scarf;

TEST(dispatch, mov_writer) {
  const std::string test_filepath = "test_output.mov";

  if (std::filesystem::exists(test_filepath)) {
    std::filesystem::remove(test_filepath);
  }

  // Write frames to the file in a limited scope to ensure destructor is called
  {
    const int width = 640;
    const int height = 480;

    auto writer = dispatch::make_video_writer("mov", test_filepath);

    Pixel bg_color(128, 128, 128);
    Grid<Pixel> grid(width, height, bg_color);

    const int frame_count = 100;
    for (int i = 0; i < frame_count; ++i) {
      writer->write_frame(grid);
    }
  }

  std::filesystem::path path(test_filepath);
  EXPECT_TRUE(std::filesystem::exists(path));

  auto sz = std::filesystem::file_size(path);
  EXPECT_GE(sz, 2000);  // conservative lower bound

  // Verify that file can be opened
  std::ofstream file(path);
  EXPECT_TRUE(file.is_open());
  file.close();

  std::filesystem::remove(test_filepath);
}
