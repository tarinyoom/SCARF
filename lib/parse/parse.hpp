#pragma once

#include <filesystem>

#include "config.hpp"

namespace scarf::parse {

auto parse_file(const std::filesystem::path&) -> Config;

}
