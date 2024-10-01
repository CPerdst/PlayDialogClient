#include "playdialog.h"
#include <QApplication>
#include <QDebug>

extern "C"
{
    #include "libavcodec/avcodec.h"
    #include "libavformat/avformat.h"
    #include "libavutil/pixfmt.h"
    #include "libswscale/swscale.h"
    #include "libavformat/avformat.h"
    #include "libavdevice/avdevice.h"
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PlayDialog w;
    w.show();

    // 注册 ffmpeg
    av_register_all();
    qDebug() << avcodec_version();

    return a.exec();
}
