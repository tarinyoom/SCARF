#include "make_mov_writer.hpp"

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}

class MovWriter : public IVideoWriter {
  AVFormatContext* format_context = nullptr;
  AVStream* video_stream = nullptr;
  AVCodecContext* codec_context = nullptr;
  AVFrame* frame = nullptr;
  AVPacket packet;
  const AVOutputFormat* output_format;
  struct SwsContext* sws_context;

  int width;
  int height;
  int fps;
  int frame_number;

 public:
  MovWriter(std::string filepath, int width, int height, int fps)
      : width(width), height(height), fps(fps), frame_number(1) {
    // Allocate memory for the char* and copy the string into it
    char* path_cstr =
        new char[filepath.size() + 1];  // +1 for the null terminator
    std::strcpy(path_cstr, filepath.c_str());

    avformat_alloc_output_context2(&format_context, nullptr, "mov", path_cstr);
    if (!format_context) {
      std::cerr << "Could not create output context" << std::endl;
    }

    output_format = format_context->oformat;

    const AVCodec* codec = avcodec_find_encoder(AV_CODEC_ID_H264);
    if (!codec) {
      std::cerr << "Codec not found" << std::endl;
    }

    video_stream = avformat_new_stream(format_context, codec);
    if (!video_stream) {
      std::cerr << "Could not create video stream" << std::endl;
    }

    codec_context = avcodec_alloc_context3(codec);
    if (!codec_context) {
      std::cerr << "Could not allocate video codec context" << std::endl;
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
      std::cerr << "Could not open codec" << std::endl;
    }

    video_stream->time_base = codec_context->time_base;

    if (avcodec_parameters_from_context(video_stream->codecpar, codec_context) <
        0) {
      std::cerr << "Could not copy codec parameters" << std::endl;
    }

    if (!(output_format->flags & AVFMT_NOFILE)) {
      if (avio_open(&format_context->pb, path_cstr, AVIO_FLAG_WRITE) < 0) {
        std::cerr << "Could not open output file" << std::endl;
      }
    }

    if (avformat_write_header(format_context, nullptr) < 0) {
      std::cerr << "Error occurred when writing header" << std::endl;
    }

    frame = av_frame_alloc();
    if (!frame) {
      std::cerr << "Could not allocate video frame" << std::endl;
    }
    frame->format = codec_context->pix_fmt;
    frame->width = codec_context->width;
    frame->height = codec_context->height;

    if (av_frame_get_buffer(frame, 32) < 0) {
      std::cerr << "Could not allocate frame data" << std::endl;
    }

    sws_context = sws_getContext(width, height, AV_PIX_FMT_RGB24, width, height,
                                 codec_context->pix_fmt, SWS_BICUBIC, nullptr,
                                 nullptr, nullptr);
  }

  virtual void write_frame(const uint8_t* rgb_data) {
    if (av_frame_make_writable(frame) < 0) {
      std::cerr << "Frame not writable" << std::endl;
    }

    int rgb_linesize[1] = {3 * width};
    std::cout << "Generating frame " << frame_number << ". " << std::endl;

    sws_scale(sws_context, &rgb_data, rgb_linesize, 0, height, frame->data,
              frame->linesize);

    frame->pts = frame_number;

    av_init_packet(&packet);
    packet.data = nullptr;
    packet.size = 0;

    int ret = avcodec_send_frame(codec_context, frame);
    if (ret < 0) {
      std::cerr << "Error sending frame to codec context" << std::endl;
    }

    ret = avcodec_receive_packet(codec_context, &packet);
    if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
      std::cerr << "Error during encoding" << std::endl;
    } else if (ret < 0) {
      std::cerr << "Error during encoding" << std::endl;
    }

    av_packet_rescale_ts(&packet, codec_context->time_base,
                         video_stream->time_base);
    packet.stream_index = video_stream->index;

    ret = av_interleaved_write_frame(format_context, &packet);
    if (ret < 0) {
      std::cerr << "Error while writing video frame" << std::endl;
    }

    av_packet_unref(&packet);
    frame_number++;
  }

  ~MovWriter() {
    av_write_trailer(format_context);

    avcodec_free_context(&codec_context);
    av_frame_free(&frame);
    sws_freeContext(sws_context);
    if (!(output_format->flags & AVFMT_NOFILE)) {
      avio_closep(&format_context->pb);
    }
    avformat_free_context(format_context);
  }
};

auto make_mov_writer(std::string_view filepath, int height, int width, int fps)
    -> std::unique_ptr<IVideoWriter> {
  return std::make_unique<MovWriter>(std::string(filepath), height, width, fps);
}
