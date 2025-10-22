#ifndef CUSTUMRADIOBUTTON_H
#define CUSTUMRADIOBUTTON_H

#include <QObject>
#include <QRadioButton>

class CustumRadioButton : public QRadioButton
{
    Q_OBJECT
public:
    explicit CustumRadioButton(QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent *event) override ;
signals:
};

#endif // CUSTUMRADIOBUTTON_H
