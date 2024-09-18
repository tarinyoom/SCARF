#include "step.hpp"

#include <cmath>
#include <functional>

#include "dynamics.hpp"
#include "kernel.hpp"

namespace scarf::model {

auto update_velocities(State& s, const std::vector<double>& pressures,
                       double h) -> void {
  for (auto i = 0; i < s.n_particles; i++) {
    for (auto j = 0; j < i; j++) {
      auto grad = kernel_gradient(s.positions[i], s.positions[j], OUTER_R, 1.0);
      auto l = pressures[i] / (s.densities[i] * s.densities[i]);
      auto r = pressures[j] / (s.densities[j] * s.densities[j]);
      auto acc = h * (l + r) * grad;
      s.velocities[i] += acc;
      s.velocities[j] += -1.0 * acc;
    }
    s.velocities[i] += h * Vector<double, 2>(0.0, 2.0);
  }
}

auto step(const State& pre, double h) -> State {
  State post(pre.n_particles);
  for (auto i = 0; i < pre.positions.size(); i++) {
    post.positions[i] = pre.positions[i] + h * pre.velocities[i];
    post.velocities[i] = pre.velocities[i];
    post.boundary = pre.boundary;
    post.densities = compute_densities(post.positions);
    auto pressures = compute_pressures(post);
    update_velocities(post, pressures, h);

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
