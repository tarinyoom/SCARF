#pragma once

struct Scene {
  // Points present in scene
  std::vector<std::array<double, 2>> points;

  // Rendered luminosity, given a passed radius
  std::function<double(double)> falloff;

  // For determining the region over which falloff is calculated
  double outer_radius_ub;

  // Illuminated center radius of each point
  double inner_radius;
};

// Scene&& -> Grid
