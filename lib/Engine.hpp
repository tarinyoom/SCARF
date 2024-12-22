#pragma once

#include <functional>
#include <vector>

namespace scarf {

template <typename State>
struct Engine {
  std::function<void(State&, double h)> step;
  std::vector<std::function<void(const State&)>> observers;
};

}  // namespace scarf
