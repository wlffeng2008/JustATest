#include "ringloop.h"

#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>

RingLoop::RingLoop(QWidget *parent):QLabel(parent)
{
    setStyleSheet("QLabel{min-width:20px;min-height:20px;}");
    connect(&m_TMDraw,&QTimer::timeout,this,[=]{
        if(!m_pause)
        {
            m_curAngle += 10 ;
            update();
        }
    });
    m_TMDraw.start(20);
}


void RingLoop::paintEvent(QPaintEvent *event)
{
    QPainter painter(this) ;
    painter.setRenderHint(QPainter::Antialiasing);
    QRect rect = this->rect().adjusted(2,2,-2,-2);//QRect(250,200,nRadius*2,nRadius*2);
    if(rect.height()%2)
        rect.setHeight(rect.height()+1);
    rect.setWidth(rect.height());

    int nPW = 2 ;

    painter.setPen(QPen(QBrush(Qt::black),nPW));
    painter.drawArc(rect,m_curAngle*16,360*12);
    event->accept();
}

void RingLoop::mousePressEvent(QMouseEvent *event)
{
    m_pause = !m_pause ;
    event->accept();
}
