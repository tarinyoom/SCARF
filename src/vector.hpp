#pragma once

#include <array>
#include <cstddef>

template <std::size_t N>
struct Vector {
	auto operator+(const Vector<N>& other) const -> Vector<N>;
    auto operator-(const Vector<N>& other) const -> Vector<N>;
	auto operator*(const Vector<N>& other) const -> double;
    auto operator*(double scalar) const -> Vector<N>;
	std::array<double, N> data_;
};

template <std::size_t N>
auto Vector<N>::operator+(const Vector<N>& other) const -> Vector<N> {
    Vector<N> result;
    for (std::size_t i = 0; i < N; i++) {
        result.data_[i] = data_[i] + other.data_[i];
    }
    return result;
}

template <std::size_t N>
auto Vector<N>::operator-(const Vector<N>& other) const -> Vector<N> {
    Vector<N> result;
    for (std::size_t i = 0; i < N; i++) {
		double x = data_[i];
		double y = other.data_[i];
        result.data_[i] = data_[i] - other.data_[i];
    }
    return result;
}

template <std::size_t N>
auto Vector<N>::operator*(const Vector<N>& other) const -> double {
    auto result = 0.0;
    for (std::size_t i = 0; i < N; i++) {
        result += data_[i] * other.data_[i];
    }
    return result;
}

template <std::size_t N>
auto Vector<N>::operator*(double scalar) const -> Vector<N> {
    Vector<N> result;
    for (std::size_t i = 0; i < N; i++) {
        result.data_[i] = data_[i] * scalar;
    }
    return result;
}
