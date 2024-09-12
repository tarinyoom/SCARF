#pragma once

#include "state.hpp"

namespace scarf::model {

auto step(const SPHState& pre, double h) -> SPHState;

}
