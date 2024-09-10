#pragma once

#include "state.hpp"

namespace scarf::model {

auto step(SPHState&& pre, double h) -> SPHState;

}
