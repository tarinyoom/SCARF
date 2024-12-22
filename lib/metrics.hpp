#pragma once

#include <map>
#include <string>
#include <variant>

namespace scarf {

using Metrics = std::map<std::string, std::variant<int, double>>;
auto print_metrics(const Metrics& m) -> void;

}  // namespace scarf
