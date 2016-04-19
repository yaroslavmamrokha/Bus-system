#include "mainwindow.h"
#include <QApplication>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //set window size and title
    w.setMaximumSize(780, 410);
    w.setMinimumSize(780, 410);
    w.setWindowTitle("BusSystemClientManager");
    w.show();

    return a.exec();
}


