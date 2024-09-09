#pragma once

#include <array>
#include <cstddef>
#include <ostream>
#include <vector>

#include "animation.hpp"
#include "bbox.hpp"

namespace scarf {

constexpr auto OUTER_R = 3.0;
constexpr auto OUTER_R2 = OUTER_R * OUTER_R;
constexpr auto INNER_R = 0.1;
constexpr auto INNER_R2 = INNER_R * INNER_R;
constexpr auto MSAA_LINEAR_DENSITY = 1;
constexpr auto N_PARTICLES = 64;

struct SPHState {
  SPHState(std::size_t n_particles);

  std::vector<Vector<double, 2>> positions;
  std::vector<Vector<double, 2>> velocities;
  std::vector<double> densities;
  std::vector<double> pressures;
  Bbox<double, 2> boundary;
  std::size_t n_particles;
};

auto build_animation() -> Animation<SPHState>;

}  // namespace scarf
