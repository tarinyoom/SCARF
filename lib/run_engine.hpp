#pragma once

#include <array>
#include <utility>

#include "Engine.hpp"

namespace scarf {

template <typename State>
auto run_engine(const Engine<State>& engine, int n_steps, double h,
                State&& initial_state) -> State {
  auto state = std::move(initial_state);

  for (auto i = 0; i < n_steps; i++) {
    engine.step(state, h);

    for (const auto& obs : engine.observers) {
      obs(state);
    }
  }

  return state;
}

}  // namespace scarf
