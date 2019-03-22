#include "mainwindow.h"
#include "globals.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    gWindow = &w;
    w.show();

    return a.exec();
}
