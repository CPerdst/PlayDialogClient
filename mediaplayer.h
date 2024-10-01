#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QThread>
#include <QImage>
#include <QString>

class MediaPlayer : public QThread {
Q_OBJECT
signals:
    void SIG_sendImage(QImage image);

public:
    MediaPlayer();
    void run();

private:
    QString m_filePath;
};


#endif // MEDIAPLAYER_H
