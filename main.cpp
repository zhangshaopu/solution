#include "mainwindow.h"

#include <QApplication>
#include <QTime>
#include <QSplashScreen>
#include <QPixmap>
#include <qthread.h>

void sleep(unsigned int msec)
{
    QTime reachTime=QTime::currentTime().addMSecs(msec);
    while(QTime::currentTime()<reachTime)
    QApplication::processEvents(QEventLoop::AllEvents,100);
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //w.showMaximized();
    w.show();
    return a.exec();
}
