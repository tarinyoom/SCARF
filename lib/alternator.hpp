#pragma once

#include <array>
#include <functional>
#include <memory>

namespace scarf {

// Abstractly, calls `transition` repeatedly on the initial value.
// Uses a two-sided buffer to reduce memory waste.
template <typename T>
struct Alternator {
  using Transformation = std::function<T(const T&, double)>;

  Alternator(T init, Transformation transformation)
      : state(0), transformation(std::move(transformation)) {
    buffer[0] = std::make_shared<T>(
        std::move(init));  // Initialize with the provided init
    buffer[1] =
        std::make_shared<T>();  // Default-initialize the second buffer element
  }

  auto next(double h) -> std::shared_ptr<T> {
    *buffer[!state] = transformation(*buffer[state], h);
    state = !state;
    return buffer[state];
  }

 private:
  int state;  // which side of the buffer is active
  std::array<std::shared_ptr<T>, 2> buffer;  // two-sided buffer
  Transformation transformation;  // function to advance to the next state
};

}  // namespace scarf
