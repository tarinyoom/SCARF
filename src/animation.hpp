#pragma once

#include <functional>

#include "pixel.hpp"
#include "state.hpp"

struct Animation {
	std::function<State()> init;
	std::function<State(State&&)> step;
	std::function<Pixel(const State&, int, int)> render;
};
