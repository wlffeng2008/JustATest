#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "EasyToast.h"

#include <QPainter>
#include <QWindow>
#include <QApplication>
#include <QStyle>

// #include <QtWaylandClient/qwaylandwindow.h>
// #include <QtWaylandClient/qwaylandxdgshell.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButtonTest,&QPushButton::clicked,this,[=]{
        toast()->active("Hello!",2);// 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
    });

    QString strOS = "Unkown!" ;
#ifdef Q_OS_WIN
    strOS = "Windows";
#elif defined(Q_OS_LINUX)  // 正确写法：检查 Q_OS_LINUX 是否定义
    strOS = "Linux";
#elif defined(Q_OS_MACOS)
    strOS = "MacOS";
#endif

    ui->labelOS->setText(strOS);

    m_mqtt.setUsername("");
    m_mqtt.setPassword("");
    m_mqtt.setClientId("qwerty12345678");
    m_mqtt.setHostname("test.mosquitto.org");
    m_mqtt.setPort(1883);
    m_mqtt.connectToHost();
    connect(&m_mqtt,&QMqttClient::connected,this,[=]{
        qDebug() << "MqttClient::connected"  ;
        EasyToast::information("MqttClient::connected!") ;
    }) ;

    connect(ui->checkBoxTop,&QCheckBox::clicked,this,[=](bool checked){
        QWindow *pWin = windowHandle() ;
        Qt::WindowFlags flags = windowFlags() ;
        if(checked)
            flags |= Qt::WindowStaysOnTopHint;
        else
            flags &= ~Qt::WindowStaysOnTopHint;

        pWin->setFlags(flags) ;

        //setWindowFlags(flags);
        show() ;
    });

    m_player = new FFMpegPlayer(this);
    connect(m_player,&FFMpegPlayer::onImage,this,[=](const QImage&image){
        m_img = image ;
        ui->labelVideo->update();
    });
    m_player->setUrl("rtmp://liteavapp.qcloud.com/live/liteavdemoplayerstreamid");
    //m_player->setUrl("rtsp://218.204.223.237:554/live/1/0547424F573B085C/gsfp90ef4k0a6iap.sdp");
    //m_player->setUrl("rtsp://184.72.239.149/vod/mp4://BigBuckBunny_175k.mov");
    //m_player->setUrl("rtmp://ns8.indexforce.com/home/mystream");
    //m_player->setUrl("rtmp://mobliestream.c3tv.com:554/live/goodtv.sdp");
    //m_player->setUrl("rtmp://media3.scctv.net/live/scctv_800");
    ui->labelVideo->installEventFilter(this);

    m_http = new HttpHandler(this) ;
    connect(m_http,&HttpHandler::onHttpReturn,this,[=](const QString&text,int code){
        qDebug() << text ;
        ui->plainTextEdit2->setPlainText(text);
    });

    connect(ui->pushButtonHttp,&QPushButton::clicked,this,[=]{
        QString text = ui->plainTextEdit1->toPlainText().trimmed();
        m_http->get(text);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject *watched,QEvent *event)
{
    if(watched == ui->labelVideo && event->type() == QEvent::Paint)
    {
        QPainter painter(ui->labelVideo);
        if(!m_img.isNull())
        {
            painter.drawImage(ui->labelVideo->rect(),m_img);
            return true ;
        }
    }
    return QMainWindow::eventFilter(watched,event);
}


void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter  painter(this) ;
    painter.setRenderHint(QPainter::Antialiasing);
    QRect rect(20,160,101,101) ;

    painter.setPen(Qt::red);
    painter.drawArc(rect,0,360*16);
    painter.drawRect(rect);

    int nPW = 2 ;
    int nRadius = 18 ;
    rect=QRect(150,160,nRadius*2,nRadius*2);
    painter.setPen(QPen(QBrush(0x6329B6),nPW));
    painter.setBrush(0x6329B6);
    painter.drawArc(rect,0,360*16);
    painter.drawEllipse(rect.center()+QPoint(1,1),nRadius-6,nRadius-6);

    QImage checkImg(20,20,QImage::Format_ARGB32) ;
    checkImg.fill(Qt::transparent);
    {
        QRect rect=QRect(0,0,20,20).adjusted(1,1,-1,-1) ;

        QPainter  painter(&checkImg) ;
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(QPen(Qt::red,2));
        painter.setBrush(Qt::NoBrush);
        //painter.drawRect(rect);
        painter.drawLine(rect.topLeft(),rect.topRight());
        painter.drawLine(rect.topLeft(),rect.bottomLeft());
        painter.drawLine(rect.bottomLeft(),rect.bottomRight());
        painter.drawLine(rect.topRight(),rect.bottomRight());

        QRect rcCheck = rect.adjusted(4,4,-4,-4);

        QPoint p1 = rcCheck.topRight() + QPoint(0,1);
        QPoint p2 = rcCheck.topLeft() + QPoint(0,rcCheck.height()*0.618) ;
        QPoint p3 = rcCheck.bottomLeft() + QPoint(rcCheck.width()*(1-0.618),0) ;

        painter.setPen(QPen(Qt::red,2,Qt::SolidLine,Qt::RoundCap));
        painter.drawLine(p1,p3);
        painter.drawLine(p2,p3);
    }

    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.drawImage(QRect(250,150,64,64),checkImg);
    painter.drawImage(QRect(600,150,128,128),checkImg);
    painter.drawImage(QRect(320,150,32,32),checkImg);
    painter.drawImage(QRect(360,150,20,20),checkImg);
    painter.drawImage(QRect(400,150,16,16),checkImg);
    painter.drawImage(QRect(420,150,12,12),checkImg);
    painter.drawImage(QPoint(420,160),QApplication::style()->standardIcon(QStyle::SP_MessageBoxInformation).pixmap(128,128).toImage());
}
