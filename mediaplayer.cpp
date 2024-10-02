#include "mediaplayer.h"
#include "QDebug"

MediaPlayer::MediaPlayer(){

}

void MediaPlayer::find_stream_form_context(AVFormatContext* context, int* video_stream , int* audio_stream){
    if(!context) return;
    for(unsigned int i = 0; i < context->nb_streams; i++){
        if(context->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO){
            *video_stream = i;
        }else if(context->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO){
            *audio_stream = i;
        }
    }
    return;
}

void MediaPlayer::run(){
    qDebug() << __func__ << "\n";
//    m_filePath = "C:/Users/l1Akr/Pictures/微信图片_20240918174554.jpg";
    m_filePath = "D:/h.mp4";
    char* path = const_cast<char*>(m_filePath.toStdString().c_str());

    // 申请formatctf
    int res = -1;
    AVFormatContext* formatContext = avformat_alloc_context();
    res = avformat_open_input(&formatContext, path, NULL, NULL);
    if(res < 0){
        qDebug() << "open format context failed\n";
        return;
    }

    res = avformat_find_stream_info(formatContext, NULL);
    if(res < 0){
        qDebug() << "can not find stream info\n";
        avformat_close_input(&formatContext);
        avformat_free_context(formatContext);
        return;
    }

    // 打印一下媒体文件信息
    av_dump_format(formatContext, 0, path, false);

    int video_stream_index = -1;
    int audio_stream_index = -1;

    // 查找音频视频流上下文信息
    find_stream_form_context(formatContext, &video_stream_index, &audio_stream_index);

    if(video_stream_index == -1){
        qDebug() << "video stream index not found\n";
        avformat_close_input(&formatContext);
        avformat_free_context(formatContext);
        return;
    }

    // 查找视频解码器
    AVCodecContext* videoCodecCtx = formatContext->streams[video_stream_index]->codec;
    AVCodec* videoCodec = avcodec_find_decoder(videoCodecCtx->codec_id);
    if(!videoCodec){
        qDebug() << "can not find video codec\n";
        avformat_close_input(&formatContext);
        avformat_free_context(formatContext);
        return;
    }

    // 打开解码器
    res = avcodec_open2(videoCodecCtx, videoCodec, NULL);
    if(res < 0){
        qDebug() << "video codec open failed\n";
        avformat_close_input(&formatContext);
        avformat_free_context(formatContext);
        return;
    }

    // 开始提取视频包

    AVPacket* packet;
    AVFrame* frame, *frameRGB;
    frame = av_frame_alloc();
    frameRGB = av_frame_alloc();
    int y_size = videoCodecCtx->width * videoCodecCtx->height;
    packet = (AVPacket*)malloc(sizeof(AVPacket));
    av_new_packet(packet, y_size);

    // 初始化用于转换frame为RGB格式的转化器上下文
    struct SwsContext* image_convert_context;
    image_convert_context = sws_getContext(videoCodecCtx->width, videoCodecCtx->height, videoCodecCtx->pix_fmt,\
                                           videoCodecCtx->width, videoCodecCtx->height, AV_PIX_FMT_RGB32, SWS_BICUBIC, NULL, NULL, NULL);
    int numBytes = avpicture_get_size(AV_PIX_FMT_RGB32, videoCodecCtx->width, videoCodecCtx->height);
    uint8_t* out_buffer = (uint8_t*)av_malloc(numBytes * sizeof(uint8_t));
    avpicture_fill((AVPicture*)frameRGB, out_buffer, AV_PIX_FMT_RGB32, videoCodecCtx->width, videoCodecCtx->height);

    auto begin_time = av_gettime();
    int64_t pts = 0;

    // 开始循环获取文件中的packet
    int ret, got_picture;
    while(true){
        if(av_read_frame(formatContext, packet) < 0){
            break;
        }
        if(packet->stream_index == video_stream_index){
            ret = avcodec_decode_video2(videoCodecCtx, frame, &got_picture, packet);
            if(ret < 0){
                qDebug() << "decode video frame error\n";
                return;
            }
            pts = frame->pts = frame->best_effort_timestamp;
            pts *= 1000000 * av_q2d(formatContext->streams[video_stream_index]->time_base);
            int64_t real_time = av_gettime() - begin_time;
            while(pts > real_time){
                msleep(10);
                real_time = av_gettime() - begin_time;
            }

            if(got_picture){
                sws_scale(image_convert_context, (uint8_t const * const *)frame->data,
                          frame->linesize, 0, videoCodecCtx->height,
                          frameRGB->data, frameRGB->linesize);
                QImage tmpImage = QImage((uchar*)out_buffer, videoCodecCtx->width, videoCodecCtx->height, QImage::Format_RGB32);
                Q_EMIT SIG_sendImage(tmpImage);
            }
        }else if(packet->stream_index == audio_stream_index){
            av_free_packet(packet);
        }else{
            av_free_packet(packet);
        }
    }

    av_free(frame);
    av_free(frameRGB);
    av_free(out_buffer);
    avcodec_close(videoCodecCtx);
    avformat_close_input(&formatContext);



}


