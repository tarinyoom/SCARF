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

auto screen_to_world(int row, int col) -> std::vector<Vector<2>> {
    std::vector<Vector<2>> pts;
    pts.reserve(25); // for each point in MSAA grid
    for (int i = -2; i <= 2; i++) {
        for (int j = -2; j <= 2; j++) {
            Vector<2> pt = {
                (static_cast<double>(row) - 320.0) * 0.1 + i * 0.02,
                (static_cast<double>(col) - 240.0) * 0.1 + j * 0.02
            };
            pts.push_back(std::move(pt));
        }
    }
    return pts;
}

auto mean(const std::vector<Pixel>& pxs) -> Pixel {
    double r = 0.0;
    double g = 0.0;
    double b = 0.0;

    for (auto& px: pxs) {
        r += static_cast<double>(px.r);
        g += static_cast<double>(px.g);
        b += static_cast<double>(px.b);
    }

    auto n = static_cast<double>(pxs.size());
    r /= n;
    g /= n;
    b /= n;

    return {
        static_cast<std::uint8_t>(r),
        static_cast<std::uint8_t>(g),
        static_cast<std::uint8_t>(b)
    };
}

auto render_v(const ThreeBodyState& s, const Vector<2>& v) -> Pixel {
    for (auto& b : s.bodies) {
        if (norm2(v, b.position) < s.radius * s.radius){
            return Pixel {127, 127, 255};
        }
    }
    return Pixel {0, 0, 0};
}

auto render_pixel(const ThreeBodyState& s, int row, int col) -> Pixel {
    auto pts = screen_to_world(row, col);
    std::vector<Pixel> pxs;
    std::transform(pts.cbegin(), pts.cend(), std::back_inserter(pxs), [&](auto pt) {
        return render_v(s, pt);
    });
    return mean(pxs);
}

auto render(const ThreeBodyState& s) -> std::vector<std::vector<Pixel>> {
    std::vector<std::vector<Pixel>> rendering;
    for (int i = 0; i < 640; i++) {
        std::vector<Pixel> row;
        for (int j = 0; j < 480; j++) {
            row.push_back(render_pixel(s, i, j));
        }
        rendering.push_back(std::move(row));
    }
    return rendering;
}

Animation<ThreeBodyState> three_body = {
    init,
    step,
    render
};
