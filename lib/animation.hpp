#pragma once

#include <functional>

#include "grid.hpp"
#include "pixel.hpp"

namespace scarf {

struct Animation {
  std::function<Grid<Pixel>(double)> next;
};

}  // namespace scarf
