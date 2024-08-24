#pragma once

#include <array>
#include <cstddef>
#include <vector>

#include "animation.hpp"

constexpr auto r = 3.0;
constexpr auto r2 = r * r;

struct SPHState {
  std::array<std::array<double, 2>, 8> positions;
};

extern Animation<SPHState> sph_animation;
