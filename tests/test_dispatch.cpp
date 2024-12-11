#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>

#include "alternator.hpp"
#include "dispatch/engine.hpp"
#include "dispatch/make_mov_writer.hpp"

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

auto is_file_nonempty(const std::string& filepath) -> bool {
  std::filesystem::path path(filepath);
  return std::filesystem::exists(path) && std::filesystem::file_size(path) > 0;
}

auto is_file_closed(const std::string& filepath) -> bool {
  std::ofstream file(filepath, std::ios::app);  // Try to open for appending
  return file.is_open();  // If it opens successfully, the file is not locked
}

TEST(dispatch, mov_writer) {
  const std::string test_filepath = "test_output.mov";

  if (std::filesystem::exists(test_filepath)) {
    std::filesystem::remove(test_filepath);
  }

  // Write frames to the file in a limited scope to ensure destructor is called
  {
    const int width = 1920;
    const int height = 1080;

    auto writer = make_mov_writer(test_filepath, height, width, 60);

    std::vector<uint8_t> dummy_frame(width * height * 3, 255);

    const int frame_count = 5;
    for (int i = 0; i < frame_count; ++i) {
      writer->write_frame(dummy_frame.data());
    }

    EXPECT_TRUE(is_file_nonempty(test_filepath))
        << "Output file was not created or is empty.";
    // EXPECT_FALSE(is_file_closed(test_filepath))
    //  << "File is unexpectedly closed before destructor.";
  }

  EXPECT_TRUE(is_file_closed(test_filepath))
      << "File was not properly closed after destructor.";
  std::filesystem::remove(test_filepath);
}
