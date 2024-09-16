#include "step.hpp"

#include <cmath>
#include <functional>

#include "kernel.hpp"

namespace scarf::model {

// Given a state and an index, perform a symmetric action on all neighbors of an
// index i with index at least i.
auto act_on_neighbors(State& s, int i,
                      std::function<void(State&, int, int)> act) -> void {
  // Act once on self
  act(s, i, i);
  for (int j = i + 1; j < s.n_particles; j++) {
    // Act symmetrically on other
    act(s, i, j);
    act(s, j, i);
  }
}

// Update the density at index i based on index j's contribution.
auto update_density(State& s, int i, int j) -> void {
  auto v = kernel(s.positions[i], s.positions[j], OUTER_R, 1.0);
  s.densities[i] += v;
}

auto update_densities(State& s) -> void {
  for (auto i = 0; i < s.n_particles; i++) {
    s.densities[i] = 0.0;
  }

  for (auto i = 0; i < s.positions.size(); i++) {
    act_on_neighbors(s, i, update_density);
  }
}

auto update_pressures(State& s) -> void {
  auto ref_density = s.n_particles / s.boundary.volume();
  for (auto i = 0; i < s.n_particles; i++) {
    s.pressures[i] = std::pow(s.densities[i], 7.0) - std::pow(ref_density, 7.0);
    s.pressures[i] *= 100000.0;
  }
}

auto update_velocities(State& s, double h) -> void {
  for (auto i = 0; i < s.n_particles; i++) {
    for (auto j = 0; j < i; j++) {
      auto grad = kernel_gradient(s.positions[i], s.positions[j], OUTER_R, 1.0);
      auto l = s.pressures[i] / (s.densities[i] * s.densities[i]);
      auto r = s.pressures[j] / (s.densities[j] * s.densities[j]);
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
    update_densities(post);
    update_pressures(post);
    update_velocities(post, h);

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
