#pragma once

#include <cstddef>

#include "../vector.hpp"

namespace scarf::renderer {

template <typename T, std::size_t M, std::size_t N>
  requires(M > 0 && N > 0)
struct Matrix {
  Matrix(std::array<std::array<T, N>, M>&& data) : data_(data) {}

  // Matrix-vector multiplication
  auto operator*(const Vector<T, N>& v) const -> Vector<T, M> {
    Vector<T, M> result;
    for (auto i = 0; i < M; i++) {
      result[i] = data_[i][0] * v[0];
      for (auto j = 1; j < N; j++) {
        result[i] += data_[i][j] * v[j];
      }
    }
    return result;
  }

 private:
  std::array<std::array<T, N>, M> data_;
};

}  // namespace scarf::renderer
