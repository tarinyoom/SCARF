#include "blackbox.hpp"

#include <cmath>
#include <numbers>

#include "pixel.hpp"

auto init() -> BlackboxState {
    return BlackboxState{
        .i = 0
    };
}

auto step(BlackboxState&& s) -> BlackboxState {
    return BlackboxState{
        .i = s.i + 1
    };
}

auto render(const BlackboxState& s, int row, int col) -> Pixel {
    if (row < 260 || row >= 380 || col < 180 || col >= 300) {
        return Pixel {
            .r = 0,
            .g = 0,
            .b = 0
        };
    } else {
        return Pixel {
            .r = static_cast<uint8_t>(127.5 + 127.5 * std::sin(static_cast<double>(s.i) * std::numbers::pi / 150.0 + 2.0 * std::numbers::pi / 3.0)),
            .g = static_cast<uint8_t>(127.5 + 127.5 * std::sin(static_cast<double>(s.i) * std::numbers::pi / 150.0 + 1.0 * std::numbers::pi / 3.0)),
            .b = static_cast<uint8_t>(127.5 + 127.5 * std::sin(static_cast<double>(s.i) * std::numbers::pi / 150.0))
        };
    }
}

Animation<BlackboxState> black_box = {
    .init = init,
    .step = step,
    .render = render
};
