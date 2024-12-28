#include <gtest/gtest.h>

#include <algorithm>
#include <glm/glm.hpp>
#include <limits>
#include <ranges>
#include <unordered_set>
#include <utility>
#include <vector>

#include "model/dynamics.hpp"
#include "model/hash.hpp"
#include "model/state.hpp"
#include "model/step.hpp"
#include "sph.hpp"

using namespace scarf::model;

TEST(model, initialization) {
  auto state = init_entt();
  auto view = state.registry.view<Position, Velocity>();
  auto min_x = std::numeric_limits<double>::max();
  auto max_x = std::numeric_limits<double>::min();
  auto n = 0;
  view.each([&](const Position& pos, const Velocity& vel) {
    n++;
    min_x = std::min(min_x, pos.value.x);
    max_x = std::max(max_x, pos.value.x);
  });
  EXPECT_EQ(n, 64);               // 64 particles present
  EXPECT_GT(max_x - min_x, 5.0);  // points are not coincident
}

TEST(model, hash_coords) {
  auto cell_counts = glm::ivec2(5, 5);

  EXPECT_EQ(detail::hash_coords({0, 0}, cell_counts), 0);
  EXPECT_EQ(detail::hash_coords({0, 4}, cell_counts), 4);
  EXPECT_EQ(detail::hash_coords({4, 0}, cell_counts), 20);
  EXPECT_EQ(detail::hash_coords({4, 4}, cell_counts), 24);
  EXPECT_EQ(detail::hash_coords({2, 2}, cell_counts), 12);
}

TEST(model, discretize_coords) {
  auto anchor = glm::dvec2(-1.0, -1.0);
  auto cell_sizes = glm::dvec2(0.3, 0.4);

  auto expect_discretization = [=](glm::dvec2 input,
                                   glm::ivec2 expected) -> void {
    auto discretization = detail::discretize_coords(input, anchor, cell_sizes);
    EXPECT_EQ(discretization, expected);
  };

  expect_discretization({-1.0, -1.0}, {0, 0});
  expect_discretization({0.2, 0.3}, {4, 3});
  expect_discretization({0.4, 0.5}, {4, 3});
}

TEST(model, build_hash) {
  auto anchor = glm::dvec2(0.0, 0.0);
  auto cell_counts = glm::ivec2(3, 2);
  auto cell_sizes = glm::dvec2(0.6, 0.6);
  auto hash = detail::build_hash(anchor, cell_counts, cell_sizes);

  EXPECT_EQ(hash({0.3, 0.3}), 0);
  EXPECT_EQ(hash({0.9, 0.3}), 2);
  EXPECT_EQ(hash({1.5, 0.3}), 4);
  EXPECT_EQ(hash({0.3, 0.9}), 1);
  EXPECT_EQ(hash({0.9, 0.9}), 3);
  EXPECT_EQ(hash({1.5, 0.9}), 5);
}

TEST(model, grid_neighbors) {
  auto bounds = std::pair<glm::dvec2, glm::dvec2>({0.0, 0.0}, {10.0, 10.0});
  std::vector<glm::dvec2> positions;
  positions.push_back({3.1, 3.4});
  for (auto i = 0; i < 10; i++) {
    for (auto j = 0; j < 10; j++) {
      positions.push_back({static_cast<double>(i), static_cast<double>(j)});
    }
  }

  auto map = map_neighbors(positions, bounds);
  for (auto i = 0; i < positions.size(); i++) {
    auto neighbors = map(i);

    std::unordered_set<int> neighbor_set;
    for (auto& n : neighbors) {
      neighbor_set.insert(n);
    }

    for (auto j = 0; j < positions.size(); j++) {
      auto diff = positions[i] - positions[j];
      if (glm::dot(diff, diff) < OUTER_R * OUTER_R) {
        EXPECT_TRUE(neighbor_set.contains(j));
      }
    }

    for (auto& n : neighbors) {
      auto diff = positions[i] - positions[n];
      EXPECT_LE(glm::dot(diff, diff), 4 * OUTER_R * OUTER_R);
    }
  }
}

TEST(model, density_approximation) {
  // Assemble a regular grid of points
  State s(100);
  for (auto i = 0; i < 10; i++) {
    for (auto j = 0; j < 10; j++) {
      s.positions[10 * i + j] = {static_cast<double>(i),
                                 static_cast<double>(j)};
    }
  }
  s.boundary = std::pair<glm::dvec2, glm::dvec2>({0.0, 0.0}, {10.0, 10.0});

  auto neighbor_map = map_neighbors(s.positions, s.boundary);
  auto densities = compute_densities(neighbor_map, s.positions);

  // For points in the interior and boundary of the grid, expect their
  // densities to closely match the ideal continuously calculated density
  auto n_interior = 0;
  auto n_boundary = 0;
  auto boundary_min = std::numeric_limits<double>::max();
  auto boundary_max = std::numeric_limits<double>::min();
  for (auto i = 0; i < 100; i++) {
    auto pt = s.positions[i];

    // Points in interior should have the ideal continuous density value
    if (pt[0] > 2.9 && pt[0] < 6.1 && pt[1] > 2.9 && pt[1] < 6.1) {
      n_interior++;
      EXPECT_EQ(densities[i], 1.0023292559910253);  // within 1%
    }

    // Points in corners should have roughly a quarter of the ideal continuous
    // density value
    if (pt[0] < 0.1 || pt[0] > 8.9 || pt[1] < 0.1 || pt[1] > 8.9) {
      n_boundary++;
      boundary_min = std::min(boundary_min, densities[i]);
      boundary_max = std::max(boundary_max, densities[i]);
    }
  }

  // Boundary min and max will overestimate continuous ideal because of
  // discretization
  EXPECT_EQ(boundary_min, 0.48030298326772253);
  EXPECT_EQ(boundary_max, 0.69551753213394663);

  EXPECT_EQ(n_interior, 16);
  EXPECT_EQ(n_boundary, 36);
}

TEST(model, pressure_approximation) {
  State s(3);
  s.boundary = std::pair<glm::dvec2, glm::dvec2>({0.0, 0.0}, {7.0, 7.0});
  s.positions = {{3.0, 3.0}, {3.0, 3.2}, {3.4, 3.8}};
  std::vector<double> expected_pressures = {
      132.5320808527114, 162.13632790663783, 89.347770462730779};
  auto neighbor_map = map_neighbors(s.positions, s.boundary);
  auto densities = compute_densities(neighbor_map, s.positions);
  auto pressures = compute_pressures(s.reference_density, densities);
  for (auto i = 0; i < s.n_particles; i++) {
    EXPECT_EQ(pressures[i], expected_pressures[i]);
  }
}

TEST(model, velocity_approximation) {
  State s(3);
  s.boundary = std::pair<glm::dvec2, glm::dvec2>({0.0, 0.0}, {7.0, 7.0});
  s.positions = {{3.0, 3.0}, {3.0, 3.2}, {3.4, 3.8}};
  std::vector<glm::dvec2> expected_velocities = {
      {-4.8357030681993383, -12.216913573292175},
      {-5.6281409177181496, -3.8967039396837233},
      {10.463843985917487, 19.1136175129759}};
  for (auto i = 0; i < s.n_particles; i++) {
    EXPECT_EQ(s.velocities[i][0], 0.0);
    EXPECT_EQ(s.velocities[i][1], 0.0);
  }
  step(s, 0.1);
  for (auto i = 0; i < s.n_particles; i++) {
    EXPECT_EQ(s.velocities[i][0], expected_velocities[i][0]);
    EXPECT_EQ(s.velocities[i][1], expected_velocities[i][1]);
  }
}

TEST(model, hash_consistent) {
  glm::dvec2 coords{12.34, 56.78};
  double r = 1.0;

  int hash1 = hash_coords(coords, r);
  int hash2 = hash_coords(coords, r);

  ASSERT_EQ(hash1, hash2) << "Hashes for the same input should match.";
}

TEST(model, hash_distinct) {
  glm::dvec2 coords1{12.34, 56.78};
  glm::dvec2 coords2{13.34, 56.78};
  double r = 1.0;

  int hash1 = hash_coords(coords1, r);
  int hash2 = hash_coords(coords2, r);

  ASSERT_NE(hash1, hash2) << "Hashes for different inputs should not match.";
}

TEST(model, hash_negative) {
  glm::dvec2 coords{-12.34, -56.78};
  double r = 1.0;

  int hash = hash_coords(coords, r);
  // Just verify it executes correctly
  SUCCEED() << "Hash for negative coordinates computed: " << hash;
}

TEST(model, hash_boundaries) {
  glm::dvec2 coords{10.0, 20.0};
  double r = 1.0;

  int hash = hash_coords(coords, r);
  // Just verify it executes correctly
  SUCCEED() << "Hash for boundary coordinates computed: " << hash;
}

TEST(model, hash_distinct_neighbors) {
  double r = 1.0;
  std::unordered_set<int> hashes;

  hashes.insert(hash_coords(glm::dvec2{0.0, 0.0}, r));
  hashes.insert(hash_coords(glm::dvec2{r, 0.0}, r));
  hashes.insert(hash_coords(glm::dvec2{0.0, r}, r));
  hashes.insert(hash_coords(glm::dvec2{r, r}, r));

  ASSERT_EQ(hashes.size(), 4)
      << "Hashes for distinct grid cells should be unique.";
}

TEST(model, build_reverse_lookup_empty_range) {
  std::vector<glm::dvec2> empty_input;
  double r = 1.0;

  auto result = build_reverse_lookup(empty_input, r);

  EXPECT_TRUE(result.empty())
      << "Reverse lookup table should be empty for an empty input range.";
}

TEST(model, build_reverse_lookup_single_element) {
  std::vector<glm::dvec2> single_input = {glm::dvec2(1.0, 2.0)};
  double r = 1.0;

  auto result = build_reverse_lookup(single_input, r);

  ASSERT_EQ(result.size(), 1u)
      << "Reverse lookup table should have one key for a single input.";
  int hash = hash_coords(single_input[0], r);
  EXPECT_EQ(result[hash], std::vector<size_t>({0}))
      << "Index for the single hash value should be 0.";
}

TEST(model, build_reverse_lookup_multiple_unique_elements) {
  std::vector<glm::dvec2> unique_input = {
      glm::dvec2(1.0, 2.0), glm::dvec2(3.0, 4.0), glm::dvec2(5.0, 6.0)};
  double r = 1.0;

  auto result = build_reverse_lookup(unique_input, r);

  ASSERT_EQ(result.size(), 3u)
      << "Reverse lookup table should have three keys for unique input.";
  for (size_t i = 0; i < unique_input.size(); ++i) {
    int hash = hash_coords(unique_input[i], r);
    EXPECT_EQ(result[hash], std::vector<size_t>({i}))
        << "Index for hash value should match input index.";
  }
}

TEST(model, build_reverse_lookup_duplicate_hashes) {
  std::vector<glm::dvec2> duplicate_input = {
      glm::dvec2(1.0, 2.0), glm::dvec2(3.0, 4.0),
      glm::dvec2(1.0, 2.0),                         // Same as first
      glm::dvec2(5.0, 6.0), glm::dvec2(3.0, 4.0)};  // Same as second
  double r = 1.0;

  auto result = build_reverse_lookup(duplicate_input, r);

  ASSERT_EQ(result.size(), 3u)
      << "Reverse lookup table should have keys for unique hashes.";
  std::unordered_map<int, std::vector<size_t>> expected{
      {hash_coords(duplicate_input[0], r), {0, 2}},
      {hash_coords(duplicate_input[1], r), {1, 4}},
      {hash_coords(duplicate_input[3], r), {3}}};

  for (const auto& [hash, indices] : expected) {
    EXPECT_EQ(result[hash], indices)
        << "Indices for hash " << hash << " do not match expected values.";
  }
}

TEST(model, build_reverse_lookup_large_input) {
  constexpr size_t input_size = 1000;
  std::vector<glm::dvec2> large_input(input_size);

  for (size_t i = 0; i < input_size; ++i) {
    large_input[i] = glm::dvec2(static_cast<double>(i % 10) / 2.0,
                                static_cast<double>(i / 10) / 2.0);
  }

  double r = 1.0;
  auto result = build_reverse_lookup(large_input, r);

  ASSERT_EQ(result.size(), 250u)
      << "Reverse lookup table should have 250 unique keys.";
  for (size_t i = 0; i < input_size; ++i) {
    int hash = hash_coords(large_input[i], r);
    EXPECT_NE(result.find(hash), result.end())
        << "Hash " << hash << " should exist in the reverse lookup.";
    EXPECT_NE(std::find(result[hash].begin(), result[hash].end(), i),
              result[hash].end())
        << "Index " << i << " should be mapped to hash " << hash << ".";
  }
}
