#pragma once

#include <QtPlugin>
#include <QSet>
#include <QSqlDatabase>
#include <QStringList>
#include <QList>
#include <QString>
#include <QVector>

QT_BEGIN_NAMESPACE
class QSqlQuery;
QT_END_NAMESPACE

//for table user
enum {
    UserID = 0,
    UserName = 1
};

//for table role
enum {
    RoleID = 0,
    RoleName = 1
};

//for table schemaname
enum {
    SchemaID = 0,
    SchemaName = 1
};

//for table userrole
enum {
    UserRoleID = 0,
    UserRoleUserID = 1,
    UserRoleRoleID = 2
};

//for table userschema
enum {
    UserSchemaID = 0,
    UserSchemaUserID = 1,
    UserSchemaSchemaID = 2
};

class UserChangeNotifyInterface
{
public:
    virtual ~UserChangeNotifyInterface() {}
    virtual void userChanged() = 0;
};

class UserManagementInterface
{
public:
    virtual ~UserManagementInterface() {}
    //for db usage
    virtual QString getCurrentUserName() const = 0;
    virtual QString getCurrentIPAdress() const = 0;
    virtual int getCurrentPort() const = 0;
    virtual QSqlQuery getSqlQuery() const = 0;
    virtual QSqlDatabase getDatabase(QString username = "") const = 0;
    virtual bool openDatabase(QString username, QString password, QString ipaddress, int port) = 0;
    virtual void changePassword(QString username, QString password) const = 0;
    //for user change notify
    virtual void registeObserver(UserChangeNotifyInterface *obsever) = 0;
    virtual void deregisteObserver(UserChangeNotifyInterface *obsever) = 0;
    virtual void notifyAllObserver() const = 0;
    //for access checking
    virtual bool checkAccess(QSet<QString> accessRoleNameSet) const = 0;
    virtual bool isAdmin(QString username) const = 0;
    virtual bool isAuditor(QString username) const  = 0;
    //for table schemaname
    virtual QSet<QString> getAllSchemaName() const = 0;
    virtual QSet<int> getAllSchemaID() const = 0;
    virtual int getSchemaIDBySchemaName(QString dbSchema) const = 0;
    virtual QString getSchemaNameBySchemaID(int schemaID) const = 0;
    virtual void addSchema(QString dbSchema) const = 0;
    virtual bool deleteSchema(QString dbSchema) const = 0;
    //for table role
    virtual int getRoleIDByRoleName(QString role) const = 0;
    virtual QString getRoleNameByRoleID(int roleID) const = 0;
    //for table user
    virtual int getUserIDByUserName(QString username) const = 0;
    virtual QString getUserNameByUserID(int userID) const = 0;
    virtual void addUser(QString username, QString password) const = 0;
    virtual bool deleteUser(QString username) const = 0;
    //for table userrole
    virtual QSet<int> getRoleIDSetByUserID(int userID) const = 0;
    virtual QSet<int> getUserIDSetByRoleID(int roleID) const = 0;
    virtual bool addUserRoleByUserIDRoleID(int userID, int roleID) const = 0;
    //for table userschema
    virtual QSet<int> getSchemaIDSetByUserID(int userID) const = 0;
    virtual QSet<int> getUserIDSetBySchemaID(int schemaID) const = 0;
    virtual bool addUserSchemaByUserIDSchemaID(int userID, int schemaID) const = 0;
    //for default data
    virtual QStringList getDefaultSchema() const = 0;
    virtual QStringList getDefaultRole() const = 0;
    virtual QStringList getDefaultUser() const = 0;

    //help function
    static bool checkPassword(QString passWord)
    {
        QChar DEL(127);
        for (QString::iterator i=passWord.begin();i!=passWord.end();++i) {
            if(*i>DEL) {
                return false;
            }
        }
        return true;
    }

    //help function to convert a chinese name to a unique id
    static QString getDBLoginUserName(QString username)
    {
        QChar NUL(0);
        QChar DEL(127);
        QString dbLoginName;
        for (QString::iterator i=username.begin();i!=username.end();++i) {
            if(*i>=NUL&&*i<=DEL) {
                dbLoginName.append(QString("%1").arg(*i));
            }
            else {
                QList<uint>  namelist = QString("%1").arg(*i).toUcs4().toList();
                foreach(uint tmp, namelist) {
                    dbLoginName.append(QString("%1").arg(tmp));
                }
            }
        }
        return dbLoginName;
    }

    //pre-define data
    static const QStringList defaultSchema;
    static const QStringList defaultRole;
    static const QStringList defaultUser;
};


QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(UserManagementInterface, "com.emma.UserManagementInterface/1.0")
Q_DECLARE_INTERFACE(UserChangeNotifyInterface, "com.emma.UserChangeNotifyInterface/1.0")
QT_END_NAMESPACE
