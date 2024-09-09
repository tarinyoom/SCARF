#pragma once

#include <array>
#include <cstddef>
#include <type_traits>

namespace scarf {

template <typename T, std::size_t N>
struct Vector {
  Vector() = default;

  // Constructor using parameter pack expansion to check size at compile-time
  template <typename... Args,
            typename = std::enable_if_t<(sizeof...(Args) == N)>>
  Vector(Args... args) : value{{static_cast<T>(args)...}} {}

  // Non-const operator[] for modifying elements
  auto operator[](std::size_t i) -> T& { return value[i]; }

  // Const operator[] for read-only access to elements
  auto operator[](std::size_t i) const -> const T& { return value[i]; }

  auto operator==(const Vector& other) const -> bool {
    for (auto i = 0; i < N; i++) {
      if (value[i] != other.value[i]) {
        return false;
      }
    }
    return true;
  }

  std::array<T, N> value;
};

template <typename T, std::size_t N>
auto operator+(const Vector<T, N>& u, const Vector<T, N>& v) -> Vector<T, N> {
  Vector<T, N> result;
  for (auto i = 0; i < N; i++) {
    result[i] = u[i] + v[i];
  }
  return result;
}

template <typename T, std::size_t N>
auto operator+=(Vector<T, N>& u, const Vector<T, N>& v) -> void {
  for (auto i = 0; i < N; i++) {
    u[i] += v[i];
  }
}

template <typename T, std::size_t N>
auto operator-(const Vector<T, N>& u, const Vector<T, N>& v) -> Vector<T, N> {
  Vector<T, N> result;
  for (auto i = 0; i < N; i++) {
    result[i] = u[i] - v[i];
  }
  return result;
}

template <typename T, std::size_t N>
auto operator*(const Vector<T, N>& u, const Vector<T, N>& v) -> double {
  auto result = 0.0;
  for (auto i = 0; i < N; i++) {
    result += u[i] * v[i];
  }
  return result;
}

template <typename T, std::size_t N>
auto operator*(const Vector<T, N>& v, double f) -> Vector<T, N> {
  Vector<T, N> result;
  for (auto i = 0; i < N; i++) {
    result[i] = v[i] * f;
  }
  return result;
}

template <typename T, std::size_t N>
auto operator*(double f, const Vector<T, N>& v) -> Vector<T, N> {
  return v * f;
}

template <typename T, std::size_t N>
auto operator/(const Vector<T, N>& v, double f) -> Vector<T, N> {
  Vector<T, N> result;
  for (auto i = 0; i < N; i++) {
    result += v[i] / f;
  }
  return result;
}

}  // namespace scarf
