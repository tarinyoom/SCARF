#include "sph.hpp"

#include <array>
#include <cmath>
#include <cstddef>
#include <memory>
#include <numbers>
#include <random>

#include "bbox.hpp"
#include "kernel.cpp"
#include "renderer/grid.hpp"
#include "renderer/pixel.hpp"
#include "renderer/render.hpp"
#include "renderer/scene.hpp"
#include "vector.hpp"

namespace scarf {

SPHState::SPHState(std::size_t n_particles)
    : n_particles(n_particles),
      positions(std::vector<Vector<double, 2>>(n_particles, {0.0, 0.0})),
      velocities(std::vector<Vector<double, 2>>(n_particles, {0.0, 0.0})),
      densities(std::vector<double>(n_particles, 0.0)),
      pressures(std::vector<double>(n_particles, 0.0)) {}

auto init() -> SPHState {
  SPHState state(N_PARTICLES);
  state.boundary = {{-20.0, -10.0}, {20.0, 10.0}};

  std::mt19937 gen(0);
  std::uniform_real_distribution<> p_x(-20.0, 20.0);
  std::uniform_real_distribution<> p_y(-10.0, 10.0);

  for (auto i = 0; i < state.positions.size(); i++) {
    auto x = p_x(gen);
    auto y = p_y(gen);
    state.positions[i] = {x, y};
    state.velocities[i] = {-0.5 * x, -0.5 * y};
  }
  return state;
}

auto update_densities(SPHState& s) -> void {
  for (auto i = 0; i < s.n_particles; i++) {
    s.densities[i] = 0.0;
  }

  for (auto i = 0; i < s.positions.size(); i++) {
    for (auto j = 0; j <= i; j++) {
      auto v = kernel(s.positions[i], s.positions[j], OUTER_R, 1.0);
      if (i != j) {  // asymmetric case
        s.densities[i] += v;
        s.densities[j] += v;
      } else {  // symmetric case
        s.densities[i] += v;
      }
    }
  }
}

auto update_pressures(SPHState& s) -> void {
  auto ref_density = s.n_particles / s.boundary.volume();
  for (auto i = 0; i < s.n_particles; i++) {
    s.pressures[i] = std::pow(s.densities[i], 7.0) - std::pow(ref_density, 7.0);
    s.pressures[i] *= 100000.0;
  }
}

auto update_velocities(SPHState& s, double h) -> void {
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

auto step(SPHState&& pre, double h) -> SPHState {
  SPHState post(pre.n_particles);
  for (auto i = 0; i < pre.positions.size(); i++) {
    post.positions[i] = std::move(pre.positions[i]) + h * pre.velocities[i];
    post.velocities[i] = std::move(pre.velocities[i]);
    post.boundary = std::move(pre.boundary);
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

auto render(const SPHState& state) -> renderer::Grid<renderer::Pixel> {
  renderer::Scene scene;
  scene.points.reserve(state.n_particles);
  for (auto i = 0; i < state.n_particles; i++) {
    scene.points.push_back(state.positions[i]);
    scene.falloff = [](double r2) -> double { return kernel_1d(r2, 3.0, 1.0); };
    scene.outer_radius = 3.0;
    scene.inner_radius = 0.1;
    scene.msaa_linear_density = 1;
  }
  return renderer::render(std::move(scene));
}

auto build_animation() -> Animation<SPHState> { return {init, step, render}; }

}  // namespace scarf
