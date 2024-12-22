#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <glm/glm.hpp>
#include <utility>

namespace scarf::render {

auto conservative_integral_bounds(const std::pair<glm::dvec2, glm::dvec2>& b)
    -> std::pair<glm::ivec2, glm::ivec2> {
  glm::ivec2 new_min;
  glm::ivec2 new_max;
  for (auto i = 0; i < 2; i++) {
    new_min[i] = static_cast<int>(std::floor(b.first[i]));
    new_max[i] = static_cast<int>(std::floor(b.second[i])) + 1;
  }
  return {new_min, new_max};
}

auto homogenize(const glm::dvec2& v) -> glm::dvec3 {
  glm::dvec3 result;
  for (std::size_t i = 0; i < 2; ++i) {
    result[i] = v[i];
  }
  result[2] = 1.0;
  return result;
}

auto dehomogenize(const glm::dvec3& v) -> glm::dvec2 {
  glm::dvec2 result;
  auto scale = 1.0 / v[2];
  for (auto i = 0; i < 2; i++) {
    result[i] = v[i] * scale;
  }
  return result;
}

auto dehomogenize(const std::pair<glm::dvec3, glm::dvec3>& b)
    -> std::pair<glm::dvec2, glm::dvec2> {
  auto new_min = dehomogenize(b.first);
  auto new_max = dehomogenize(b.second);
  return {new_min, new_max};
}

}  // namespace scarf::render
