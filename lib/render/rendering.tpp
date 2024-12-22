#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <glm/glm.hpp>
#include <utility>

#include "bbox.hpp"
#include "vector.hpp"

namespace scarf::render {

auto conservative_integral_bounds(const Bbox<double, 2>& b) -> Bbox<int, 2> {
  Vector<int, 2> new_min;
  Vector<int, 2> new_max;
  for (auto i = 0; i < 2; i++) {
    new_min[i] = static_cast<int>(std::floor(b.min[i]));
    new_max[i] = static_cast<int>(std::floor(b.max[i]));
  }
  return {new_min, new_max};
}

auto homogenize(const Vector<double, 2>& v) -> glm::dvec3 {
  glm::dvec3 result;
  for (std::size_t i = 0; i < 2; ++i) {
    result[i] = v.value[i];
  }
  result[2] = 1.0;
  return result;
}

auto dehomogenize(const glm::dvec3& v) -> Vector<double, 2> {
  Vector<double, 2> result;
  auto scale = 1.0 / v[2];
  for (auto i = 0; i < 2; i++) {
    result[i] = v[i] * scale;
  }
  return result;
}

auto dehomogenize(const std::pair<glm::dvec3, glm::dvec3>& b)
    -> const Bbox<double, 2> {
  auto new_min = dehomogenize(b.first);
  auto new_max = dehomogenize(b.second);
  return {new_min, new_max};
}

}  // namespace scarf::render
