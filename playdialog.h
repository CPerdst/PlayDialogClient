#ifndef PLAYDIALOG_H
#define PLAYDIALOG_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class PlayDialog; }
QT_END_NAMESPACE

class PlayDialog : public QMainWindow
{
    Q_OBJECT

public:
    PlayDialog(QWidget *parent = nullptr);
    ~PlayDialog();

private:
    Ui::PlayDialog *ui;
};
#endif // PLAYDIALOG_H
