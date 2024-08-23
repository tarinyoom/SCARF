#include "sph.hpp"

#include <array>
#include <cmath>
#include <cstddef>
#include <memory>

#include "bbox.hpp"
#include "grid.hpp"
#include "matrix.hpp"
#include "pixel.hpp"
#include "rendering.tpp"

template <std::size_t N>
using Vector = std::array<double, N>;

auto init() -> SPHState {
  std::array<Vector<2>, N_PARTICLES> positions;
  for (int i = 0; i < N_PARTICLES; i++) {
    positions[i] = {3.0 + i * 1.0, 3.0 + i * 1.0};
  }
  return {positions};
}

auto step_vector(Vector<2>&& pre) -> Vector<2> {
  auto rotation_velocity = 0.05;
  auto s = std::sin(rotation_velocity);
  auto c = std::cos(rotation_velocity);
  return {c * pre[0] - s * pre[1], s * pre[0] + c * pre[1]};
}

auto step(SPHState&& pre) -> SPHState {
  SPHState post;
  for (int i = 0; i < N_PARTICLES; i++) {
    post.positions[i] = step_vector(std::move(pre.positions[i]));
  }
  return post;
}

static Matrix<double, 3, 3> world_to_screen{
    {{{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}}}};

static Matrix<double, 3, 3> screen_to_world{
    {{{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}}}};

auto render_circle(const Vector<2>& center, const Bbox<int, 2>& bounds,
                   Grid<Pixel>& buffer) {
  for (auto i = bounds.min[0]; i < bounds.max[0]; i++) {
    for (auto j = bounds.min[1]; j < bounds.max[1]; j++) {
      auto p = cast_double(std::array{i, j});
      auto diff = p - center;
      if (diff * diff < 0.5) {
        buffer[i][j] = Blue;
      }
    }
  }
}

auto render(const SPHState& s) -> Grid<Pixel> {
  Grid<Pixel> buffer(640, 480, Black);
  auto buffer_bounds = buffer.bounds();

  // For each position, render a circle
  for (auto& pos : s.positions) {
    // Find bounding box in world space
    auto pos_w = homogenize(pos);
    Vector<3> radius_offset = {0.5, 0.5, 1.0};
    Bbox<double, 3> bounds_w = {pos_w - radius_offset, pos_w + radius_offset};

    // Convert bounding box to pixel space
    auto bounds_s = bounds_w.transform(world_to_screen);
    auto bounds_p = conservative_integral_bounds(dehomogenize(bounds_s));
    auto clipped_bounds = bounds_p * buffer_bounds;

    // Render circle into buffer
    render_circle(pos, clipped_bounds, buffer);
  }
  return buffer;
}

Animation<SPHState> sph_animation = {init, step, render};
