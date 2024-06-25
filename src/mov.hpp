#pragma once

#include <functional>

#include "pixel.hpp"
#include "blackbox.hpp"

const int width = 640;
const int height = 480;
const int fps = 30;
const int duration = 5; // seconds

auto make_mov(std::function<State()> init, std::function<State(State&&)> step, std::function<Pixel(const State&, int, int)> render) -> int;
