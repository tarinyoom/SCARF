#include "metrics.hpp"

#include <iostream>

namespace scarf {

auto print_metrics(const Metrics& m) -> void {
  for (const auto& [key, value] : m) {
    std::cout << key << std::endl;
  }
}

}  // namespace scarf
