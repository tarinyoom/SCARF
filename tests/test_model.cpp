#include <gtest/gtest.h>

#include <algorithm>
#include <limits>
#include <utility>
#include <vector>

#include "model/dynamics.hpp"
#include "model/state.hpp"
#include "model/step.hpp"
#include "sph.hpp"

using namespace scarf;

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

TEST(model, velocity_approximation) {
  model::State s(3);
  s.boundary = Bbox<double, 2>({0.0, 0.0}, {7.0, 7.0});
  s.positions = {{3.0, 3.0}, {3.0, 3.2}, {3.4, 3.8}};
  std::vector<Vector<double, 2>> expected_velocities = {
      {-4.8356888570142473, -10.0},
      {-5.6281261208778641, -4.6966895267452689},
      {-0.0, -0.0}};
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
