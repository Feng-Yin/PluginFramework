#include "mainwindow.h"
#if QT_VERSION >= 0x050000
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QInputDialog>
#include <QtWidgets>
#include <QHBoxLayout>
#else
#include <QtGui>
#endif

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      registerWidget(NULL),
      machineCodeLabel(NULL),
      machineCodeLineEdit(NULL),
      registerCodeLabel(NULL),
      registerCodeLineEdit(NULL)
{
    generateRegisterCode();
}

MainWindow::~MainWindow()
{

}

void MainWindow::generateRegisterCode()
{
    if(!registerWidget)
    {
        registerWidget = new QWidget(this);
        setWindowTitle(tr("Register Softeware"));
        setWindowIcon(QIcon(":/Icon/register_icon.png"));

        machineCodeLabel = new QLabel(tr("Machine Code: "));
        machineCodeLineEdit = new QLineEdit();

        registerCodeLabel = new QLabel(tr("Register Code: "));
        registerCodeLineEdit = new QLineEdit();

        connect(machineCodeLineEdit, SIGNAL(textChanged(QString)),
                this, SLOT(calActiveCode(QString)));
        connect(this, SIGNAL(activeCodeChanged(QString)), registerCodeLineEdit, SLOT(setText(QString)));

        QHBoxLayout *machineCodeLayout = new QHBoxLayout();
        machineCodeLayout->addWidget(machineCodeLabel);
        machineCodeLayout->addWidget(machineCodeLineEdit);

        QHBoxLayout *registerCodeLayout = new QHBoxLayout();
        registerCodeLayout->addWidget(registerCodeLabel);
        registerCodeLayout->addWidget(registerCodeLineEdit);

        QVBoxLayout *containerLayout = new QVBoxLayout();

        containerLayout->addLayout(machineCodeLayout);
        containerLayout->addLayout(registerCodeLayout);

        registerWidget->setLayout(containerLayout);
        setCentralWidget(registerWidget);
        resize(this->sizeHint().width()*2, this->sizeHint().height());
    }
}

QString MainWindow::calActiveCode(QString machineCode)
{
    const QString parm1("!@#");
    const QString parm2("$%^");
    const QString parm3("&*(");
    const QString parm4(")_+");

    QString seed = parm1 + machineCode + parm2 + parm3 + parm4;

    QCryptographicHash sha1(QCryptographicHash::Sha1);

#if QT_VERSION < 0x050000
    QByteArray datagram(seed.toAscii());
#else
    QByteArray datagram(seed.toLatin1());
#endif
    const char* tempConstChar = datagram.data();
    sha1.addData(tempConstChar);
    QString  activeCode=sha1.result().toHex();

    //QClipboard *board = QApplication::clipboard();
    //board->setText(activeCode);

    emit(activeCodeChanged(activeCode));
    return activeCode;
}
