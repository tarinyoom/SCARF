#pragma once

#include <memory>
#include <string>

#include "IVideoWriter.hpp"

auto make_mov_writer(std::string_view filepath, int height, int width, int fps)
    -> std::unique_ptr<IVideoWriter>;
