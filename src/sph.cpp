#include "sph.hpp"

#include <array>
#include <iterator>
#include <cstddef>
#include <algorithm>
#include <cmath>
#include <numbers>
#include <memory>

#include "grid.hpp"
#include "pixel.hpp"

auto init() -> SPHState {
	std::array<Vector<2>, N_PARTICLES> positions;
	for (int i = 0; i < N_PARTICLES; i++) {
		positions[i] = {3.0 + i * 2.5, 3.0 + i * 2.5};
	}
	return {positions};
}

auto step_vector(Vector<2>&& pre) -> Vector<2> {
	auto rotation_velocity = 0.05;
	auto s = std::sin(rotation_velocity);
	auto c = std::cos(rotation_velocity);
	return { c * pre.data_[0] - s * pre.data_[1], s * pre.data_[0] + c * pre.data_[1] };
}

auto step(SPHState&& pre) -> SPHState {
	SPHState post;
	for (int i = 0; i < N_PARTICLES; i++) {
		post.positions[i] = step_vector(std::move(pre.positions[i]));
	}
	return post;
}

auto initialize_empty_buffer() -> std::vector<std::vector<Pixel>> {
	std::vector<std::vector<Pixel>> buffer;
	buffer.reserve(640);
	for (int i = 0; i < 640; i++) {
		std::vector<Pixel> row;
		row.reserve(480);
		for (int j = 0; j < 480; j++) {
			row.push_back({0, 0, 0});
		}
		buffer.push_back(std::move(row));
	}
	return buffer;
}

auto world_to_screen(const Vector<2>& v) -> std::pair<int, int> {
	return {
		static_cast<int>(std::round(v.data_[0] * pixels_per_unit) + 320.0),
		static_cast<int>(std::round(v.data_[1] * pixels_per_unit) + 240.0)
		};
}

auto screen_to_world(const std::pair<int, int>& v) -> Vector<2> {
	return {
		(static_cast<double>(v.first) - 320.0) * 0.1,
		(static_cast<double>(v.second) - 240.0) * 0.1
    };
}

// Iterate through a bounding box for the sphere, checking inclusion
auto circle_interior(const Vector<2>& center, double radius) -> std::vector<std::pair<int, int>> {
	std::vector<std::pair<int, int>> pts;

	// Find center pixel of circle
	auto center_s = world_to_screen(center);

	// Find conservative estimate of radius, in pixels
	auto radius_s = static_cast<int>(std::ceil(radius / pixel_size));

	// Scan across bounding box, including pixels within range
	for (int i = center_s.first - radius_s; i <= center_s.first + radius_s; i++) {
		for (int j = center_s.second - radius_s; j <= center_s.second + radius_s; j++) {
			auto world = screen_to_world({i, j});
			auto diff = world - center;
			if (diff * diff < radius * radius) {
				pts.push_back({i, j});
			}
		}
	}

	return pts;
}

auto render(const SPHState& s) -> Grid {
	Grid buffer = { initialize_empty_buffer() };
	for (auto& p : s.positions) {
		auto interior = circle_interior(p, 0.3);
		for (auto& px : interior) {
			buffer.data_[px.first][px.second] = Pixel {127, 127, 255};
		}
	}
	return buffer;
}

Animation<SPHState> sph_animation = {
    init,
    step,
    render
};
