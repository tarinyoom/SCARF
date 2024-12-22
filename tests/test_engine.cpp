#include <gtest/gtest.h>

#include "Engine.hpp"
#include "run_engine.hpp"

using namespace scarf;

TEST(dispatch, run_engine) {
  // Step 1: Set up the engine with an `int` as the state
  Engine<int> engine;
  engine.step = [](int& state, auto) -> void {
    state++;  // Increment state
  };

  // Step 2: Attach a mock observer to track observed states
  std::vector<int> observed_values;
  engine.observers.push_back(
      [&observed_values](int state) { observed_values.push_back(state); });

  // Step 3: Run the engine
  int initial_state = 0;
  int n_steps = 5;
  int final_state =
      run_engine<int>(engine, n_steps, 0.0, std::move(initial_state));

  // Step 4: Validate the final state value
  EXPECT_EQ(final_state, 5)
      << "Final state value should match the number of steps";

  // Step 5: Validate the observer received correct updates
  std::vector<int> expected_values = {1, 2, 3, 4, 5};
  EXPECT_EQ(observed_values, expected_values)
      << "Observer should track all state updates";
}
