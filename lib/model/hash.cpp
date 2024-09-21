#include "hash.hpp"

namespace scarf::model {

auto build_hash(const Vector<double, 2>& anchor,
                const Vector<int, 2>& cell_counts,
                const Vector<double, 2>& cell_sizes)
    -> std::function<int(const Vector<double, 2>&)> {
  return [=](const Vector<double, 2>& point) -> int {
    Vector<int, 2> coords;
    for (auto i = 0; i < 2; i++) {
      coords[i] = static_cast<int>((point[i] - anchor[i]) / cell_sizes[i]);
    }
    auto idx = 0;
    for (auto i = 0; i < 2; i++) {
      idx *= cell_counts[i];
      idx += coords[i];
    }
    return idx;
  };
}

}  // namespace scarf::model
