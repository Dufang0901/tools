#include "mainwindow.h"
#include <QApplication>
#include <C_RecvCanThread.h>
#include <QDesktopWidget>
#include <QDebug>
#include <QFontDatabase>
#include <QSettings>
#include <QTextCodec>
#include <QDebug>
#include <QCursor>
#include <QDate>
#include <QMutex>
#include <QDir>

extern "C"
{
    #include "Rtc.h"
}

void myMessageOutput(QtMsgType type, const char *msg)
 {
    static QMutex mutex;
    mutex.lock();

    QString text;
     switch (type) {
     case QtDebugMsg:
         text = QString("|Debug:");
         break;
     case QtWarningMsg:
         text = QString("|Warning:");
         break;
     case QtCriticalMsg:
         text = QString("|Critial:");
         break;
     case QtFatalMsg:
         text = QString("|Fatal:");
         abort();
     }
     QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
     QString message = QString("%1 %2 %3").arg(current_date_time).arg(text).arg(msg);


     QFile file("/home/bic/Code/log/qtlog_"+QDateTime::currentDateTime().toString("yyyy-MM-dd_hh")+".txt");
     file.open(QIODevice::WriteOnly | QIODevice::Append);
     QTextStream text_stream(&file);
     text_stream << message << "\r\n";
     file.flush();
     file.close();
     mutex.unlock();
 }

int main(int argc, char *argv[])
{
    RtcInit();
    RtcGetTimeToSys();

#if 1
    QDir *tempDir = new QDir;
    bool exist = tempDir->exists("/home/bic/Code/log");
    if(!exist)
    {
        tempDir->mkdir("/home/bic/Code/log");
    }
    qInstallMsgHandler(myMessageOutput);
#endif

    QApplication a(argc, argv);

    //±‡¬Î∏Ò Ω…Ë÷√
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);

    //QWSServer::setCursorVisible(true);

    //qDebug()<<QApplication::desktop()->screenGeometry();
    //qDebug()<<QApplication::desktop()->width()<<","<<QApplication::desktop()->height();


    MainWindow::getInstance()->show();

    C_RecvCanThread recvCanThread;
    recvCanThread.start();

    return a.exec();
}
