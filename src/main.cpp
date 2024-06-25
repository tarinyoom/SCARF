#include "mov.hpp"
#include "blackbox.hpp"

int main() {
    return make_mov(black_box.init, black_box.step, black_box.render);
}
