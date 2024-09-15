#include "parse.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>

namespace scarf::parser {

auto parse_file(const std::filesystem::path& relative_path) -> Config {
  auto cwd = std::filesystem::current_path();
  auto full_path = cwd / relative_path;

  if (!std::filesystem::exists(full_path)) {
    throw std::runtime_error("File not found: " + full_path.string());
  }

  std::ifstream fs(full_path);
  if (!fs) {
    throw std::runtime_error("Unable to open file: " + full_path.string());
  }

  std::ostringstream content;
  content << fs.rdbuf();

  return Config{.output_file = content.str()};
}
}  // namespace scarf::parser
