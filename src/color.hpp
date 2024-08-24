#pragma once

struct Color {
  auto operator+=(const Color& other) -> void {
    r += other.r;
    g += other.g;
    b += other.b;
  }

  double r;
  double g;
  double b;
};

static const Color Black = {0.0, 0.0, 0.0};
static const Color Blue = {0.15, 0.25, 0.75};
