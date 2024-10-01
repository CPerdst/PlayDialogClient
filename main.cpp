#include "playdialog.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PlayDialog w;
    w.show();
    return a.exec();
}
