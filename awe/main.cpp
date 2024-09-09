#include "mainwindow.h"

#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow mainwin;
    mainwin.showMaximized();
    return app.exec();
}
