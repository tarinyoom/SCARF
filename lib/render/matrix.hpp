#pragma once

#include <cstddef>
#include <glm/glm.hpp>

#include "vector.hpp"

namespace scarf::render {

struct Matrix {
  Matrix(glm::dmat3&& data) : data_(data) {}

  // Matrix-vector multiplication
  auto operator*(const glm::dvec3& v) const -> glm::dvec3 {
    glm::dvec3 result;
    for (auto i = 0; i < 3; i++) {
      result[i] = data_[i][0] * v[0];
      for (auto j = 1; j < 3; j++) {
        result[i] += data_[i][j] * v[j];
      }
    }
    return result;
  }

 private:
  glm::dmat3 data_;
};

}  // namespace scarf::render
