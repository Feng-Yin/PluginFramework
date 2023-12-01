#include "usermanagement_invoicing.h"
#include <QtGui>
#include <QtSql>
#include "helper.h"
#include "logindialog.h"
#include "mainwindow.h"
#include "usermanagementdialog.h"

UserManagement_Invoicing::UserManagement_Invoicing() :
    loginDialog(NULL),
    mainWidget(NULL),
    userManagementAction(NULL),
    changePasswordAction(NULL),
    switchUserAction(NULL),
    logOutAction(NULL),
    toolBar(NULL),
    userManagementInterface(NULL),
    parent(NULL)
{
    INSTALL_TRANSLATION;
}

UserManagement_Invoicing::~UserManagement_Invoicing()
{
}

QWidget* UserManagement_Invoicing::getMainWidget() const
{
    return mainWidget;
}

QAction* UserManagement_Invoicing::getAction() const
{
    return NULL;
}

QMenu* UserManagement_Invoicing::getMenu() const
{
    return NULL;
}

QToolBar* UserManagement_Invoicing::getToolBar() const
{
    return toolBar;
}

void UserManagement_Invoicing::initAction()
{
    userManagementAction = new QAction(this);
    userManagementAction->setIcon(QIcon(":/Icon/usermanagement_icon.png"));
    userManagementAction->setToolTip(tr("User Management"));
    connect(userManagementAction, SIGNAL(triggered()), this, SLOT(userManagement()));

    changePasswordAction = new QAction(this);
    changePasswordAction->setIcon(QIcon(":/Icon/changepassword_icon.png"));
    changePasswordAction->setToolTip(tr("Change Password"));
    connect(changePasswordAction, SIGNAL(triggered()), this, SLOT(changeCurrentUserPassword()));

    switchUserAction = new QAction(this);
    switchUserAction->setIcon(QIcon(":/Icon/switchuser_icon.png"));
    switchUserAction->setToolTip(tr("Switch User"));
    connect(switchUserAction, SIGNAL(triggered()), this, SLOT(switchUser()));

    logOutAction = new QAction(this);
    logOutAction->setIcon(QIcon(":/Icon/logout_icon.png"));
    logOutAction->setToolTip(tr("Log Off"));
    connect(logOutAction, SIGNAL(triggered()), this, SLOT(logout()));
}

void UserManagement_Invoicing::initToolBar()
{
    toolBar = new QToolBar();
    //toolBar->addAction(userManagementAction);
    toolBar->addAction(changePasswordAction);
    toolBar->addAction(switchUserAction);
    toolBar->addAction(logOutAction);
}

bool UserManagement_Invoicing::init(MainWindow *parent)
{
    PluginInterface *plugin = parent->getPlugin("UserManagementIF");
    userManagementInterface = dynamic_cast<UserManagementInterface *>(plugin);
    if(!userManagementInterface) {
        return false;
    }
    else {
        userManagementInterface->registeObserver(this);
    }
    initAction();
    initToolBar();

    if(!showLoginWindow()) {
        exit(0);
    }
    userManagement();
    userChanged();
    this->parent = parent;
    return true;
}

bool UserManagement_Invoicing::deInit()
{
    userManagementInterface->deregisteObserver(this);
    delete mainWidget;
    mainWidget = NULL;
    return true;
}

void UserManagement_Invoicing::userChanged()
{
    if(userManagementInterface->checkAccess(getAccessRoleNameSet())) {
        if(mainWidget) {
            QObjectList children = mainWidget->children();
            foreach(QObject *object, children) {
                QWidget *widget = qobject_cast<QWidget *>(object);
                if(widget) {
                    widget->show();
                }
            }
        }
    }
    else {
        if(mainWidget) {
            QObjectList children = mainWidget->children();
            foreach(QObject *object, children) {
                QWidget *widget = qobject_cast<QWidget *>(object);
                if(widget) {
                    widget->hide();
                }
            }
        }
    }
}

QString UserManagement_Invoicing::moduleName() const
{
    return this->metaObject()->className();
}

QString UserManagement_Invoicing::moduleDescription() const
{
    return tr("User Management Function");
}

QSet<QString> UserManagement_Invoicing::getAccessRoleNameSet() const
{
    return QSet<QString>()<<"管理员";
}

QSet<QString> UserManagement_Invoicing::getDependencySet() const
{
    return QSet<QString>()<<"UserManagementIF";
}

void UserManagement_Invoicing::logout()
{
    parent->hide();
    if(!showLoginWindow()) {
        exit(0);
    }
    else {
        parent->show();
        update();
    }

}

bool UserManagement_Invoicing::showLoginWindow()
{
    delete loginDialog;
    loginDialog = new LoginDialog(userManagementInterface);
    if(loginDialog->exec() != QDialog::Accepted) {
        return false;
    }
    else
    {
        return true;
    }
}

void UserManagement_Invoicing::userManagement()
{
    if(!mainWidget) {
        mainWidget = new UserManagementDialog(userManagementInterface);
        connect(mainWidget, SIGNAL(trigerChangePassword(QString)), this, SLOT(changePassword(QString)));
    }
    QString username = userManagementInterface->getCurrentUserName();
    if(userManagementInterface->isAdmin(username)) {
        mainWidget->setEnabled(true);
    }
    else {
        mainWidget->setEnabled(false);
    }
}

void UserManagement_Invoicing::changeCurrentUserPassword()
{
    bool okOldPassword = false;
    bool okDBOpen = false;
    do {
        QString username = userManagementInterface->getCurrentUserName();
        QString oldPassword = QInputDialog::getText(0, tr("Change Password"),
                                                    QString(tr("Old Password for user '%1': ")).arg(username),
                                                    QLineEdit::Password, "", &okOldPassword);
        if(!okOldPassword) {
            //QMessageBox::critical(0, tr("Change Password"), tr("Input Old Password error !"));
            return;
        }

        QString ipaddress = userManagementInterface->getCurrentIPAdress();
        int port = userManagementInterface->getCurrentPort();
        okDBOpen = userManagementInterface->openDatabase(username, oldPassword, ipaddress, port);
        if (!okDBOpen) {
            QMessageBox::critical(0, tr("Change Password"), tr("Old Password is wrong, try it again"));
        }
    }while(!okDBOpen);

    changePassword(userManagementInterface->getCurrentUserName());
}

void UserManagement_Invoicing::changePassword(QString username)
{
    bool okNewPassword1 = false;
    QString newPassword1;
    bool okNewPassword2 = false;
    QString newPassword2;
    do {
        do {
            newPassword1 = QInputDialog::getText(0, tr("Change Password"),
                                                 QString(tr("New Password for user '%1': ")).arg(username),
                                                 QLineEdit::Password, "", &okNewPassword1);
            if(!okNewPassword1) {
                //QMessageBox::critical(0, tr("Change Password"), tr("Input New Password error !"));
                return;
            }
            if(newPassword1.isEmpty()) {
                QMessageBox::critical(0, tr("Change Password"), tr("Password can't be empty !"));
            }
            else if(!UserManagementInterface::checkPassword(newPassword1)) {
                QMessageBox::critical(0, tr("Change Password"), tr("Password illegal !"));
            }
        }while(newPassword1.isEmpty()||!UserManagementInterface::checkPassword(newPassword1));

        newPassword2 = QInputDialog::getText(0, tr("Change Password"),
                                             QString(tr("Re-confirm New Password for user '%1': ")).arg(username),
                                             QLineEdit::Password, "", &okNewPassword2);

        if(!okNewPassword2) {
            //QMessageBox::critical(0, tr("Change Password"), tr("Re-confirm New Password error !"));
            return;
        }

        if(newPassword1.compare(newPassword2)!=0) {
            QMessageBox::critical(0, tr("Change Password"),
                                  tr("The two entries of the new password don't match , Retry !"));
        }
    }while(newPassword1.compare(newPassword2)!=0);
    userManagementInterface->changePassword(username, newPassword1);
    QSettings setting("BenYin", "LoginWindow");
    setting.setValue(QString("%1/passWord").arg(username), "");
    setting.setValue(QString("%1/rememberPassword").arg(username), Qt::Unchecked);
    QMessageBox::information(0, tr("Change Password"), tr("Change Password success !"));
}

void UserManagement_Invoicing::switchUser()
{
    if (showLoginWindow()) {
        update();
    }
}

void UserManagement_Invoicing::update()
{
    //qDebug()<<"UserManagement_Invoicing::update()"<<endl;
    mainWidget->update();
    parent->updateCurrentUserInfo();
    userManagement();
}

QT_BEGIN_NAMESPACE
#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(UserManagement_Invoicing, UserManagement_Invoicing)
#endif
QT_END_NAMESPACE
