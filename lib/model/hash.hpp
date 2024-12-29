#pragma once

#include <functional>
#include <glm/glm.hpp>
#include <utility>

namespace scarf::model {

auto map_neighbors(const std::vector<glm::dvec2>& positions,
                   const std::pair<glm::dvec2, glm::dvec2>& bounds)
    -> std::function<std::vector<int>(int)>;

auto hash_coords(const glm::dvec2& coords, double r) -> int;

namespace detail {

auto hash_coords(const glm::ivec2& coords, const glm::ivec2& cell_counts)
    -> int;

auto discretize_coords(const glm::dvec2& point, const glm::dvec2& anchor,
                       const glm::dvec2& cell_sizes) -> glm::ivec2;

auto build_hash(const glm::dvec2& anchor, const glm::ivec2& cell_counts,
                const glm::dvec2& cell_sizes)
    -> std::function<int(const glm::dvec2&)>;

}  // namespace detail

}  // namespace scarf::model
