#pragma once

#include <vector>

#include "state.hpp"

namespace scarf::model {

auto compute_pressures(const State& s) -> std::vector<double>;

}
