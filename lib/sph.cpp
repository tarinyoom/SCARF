#include "sph.hpp"

#include <chrono>
#include <glm/glm.hpp>
#include <iostream>
#include <memory>

#include "Engine.hpp"
#include "dispatch/make_video_writer.hpp"
#include "grid.hpp"
#include "kernel.cpp"
#include "metrics.hpp"
#include "model/state.hpp"
#include "model/step.hpp"
#include "pixel.hpp"
#include "render/render.hpp"
#include "render/scene.hpp"
#include "run_engine.hpp"

namespace scarf {

auto lift(const model::State& state) -> render::Scene {
  render::Scene scene;
  scene.points.reserve(state.n_particles);
  for (auto i = 0; i < state.n_particles; i++) {
    scene.points.push_back(
        glm::vec2(state.positions[i][0], state.positions[i][1]));
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
  using Clock = std::chrono::high_resolution_clock;

  auto start = Clock::now();

  auto writer = dispatch::make_video_writer("mov", "examples/generated.mov");
  int i = 0;
  Metrics m;
  m["n_steps"] = 0;
  m["n_frames"] = 0;

  using Callback = std::function<void(const model::State& state)>;

  std::chrono::milliseconds render_duration(0);

  Callback start_callback = [&](const auto&) { m["n_steps"]++; };

  Callback render_callback = [&](const auto& state) {
    auto start = Clock::now();

    if (i++ % 10 == 0) {
      auto image = render_state(state);
      writer->write_frame(image);
      m["n_frames"]++;
      std::cout << "Generating frame " << i / 10 + 1 << std::endl;
    }
    auto end = Clock::now();

    render_duration +=
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  };

  Engine<model::State> engine{.step = model::step,
                              .observers = {start_callback, render_callback}};

  auto initial_state = model::init();

  run_engine<model::State>(engine, 3000, 0.001, std::move(initial_state));
  auto end = Clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  m["total_duration"] = duration.count();
  m["render_duration"] = render_duration.count();
  print_metrics(m);
  return 0;
}

}  // namespace scarf
