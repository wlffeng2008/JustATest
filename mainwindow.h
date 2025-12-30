#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtMqtt>
#include <QMqttClient>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

#include "gencomport.h"
#include "ffmpegplayer.h"

class HttpHandler : public QObject {
    Q_OBJECT
public:
    explicit HttpHandler(QObject *parent = nullptr) : QObject(parent) {
        manager = new QNetworkAccessManager(this);
    }

    // 发送 GET 请求
    void get(const QString &url) {
        QUrl xxx(url) ;
        QNetworkRequest request(xxx);
        QNetworkReply *reply = manager->get(request);
        connect(reply, &QNetworkReply::finished, this, [=]() {
            handleReply(reply);
        });
    }

    // 发送 POST 请求
    // void post(const QString &url, const QByteArray &data) {
    //     QNetworkRequest request(QUrl(url));
    //     request.setHeader(QNetworkRequest::ContentTypeHeader, QByteArray("application/json"));
    //     QNetworkReply *reply = manager->post(request, data);
    //     connect(reply, &QNetworkReply::finished, this, [=]() {
    //         handleReply(reply);
    //     });
    // }

private slots:
    // 统一处理响应
    void handleReply(QNetworkReply *reply) {

        if (reply->error() != QNetworkReply::NoError) {
            QString error = reply->errorString();
            qDebug() << "错误：" << error;
        } else {
            int code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            QString text = reply->readAll();
            //qDebug() << "状态码：" << code;
            //qDebug() << "响应：" << text;
            onHttpReturn(text,code);
        }
        reply->deleteLater();
    }

signals:
    void onHttpReturn(const QString&text,int code);
    void onHttpWrror(const QString&error);

private:
    QNetworkAccessManager *manager;
};


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

    HttpHandler *m_http;

    QImage m_img ;
    FFMpegPlayer *m_player=nullptr;
};
#endif // MAINWINDOW_H
