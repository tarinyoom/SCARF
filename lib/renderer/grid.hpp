#pragma once

#include <vector>

#include "bbox.hpp"

namespace scarf::renderer {

template <typename T>
struct Grid {
  Grid(int m, int n, const T& val)
      : m_(m),
        n_(n),
        data_(std::vector<std::vector<T>>(m, std::vector<T>(n, val))) {}

  auto operator[](int i) -> std::vector<T>& { return data_[i]; }

  auto size() const -> std::array<int, 2> { return {m_, n_}; }

  auto bounds() const -> Bbox<int, 2> {
    Vector<int, 2> min(0, 0);
    Vector<int, 2> max(m_, n_);
    return Bbox<int, 2>(min, max);
  }

 private:
  int m_;
  int n_;
  std::vector<std::vector<T>> data_;
};

}  // namespace scarf::renderer
