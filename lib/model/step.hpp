#pragma once

#include <entt/entt.hpp>

#include "state.hpp"

namespace scarf::model {

auto step(State&, double h) -> void;

}
