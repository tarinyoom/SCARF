#pragma once

#include <array>
#include <utility>

#include "Engine.hpp"

namespace scarf {

template <typename State>
auto run_engine(const Engine<State>& engine, int n_steps, double h,
                State&& state) -> State {
  std::array<State, 2> buffer;
  buffer[0] = std::move(state);

  for (auto i = 0; i < n_steps; i++) {
    const auto& pre = buffer[i % 2];
    auto& post = buffer[(i + 1) % 2];

    post = engine.step(pre, h);

    for (const auto& obs : engine.observers) {
      obs(post);
    }
  }

  return std::move(buffer[n_steps % 2]);
}

}  // namespace scarf
