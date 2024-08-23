#pragma once

#include <array>
#include <cstddef>
#include <vector>

#include "animation.hpp"

constexpr std::size_t N_PARTICLES = 8;

constexpr auto pixel_size = 0.1;
constexpr auto pixels_per_unit = 1.0 / pixel_size;
constexpr auto r = 0.5;
constexpr auto r2 = r * r;

struct SPHState {
  std::array<std::array<double, 2>, N_PARTICLES> positions;
};

extern Animation<SPHState> sph_animation;
