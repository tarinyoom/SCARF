#include "sph.hpp"

#include <memory>

#include "dispatch/Engine.hpp"
#include "dispatch/make_video_writer.hpp"
#include "dispatch/run_engine.hpp"
#include "grid.hpp"
#include "kernel.cpp"
#include "model/state.hpp"
#include "model/step.hpp"
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

auto run(int argc, char* argv[]) -> int {
  auto writer = dispatch::make_video_writer("mov", "examples/generated.mov");
  int i;

  std::function<void(const model::State& state)> render_callback =
      [&](const auto& state) {
        if (i++ % 10 == 0) {
          auto image = render_state(state);
          writer->write_frame(image);
        }
      };

  dispatch::Engine<model::State> engine{.step = model::step,
                                        .observers = {render_callback}};

  auto initial_state = model::init();

  dispatch::run<model::State>(engine, 3000, 0.001, std::move(initial_state));
  return 0;
}

}  // namespace scarf
