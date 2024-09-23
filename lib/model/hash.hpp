#pragma once

#include <functional>

#include "bbox.hpp"
#include "vector.hpp"

namespace scarf::model {

auto map_neighbors(const std::vector<Vector<double, 2>>& positions,
                   const Bbox<double, 2>& bounds)
    -> std::function<std::vector<int>(int)>;

namespace detail {

auto hash_coords(const Vector<int, 2>& coords,
                 const Vector<int, 2>& cell_counts) -> int;

auto discretize_coords(const Vector<double, 2>& point,
                       const Vector<double, 2>& anchor,
                       const Vector<double, 2>& cell_sizes) -> Vector<int, 2>;

auto build_hash(const Vector<double, 2>& anchor,
                const Vector<int, 2>& cell_counts,
                const Vector<double, 2>& cell_sizes)
    -> std::function<int(const Vector<double, 2>&)>;

}  // namespace detail

}  // namespace scarf::model
