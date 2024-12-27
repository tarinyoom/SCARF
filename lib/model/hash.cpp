#include "hash.hpp"

#include "state.hpp"

namespace scarf::model {

auto map_neighbors(const std::vector<glm::dvec2>& positions,
                   const std::pair<glm::dvec2, glm::dvec2>& bounds)
    -> std::function<std::vector<int>(int)> {
  // Construct grid to conservatively cover entire domain
  auto padding = OUTER_R * glm::dvec2(0.5, 0.5);
  auto grid_bounds = std::pair<glm::dvec2, glm::dvec2>(bounds.first - padding,
                                                       bounds.second + padding);

  // Grid cells each have the same dimensions, and must not underestimate the
  // kernel radius
  glm::ivec2 cell_counts;
  glm::dvec2 cell_sizes;
  for (auto i = 0; i < 2; i++) {
    auto grid_length = grid_bounds.second[i] - grid_bounds.first[i];
    cell_counts[i] = static_cast<int>(std::ceil(grid_length / OUTER_R));
    cell_sizes[i] = grid_length / static_cast<double>(cell_counts[i]);
  }

  auto hash = detail::build_hash(grid_bounds.first, cell_counts, cell_sizes);

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
        detail::discretize_coords(positions[i], grid_bounds.first, cell_sizes);
    for (auto i = -1; i <= 1; i++) {
      for (auto j = -1; j <= 1; j++) {
        auto n = p + glm::ivec2(i, j);
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

constexpr glm::ivec2 primes{15485863, 7368787};
constexpr int offset = 1299709;

auto hash_coords(const glm::dvec2& coords, double r) -> int {
  // Map coords to grid indices
  int grid_x = static_cast<int>(std::floor(coords.x / r));
  int grid_y = static_cast<int>(std::floor(coords.y / r));

  // Compute the spatial hash
  return (grid_x * primes.x + grid_y * primes.y + offset);
}

namespace detail {

auto hash_coords(const glm::ivec2& coords, const glm::ivec2& cell_counts)
    -> int {
  auto idx = 0;
  for (auto i = 0; i < 2; i++) {
    idx *= cell_counts[i];
    idx += coords[i];
  }
  return idx;
}

auto discretize_coords(const glm::dvec2& point, const glm::dvec2& anchor,
                       const glm::dvec2& cell_sizes) -> glm::ivec2 {
  glm::ivec2 discretized;
  for (auto i = 0; i < 2; i++) {
    auto continuous = (point[i] - anchor[i]) / cell_sizes[i];
    discretized[i] = static_cast<int>(std::floor(continuous));
  }
  return discretized;
}

auto build_hash(const glm::dvec2& anchor, const glm::ivec2& cell_counts,
                const glm::dvec2& cell_sizes)
    -> std::function<int(const glm::dvec2&)> {
  return [=](const glm::dvec2& point) -> int {
    auto coords = discretize_coords(point, anchor, cell_sizes);
    return hash_coords(coords, cell_counts);
  };
}

}  // namespace detail

}  // namespace scarf::model
