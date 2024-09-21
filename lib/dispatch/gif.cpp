#include "gif.hpp"  // Assuming Animation and Config types are defined here

#include <functional>
#include <iostream>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
}

#include "grid.hpp"
#include "pixel.hpp"

namespace scarf::dispatch {

void fill_gradient_gif(uint8_t* data, int linesize, Animation& anim) {
  auto rendering = anim.next(TIMESTEP);
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      auto px = rendering[x][y];
      data[y * linesize + x * 3] = px.r;      // Red
      data[y * linesize + x * 3 + 1] = px.g;  // Green
      data[y * linesize + x * 3 + 2] = px.b;  // Blue
    }
  }
}

auto make_gif(Animation anim, const Config& cfg) -> int {
  // Convert the path to a string
  std::string path_str = cfg.output_file.string();

  // Allocate memory for the char* and copy the string into it
  char* path_cstr =
      new char[path_str.size() + 1];  // +1 for the null terminator
  std::strcpy(path_cstr, path_str.c_str());

  AVFormatContext* format_context = nullptr;
  AVStream* video_stream = nullptr;
  AVCodecContext* codec_context = nullptr;
  AVFrame* frame = nullptr;
  AVPacket packet;

  avformat_alloc_output_context2(&format_context, nullptr, "gif", path_cstr);
  if (!format_context) {
    std::cerr << "Could not create output context" << std::endl;
    return 1;
  }

  const AVOutputFormat* output_format = format_context->oformat;

  const AVCodec* codec = avcodec_find_encoder(AV_CODEC_ID_GIF);
  if (!codec) {
    std::cerr << "Codec not found" << std::endl;
    return 1;
  }

  video_stream = avformat_new_stream(format_context, codec);
  if (!video_stream) {
    std::cerr << "Could not create video stream" << std::endl;
    return 1;
  }

  codec_context = avcodec_alloc_context3(codec);
  if (!codec_context) {
    std::cerr << "Could not allocate video codec context" << std::endl;
    return 1;
  }

  codec_context->codec_id = codec->id;
  codec_context->bit_rate = 400000;
  codec_context->width = width;
  codec_context->height = height;
  codec_context->time_base = {1, fps};
  codec_context->framerate = {fps, 1};
  codec_context->gop_size = 10;
  codec_context->max_b_frames = 1;
  codec_context->pix_fmt = AV_PIX_FMT_PAL8;  // GIF uses a palette-based format

  if (output_format->flags & AVFMT_GLOBALHEADER) {
    codec_context->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
  }

  if (avcodec_open2(codec_context, codec, nullptr) < 0) {
    std::cerr << "Could not open codec" << std::endl;
    return 1;
  }

  video_stream->time_base = codec_context->time_base;

  if (avcodec_parameters_from_context(video_stream->codecpar, codec_context) <
      0) {
    std::cerr << "Could not copy codec parameters" << std::endl;
    return 1;
  }

  if (!(output_format->flags & AVFMT_NOFILE)) {
    if (avio_open(&format_context->pb, path_cstr, AVIO_FLAG_WRITE) < 0) {
      std::cerr << "Could not open output file" << std::endl;
      return 1;
    }
  }

  if (avformat_write_header(format_context, nullptr) < 0) {
    std::cerr << "Error occurred when writing header" << std::endl;
    return 1;
  }

  frame = av_frame_alloc();
  if (!frame) {
    std::cerr << "Could not allocate video frame" << std::endl;
    return 1;
  }
  frame->format = codec_context->pix_fmt;
  frame->width = codec_context->width;
  frame->height = codec_context->height;

  if (av_frame_get_buffer(frame, 32) < 0) {
    std::cerr << "Could not allocate frame data" << std::endl;
    return 1;
  }

  struct SwsContext* sws_context = sws_getContext(
      width, height, AV_PIX_FMT_RGB24, width, height, codec_context->pix_fmt,
      SWS_BICUBIC, nullptr, nullptr, nullptr);

  for (int i = 0; i < fps * duration; ++i) {
    if (av_frame_make_writable(frame) < 0) {
      std::cerr << "Frame not writable" << std::endl;
      break;
    }

    uint8_t* rgb_data[1] = {new uint8_t[width * height * 3]};
    int rgb_linesize[1] = {3 * width};
    fill_gradient_gif(rgb_data[0], rgb_linesize[0], anim);
    static int frame_number = 1;
    std::cout << "Generating frame " << frame_number++ << " of "
              << duration * fps << std::endl;

    sws_scale(sws_context, rgb_data, rgb_linesize, 0, height, frame->data,
              frame->linesize);

    frame->pts = i;

    av_init_packet(&packet);
    packet.data = nullptr;
    packet.size = 0;

    int ret = avcodec_send_frame(codec_context, frame);
    if (ret < 0) {
      std::cerr << "Error sending frame to codec context" << std::endl;
      break;
    }

    ret = avcodec_receive_packet(codec_context, &packet);
    if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
      continue;
    } else if (ret < 0) {
      std::cerr << "Error during encoding" << std::endl;
      break;
    }

    av_packet_rescale_ts(&packet, codec_context->time_base,
                         video_stream->time_base);
    packet.stream_index = video_stream->index;

    ret = av_interleaved_write_frame(format_context, &packet);
    if (ret < 0) {
      std::cerr << "Error while writing video frame" << std::endl;
      break;
    }

    av_packet_unref(&packet);
    delete[] rgb_data[0];
  }

  av_write_trailer(format_context);

  avcodec_free_context(&codec_context);
  av_frame_free(&frame);
  sws_freeContext(sws_context);
  if (!(output_format->flags & AVFMT_NOFILE)) {
    avio_closep(&format_context->pb);
  }
  avformat_free_context(format_context);

  delete[] path_cstr;

  return 0;
}

}  // namespace scarf::dispatch
