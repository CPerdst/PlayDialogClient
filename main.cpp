#include "playdialog.h"
#include <QApplication>
#include <QDebug>

#undef main
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PlayDialog w;
    w.show();

    av_register_all();
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        qDebug() << "SDL Init failed\n";
        return 0;
    }
    qDebug() << avcodec_version() << "\n";

    return a.exec();
}
