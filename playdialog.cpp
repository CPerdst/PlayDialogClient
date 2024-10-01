#include "playdialog.h"
#include "ui_playdialog.h"
#include "QDebug"

PlayDialog::PlayDialog(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PlayDialog)
{
    ui->setupUi(this);
    p_mediaPlayer = new MediaPlayer();
    connect(p_mediaPlayer, SIGNAL(SIG_sendImage(QImage)), this, SLOT(SLT_recvImage(QImage)));
}

PlayDialog::~PlayDialog()
{
    delete ui;
}


void PlayDialog::on_pb_start_clicked()
{
    p_mediaPlayer->start();
}

void PlayDialog::SLT_recvImage(QImage image){
//    qDebug() << __func__ << "\n";
    const QPixmap pixmap = QPixmap::fromImage(image.scaled(ui->lb_show->width(), ui->lb_show->height(), Qt::KeepAspectRatio));
    ui->lb_show->setPixmap(pixmap);
}
