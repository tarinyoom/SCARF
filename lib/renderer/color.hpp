#pragma once

namespace scarf::renderer {

struct Color {
  auto operator+=(const Color& other) -> void {
    r += other.r;
    g += other.g;
    b += other.b;
  }

  auto operator*(double f) const -> Color { return {r * f, g * f, b * f}; }

  auto operator+(const Color& other) const -> Color {
    return {r + other.r, g + other.g, b + other.b};
  }

  double r;
  double g;
  double b;
};

static const Color Black = {0.0, 0.0, 0.0};
static const Color Blue = {0.2, 0.4, 0.8};
static const Color White = {1.0, 1.0, 1.0};

}  // namespace scarf::renderer
