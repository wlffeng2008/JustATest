#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "DialogToast.h"

#include <QPainter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButtonTest,&QPushButton::clicked,this,[=]{

        toast()->active("Hello!",2);// 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
    });

    m_mqtt.setUsername("");
    m_mqtt.setPassword("");
    m_mqtt.setHostname("test.mosquitto.org");
    m_mqtt.setPort(1883);
    m_mqtt.connectToHost();
    connect(&m_mqtt,&QMqttClient::connected,this,[=]{
        qDebug() << "MqttClient::connected"  ;
        EasyToast::information("MqttClient::connected!") ;
    }) ;
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
    qDebug() << rect.center()+QPoint(1,1);
}
