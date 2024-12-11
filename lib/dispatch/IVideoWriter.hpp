#pragma once

#include <cstdint>

/*
 * A simple video writer interface. Assumes RAII style initialization and
 * finalization.
 */
struct IVideoWriter {
  virtual void write_frame(const uint8_t* rgb_data) = 0;
  virtual ~IVideoWriter() = default;
};
