#pragma once

#include <string_view>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
}

#include "IVideoWriter.hpp"
#include "animation.hpp"
#include "config.hpp"
#include "grid.hpp"
#include "pixel.hpp"

namespace scarf::dispatch {

const int width = 640;
const int height = 480;
const int fps = 60;
const int duration = 5;  // seconds
constexpr auto TIMESTEP = 1.0 / static_cast<double>(fps);

class ProtoMovWriter : public IVideoWriter {
  std::string output_path_;

  AVFormatContext* format_context;
  AVStream* video_stream;
  AVCodecContext* codec_context;
  AVFrame* frame;

  const AVOutputFormat* output_format;

  struct SwsContext* sws_context;
  AVPacket packet;

  int frame_number;

 public:
  ProtoMovWriter(std::string_view output_path);
  ~ProtoMovWriter();
  void write_frame(scarf::Grid<scarf::Pixel>& rendering);
};

}  // namespace scarf::dispatch
