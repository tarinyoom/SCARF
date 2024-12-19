#pragma once

#include <cstddef>

#include "vector.hpp"

namespace scarf::render {

struct Matrix {
  Matrix(std::array<std::array<double, 3>, 3>&& data) : data_(data) {}

  // Matrix-vector multiplication
  auto operator*(const Vector<double, 3>& v) const -> Vector<double, 3> {
    Vector<double, 3> result;
    for (auto i = 0; i < 3; i++) {
      result[i] = data_[i][0] * v[0];
      for (auto j = 1; j < 3; j++) {
        result[i] += data_[i][j] * v[j];
      }
    }
    return result;
  }

 private:
  std::array<std::array<double, 3>, 3> data_;
};

}  // namespace scarf::render
