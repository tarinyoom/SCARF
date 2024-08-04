#pragma once

#include <array>

#include "animation.hpp"

struct Body {
    std::array<double, 2> position;
    std::array<double, 2> velocity;
};

struct ThreeBodyState {
    std::vector<Body> bodies;
    double radius;
};

extern Animation<ThreeBodyState> three_body;
