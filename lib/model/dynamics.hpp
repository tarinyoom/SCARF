#pragma once

#include <vector>

#include "state.hpp"
#include "vector.hpp"

namespace scarf::model {

auto compute_densities(const std::vector<Vector<double, 2>>& pressures)
    -> std::vector<double>;

auto compute_pressures(const State& s, const std::vector<double>& densities)
    -> std::vector<double>;

}  // namespace scarf::model
