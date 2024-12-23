#include "state.hpp"

#include <random>

namespace scarf::model {

State::State(std::size_t n_particles)
    : n_particles(n_particles),
      positions(std::vector<glm::dvec2>(n_particles, {0.0, 0.0})),
      velocities(std::vector<glm::dvec2>(n_particles, {0.0, 0.0})) {}

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

  auto& boundary = state.boundary;
  auto volume = (boundary.second[0] - boundary.first[0]) *
                (boundary.second[1] - boundary.first[1]);

  state.reference_density = static_cast<double>(state.n_particles) / volume;

  return state;
}

auto init_entt() -> EnTTState {
  EnTTState state;
  state.boundary = {{-20.0, -10.0}, {20.0, 10.0}};

  std::mt19937 gen(0);
  std::uniform_real_distribution<> p_x(-20.0, 20.0);
  std::uniform_real_distribution<> p_y(-10.0, 10.0);

  for (auto i = 0; i < N_PARTICLES; i++) {
    auto& registry = state.registry;
    auto x = p_x(gen);
    auto y = p_y(gen);

    auto entity = registry.create();
    registry.emplace<Position>(entity, Position{glm::dvec2(x, y)});
    registry.emplace<Velocity>(entity,
                               Velocity{glm::dvec2(-0.5 * x, -0.5 * y)});
  }

  auto& boundary = state.boundary;
  auto volume = (boundary.second[0] - boundary.first[0]) *
                (boundary.second[1] - boundary.first[1]);

  state.reference_density = static_cast<double>(state.n_particles) / volume;

  return state;
}

}  // namespace scarf::model
