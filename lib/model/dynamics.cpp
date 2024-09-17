#include "dynamics.hpp"

#include <cmath>

namespace scarf::model {

auto compute_pressures(const State& s) -> std::vector<double> {
  auto ref_density = s.n_particles / s.boundary.volume();
  std::vector<double> pressures;
  pressures.reserve(s.n_particles);
  for (auto i = 0; i < s.n_particles; i++) {
    pressures[i] = std::pow(s.densities[i], 7.0) - std::pow(ref_density, 7.0);
    pressures[i] *= 100000.0;
  }
  return pressures;
}

}  // namespace scarf::model
