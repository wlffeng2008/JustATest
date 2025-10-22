/********************************************************************************
** Form generated from reading UI file 'DialogToast.ui'
**
** Created by: Qt User Interface Compiler version 6.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGTOAST_H
#define UI_DIALOGTOAST_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>

QT_BEGIN_NAMESPACE

class Ui_DialogToast
{
public:
    QFrame *frame;
    QLabel *labelIcon;
    QLabel *label;

    void setupUi(QDialog *DialogToast)
    {
        if (DialogToast->objectName().isEmpty())
            DialogToast->setObjectName("DialogToast");
        DialogToast->resize(600, 50);
        DialogToast->setStyleSheet(QString::fromUtf8(""));
        frame = new QFrame(DialogToast);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(0, 0, 600, 50));
        frame->setFrameShape(QFrame::Shape::StyledPanel);
        frame->setFrameShadow(QFrame::Shadow::Raised);
        labelIcon = new QLabel(frame);
        labelIcon->setObjectName("labelIcon");
        labelIcon->setGeometry(QRect(10, 5, 40, 40));
        labelIcon->setMinimumSize(QSize(40, 40));
        label = new QLabel(frame);
        label->setObjectName("label");
        label->setGeometry(QRect(70, 5, 520, 40));
        QFont font;
        font.setPointSize(14);
        label->setFont(font);
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label->setWordWrap(true);

        retranslateUi(DialogToast);

        QMetaObject::connectSlotsByName(DialogToast);
    } // setupUi

    void retranslateUi(QDialog *DialogToast)
    {
        DialogToast->setWindowTitle(QCoreApplication::translate("DialogToast", "Dialog", nullptr));
        labelIcon->setText(QCoreApplication::translate("DialogToast", "TextLabel", nullptr));
        label->setText(QCoreApplication::translate("DialogToast", "\346\217\220\347\244\272\344\277\241\346\201\257\357\274\201", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DialogToast: public Ui_DialogToast {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGTOAST_H
