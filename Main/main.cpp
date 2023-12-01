#include <QtWidgets>
#include <singleapplication.h>

#include "helper.h"
#include "mainwindow.h"

void raiseWidget(QWidget *widget) {
#ifdef Q_OS_WINDOWS
    HWND hwnd = (HWND)widget->winId();
    // check if widget is minimized to Windows task bar
    if (::IsIconic(hwnd)) {
        ::ShowWindow(hwnd, SW_RESTORE);
    }
    ::SetForegroundWindow(hwnd);
#else
    widget->show();
    widget->raise();
    widget->activateWindow();
#endif
}

int main(int argv, char *args[]) {
#ifdef Q_OS_WINDOWS
    SingleApplication app(argv, args, true);
    if (app.isSecondary()) {
        AllowSetForegroundWindow(DWORD(app.primaryPid()));
        app.sendMessage("RAISE_WIDGET");
        return 0;
    }
#else
    SingleApplication app(argv, args);
#endif

    INSTALL_TRANSLATION;

    if (!MainWindow::isRegistered()) {
        QMessageBox::information(
            0, QObject::tr("Error"),
            QObject::tr("This software has not been registered!"));
    }
    MainWindow *mainWindow = MainWindow::getInstance();

#ifdef Q_OS_WINDOWS
    QObject::connect(&app, &SingleApplication::receivedMessage, mainWindow,
                     [mainWindow]() { raiseWidget(mainWindow); });
#else
    QObject::connect(&app, &SingleApplication::instanceStarted, mainWindow,
                     [mainWindow]() { raiseWidget(mainWindow); });
#endif

    // mainWindow->setGeometry(100, 100, 800, 500);
    mainWindow->showMaximized();
    mainWindow->show();
    int ret = app.exec();
    MainWindow::release();
    return ret;
}
