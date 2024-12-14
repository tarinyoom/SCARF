#pragma once

#include <cstdint>

#include "grid.hpp"
#include "pixel.hpp"

/*
 * A simple video writer interface. Assumes RAII style initialization and
 * finalization.
 */
struct IVideoWriter {
  virtual void write_frame(scarf::Grid<scarf::Pixel>& rendering) = 0;
  virtual ~IVideoWriter() = default;
};
