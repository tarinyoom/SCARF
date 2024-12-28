#pragma once

#include <functional>
#include <glm/glm.hpp>
#include <ranges>
#include <unordered_map>
#include <utility>
#include <vector>

namespace scarf::model {

auto map_neighbors(const std::vector<glm::dvec2>& positions,
                   const std::pair<glm::dvec2, glm::dvec2>& bounds)
    -> std::function<std::vector<int>(int)>;

auto hash_coords(const glm::dvec2& coords, double r) -> int;

template <std::ranges::input_range Range>
  requires std::same_as<std::ranges::range_value_t<Range>, glm::dvec2>
auto build_reverse_lookup(const Range& position_range, double r)
    -> std::unordered_map<int, std::vector<size_t>> {
  // Create a lazily transformed range of hash values
  auto hash_view =
      position_range | std::views::transform([r](const glm::dvec2& coords) {
        return hash_coords(coords, r);
      });

  // Build the reverse lookup map using hash values
  std::unordered_map<int, std::vector<size_t>> reverse_lookup;
  size_t index = 0;
  for (const auto& hash : hash_view) {
    reverse_lookup[hash].push_back(index++);
  }

  return reverse_lookup;
}

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
