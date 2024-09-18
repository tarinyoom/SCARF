#include "step.hpp"

#include <cmath>
#include <functional>

#include "dynamics.hpp"
#include "kernel.hpp"

namespace scarf::model {

auto step(const State& pre, double h) -> State {
  auto densities = compute_densities(pre.positions);
  auto pressures = compute_pressures(pre, densities);
  auto accelerations =
      compute_accelerations(pre.positions, densities, pressures);

  State post(pre.n_particles);
  for (auto i = 0; i < pre.positions.size(); i++) {
    post.positions[i] = pre.positions[i] + h * pre.velocities[i];
    post.velocities[i] = pre.velocities[i] + h * accelerations[i];
    post.boundary = pre.boundary;
  }

  for (auto i = 0; i < pre.positions.size(); i++) {
    for (auto dim = 0; dim < 2; dim++) {
      auto& proj = post.positions[i][dim];
      if (proj < post.boundary.min[dim]) {
        proj = post.boundary.min[dim];
        post.velocities[i][dim] *= -1;
      } else if (proj > post.boundary.max[dim]) {
        proj = post.boundary.max[dim];
        post.velocities[i][dim] *= -1;
      }
      auto& v = post.velocities[i][dim];
      if (std::abs(v) > 10.0) {
        v = -1.0 * std::signbit(v) * 10.0;
      }
    }
  }
  return post;
}

}  // namespace scarf::model
