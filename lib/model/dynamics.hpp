#pragma once

#include <functional>
#include <vector>

#include "state.hpp"
#include "vector.hpp"

namespace scarf::model {

auto compute_densities(std::function<std::vector<int>(int)> neighbor_map,
                       const std::vector<Vector<double, 2>>& positions)
    -> std::vector<double>;

auto compute_pressures(double reference_density,
                       const std::vector<double>& densities)
    -> std::vector<double>;

auto compute_accelerations(std::function<std::vector<int>(int)> neighbor_map,
                           const std::vector<Vector<double, 2>>& positions,
                           const std::vector<double>& densities,
                           const std::vector<double>& pressures)
    -> std::vector<Vector<double, 2>>;

}  // namespace scarf::model
