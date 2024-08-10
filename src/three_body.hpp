#pragma once

#include <array>

#include <vector>

#include "animation.hpp"
#include "vector.hpp"

struct Body {
    Vector<2> position;
    Vector<2> velocity;
};

struct ThreeBodyState {
    std::vector<Body> bodies;
    double radius;
};

extern Animation<ThreeBodyState> three_body;
