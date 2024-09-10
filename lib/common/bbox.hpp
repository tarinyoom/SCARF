#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <limits>
#include <string>

#include "vector.hpp"

namespace scarf {

template <typename T, std::size_t N>
  requires(N > 0)
struct Bbox {
  Bbox() : min(), max() {
    min.value.fill(std::numeric_limits<T>::min());
    max.value.fill(std::numeric_limits<T>::max());
  }

  Bbox(const Vector<T, N>& min, const Vector<T, N>& max) : min(min), max(max) {}

  auto operator*(const Bbox<T, N>& other) const -> Bbox {
    Bbox<T, N> result;
    for (auto i = 0; i < N; i++) {
      result.min[i] = std::max(min[i], other.min[i]);
      result.max[i] = std::min(max[i], other.max[i]);
    }
    return result;
  }

  auto volume() -> T {
    auto vol = max[0] - min[0];  // N > 0
    for (auto i = 1; i < N; i++) {
      vol *= max[i] - min[i];
    }
    return vol;
  }

  Vector<T, N> min;
  Vector<T, N> max;
};

}  // namespace scarf
