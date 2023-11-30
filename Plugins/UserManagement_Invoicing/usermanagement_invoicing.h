#pragma once

#include <QObject>
#include <QString>
#include <QSet>
#include <plugin_interface.h>
#include <usermanagement_interface.h>
#include "UserManagement_Invoicing_global.h"

class LoginDialog;
class UserManagementDialog;
class MainWindow;

QT_BEGIN_NAMESPACE
class QToolBar;
class QAction;
class QSqlQuery;
class QSqlDatabase;
class QLayout;
QT_END_NAMESPACE

class USERMANAGEMENT_INVOICINGSHARED_EXPORT UserManagement_Invoicing :
        public QObject, public PluginInterface, public UserChangeNotifyInterface
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "UserManagement_Invoicing")
#endif
    Q_INTERFACES(PluginInterface UserChangeNotifyInterface)
public:
    UserManagement_Invoicing();
    ~UserManagement_Invoicing();

    //PluginInterface
    virtual QWidget* getMainWidget() const;
    virtual QAction* getAction() const;
    virtual QMenu* getMenu() const;
    virtual QToolBar* getToolBar() const;
    virtual bool init(MainWindow *parent);
    virtual bool deInit();
    virtual void userChanged();
    virtual QString moduleName() const;
    virtual QString moduleDescription() const;
    virtual QSet<QString> getAccessRoleNameSet() const;
    virtual QSet<QString> getDependencySet() const;
    virtual void update();

private slots:
    void logout();
    bool showLoginWindow();

    void initAction();
    void initToolBar();

    void userManagement();
    void changeCurrentUserPassword();
    void switchUser();
    void changePassword(QString username);

private:

    LoginDialog *loginDialog;
    UserManagementDialog *mainWidget;
    QAction *userManagementAction;
    QAction *changePasswordAction;
    QAction *switchUserAction;
    QAction *logOutAction;
    QToolBar *toolBar;

    UserManagementInterface *userManagementInterface;
    MainWindow *parent;
};
