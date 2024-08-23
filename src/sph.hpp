#pragma once

#include <array>
#include <cstddef>
#include <vector>

#include "animation.hpp"

constexpr std::size_t N_PARTICLES = 10;

constexpr auto pixel_size = 0.1;
constexpr auto pixels_per_unit = 1.0 / pixel_size;

struct SPHState {
  std::array<std::array<double, 2>, N_PARTICLES> positions;
};

extern Animation<SPHState> sph_animation;
