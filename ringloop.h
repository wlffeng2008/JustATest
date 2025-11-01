#ifndef RINGLOOP_H
#define RINGLOOP_H

#include <QLabel>
#include <QTimer>

class RingLoop : public QLabel
{
    Q_OBJECT
public:
    RingLoop(QWidget *parent=nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
private:
    bool m_pause = false ;
    QTimer m_TMDraw ;
    float m_curAngle = 0;
};

#endif // RINGLOOP_H
