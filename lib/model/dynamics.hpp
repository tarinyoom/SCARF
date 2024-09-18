#pragma once

#include <vector>

#include "state.hpp"
#include "vector.hpp"

namespace scarf::model {

auto compute_densities(const std::vector<Vector<double, 2>>&)
    -> std::vector<double>;

auto compute_pressures(const State& s) -> std::vector<double>;

}  // namespace scarf::model
