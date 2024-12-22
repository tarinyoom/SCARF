#pragma once

#include <glm/glm.hpp>
#include <span>
#include <utility>
#include <vector>

#include "bbox.hpp"

namespace scarf {

template <typename T>
struct Grid {
  Grid(size_t rows, size_t cols, const T& default_value = T())
      : data_(rows * cols, default_value), rows_(rows), cols_(cols) {}

  std::span<T> operator[](size_t row) {
    return std::span<T>(&data_[row * cols_], cols_);
  }

  std::span<const T> operator[](size_t row) const {
    return std::span<const T>(&data_[row * cols_], cols_);
  }

  auto size() const -> std::array<int, 2> { return {rows_, cols_}; }

  auto bounds() const -> Bbox<int, 2> {
    Vector<int, 2> min(0, 0);
    Vector<int, 2> max(rows_, cols_);
    return Bbox<int, 2>(min, max);
  }

  auto glm_bounds() const -> std::pair<glm::ivec2, glm::ivec2> {
    glm::ivec2 min(0, 0);
    glm::ivec2 max(rows_, cols_);
    return {min, max};
  }

 private:
  std::vector<T> data_;  // Linear storage
  int rows_;
  int cols_;
};

}  // namespace scarf
