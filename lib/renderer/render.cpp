#include "render.hpp"

#include <array>

#include "array.tpp"
#include "color.hpp"
#include "matrix.hpp"
#include "rendering.tpp"

namespace scarf::renderer {

template <std::size_t N>
using Vector = std::array<double, N>;

static Matrix<double, 3, 3> world_to_screen{
    {{{10.0, 0.0, 320.0}, {0.0, 10.0, 240.0}, {0.0, 0.0, 1.0}}}};

static Matrix<double, 3, 3> screen_to_world{
    {{{0.1, 0.0, -32.0}, {0.0, 0.1, -24.0}, {0.0, 0.0, 1.0}}}};

auto get_light(const std::array<double, 2>& p,
               const std::array<double, 2>& center, const Scene& s) -> Color {
  auto p_screen = homogenize(p);
  auto p_world = screen_to_world * p_screen;
  auto dehom = dehomogenize(p_world);
  std::array<double, 2> diff = {p[0] - center[0], p[1] - center[1]};
  auto r2 = diff[0] * diff[0] + diff[1] * diff[1];
  if (r2 <= 0.01) {
    return White;
  } else {
    return Blue * s.falloff(r2);
  }
}

auto clamp(const Color& c) -> Color {
  return {std::clamp(c.r, 0.0, 1.0), std::clamp(c.g, 0.0, 1.0),
          std::clamp(c.b, 0.0, 1.0)};
}

// Center and bounds passed in pixel space
auto render_circle(const Vector<2>& center, const Bbox<int, 2>& bounds,
                   Grid<Color>& buffer, const Scene& s) {
  Bbox<int, 2> clipped_bounds = bounds * buffer.bounds();
  for (auto i = clipped_bounds.min[0]; i < clipped_bounds.max[0]; i++) {
    for (auto j = clipped_bounds.min[1]; j < clipped_bounds.max[1]; j++) {
      Color c = {0.0, 0.0, 0.0};
      const auto ss_d = 1.0 / static_cast<double>(s.msaa_linear_density);
      for (auto di = 0; di < s.msaa_linear_density; di++) {
        for (auto dj = 0; dj < s.msaa_linear_density; dj++) {
          // Subsampling at -.4, -.2, 0, .2, .4
          auto i_subsample = static_cast<double>(i) - -0.5 +
                             ss_d * (0.5 + static_cast<double>(di));
          auto j_subsample =
              static_cast<double>(j) - 0.4 + 0.2 * static_cast<double>(dj);
          auto c_w = screen_to_world * homogenize(std::array<double, 2>{
                                           i_subsample, j_subsample});
          Color dc = get_light(dehomogenize(c_w), center, s) *
                     (1.0 / s.msaa_linear_density / s.msaa_linear_density);
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

auto render(Scene&& s) -> Grid<Pixel> {
  Grid<Color> buffer(640, 480, Black);

  // For each position, render a circle
  for (auto& pos : s.points) {
    // Find bounding box in world space
    auto pos_w = pos;
    Vector<2> radius_offset = {s.outer_radius, s.outer_radius};
    Bbox<double, 3> bounds_w = {
        homogenize(std::array<double, 2>{pos_w[0] - radius_offset[0],
                                         pos_w[1] - radius_offset[1]}),
        homogenize(std::array<double, 2>{pos_w[0] + radius_offset[0],
                                         pos_w[1] + radius_offset[1]})};

    // Convert bounding box to pixel space
    auto bounds_s = bounds_w.transform(world_to_screen);
    auto bounds_p = conservative_integral_bounds(dehomogenize(bounds_s));

    // Render circle into buffer
    render_circle(pos, bounds_p, buffer, s);
    auto sz = buffer.size();
  }

  return finalize(std::move(buffer));
}

}  // namespace scarf::renderer
