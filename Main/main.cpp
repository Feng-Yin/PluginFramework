#include <QtGui>

#include "mainwindow.h"

int main(int argv, char *args[])
{
    QApplication app(argv, args);
    QTextCodec *codec = QTextCodec::codecForName("System");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);

    MainWindow* mainWindow = MainWindow::getInstance();
    //mainWindow->setGeometry(100, 100, 800, 500);
    mainWindow->showMaximized();
    mainWindow->show();
    int ret = app.exec();
    MainWindow::release();
    return ret;
}
