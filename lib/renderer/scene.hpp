#pragma once

#include <functional>
#include <vector>

namespace scarf::renderer {

struct Scene {
  // Points present in scene
  std::vector<std::array<double, 2>> points;

  // Rendered luminosity, given a passed *SQUARED* radius
  std::function<double(double)> falloff;

  // For determining the region over which falloff is calculated
  double outer_radius;

  // Illuminated center radius of each point
  double inner_radius;

  std::size_t msaa_linear_density;
};

}  // namespace scarf::renderer
