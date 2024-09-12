#pragma once

#include "state.hpp"

namespace scarf::model {

auto step(const State& pre, double h) -> State;

}
