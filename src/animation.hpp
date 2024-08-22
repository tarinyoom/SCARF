#pragma once

#include <functional>

#include "grid.hpp"
#include "pixel.hpp"

template <typename State>
struct Animation {
  std::function<State()> init;
  std::function<State(State&&)> step;
  std::function<Grid<Pixel>(const State&)> render;
};
