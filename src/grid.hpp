#pragma once

#include <vector>

#include "bbox.hpp"

template <typename T>
struct Grid {
  Grid(int m, int n, const T& val)
      : m_(m),
        n_(n),
        data_(std::vector<std::vector<T>>(m, std::vector<T>(n, val))) {}

  auto operator[](int i) -> std::vector<T>& { return data_[i]; }

  auto bounds() -> Bbox<int, 2> { return {{0, 0}, {m_, n_}}; }

 private:
  int m_;
  int n_;
  std::vector<std::vector<T>> data_;
};
