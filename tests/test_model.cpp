#include <gtest/gtest.h>

#include <algorithm>
#include <limits>
#include <utility>

#include "sph.hpp"

TEST(model, density_approximation) {
  // Assemble a regular grid of points
  SPHState s(100);
  for (auto i = 0; i < 10; i++) {
    for (auto j = 0; j < 10; j++) {
      s.positions[10 * i + j] = {static_cast<double>(i),
                                 static_cast<double>(j)};
    }
  }

  // Calculate density for grid
  s = sph_animation.step(std::move(s), 0.1);

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
      EXPECT_EQ(s.densities[i], 1.0023292559910253);  // within 1%
    }

    // Points in corners should have roughly a quarter of the ideal continuous
    // density value
    if (pt[0] < 0.1 || pt[0] > 8.9 || pt[1] < 0.1 || pt[1] > 8.9) {
      n_boundary++;
      boundary_min = std::min(boundary_min, s.densities[i]);
      boundary_max = std::max(boundary_max, s.densities[i]);
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
  SPHState s(3);
  s.boundary = Bbox<double, 2>({0.0, 0.0}, {7.0, 7.0});
  s.positions = {{3.0, 3.0}, {3.0, 3.2}, {3.4, 3.8}};
  std::vector<double> expected_pressures = {
      132.53175839243431, 162.13600544636074, 89.347448002453689};
  s = sph_animation.step(std::move(s), 0.1);
  for (auto i = 0; i < s.n_particles; i++) {
    EXPECT_EQ(s.pressures[i], expected_pressures[i]);
  }
}

TEST(model, velocity_approximation) {
  SPHState s(3);
  s.boundary = Bbox<double, 2>({0.0, 0.0}, {7.0, 7.0});
  s.positions = {{3.0, 3.0}, {3.0, 3.2}, {3.4, 3.8}};
  std::vector<std::array<double, 2>> expected_velocities = {
      {-4.8356888570142473, -13.272697857628378},
      {-5.6281261208778641, -3.8408690377169101},
      {-0.0, -0.0}};
  for (auto i = 0; i < s.n_particles; i++) {
    EXPECT_EQ(s.velocities[i][0], 0.0);
    EXPECT_EQ(s.velocities[i][1], 0.0);
  }
  s = sph_animation.step(std::move(s), 0.1);
  for (auto i = 0; i < s.n_particles; i++) {
    EXPECT_EQ(s.velocities[i], expected_velocities[i]);
  }
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
