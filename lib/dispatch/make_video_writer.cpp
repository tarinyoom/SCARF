#include "make_video_writer.hpp"

#include <stdexcept>

#include "MovWriter.hpp"

namespace scarf::dispatch {

auto make_video_writer(std::string_view writer_type,
                       std::string_view output_path)
    -> std::unique_ptr<IVideoWriter> {
  if (writer_type == "mov") {
    return std::make_unique<MovWriter>(output_path);
  } else {
    throw std::runtime_error("Invalid video writer type");
  }
}

}  // namespace scarf::dispatch
