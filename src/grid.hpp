#pragma once

#include <cstddef>
#include <vector>

template <typename T>
struct Grid {
	Grid(std::size_t m, std::size_t n, const T& val) : data_(std::vector<std::vector<T>>(m, std::vector<T>(n, val))) {}

	std::vector<std::vector<T>> data_;
};
