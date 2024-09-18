#include "dynamics.hpp"

#include <cmath>

#include "kernel.hpp"

namespace scarf::model {

auto compute_density(const std::vector<Vector<double, 2>>& positions, int i,
                     int j) -> double {
  return kernel(positions[i], positions[j], OUTER_R, 1.0);
}

auto compute_densities(const std::vector<Vector<double, 2>>& positions)
    -> std::vector<double> {
  auto n = positions.size();
  std::vector<double> densities;
  densities.reserve(n);

  for (auto i = 0; i < n; i++) {
    densities[i] = 0.0;
  }

  for (auto i = 0; i < n; i++) {
    densities[i] += compute_density(positions, i, i);
    for (auto j = i + 1; j < n; j++) {
      auto v = compute_density(positions, i, j);
      densities[i] += v;
      densities[j] += v;
    }
  }

  return densities;
}

auto compute_pressures(const State& s, const std::vector<double>& densities)
    -> std::vector<double> {
  auto ref_density = s.n_particles / s.boundary.volume();
  std::vector<double> pressures;
  pressures.reserve(s.n_particles);
  for (auto i = 0; i < s.n_particles; i++) {
    pressures[i] = std::pow(densities[i], 7.0) - std::pow(ref_density, 7.0);
    pressures[i] *= 100000.0;
  }
  return pressures;
}

}  // namespace scarf::model
