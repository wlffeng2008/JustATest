#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtMqtt>
#include <QMqttClient>

#include "gencomport.h"
#include "ffmpegplayer.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *event) override;
    bool eventFilter(QObject *watched,QEvent *event) override;
private:
    Ui::MainWindow *ui;

    QMqttClient m_mqtt;
    GenComport  m_port;

    QImage m_img ;
    FFMpegPlayer *m_player=nullptr;
};
#endif // MAINWINDOW_H
