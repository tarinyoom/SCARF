#pragma once

#include <functional>

#include "grid.hpp"
#include "pixel.hpp"

namespace scarf::dispatch {

struct Animation {
  std::function<Grid<Pixel>(double)> next;
};

}  // namespace scarf::dispatch
