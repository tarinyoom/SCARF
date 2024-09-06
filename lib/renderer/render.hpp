#pragma once

#include "grid.hpp"
#include "pixel.hpp"
#include "scene.hpp"

namespace scarf::renderer {

auto render(Scene&&) -> Grid<Pixel>;

}
