#pragma once

#include <functional>

#include "renderer/grid.hpp"
#include "renderer/pixel.hpp"

namespace scarf {

template <typename State>
struct Animation {
  std::function<State()> init;
  std::function<State(State&&, double h)> step;
  std::function<renderer::Grid<renderer::Pixel>(const State&)> render;
};

}  // namespace scarf
