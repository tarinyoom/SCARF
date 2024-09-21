#pragma once

#include <functional>

#include "vector.hpp"

namespace scarf::model {

auto build_hash(const Vector<double, 2>& anchor,
                const Vector<int, 2>& cell_counts,
                const Vector<double, 2>& cell_sizes)
    -> std::function<int(const Vector<double, 2>&)>;

}
