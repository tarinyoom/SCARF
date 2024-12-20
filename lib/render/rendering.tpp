#pragma once

#include <algorithm>
#include <array>
#include <cstddef>

#include "bbox.hpp"
#include "vector.hpp"

namespace scarf::render {

template <std::size_t N>
auto cast_double(const std::array<int, N>& v) -> std::array<double, N> {
  std::array<double, N> result;
  for (auto i = 0; i < N; i++) {
    result[i] = static_cast<double>(v[i]);
  }
  return result;
}

template <std::size_t N>
auto conservative_integral_bounds(const Bbox<double, N>& b) -> Bbox<int, N> {
  Vector<int, N> new_min;
  Vector<int, N> new_max;
  for (auto i = 0; i < N; i++) {
    new_min[i] = static_cast<int>(std::floor(b.min[i]));
    new_max[i] = static_cast<int>(std::floor(b.max[i]));
  }
  return {new_min, new_max};
}

auto homogenize(const Vector<double, 2>& v) -> Vector<double, 3> {
  Vector<double, 3> result;
  for (std::size_t i = 0; i < 2; ++i) {
    result.value[i] = v.value[i];
  }
  result.value[2] = 1.0;
  return result;
}

auto dehomogenize(const Vector<double, 3>& v) -> Vector<double, 2> {
  Vector<double, 2> result;
  auto scale = 1.0 / v[2];
  for (auto i = 0; i < 2; i++) {
    result[i] = v[i] * scale;
  }
  return result;
}

auto dehomogenize(const Bbox<double, 3>& b) -> const Bbox<double, 2> {
  auto new_min = dehomogenize(b.min);
  auto new_max = dehomogenize(b.max);
  return {new_min, new_max};
}

}  // namespace scarf::render
