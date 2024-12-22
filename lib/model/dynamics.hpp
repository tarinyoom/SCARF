#pragma once

#include <functional>
#include <glm/glm.hpp>
#include <vector>

#include "state.hpp"

namespace scarf::model {

auto compute_densities(std::function<std::vector<int>(int)> neighbor_map,
                       const std::vector<glm::dvec2>& positions)
    -> std::vector<double>;

auto compute_pressures(double reference_density,
                       const std::vector<double>& densities)
    -> std::vector<double>;

auto compute_accelerations(std::function<std::vector<int>(int)> neighbor_map,
                           const std::vector<glm::dvec2>& positions,
                           const std::vector<double>& densities,
                           const std::vector<double>& pressures)
    -> std::vector<glm::dvec2>;

}  // namespace scarf::model
