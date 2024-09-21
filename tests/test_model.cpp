#include <gtest/gtest.h>

#include <algorithm>
#include <limits>
#include <utility>
#include <vector>

#include "model/dynamics.hpp"
#include "model/hash.hpp"
#include "model/state.hpp"
#include "model/step.hpp"
#include "sph.hpp"
#include "vector.hpp"

using namespace scarf;

TEST(model, hash_ints) {
  auto cell_counts = Vector<int, 2>(5, 5);

  EXPECT_EQ(model::hash({0, 0}, cell_counts), 0);
  EXPECT_EQ(model::hash({0, 4}, cell_counts), 4);
  EXPECT_EQ(model::hash({4, 0}, cell_counts), 20);
  EXPECT_EQ(model::hash({4, 4}, cell_counts), 24);
  EXPECT_EQ(model::hash({2, 2}, cell_counts), 12);
}

TEST(model, build_hash) {
  auto anchor = Vector<double, 2>(0.0, 0.0);
  auto cell_counts = Vector<int, 2>(3, 2);
  auto cell_sizes = Vector<double, 2>(0.6, 0.6);
  auto hash = model::build_hash(anchor, cell_counts, cell_sizes);

  EXPECT_EQ(hash({0.3, 0.3}), 0);
  EXPECT_EQ(hash({0.9, 0.3}), 2);
  EXPECT_EQ(hash({1.5, 0.3}), 4);
  EXPECT_EQ(hash({0.3, 0.9}), 1);
  EXPECT_EQ(hash({0.9, 0.9}), 3);
  EXPECT_EQ(hash({1.5, 0.9}), 5);
}

TEST(model, density_approximation) {
  // Assemble a regular grid of points
  model::State s(100);
  for (auto i = 0; i < 10; i++) {
    for (auto j = 0; j < 10; j++) {
      s.positions[10 * i + j] = {static_cast<double>(i),
                                 static_cast<double>(j)};
    }
  }

  auto densities = model::compute_densities(s.positions);

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
  model::State s(3);
  s.boundary = Bbox<double, 2>({0.0, 0.0}, {7.0, 7.0});
  s.positions = {{3.0, 3.0}, {3.0, 3.2}, {3.4, 3.8}};
  std::vector<double> expected_pressures = {
      132.5320808527114, 162.13632790663783, 89.347770462730779};

  auto densities = model::compute_densities(s.positions);
  auto pressures = model::compute_pressures(s.reference_density, densities);
  for (auto i = 0; i < s.n_particles; i++) {
    EXPECT_EQ(pressures[i], expected_pressures[i]);
  }
}

TEST(model, velocity_approximation) {
  model::State s(3);
  s.boundary = Bbox<double, 2>({0.0, 0.0}, {7.0, 7.0});
  s.positions = {{3.0, 3.0}, {3.0, 3.2}, {3.4, 3.8}};
  std::vector<Vector<double, 2>> expected_velocities = {
      {-4.8357030681993383, -12.216913573292175},
      {-5.6281409177181496, -3.8967039396837233},
      {10.463843985917487, 19.1136175129759}};
  for (auto i = 0; i < s.n_particles; i++) {
    EXPECT_EQ(s.velocities[i][0], 0.0);
    EXPECT_EQ(s.velocities[i][1], 0.0);
  }
  s = model::step(std::move(s), 0.1);
  for (auto i = 0; i < s.n_particles; i++) {
    EXPECT_EQ(s.velocities[i][0], expected_velocities[i][0]);
    EXPECT_EQ(s.velocities[i][1], expected_velocities[i][1]);
  }
}
