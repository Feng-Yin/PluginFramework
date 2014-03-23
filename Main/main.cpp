#include <QtGui>
#include "mainwindow.h"
#include "qtsingleapplication.h"

int main(int argv, char *args[])
{
    //QApplication app(argv, args);
    QtSingleApplication app(argv, args);
    QTextCodec *codec = QTextCodec::codecForName("System");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);

    QDir qmdir(":/Translations");
    foreach (QString fileName, qmdir.entryList(QDir::Files)) {
        //qDebug()<<QFileInfo(fileName).baseName();
        QTranslator *qtTranslator = new QTranslator();
        qtTranslator->load(QFileInfo(fileName).baseName(), ":/Translations");
        QApplication::instance()->installTranslator(qtTranslator);
    }

    //QMessageBox::information(0, QObject::tr("Error"), getHDLogicalID());
    //QMessageBox::information(0, QObject::tr("Error"), calActiveCode(getHDLogicalID()));

    if(!MainWindow::isRegistration()) {
        QMessageBox::information(0, QObject::tr("Error"), QObject::tr("This software has not been registered!"));
        //return 0;
    }

    if(app.isRunning()) {
        QMessageBox::information(0, QObject::tr("Error"), QObject::tr("Application is alreay running!"));
        return 0;
    }

    MainWindow* mainWindow = MainWindow::getInstance();

    //mainWindow->setGeometry(100, 100, 800, 500);
    mainWindow->showMaximized();
    mainWindow->show();
    int ret = app.exec();
    MainWindow::release();
    return ret;
}


