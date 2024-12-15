#pragma once

#include <array>
#include <functional>
#include <vector>

namespace scarf::engine {

template <typename State>
struct Engine {
  std::function<State(const State&, double h)> step;
  std::vector<std::function<void(const State&)>> observers;
};

template <typename State>
auto run(const Engine<State>& engine, int n_steps, double h, State&& state)
    -> State {
  // Initialize two-sided buffer
  std::array<State, 2> buffer;
  buffer[0] = std::move(state);

  // Run the engine
  for (auto i = 0; i < n_steps; i++) {
    const auto& pre = buffer[i % 2];
    auto& post = buffer[(i + 1) % 2];

    post = engine.step(pre, h);

    // Update observers
    for (const auto& obs : engine.observers) {
      obs(post);
    }
  }

  // Return final state
  return std::move(buffer[n_steps % 2]);
}

}  // namespace scarf::engine
