#include <gtest/gtest.h>

#include "alternator.hpp"
#include "dispatch/engine.hpp"

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
