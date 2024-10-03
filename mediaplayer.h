#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QThread>
#include <QImage>
#include <QString>
#include "packetqueue.h"

#ifdef __cplusplus
extern "C"
{
#endif
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/pixfmt.h"
#include "libswscale/swscale.h"
#include "libavformat/avformat.h"
#include "libavdevice/avdevice.h"
#include "libavutil/time.h"
#include "libswresample/swresample.h"
#include "SDL.h"
#ifdef __cplusplus
}
#endif


class MediaPlayer : public QThread {
Q_OBJECT
signals:
    void SIG_sendImage(QImage image);

public:
    MediaPlayer();
    void run();
    static void audio_callback(void* userdata, uint8_t* stream, int len);

private:
    void find_stream_form_context(AVFormatContext* context, int* video_stream, int* audio_stream);

public:
    static PacketQueue* audioQueue;

private:
    QString m_filePath;
};


#endif // MEDIAPLAYER_H
