#pragma once

#include <vector>

#include "state.hpp"
#include "vector.hpp"

namespace scarf::model {

auto compute_densities(const std::vector<Vector<double, 2>>& positions)
    -> std::vector<double>;

auto compute_pressures(const State& s, const std::vector<double>& densities)
    -> std::vector<double>;

auto compute_accelerations(const std::vector<Vector<double, 2>>& positions,
                           const std::vector<double>& densities,
                           const std::vector<double>& pressures)
    -> std::vector<Vector<double, 2>>;

}  // namespace scarf::model
