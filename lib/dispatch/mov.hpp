#pragma once

#include <string_view>

#include "animation.hpp"
#include "config.hpp"

namespace scarf::dispatch {

const int width = 640;
const int height = 480;
const int fps = 60;
const int duration = 5;  // seconds
constexpr auto TIMESTEP = 1.0 / static_cast<double>(fps);

class ProtoMovWriter {
  std::string output_path_;

 public:
  ProtoMovWriter(std::string_view output_path);
  auto make_mov(Animation) -> int;
};

}  // namespace scarf::dispatch
