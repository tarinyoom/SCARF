#pragma once

#include "animation.hpp"
#include "sph.hpp"

const int width = 640;
const int height = 480;
const int fps = 60;
const int duration = 5;  // seconds
constexpr auto h = 1.0 / static_cast<double>(fps);

auto make_mov(Animation<SPHState> anim) -> int;
