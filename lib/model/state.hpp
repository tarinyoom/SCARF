#pragma once

#include <cstddef>
#include <glm/glm.hpp>
#include <utility>
#include <vector>

namespace scarf::model {

constexpr auto OUTER_R = 3.0;
constexpr auto OUTER_R2 = OUTER_R * OUTER_R;
constexpr auto INNER_R = 0.1;
constexpr auto INNER_R2 = INNER_R * INNER_R;
constexpr auto MSAA_LINEAR_DENSITY = 1;
constexpr auto N_PARTICLES = 64;
constexpr auto TOTAL_MASS = 64.0;
constexpr auto PARTICLE_MASS = TOTAL_MASS / static_cast<double>(N_PARTICLES);

struct State {
  State() = default;
  State(std::size_t n_particles);

  std::vector<glm::dvec2> positions;
  std::vector<glm::dvec2> velocities;

  std::pair<glm::dvec2, glm::dvec2> boundary;
  std::size_t n_particles;
  double reference_density;
};

auto init() -> State;

}  // namespace scarf::model
