#pragma once

#include <glm/glm.hpp>

namespace scarf {

auto kernel_1d(double r2, double R, double scale) -> double;

auto kernel(const glm::dvec2& a, const glm::dvec2& b, double r, double scale)
    -> double;

auto kernel_gradient(const glm::dvec2& p, const glm::dvec2& c, double r,
                     double scale) -> glm::dvec2;

}  // namespace scarf
