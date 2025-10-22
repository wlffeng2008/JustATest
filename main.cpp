#include "mainwindow.h"

#include <QApplication>
#include "LogToFile.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    tcl_log::LogInit() ;
    MainWindow w;
    w.show();
    return a.exec();
}
