#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);
//    a.setStyle(QStyleFactory::create("windowsvista"));
    a.setStyle("windowsvista");
//    QApplication::setStyle("fusion");

    Widget w;
//    w.show();

    return a.exec();
}

