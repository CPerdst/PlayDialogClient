#include "playdialog.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PlayDialog w;
    w.show();

    av_register_all();
    qDebug() << avcodec_version() << "\n";

    return a.exec();
}
