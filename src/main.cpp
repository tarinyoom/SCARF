#include "state.h"
#include "pixel.h"
#include "mov.h"

auto initialize_state() -> State {
    return State{
        .i = 0
    };
}

auto step_state(State&& s) -> State {
    return State{
        .i = s.i + 1
    };
}

auto get_pixel(const State& s, int row, int col) -> Pixel {
    return Pixel {
        .r = static_cast<uint8_t>(row % 256),
        .g = static_cast<uint8_t>(col % 256),
        .b = static_cast<uint8_t>(s.i % 256)
    };
}

int main() {
    return make_mov(initialize_state, step_state, get_pixel);
}
