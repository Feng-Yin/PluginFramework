#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QDialog;
class QLabel;
class QLineEdit;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void activeCodeChanged(QString);

private slots:
    QString calActiveCode(QString machineCode);
private:
    void generateRegisterCode();

    QWidget *registerWidget;
    QLabel *machineCodeLabel;
    QLineEdit *machineCodeLineEdit;
    QLabel *registerCodeLabel;
    QLineEdit *registerCodeLineEdit;
};

#endif // MAINWINDOW_H
