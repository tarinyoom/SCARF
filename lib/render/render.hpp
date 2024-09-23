#pragma once

#include "grid.hpp"
#include "pixel.hpp"
#include "scene.hpp"

namespace scarf::render {

auto render(Scene&&) -> Grid<Pixel>;

}
