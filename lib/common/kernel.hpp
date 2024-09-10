#pragma once

#include "vector.hpp"

namespace scarf {

auto kernel_1d(double r2, double R, double scale) -> double;

auto kernel(const Vector<double, 2>& a, const Vector<double, 2>& b, double r,
            double scale) -> double;

auto kernel_gradient(const Vector<double, 2>& p, const Vector<double, 2>& c,
                     double r, double scale) -> Vector<double, 2>;

}  // namespace scarf
