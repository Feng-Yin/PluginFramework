#pragma once

#include <QObject>
#include <QString>
#include <QSet>
#include <plugin_interface.h>
#include <usermanagement_interface.h>
#include "UserManagementIF_global.h"

QT_BEGIN_NAMESPACE
class QToolBar;
class QAction;
class QSqlQuery;
class QSqlDatabase;
QT_END_NAMESPACE

class USERMANAGEMENTIFSHARED_EXPORT UserManagementIF :
        public QObject, public PluginInterface, public UserManagementInterface
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "UserManagementIF")
#endif
    Q_INTERFACES(PluginInterface UserManagementInterface)
public:
    UserManagementIF();
    ~UserManagementIF();

    //PluginInterface
    virtual QWidget* getMainWidget() const;
    virtual QAction* getAction() const;
    virtual QMenu* getMenu() const;
    virtual QToolBar* getToolBar() const;
    virtual bool init(MainWindow *parent);
    virtual bool deInit();
    virtual QString moduleName() const;
    virtual QString moduleDescription() const;
    virtual QSet<QString> getAccessRoleNameSet() const;
    virtual QSet<QString> getDependencySet() const;
    virtual void update() { }

    //UserManagementInterface
    //for db usage
    virtual QString getCurrentUserName() const;
    virtual QString getCurrentIPAdress() const;
    virtual int getCurrentPort() const;
    virtual QSqlQuery getSqlQuery() const;
    virtual QSqlDatabase getDatabase(QString name = "") const;
    virtual bool openDatabase(QString username, QString password, QString ipaddress, int port);
    virtual void changePassword(QString username, QString password) const;
    //for user change notify
    virtual void registeObserver(UserChangeNotifyInterface *observer);
    virtual void deregisteObserver(UserChangeNotifyInterface *observer);
    virtual void notifyAllObserver() const;
    //for access checking
    virtual bool checkAccess(QSet<QString> accessRoleNameSet) const;
    virtual bool isAdmin(QString username) const;
    virtual bool isAuditor(QString username) const;
    //for table schemaname
    virtual QSet<QString> getAllSchemaName() const;
    virtual QSet<int> getAllSchemaID() const;
    virtual int getSchemaIDBySchemaName(QString dbSchema) const;
    virtual QString getSchemaNameBySchemaID(int schemaID) const;
    virtual void addSchema(QString dbSchema) const;
    virtual bool deleteSchema(QString dbSchema) const;
    //for table role
    virtual int getRoleIDByRoleName(QString role) const;
    virtual QString getRoleNameByRoleID(int roleID) const;
    //for table user
    virtual int getUserIDByUserName(QString username) const;
    virtual QString getUserNameByUserID(int userID) const;
    virtual void addUser(QString username, QString password) const;
    virtual bool deleteUser(QString username) const;
    //for table userrole
    virtual QSet<int> getRoleIDSetByUserID(int userID) const;
    virtual QSet<int> getUserIDSetByRoleID(int roleID) const;
    virtual bool addUserRoleByUserIDRoleID(int userID, int roleID) const;
    //for table userschema
    virtual QSet<int> getSchemaIDSetByUserID(int userID) const;
    virtual QSet<int> getUserIDSetBySchemaID(int schemaID) const;
    virtual bool addUserSchemaByUserIDSchemaID(int userID, int schemaID) const;
    //for default data
    virtual QStringList getDefaultSchema() const;
    virtual QStringList getDefaultRole() const;
    virtual QStringList getDefaultUser() const;

private:
    bool createUserManagementTables() const;
    void setDataBaseName(QString dbName) const;
    QSqlDatabase addDatabase(QString username) const;

private:

    QString currentUser;
    QString currentDBSchema;
    QString currentIP;
    QString currentPassword;
    int currentPort;
    QSet<UserChangeNotifyInterface *> observerSet;
};
