#pragma once

#include "animation.hpp"
#include "model/state.hpp"

namespace scarf {

const int width = 640;
const int height = 480;
const int fps = 60;
const int duration = 5;  // seconds
constexpr auto TIMESTEP = 1.0 / static_cast<double>(fps);

auto make_mov(Animation anim) -> int;

}  // namespace scarf
