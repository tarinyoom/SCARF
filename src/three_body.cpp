#include "three_body.hpp"

#include <array>
#include <algorithm>
#include <cmath>
#include <numbers>

#include "pixel.hpp"

auto init() -> ThreeBodyState {
    return ThreeBodyState {
        {
            {1.0, 1.0}, {0.0, 0.0},
            {-1.0, 0.0}, {0.0, 0.0},
            {0.0, -2.0}, {0.0, 0.0}
        }, 0.5};
}

auto step(ThreeBodyState&& pre) -> ThreeBodyState {
    return pre;
}

auto screen_to_world(int row, int col) -> std::array<double, 2> {
    return {
        (static_cast<double>(row) - 320.0) / 10.0,
        (static_cast<double>(col) - 240.0) / 10.0};
}

auto norm2(std::array<double, 2> p) -> double {
    return p[0] * p[0] + p[1] * p[1];
}

auto norm2(std::array<double, 2> p1, std::array<double, 2> p2) -> double {
    std::array<double, 2> diff = {p1[0] - p2[0], p1[1] - p2[1]};
    return norm2(diff);
}

auto render(const ThreeBodyState& s, int row, int col) -> Pixel {
    auto p = screen_to_world(row, col);
    for (auto& b : s.bodies) {
        if (norm2(p, b.position) < s.radius * s.radius){
            return Pixel {127, 127, 255};
        }
    }
    return Pixel {0, 0, 0};
}

Animation<ThreeBodyState> three_body = {
    init,
    step,
    render
};
