#include "sph.hpp"

#include <memory>

#include "grid.hpp"
#include "kernel.cpp"
#include "model/step.hpp"
#include "pixel.hpp"
#include "renderer/render.hpp"
#include "renderer/scene.hpp"

namespace scarf {

auto render(const model::SPHState& state) -> Grid<Pixel> {
  renderer::Scene scene;
  scene.points.reserve(state.n_particles);
  for (auto i = 0; i < state.n_particles; i++) {
    scene.points.push_back(state.positions[i]);
    scene.falloff = [](double r2) -> double { return kernel_1d(r2, 3.0, 1.0); };
    scene.outer_radius = 3.0;
    scene.inner_radius = 0.1;
    scene.msaa_linear_density = 1;
  }
  return renderer::render(std::move(scene));
}

auto build_animation() -> Animation<model::SPHState> {
  return {model::init, model::step, render};
}

}  // namespace scarf
