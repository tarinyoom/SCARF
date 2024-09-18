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
    densities.push_back(0.0);
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

auto compute_pressures(double reference_density,
                       const std::vector<double>& densities)
    -> std::vector<double> {
  auto n = densities.size();
  std::vector<double> pressures;
  pressures.reserve(n);
  for (auto i = 0; i < n; i++) {
    pressures[i] =
        std::pow(densities[i], 7.0) - std::pow(reference_density, 7.0);
    pressures[i] *= 100000.0;
  }
  return pressures;
}

auto compute_accelerations(const std::vector<Vector<double, 2>>& positions,
                           const std::vector<double>& densities,
                           const std::vector<double>& pressures)
    -> std::vector<Vector<double, 2>> {
  auto n = positions.size();
  std::vector<Vector<double, 2>> accelerations;
  accelerations.reserve(n);

  for (auto i = 0; i < n; i++) {
    accelerations.push_back(Vector<double, 2>(0.0, 10.0));
  }

  for (auto i = 0; i < n; i++) {
    for (auto j = 0; j < i; j++) {
      auto grad = kernel_gradient(positions[i], positions[j], OUTER_R, 1.0);
      auto l = pressures[i] / (densities[i] * densities[i]);
      auto r = pressures[j] / (densities[j] * densities[j]);
      auto acc = (l + r) * grad;
      accelerations[i] += acc;
      accelerations[j] += -1.0 * acc;
    }
  }
  return accelerations;
}

}  // namespace scarf::model
