#pragma once

#include "grid.hpp"
#include "pixel.hpp"

namespace scarf::dispatch {

/*
 * A simple video writer interface. Assumes RAII style initialization and
 * finalization.
 */
struct IVideoWriter {
  virtual void write_frame(Grid<Pixel>& rendering) = 0;
  virtual ~IVideoWriter() = default;
};

}  // namespace scarf::dispatch
