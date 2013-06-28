#include <QtGui/QApplication>
#include <QThread>
#include <QTranslator>
#include <QTextCodec>
#include "mainwindow.h"
#include "tlasershm.h"
#include "tlasermsg.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    translator.load("Laser_pl");
    a.installTranslator(&translator);

    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));

    TLaserShm * LaserShm = new TLaserShm();
    QThread * MessangerThread = new QThread();
    TLaserMsg * messanger = new TLaserMsg();
    MainWindow w;

    QObject::connect(messanger, SIGNAL(messageReceived(QString)), &w, SLOT(messageHit(QString)));

    messanger->moveToThread(MessangerThread);
    MessangerThread->start();
    QMetaObject::invokeMethod(messanger, "doWork", Qt::QueuedConnection);

    QObject::connect(&w,SIGNAL(pwmChanged(int)),LaserShm,SLOT(ustawCzasImpulsu(int)));
   // w.show();
    w.showFullScreen();
    return a.exec();
}
