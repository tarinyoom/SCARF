#include "metrics.hpp"

#include <iostream>

namespace scarf {

template <typename... Types>
std::enable_if_t<(sizeof...(Types) > 0), std::ostream&> operator<<(
    std::ostream& os, const std::variant<Types...>& var) {
  std::visit([&os](const auto& value) { os << value; }, var);
  return os;
}

auto print_metrics(const Metrics& m) -> void {
  for (const auto& [key, value] : m) {
    std::cout << key << ": " << value << std::endl;
  }
}

}  // namespace scarf
