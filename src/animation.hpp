#pragma once

#include <functional>

#include "pixel.hpp"

template <typename State>
struct Animation {
	std::function<State()> init;
	std::function<State(State&&)> step;
	std::function<Pixel(const State&, int, int)> render;
};
