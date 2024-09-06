#pragma once

#include <functional>

#include "renderer/grid.hpp"
#include "renderer/pixel.hpp"

template <typename State>
struct Animation {
  std::function<State()> init;
  std::function<State(State&&, double h)> step;
  std::function<Grid<Pixel>(const State&)> render;
};
