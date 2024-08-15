#pragma once

#include <array>
#include <vector>
#include <cstddef>

#include "animation.hpp"
#include "vector.hpp"

constexpr std::size_t N_PARTICLES = 3;

constexpr auto pixel_size = 0.1;
constexpr auto pixels_per_unit = 1.0 / pixel_size;

struct SPHState {
	std::array<Vector<2>, N_PARTICLES> positions;
};

extern Animation<SPHState> sph_animation;
