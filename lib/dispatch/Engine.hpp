#pragma once

#include <functional>
#include <vector>

namespace scarf::dispatch {

template <typename State>
struct Engine {
  std::function<State(const State&, double h)> step;
  std::vector<std::function<void(const State&)>> observers;
};

}  // namespace scarf::dispatch
