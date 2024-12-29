#pragma once

#include "state.hpp"

namespace scarf::model {

auto step(State&, double h) -> void;

}
