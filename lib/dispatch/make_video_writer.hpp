#pragma once

#include <memory>
#include <string_view>

#include "IVideoWriter.hpp"

namespace scarf::dispatch {

auto make_video_writer(std::string_view writer_type,
                       std::string_view output_path)
    -> std::unique_ptr<IVideoWriter>;

}
