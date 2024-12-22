#pragma once

#include <functional>
#include <glm/glm.hpp>
#include <vector>

namespace scarf::render {

struct Scene {
  // Points present in scene
  std::vector<glm::dvec2> points;

  // Rendered luminosity, given a passed *SQUARED* radius
  std::function<double(double)> falloff;

  // For determining the region over which falloff is calculated
  double outer_radius;

  // Illuminated center radius of each point
  double inner_radius;

  std::size_t msaa_linear_density;
};

}  // namespace scarf::render
