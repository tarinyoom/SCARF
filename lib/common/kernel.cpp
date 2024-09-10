#include "kernel.hpp"

#include <cmath>
#include <numbers>

namespace scarf {

auto kernel_1d(double r2, double R, double scale) -> double {
  auto c = R * R - r2;
  auto numerator = 4.0 * c * c * c * scale;
  auto denominator = std::numbers::pi * std::pow(R, 8.0);
  return numerator / denominator;
}

auto kernel(const Vector<double, 2>& a, const Vector<double, 2>& b, double r,
            double scale) -> double {
  auto diff = a - b;
  auto d2 = diff * diff;
  if (d2 > r * r) {
    return 0.0;
  }

  return kernel_1d(d2, r, scale);
}

auto kernel_gradient(const Vector<double, 2>& p, const Vector<double, 2>& c,
                     double r, double scale) -> Vector<double, 2> {
  auto diff = p - c;
  auto d2 = diff * diff;
  if (d2 > r * r) {
    return {0.0, 0.0};
  }

  auto v = r * r - d2;
  auto common_numerator = 24.0 * v * v * scale;
  auto denominator = std::numbers::pi * std::pow(r, 8.0);
  auto common = common_numerator / denominator;
  return (p - c) * common;
}

}  // namespace scarf
