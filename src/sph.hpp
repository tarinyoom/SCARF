#pragma once

#include <array>
#include <cstddef>
#include <vector>

#include "animation.hpp"

constexpr auto OUTER_R = 3.0;
constexpr auto OUTER_R2 = OUTER_R * OUTER_R;
constexpr auto INNER_R = 0.1;
constexpr auto INNER_R2 = INNER_R * INNER_R;

struct SPHState {
  std::array<std::array<double, 2>, 8> positions;
};

extern Animation<SPHState> sph_animation;
