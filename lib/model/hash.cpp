#include "hash.hpp"

#include "state.hpp"

namespace scarf::model {

auto map_neighbors(const std::vector<Vector<double, 2>>& positions,
                   const Bbox<double, 2>& bounds)
    -> std::function<std::vector<int>(int)> {
  // Construct grid to conservatively cover entire domain
  auto padding = OUTER_R * Vector<double, 2>(0.5, 0.5);
  auto grid_bounds =
      Bbox<double, 2>(bounds.min - padding, bounds.max + padding);

  // Grid cells each have the same dimensions, and must not underestimate the
  // kernel radius
  Vector<int, 2> cell_counts;
  Vector<double, 2> cell_sizes;
  for (auto i = 0; i < 2; i++) {
    auto grid_length = grid_bounds.max[i] - grid_bounds.min[i];
    cell_counts[i] = static_cast<int>(std::ceil(grid_length / OUTER_R));
    cell_sizes[i] = grid_length / static_cast<double>(cell_counts[i]);
  }

  auto hash = detail::build_hash(grid_bounds.min, cell_counts, cell_sizes);

  auto n_cells = cell_counts[0] * cell_counts[1];
  auto cell_contents =
      std::vector<std::vector<int>>(n_cells, std::vector<int>());

  for (auto i = 0; i < positions.size(); i++) {
    auto p = positions[i];
    auto cell_idx = hash(p);
    cell_contents[cell_idx].push_back(i);
  }

  return [=](int i) -> std::vector<int> {
    std::vector<int> neighbors;
    auto p =
        detail::discretize_coords(positions[i], grid_bounds.min, cell_sizes);
    for (auto i = -1; i <= 1; i++) {
      for (auto j = -1; j <= 1; j++) {
        auto n = p + Vector<int, 2>(i, j);
        if (n[0] >= 0 && n[1] >= 0 && n[0] < cell_counts[0] &&
            n[1] < cell_counts[1]) {
          auto n_idx = detail::hash_coords(n, cell_counts);
          for (auto& elem : cell_contents[n_idx]) {
            neighbors.push_back(elem);
          }
        }
      }
    }
    return neighbors;
  };
}

namespace detail {

auto hash_coords(const Vector<int, 2>& coords,
                 const Vector<int, 2>& cell_counts) -> int {
  auto idx = 0;
  for (auto i = 0; i < 2; i++) {
    idx *= cell_counts[i];
    idx += coords[i];
  }
  return idx;
}

auto discretize_coords(const Vector<double, 2>& point,
                       const Vector<double, 2>& anchor,
                       const Vector<double, 2>& cell_sizes) -> Vector<int, 2> {
  Vector<int, 2> discretized;
  for (auto i = 0; i < 2; i++) {
    auto continuous = (point[i] - anchor[i]) / cell_sizes[i];
    discretized[i] = static_cast<int>(std::floor(continuous));
  }
  return discretized;
}

auto build_hash(const Vector<double, 2>& anchor,
                const Vector<int, 2>& cell_counts,
                const Vector<double, 2>& cell_sizes)
    -> std::function<int(const Vector<double, 2>&)> {
  return [=](const Vector<double, 2>& point) -> int {
    auto coords = discretize_coords(point, anchor, cell_sizes);
    return hash_coords(coords, cell_counts);
  };
}

}  // namespace detail

}  // namespace scarf::model
