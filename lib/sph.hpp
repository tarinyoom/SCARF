#pragma once

#include "animation.hpp"
#include "model/state.hpp"

namespace scarf {

auto build_animation() -> Animation<model::SPHState>;

}  // namespace scarf
