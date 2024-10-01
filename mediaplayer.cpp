#include "mediaplayer.h"
#include "QDebug"

MediaPlayer::MediaPlayer(){

}

void MediaPlayer::run(){
    qDebug() << __func__ << "\n";
    m_filePath = "C:/Users/l1Akr/Pictures/微信图片_20240918174554.jpg";
    char* path = const_cast<char*>(m_filePath.toStdString().c_str());
    QImage tmpImage = QImage(path);
    Q_EMIT SIG_sendImage(tmpImage);
}
