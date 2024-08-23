#pragma once

#include <array>
#include <cstddef>

template <typename T, std::size_t N>
auto operator+(const std::array<T, N> u,
               const std::array<T, N>& v) -> std::array<T, N> {
  std::array<T, N> result;
  for (auto i = 0; i < N; i++) {
    result[i] = u[i] + v[i];
  }
  return result;
}

template <typename T, std::size_t N>
auto operator-(const std::array<T, N> u,
               const std::array<T, N>& v) -> std::array<T, N> {
  std::array<T, N> result;
  for (auto i = 0; i < N; i++) {
    result[i] = u[i] - v[i];
  }
  return result;
}

template <typename T, std::size_t N>
auto operator*(const std::array<T, N> u, const std::array<T, N>& v) -> T {
  auto result = 0.0;
  for (auto i = 0; i < N; i++) {
    result += u[i] * v[i];
  }
  return result;
}

template <typename T, std::size_t N>
auto operator*(const std::array<T, N> v, const T& f) -> std::array<T, N> {
  std::array<T, N> result;
  for (auto i = 0; i < N; i++) {
    result += v[i] * f;
  }
  return result;
}

template <typename T, std::size_t N>
auto operator/(const std::array<T, N> v, const T& f) -> std::array<T, N> {
  std::array<T, N> result;
  for (auto i = 0; i < N; i++) {
    result += v[i] / f;
  }
  return result;
}