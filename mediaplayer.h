#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QThread>
#include <QImage>
#include <QString>

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/pixfmt.h"
#include "libswscale/swscale.h"
#include "libavformat/avformat.h"
#include "libavdevice/avdevice.h"
}

class MediaPlayer : public QThread {
Q_OBJECT
signals:
    void SIG_sendImage(QImage image);

public:
    MediaPlayer();
    void run();

private:
    void find_stream_form_context(AVFormatContext* context, int* video_stream, int* audio_stream);

private:
    QString m_filePath;
};


#endif // MEDIAPLAYER_H
