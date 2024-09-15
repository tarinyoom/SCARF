#pragma once

#include <filesystem>

namespace scarf {

struct Config {
  std::filesystem::path output_file;
};

}  // namespace scarf
