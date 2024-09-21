#pragma once

#include <functional>

#include "vector.hpp"

namespace scarf::model {

auto hash(const Vector<int, 2>& coords, const Vector<int, 2>& cell_counts)
    -> int;

auto build_hash(const Vector<double, 2>& anchor,
                const Vector<int, 2>& cell_counts,
                const Vector<double, 2>& cell_sizes)
    -> std::function<int(const Vector<double, 2>&)>;

}  // namespace scarf::model
