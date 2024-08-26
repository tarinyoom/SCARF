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
  std::uniform_real_distribution<> v_x(-30.0, 30.0);
  std::uniform_real_distribution<> v_y(-30.0, 30.0);

  for (auto i = 0; i < state.positions.size(); i++) {
    state.positions[i] = {p_x(gen), p_y(gen)};
    state.velocities[i] = {v_x(gen), v_y(gen)};
  }
  return state;
}

auto step(SPHState&& pre, double h) -> SPHState {
  SPHState post;
  for (auto i = 0; i < pre.positions.size(); i++) {
    post.positions[i] = std::move(pre.positions[i]) + h * pre.velocities[i];
    post.velocities[i] = std::move(pre.velocities[i]);
    post.boundary = std::move(pre.boundary);

    for (auto dim = 0; dim < 2; dim++) {
      auto& proj = post.positions[i][dim];
      if (proj < post.boundary.min[dim]) {
        proj = post.boundary.min[dim];
        post.velocities[i][dim] *= -1;
      } else if (proj > post.boundary.max[dim]) {
        proj = post.boundary.max[dim];
        post.velocities[i][dim] *= -1;
      }
    }
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

auto get_light(const std::array<double, 2>& p,
               const std::array<double, 2>& center) -> Color {
  auto p_screen = homogenize(p);
  auto p_world = screen_to_world * p_screen;
  auto dehom = dehomogenize(p_world);
  return Blue * kernel(center, dehom, OUTER_R, 3.0) +
         White * kernel(center, dehom, INNER_R, 1.0);
}

auto clamp(const Color& c) -> Color {
  return {std::clamp(c.r, 0.0, 1.0), std::clamp(c.g, 0.0, 1.0),
          std::clamp(c.b, 0.0, 1.0)};
}

auto render_circle(const Vector<2>& center, const Bbox<int, 2>& bounds,
                   Grid<Color>& buffer) {
  for (auto i = bounds.min[0]; i < bounds.max[0]; i++) {
    for (auto j = bounds.min[1]; j < bounds.max[1]; j++) {
      Color c = {0.0, 0.0, 0.0};
      constexpr auto ss_d = 1.0 / static_cast<double>(MSAA_LINEAR_DENSITY);
      for (auto di = 0; di < MSAA_LINEAR_DENSITY; di++) {
        for (auto dj = 0; dj < MSAA_LINEAR_DENSITY; dj++) {
          // Subsampling at -.4, -.2, 0, .2, .4
          auto i_subsample = static_cast<double>(i) - -0.5 +
                             ss_d * (0.5 + static_cast<double>(di));
          auto j_subsample =
              static_cast<double>(j) - 0.4 + 0.2 * static_cast<double>(dj);
          Color dc = get_light(std::array<double, 2>{i_subsample, j_subsample},
                               center) *
                     (1.0 / MSAA_LINEAR_DENSITY * MSAA_LINEAR_DENSITY);
          c += clamp(dc);
        }
      }
      buffer[i][j] += c;
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
