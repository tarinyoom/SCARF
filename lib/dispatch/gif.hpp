#pragma once

#include "animation.hpp"
#include "config.hpp"

namespace scarf::dispatch {

auto make_gif(Animation anim, const Config& cfg) -> int;

}
