#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "DialogToast.h"

#include <QPainter>
#include <QWindow>

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

    QString strOS = "unkown!" ;
#ifdef Q_OS_WIN
    // Windows 平台处理
    strOS = "Windows";
#elif defined(Q_OS_LINUX)  // 正确写法：检查 Q_OS_LINUX 是否定义
    // Linux 平台处理
    strOS = "Linux";
#elif defined(Q_OS_MACOS)
    // macOS 平台处理
    strOS = "MacOS";
#else
    // 其他平台处理
#endif

    ui->labelOS->setText(strOS);

    m_mqtt.setUsername("");
    m_mqtt.setPassword("");
    m_mqtt.setHostname("test.mosquitto.org");
    m_mqtt.setPort(1883);
    m_mqtt.connectToHost();
    connect(&m_mqtt,&QMqttClient::connected,this,[=]{
        qDebug() << "MqttClient::connected"  ;
        EasyToast::information("MqttClient::connected!") ;
    }) ;

    connect(ui->checkBoxTop,&QCheckBox::clicked,this,[=](bool checked){
        QWindow *pWin = windowHandle() ;
        if(checked)
            pWin->setFlags(windowFlags() | Qt::WindowStaysOnTopHint) ;
        else
            pWin->setFlags(windowFlags() & ~Qt::WindowStaysOnTopHint) ;

        setWindowFlag(Qt::WindowStaysOnTopHint, checked);
        show() ;
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter  painter(this) ;
    painter.setRenderHint(QPainter::Antialiasing);
    QRect rect(200,200,101,101) ;

    painter.setPen(Qt::red);
    painter.drawArc(rect,0,360*16);
    painter.drawRect(rect);

    int nPW = 2 ;
    int nRadius = 18 ;
    rect=QRect(250,200,nRadius*2,nRadius*2);
    painter.setPen(QPen(QBrush(0x6329B6),nPW));
    painter.setBrush(0x6329B6);
    painter.drawArc(rect,0,360*16);
    painter.drawEllipse(rect.center()+QPoint(1,1),nRadius-nPW*2,nRadius-nPW*2);

    QImage checkImg(20,20,QImage::Format_ARGB32) ;
    checkImg.fill(Qt::transparent);
    {

        //painter.setRenderHint(QPainter::Antialiasing,false);
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
    painter.drawImage(QRect(320,200,64,64),checkImg);
    painter.drawImage(QRect(400,200,32,32),checkImg);
    painter.drawImage(QRect(400,240,20,20),checkImg);
    painter.drawImage(QRect(400,280,16,16),checkImg);
    painter.drawImage(QRect(400,300,12,12),checkImg);
}
