#include "sph.hpp"

#include <memory>

#include "alternator.hpp"
#include "dispatch/mov.hpp"
#include "grid.hpp"
#include "kernel.cpp"
#include "model/state.hpp"
#include "model/step.hpp"
#include "parse/parse.hpp"
#include "pixel.hpp"
#include "render/render.hpp"
#include "render/scene.hpp"

namespace scarf {

auto lift(const model::State& state) -> render::Scene {
  render::Scene scene;
  scene.points.reserve(state.n_particles);
  for (auto i = 0; i < state.n_particles; i++) {
    scene.points.push_back(state.positions[i]);
    scene.falloff = [](double r2) -> double { return kernel_1d(r2, 3.0, 1.0); };
    scene.outer_radius = 3.0;
    scene.inner_radius = 0.1;
    scene.msaa_linear_density = 1;
  }
  return scene;
}

auto render_state(const model::State& state) -> Grid<Pixel> {
  return render::render(lift(state));
}

auto build_animation(int n_subsamples) -> dispatch::Animation {
  auto alt =
      std::make_shared<Alternator<model::State>>(model::init(), model::step);
  return dispatch::Animation{[=](double h) -> Grid<Pixel> {
    auto substep = h / static_cast<double>(n_subsamples);
    for (int i = 1; i < n_subsamples; i++) {
      alt->next(substep);
    }
    return render_state(*alt->next(substep));
  }};
}

auto run(int argc, char* argv[]) -> int {
  return dispatch::make_mov(build_animation(10), parse::parse_file(argv[1]));
}

}  // namespace scarf
