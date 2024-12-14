#include "mov.hpp"

#include <functional>
#include <iostream>
#include <stdexcept>

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

void fill_gradient(uint8_t* data, int linesize, Animation& anim) {
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

ProtoMovWriter::ProtoMovWriter(std::string_view output_path)
    : output_path_(output_path),
      format_context(nullptr),
      video_stream(nullptr),
      codec_context(nullptr),
      frame(nullptr),
      output_format(nullptr),
      sws_context(nullptr) {
  avformat_alloc_output_context2(&format_context, nullptr, "mov",
                                 output_path_.c_str());
  if (!format_context) {
    throw std::runtime_error("Could not create output context");
  }

  output_format = format_context->oformat;

  const AVCodec* codec = avcodec_find_encoder(AV_CODEC_ID_H264);
  if (!codec) {
    throw std::runtime_error("Codec not found");
  }

  video_stream = avformat_new_stream(format_context, codec);
  if (!video_stream) {
    throw std::runtime_error("Could not create video stream");
  }

  codec_context = avcodec_alloc_context3(codec);
  if (!codec_context) {
    throw std::runtime_error("Could not allocate video codec context");
  }

  codec_context->codec_id = codec->id;
  codec_context->bit_rate = 400000;
  codec_context->width = width;
  codec_context->height = height;
  codec_context->time_base = {1, fps};
  codec_context->framerate = {fps, 1};
  codec_context->gop_size = 10;
  codec_context->max_b_frames = 1;
  codec_context->pix_fmt = AV_PIX_FMT_YUV420P;

  if (output_format->flags & AVFMT_GLOBALHEADER) {
    codec_context->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
  }

  if (avcodec_open2(codec_context, codec, nullptr) < 0) {
    throw std::runtime_error("Could not open codec");
  }

  video_stream->time_base = codec_context->time_base;

  if (avcodec_parameters_from_context(video_stream->codecpar, codec_context) <
      0) {
    throw std::runtime_error("Could not copy codec parameters");
  }

  if (!(output_format->flags & AVFMT_NOFILE)) {
    if (avio_open(&format_context->pb, output_path_.c_str(), AVIO_FLAG_WRITE) <
        0) {
      throw std::runtime_error("Could not open output file");
    }
  }

  if (avformat_write_header(format_context, nullptr) < 0) {
    throw std::runtime_error("Error occurred when writing header");
  }

  frame = av_frame_alloc();
  if (!frame) {
    throw std::runtime_error("Could not allocate video frame");
  }
  frame->format = codec_context->pix_fmt;
  frame->width = codec_context->width;
  frame->height = codec_context->height;

  if (av_frame_get_buffer(frame, 32) < 0) {
    throw std::runtime_error("Could not allocate frame data");
  }

  sws_context = sws_getContext(width, height, AV_PIX_FMT_RGB24, width, height,
                               codec_context->pix_fmt, SWS_BICUBIC, nullptr,
                               nullptr, nullptr);
}

auto ProtoMovWriter::make_mov(Animation anim) -> int {
  for (int i = 0; i < fps * duration; ++i) {
    if (av_frame_make_writable(frame) < 0) {
      std::cerr << "Frame not writable" << std::endl;
      break;
    }

    uint8_t* rgb_data[1] = {new uint8_t[width * height * 3]};
    int rgb_linesize[1] = {3 * width};
    fill_gradient(rgb_data[0], rgb_linesize[0], anim);
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
}

ProtoMovWriter::~ProtoMovWriter() {
  av_write_trailer(format_context);
  avcodec_free_context(&codec_context);
  av_frame_free(&frame);
  sws_freeContext(sws_context);
  if (!(output_format->flags & AVFMT_NOFILE)) {
    avio_closep(&format_context->pb);
  }
  avformat_free_context(format_context);
}

}  // namespace scarf::dispatch
