#include "three_body.hpp"

#include <array>
#include <iterator>
#include <cstddef>
#include <algorithm>
#include <cmath>
#include <numbers>
#include <memory>

#include "pixel.hpp"

auto init() -> ThreeBodyState {
   return {{
        {{2.2, 2.0}, {-0.8, -2.2}},
        {{-2.2, 0.0}, {2.2, 0.0}},
        {{0.0, -3.2}, {0.0, 0.2}}
    }, 0.3};
}

template <std::size_t N>
auto norm2(const Vector<N>& p1, const Vector<N>& p2) -> double {
    auto diff = p1 - p2;
    return diff * diff;
}

auto find_acceleration(const Vector<2>& from, const Vector<2>& to) -> Vector<2> {
    constexpr auto r = 2.0 / 3.0;
    auto factor = std::pow(norm2(from, to), 1.5);
    return (to - from) * factor;
}

auto step(ThreeBodyState&& pre) -> ThreeBodyState {
    auto post = pre;
    for (int i = 0; i < pre.bodies.size(); i++) {
        for (int j = 0; j < pre.bodies.size(); j++) {
            post.bodies[i].velocity = post.bodies[i].velocity + (find_acceleration(post.bodies[i].position, post.bodies[j].position)) * 0.005;
        }
        post.bodies[i].position = post.bodies[i].position + pre.bodies[i].velocity * 0.005;
    }
    return post;
}

auto screen_to_world(int row, int col) -> Vector<2> {
    return {
        (static_cast<double>(row) - 320.0) / 10.0,
        (static_cast<double>(col) - 240.0) / 10.0};
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
