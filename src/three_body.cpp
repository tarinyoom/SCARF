#include "three_body.hpp"

#include <array>
#include <iterator>
#include <cstddef>
#include <algorithm>
#include <cmath>
#include <numbers>
#include <memory>

#include "pixel.hpp"

ThreeBodyState::ThreeBodyState(std::vector<Body>&& bs) {
    bodies = std::move(bs);
    radius = 0.5;
}

auto init() -> ThreeBodyState {
    return ThreeBodyState({
        Body {{1.1, 1.0}, {0.0, -1.1}},
        Body {{-1.1, 0.0}, {1.1, 0.0}},
        Body {{0.0, -2.2}, {0.0, 1.1}}
    });
}

auto step_body(Body&& pre) -> Body {
    return {
        pre.position + pre.velocity * 0.1,
        pre.position
    };
}

auto map(std::vector<Body>&& vs, std::function<Body(Body&&)> f) -> std::vector<Body> {
    std::vector<Body> mapped;
    std::transform(std::make_move_iterator(vs.begin()),
    std::make_move_iterator(vs.end()), std::back_inserter(mapped), f);
    return mapped;
}

auto step(ThreeBodyState&& pre) -> ThreeBodyState {
    return {
        map(std::move(pre.bodies), step_body)
    };
}

auto screen_to_world(int row, int col) -> Vector<2> {
    return {
        (static_cast<double>(row) - 320.0) / 10.0,
        (static_cast<double>(col) - 240.0) / 10.0};
}

template <std::size_t N>
auto norm2(const Vector<N>& p1, const Vector<N>& p2) -> double {
    auto diff = p1 - p2;
    return diff * diff;
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
