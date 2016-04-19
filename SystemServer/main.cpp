#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //setting window size and title
    w.setMaximumSize(621,705);
    w.setMinimumSize(621,705);
    w.setWindowTitle("BusSystemServerManager");
    w.show();

    return a.exec();
}
