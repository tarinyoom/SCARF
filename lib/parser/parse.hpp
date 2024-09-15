#pragma once

#include <filesystem>

#include "config.hpp"

namespace scarf::parser {

auto parse_file(const std::filesystem::path&) -> Config;

}
