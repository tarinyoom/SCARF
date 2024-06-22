extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libswscale/swscale.h>
}

int main() {
    // Initialize the libavformat and register all the muxers, demuxers and protocols.
    avformat_network_init();

    AVFormatContext* formatContext = nullptr;
    
    // Allocate the output media context
    avformat_alloc_output_context2(&formatContext, nullptr, nullptr, "output.mov");
    if (!formatContext) {
        fprintf(stderr, "Could not allocate output context\n");
        return -1;
    }
    
    // Find the encoder
    const AVCodec* codec = avcodec_find_encoder(AV_CODEC_ID_H264);
    if (!codec) {
        fprintf(stderr, "Codec not found\n");
        return -1;
    }
    
    // Create a new video stream in the output file
    AVStream* stream = avformat_new_stream(formatContext, codec);
    if (!stream) {
        fprintf(stderr, "Could not allocate stream\n");
        return -1;
    }
    
    // Set up the codec context for the encoder
    AVCodecContext* codecContext = avcodec_alloc_context3(codec);
    if (!codecContext) {
        fprintf(stderr, "Could not allocate codec context\n");
        return -1;
    }
    
    codecContext->bit_rate = 400000;
    codecContext->width = 640;
    codecContext->height = 480;
    codecContext->time_base = {1, 25};
    codecContext->framerate = {25, 1};
    codecContext->gop_size = 10;
    codecContext->max_b_frames = 1;
    codecContext->pix_fmt = AV_PIX_FMT_YUV420P;
    
    // Open the codec
    if (avcodec_open2(codecContext, codec, nullptr) < 0) {
        fprintf(stderr, "Could not open codec\n");
        return -1;
    }
    
    // Set the codec parameters
    if (avcodec_parameters_from_context(stream->codecpar, codecContext) < 0) {
        fprintf(stderr, "Could not copy codec parameters\n");
        return -1;
    }
    
    // Open the output file
    if (!(formatContext->oformat->flags & AVFMT_NOFILE)) {
        if (avio_open(&formatContext->pb, "output.mov", AVIO_FLAG_WRITE) < 0) {
            fprintf(stderr, "Could not open output file\n");
            return -1;
        }
    }
    
    // Write the stream header, if any
    if (avformat_write_header(formatContext, nullptr) < 0) {
        fprintf(stderr, "Error occurred when opening output file\n");
        return -1;
    }

    // Here, you would typically encode frames and write them using av_interleaved_write_frame()

    // Write the trailer
    av_write_trailer(formatContext);
    
    // Close the codec
    avcodec_free_context(&codecContext);
    
    // Close the output file
    if (!(formatContext->oformat->flags & AVFMT_NOFILE)) {
        avio_closep(&formatContext->pb);
    }
    
    // Free the format context
    avformat_free_context(formatContext);
    
    return 0;
}
