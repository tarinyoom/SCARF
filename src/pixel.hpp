#pragma once

#include <cstdint>

struct Pixel {
  std::uint8_t r;
  std::uint8_t g;
  std::uint8_t b;
};

static const Pixel Black = {0, 0, 0};
static const Pixel Blue = {127, 127, 255};