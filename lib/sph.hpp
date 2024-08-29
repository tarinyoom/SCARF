#pragma once

#include <array>
#include <cstddef>
#include <vector>

#include "animation.hpp"
#include "bbox.hpp"

constexpr auto OUTER_R = 3.0;
constexpr auto OUTER_R2 = OUTER_R * OUTER_R;
constexpr auto INNER_R = 0.1;
constexpr auto INNER_R2 = INNER_R * INNER_R;
constexpr auto MSAA_LINEAR_DENSITY = 3;
constexpr auto N_PARTICLES = 32;

struct SPHState {
  SPHState(std::size_t n_particles);
  std::vector<std::array<double, 2>> positions;
  std::vector<std::array<double, 2>> velocities;
  Bbox<double, 2> boundary;
  std::size_t n_particles;
};

extern Animation<SPHState> sph_animation;
