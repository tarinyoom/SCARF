#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <string>

#include "matrix.hpp"

template <typename T, std::size_t N>
struct Bbox {
  Bbox() : min(), max() {}

  Bbox(const std::array<T, N> min, const std::array<T, N>& max)
      : min(min), max(max) {}

  auto operator*(const Bbox<T, N>& other) const -> Bbox {
    Bbox<T, N> result;
    for (auto i = 0; i < N; i++) {
      result.min[i] = std::max(min[i], other.min[i]);
      result.max[i] = std::min(max[i], other.max[i]);
    }
    return result;
  }

  auto transform(const Matrix<T, N, N>& a) const -> Bbox<T, N> {
    return Bbox<T, N>(a * min, a * max);
  }

  std::array<T, N> min;
  std::array<T, N> max;
};
