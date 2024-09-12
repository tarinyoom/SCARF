#pragma once

#include <functional>

#include "grid.hpp"
#include "pixel.hpp"

namespace scarf {

template <typename State>
struct Animation {
  std::function<State()> init;
  std::function<State(const State&, double h)> step;
  std::function<Grid<Pixel>(const State&)> render;
};

}  // namespace scarf
