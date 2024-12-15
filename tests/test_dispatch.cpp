#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>

#include "alternator.hpp"
#include "dispatch/engine.hpp"
#include "dispatch/make_video_writer.hpp"

using namespace scarf;

TEST(dispatch, run) {
  // Step 1: Set up the engine with an `int` as the state
  engine::Engine<int> engine;
  engine.step = [](int state) -> int {
    return state + 1;  // Increment state
  };

  // Step 2: Attach a mock observer to track observed states
  std::vector<int> observed_values;
  engine.observers.push_back(
      [&observed_values](int state) { observed_values.push_back(state); });

  // Step 3: Run the engine
  int initial_state = 0;
  int n_steps = 5;
  int final_state = engine::run<int>(engine, n_steps, std::move(initial_state));

  // Step 4: Validate the final state value
  EXPECT_EQ(final_state, 5)
      << "Final state value should match the number of steps";

  // Step 5: Validate the observer received correct updates
  std::vector<int> expected_values = {1, 2, 3, 4, 5};
  EXPECT_EQ(observed_values, expected_values)
      << "Observer should track all state updates";
}

TEST(dispatch, alternator) {
  auto alt = Alternator<double>(0.0, [](double n, double h) { return n + h; });
  EXPECT_EQ(*alt.next(1.0), 1.0);
  EXPECT_EQ(*alt.next(2.0), 3.0);
  EXPECT_EQ(*alt.next(4.0), 7.0);
  EXPECT_EQ(*alt.next(8.0), 15.0);
}

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
  EXPECT_GE(sz, 2000); // conservative lower bound

  // Verify that file can be opened
  std::ofstream file(path);
  EXPECT_TRUE(file.is_open());
  file.close();

  std::filesystem::remove(test_filepath);
}
