#include "state.hpp"

#include <random>

namespace scarf::model {

State::State(std::size_t n_particles)
    : n_particles(n_particles),
      positions(std::vector<Vector<double, 2>>(n_particles, {0.0, 0.0})),
      velocities(std::vector<Vector<double, 2>>(n_particles, {0.0, 0.0})),
      densities(std::vector<double>(n_particles, 0.0)),
      pressures(std::vector<double>(n_particles, 0.0)) {}

auto init() -> State {
  State state(N_PARTICLES);
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

}  // namespace scarf::model
