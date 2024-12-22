#pragma once

#include <map>
#include <string>

namespace scarf {

using Metrics = std::map<std::string, double>;
auto print_metrics(const Metrics& m) -> void;

}  // namespace scarf
