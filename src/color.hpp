#pragma once

struct Color {
  auto operator+=(const Color& other) -> void {
    r += other.r;
    g += other.g;
    b += other.b;
  }

  auto operator*(double f) const -> Color { return {r * f, g * f, b * f}; }

  double r;
  double g;
  double b;
};

static const Color Black = {0.0, 0.0, 0.0};
static const Color Red = {1.0, 0.0, 0.0};
static const Color White = {1.0, 1.0, 1.0};
