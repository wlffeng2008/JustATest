#include "custumradiobutton.h"
#include <QPainter>
CustumRadioButton::CustumRadioButton(QWidget *parent)
    : QRadioButton{parent}
{
    setCursor(Qt::PointingHandCursor);
}

void CustumRadioButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this) ;
    painter.setRenderHint(QPainter::Antialiasing);
    QRect rect = this->rect().adjusted(1,1,-1,-1);//QRect(250,200,nRadius*2,nRadius*2);
    if(rect.height()%2)
        rect.setHeight(rect.height()+1);
    rect.setWidth(rect.height());

    int nPW = 2 ;
    int nRadius = rect.height()/2 ;
    if(nRadius>10)
        nRadius = 10 ;
    while(rect.height()>nRadius * 2)
        rect.adjust(1,1,-1,-1);
    if(isChecked())
    {
        painter.setPen(QPen(QBrush(0x6329B6),nPW));
        painter.drawArc(rect,0,360*16);
        painter.setBrush(0x6329B6);
        painter.drawEllipse(rect.center()+QPoint(1,1),nRadius-5,nRadius-5);
    }
    else
    {
        painter.setPen(QPen(QBrush(Qt::black),nPW));
        painter.drawArc(rect,0,360*16);
    }

    painter.setFont(this->font());
    rect = this->rect();
    rect.adjust(rect.height()+5,1,-1,-1);
    painter.drawText(rect,text(),Qt::AlignVCenter|Qt::AlignLeft);
}
