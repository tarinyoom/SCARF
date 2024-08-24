#include "sph.hpp"

#include <array>
#include <cmath>
#include <cstddef>
#include <memory>
#include <numbers>
#include <random>

#include "array.tpp"
#include "bbox.hpp"
#include "color.hpp"
#include "grid.hpp"
#include "matrix.hpp"
#include "pixel.hpp"
#include "rendering.tpp"

template <std::size_t N>
using Vector = std::array<double, N>;

auto init() -> SPHState {
  SPHState state;
  state.boundary = {{-30.0, -22.0}, {30.0, 22.0}};

  std::mt19937 gen(0);
  std::uniform_real_distribution<> p_x(state.boundary.min[0],
                                       state.boundary.max[0]);
  std::uniform_real_distribution<> p_y(state.boundary.min[1],
                                       state.boundary.max[1]);
  std::uniform_real_distribution<> v_x(-1.0, 1.0);
  std::uniform_real_distribution<> v_y(-1.0, 1.0);

  for (auto i = 0; i < state.positions.size(); i++) {
    state.positions[i] = {p_x(gen), p_y(gen)};
    state.velocities[i] = {v_x(gen), v_y(gen)};
  }
  return state;
}

auto step(SPHState&& pre) -> SPHState {
  SPHState post;
  for (int i = 0; i < pre.positions.size(); i++) {
    post.positions[i] = pre.positions[i] + 0.1 * pre.velocities[i];
    post.velocities[i] = pre.velocities[i];
  }
  return post;
}

static Matrix<double, 3, 3> world_to_screen{
    {{{10.0, 0.0, 320.0}, {0.0, 10.0, 240.0}, {0.0, 0.0, 1.0}}}};

static Matrix<double, 3, 3> screen_to_world{
    {{{0.1, 0.0, -32.0}, {0.0, 0.1, -24.0}, {0.0, 0.0, 1.0}}}};

auto kernel(const Vector<2>& a, const Vector<2>& b, double r,
            double scale) -> double {
  auto diff = a - b;
  auto d2 = diff * diff;
  if (d2 > r * r) {
    return 0.0;
  }

  auto c = r * r - d2;
  auto numerator = 4.0 * c * c * c * scale;
  auto denominator = std::numbers::pi * std::pow(r, 8.0);
  return numerator / denominator;
}

auto render_circle(const Vector<2>& center, const Bbox<int, 2>& bounds,
                   Grid<Color>& buffer) {
  for (auto i = bounds.min[0]; i < bounds.max[0]; i++) {
    for (auto j = bounds.min[1]; j < bounds.max[1]; j++) {
      auto p_screen = homogenize(cast_double(std::array<int, 2>{i, j}));
      auto p_world = screen_to_world * p_screen;
      auto dehom = dehomogenize(p_world);
      buffer[i][j] += (Blue * kernel(center, dehom, OUTER_R, 3.0));
      buffer[i][j] += (White * kernel(center, dehom, INNER_R, 1.0));
    }
  }
}

auto quantize(double v) -> std::uint8_t {
  auto clamped = std::clamp(v, 0.0, 1.0);
  auto rounded = std::round(clamped * 255.0);
  auto result = static_cast<std::uint8_t>(rounded);
  return result;
}

auto finalize(Grid<Color>&& buffer) -> Grid<Pixel> {
  auto [m, n] = buffer.size();
  Grid<Pixel> result(m, n, {0, 0, 0});
  for (auto i = 0; i < m; i++) {
    for (auto j = 0; j < n; j++) {
      auto& color = buffer[i][j];
      auto r = quantize(color.r);
      auto g = quantize(color.g);
      auto b = quantize(color.b);
      result[i][j] = {r, g, b};
    }
  }
  return result;
}

auto render(const SPHState& s) -> Grid<Pixel> {
  Grid<Color> buffer(640, 480, Black);
  auto buffer_bounds = buffer.bounds();

  // For each position, render a circle
  for (auto& pos : s.positions) {
    // Find bounding box in world space
    auto pos_w = pos;
    Vector<2> radius_offset = {OUTER_R, OUTER_R};
    Bbox<double, 3> bounds_w = {homogenize(pos_w - radius_offset),
                                homogenize(pos_w + radius_offset)};

    // Convert bounding box to pixel space
    auto bounds_s = bounds_w.transform(world_to_screen);
    auto bounds_p = conservative_integral_bounds(dehomogenize(bounds_s));
    auto clipped_bounds = bounds_p * buffer_bounds;

    // Render circle into buffer
    render_circle(pos, clipped_bounds, buffer);
  }
  return finalize(std::move(buffer));
}

Animation<SPHState> sph_animation = {init, step, render};
