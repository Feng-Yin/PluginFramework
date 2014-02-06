#include <QtSql>
#include <QApplication>
#include <QtGui>
#include "usermanagementif.h"


const QStringList UserManagementInterface::defaultSchema = QStringList();
const QStringList UserManagementInterface::defaultRole = QStringList() <<"管理员"<<"采购"<<"库管"<<"销售"<<"收银员"<<"审计";
const QStringList UserManagementInterface::defaultUser = QStringList() <<"root";

UserManagementIF::UserManagementIF() :
    currentUser(""),
    currentDBSchema(""),
    currentIP(""),
    currentPassword(""),
    observerSet(QSet<UserChangeNotifyInterface *>())
{
    QDir qmdir(":/Translations");
    foreach (QString fileName, qmdir.entryList(QDir::Files)) {
        //qDebug()<<QFileInfo(fileName).baseName();
        QTranslator *qtTranslator = new QTranslator(this);
        qtTranslator->load(QFileInfo(fileName).baseName(), ":/Translations");
        QApplication::instance()->installTranslator(qtTranslator);
    }
}

UserManagementIF::~UserManagementIF()
{
}

QWidget* UserManagementIF::getMainWidget() const
{
    return NULL;
}

QAction* UserManagementIF::getAction() const
{
    return NULL;
}

QMenu* UserManagementIF::getMenu() const
{
    return NULL;
}

QToolBar* UserManagementIF::getToolBar() const
{
    return NULL;
}

bool UserManagementIF::init(MainWindow *parent)
{
    return true;
}

bool UserManagementIF::deInit()
{
    removeDatabase("default");
    return true;
}

QString UserManagementIF::moduleName() const
{
    return this->metaObject()->className();
}

QString UserManagementIF::moduleDescription() const
{
    return tr("User Management Interface Implement");
}

QSet<QString> UserManagementIF::getAccessRoleNameSet() const
{
    return QSet<QString>();
}

QSet<QString> UserManagementIF::getDependencySet() const
{
    return QSet<QString>();
}

QString UserManagementIF::getCurrentUserName() const
{
    return currentUser;
}

void UserManagementIF::setCurrentUserName(QString username)
{
    currentUser = username;
}

//QString UserManagementIF::getCurrentSchemaName() const
//{
//    return currentDBSchema;
//}

//void UserManagementIF::setCurrentSchemaName(QString schemaname)
//{
//    currentDBSchema = schemaname;
//}

QString UserManagementIF::getCurrentIPAdress() const
{
    return currentIP;
}

void UserManagementIF::setCurrentIPAdress(QString ipaddress)
{
    currentIP = ipaddress;
}

QSqlQuery UserManagementIF::getSqlQuery() const
{
    return QSqlQuery(getDatabase());
}

QSqlDatabase UserManagementIF::getDatabase() const
{
    QSqlDatabase db = QSqlDatabase::database(getDBLoginUserName("default"));
    QSqlQuery query(db);
    if(db.isOpen() && !query.exec("SHOW DATABASES")) {
        QMessageBox::critical(0, tr("Error"), tr("DB connection lost! Restart the program to try again"));
    }
    return db;
}

void UserManagementIF::setDataBaseName(QString dbName) const
{
    QSqlDatabase db(getDatabase());
    if(dbName.compare(db.databaseName())!=0) {
        QSqlQuery query(getSqlQuery());
        query.exec(QString("use %1").arg(dbName));
    }
}

bool UserManagementIF::reopenDatabase() const
{
    removeDatabase(currentUser);
    {
        QSqlDatabase db(addDatabase(currentUser));
        db.setHostName(currentIP);
        db.setUserName(getDBLoginUserName(currentUser));
        db.setPassword(currentPassword);
        if(db.open()) {
            //qDebug()<<"QSqlDriver::Transactions "<<db.driver()->hasFeature(QSqlDriver::Transactions);
            //create default db
            if(!getDatabase().isValid()) {
                QSqlDatabase defaultDB(addDatabase("default"));
                defaultDB.setHostName(currentIP);
                defaultDB.setUserName(getDBLoginUserName(currentUser));
                defaultDB.setPassword(currentPassword);
                defaultDB.open();
            }
        }
    }
    removeDatabase(currentUser);
    return false;
}

bool UserManagementIF::openDatabase(QString username, QString password, QString ipaddress)
{
    removeDatabase(username);
    {
        QSqlDatabase db(addDatabase(username));
        db.setHostName(ipaddress);
        db.setUserName(getDBLoginUserName(username));
        db.setPassword(password);
        if(db.open()) {
            currentUser = username;
            currentIP = ipaddress;
            currentPassword = password;
            //qDebug()<<"QSqlDriver::Transactions "<<db.driver()->hasFeature(QSqlDriver::Transactions);
            //create default db
            if(!getDatabase().isValid()) {
                QSqlDatabase defaultDB(addDatabase("default"));
                defaultDB.setHostName(ipaddress);
                defaultDB.setUserName(getDBLoginUserName(username));
                defaultDB.setPassword(password);
                defaultDB.open();
            }
            if(createUserManagementTables()) {
                return true;
            }
        }
    }
    removeDatabase(username);
    return false;
}

QSqlDatabase UserManagementIF::addDatabase(QString username) const
{
    removeDatabase(username);
    return QSqlDatabase::addDatabase("QMYSQL", getDBLoginUserName(username));
}

void UserManagementIF::removeDatabase(QString username) const
{
    if(username.isEmpty()) {
        QString loginUserName = getDBLoginUserName(currentUser);
        QSqlDatabase::database(loginUserName).close();
        QSqlDatabase::removeDatabase(loginUserName);
    }
    else {
        QString loginUserName = getDBLoginUserName(username);
        QSqlDatabase::database(loginUserName).close();
        QSqlDatabase::removeDatabase(loginUserName);
    }
}

void UserManagementIF::changePassword(QString username, QString password) const
{
    QSqlQuery query(getSqlQuery());
    query.exec(QString("SET PASSWORD FOR '%1'@'localhost' = PASSWORD('%2')").arg(getDBLoginUserName(username)).arg(password));
    query.exec(QString("SET PASSWORD FOR '%1'@'%' = PASSWORD('%2')").arg(getDBLoginUserName(username)).arg(password));
}

void UserManagementIF::registeObserver(UserChangeNotifyInterface *observer)
{
    if(!observerSet.contains(observer)) {
        observerSet<<observer;
    }
}

void UserManagementIF::deregisteObserver(UserChangeNotifyInterface *observer)
{
    if(observerSet.contains(observer)) {
        observerSet.remove(observer);
    }
}

void UserManagementIF::notifyAllObserver() const
{
    foreach(UserChangeNotifyInterface *observer, observerSet) {
        observer->userChanged();
    }
}

bool UserManagementIF::checkAccess(QSet<QString> accessRoleNameSet) const
{
    int userID = getUserIDByUserName(currentUser);
    QSet<int> currentRoleIDSet = getRoleIDSetByUserID(userID);
    QSet<int> accessRoleIDSet;
    accessRoleIDSet.clear();
    foreach(QString roleName, accessRoleNameSet) {
        int roleID = getRoleIDByRoleName(roleName);
        if(roleID) {
            accessRoleIDSet<<roleID;
        }
    }
    return !((currentRoleIDSet & accessRoleIDSet).isEmpty());
}

bool UserManagementIF::isAdmin(QString username) const
{
    int adminRoleID = getRoleIDByRoleName("管理员");
    QSet<int> roleIDSet = getRoleIDSetByUserID(getUserIDByUserName(username));
    return roleIDSet.contains(adminRoleID);
}

bool UserManagementIF::isStatistic(QString username) const
{
    int adminRoleID = getRoleIDByRoleName("审计");
    QSet<int> roleIDSet = getRoleIDSetByUserID(getUserIDByUserName(username));
    return roleIDSet.contains(adminRoleID);
}

//for table schemaname
QSet<QString> UserManagementIF::getAllSchemaName() const
{
    QSet<QString> schemaNameSet;
    schemaNameSet.clear();
    QSqlQuery query(getSqlQuery());
    query.exec("select name from schemaname");
    while(query.next()) {
        schemaNameSet.insert(query.record().value("name").toString());
    }
    return schemaNameSet;
}

//for table schemaname
QSet<int> UserManagementIF::getAllSchemaID() const
{
    QSet<int> schemaIDSet;
    schemaIDSet.clear();
    QSqlQuery query(getSqlQuery());
    query.exec("select id from schemaname");
    while(query.next()) {
        schemaIDSet.insert(query.record().value("id").toInt());
    }
    return schemaIDSet;
}

//for table schemaname
int UserManagementIF::getSchemaIDBySchemaName(QString dbSchema) const
{
    QSqlQuery query(getSqlQuery());
    query.exec(QString("select id from schemaname where name = '%1'").arg(dbSchema));
    if(query.first()) {
        return query.record().value("id").toInt();
    }
    return 0;
}

//for table schemaname
QString UserManagementIF::getSchemaNameBySchemaID(int schemaID) const
{
    QSqlQuery query(getSqlQuery());
    query.exec(QString("select name from schemaname where id = %1").arg(schemaID));
    if(query.first()) {
        return query.record().value("name").toString();
    }
    return "";
}

//for table schemaname
void UserManagementIF::addSchema(QString dbSchema) const
{
    QSqlQuery query(getSqlQuery());
    //insert name dbSchema into schemaname
    query.exec(QString("INSERT INTO schemaname (`name`) VALUES ('%1')").arg(dbSchema));
}

//for table schemaname
bool UserManagementIF::deleteSchema(QString dbSchema) const
{
    QSqlQuery query(getSqlQuery());
    int schemaID = getSchemaIDBySchemaName(dbSchema);
    return query.exec(QString("DELETE FROM `schemaname` WHERE `id`=%1").arg(schemaID));
}

//for table role
int UserManagementIF::getRoleIDByRoleName(QString role) const
{
    QSqlQuery query(getSqlQuery());
    query.exec(QString("select id from role where name = '%1'").arg(role));
    if(query.first()) {
        return query.record().value("id").toInt();
    }
    return 0;
}

//for table role
QString UserManagementIF::getRoleNameByRoleID(int roleID) const
{
    QSqlQuery query(getSqlQuery());
    query.exec(QString("select name from role where id = %1").arg(roleID));
    if(query.first()) {
        return query.record().value("name").toString();
    }
    return "";
}

//for table user
int UserManagementIF::getUserIDByUserName(QString username) const
{
    QSqlQuery query(getSqlQuery());
    qDebug()<<QString("select id from user where name = '%1'").arg(username);
    query.exec(QString("select id from user where name = '%1'").arg(username));
    if(query.first()) {
        return query.record().value("id").toInt();
    }
    return 0;
}

//for table user
QString UserManagementIF::getUserNameByUserID(int userID) const
{
    QSqlQuery query(getSqlQuery());
    query.exec(QString("select name from user where id = %1").arg(userID));
    if(query.first()) {
        return query.record().value("name").toString();
    }
    return "";
}

//for table user
void UserManagementIF::addUser(QString username, QString password) const
{
    QSqlQuery query(getSqlQuery());
    QString loginName = getDBLoginUserName(username);
    query.exec(QString("CREATE USER '%1'@'%' IDENTIFIED BY '%2'").arg(loginName).arg(password));
    query.exec(QString("GRANT ALL PRIVILEGES ON *.* TO '%1'@'%' WITH GRANT OPTION").arg(loginName));
    query.exec(QString("CREATE USER '%1'@'localhost' IDENTIFIED BY '%2'").arg(loginName).arg(password));
    query.exec(QString("GRANT ALL PRIVILEGES ON *.* TO '%1'@'localhost' WITH GRANT OPTION").arg(loginName));
    query.exec(QString("INSERT INTO user (`name`) VALUES ('%1')").arg(username));
}

//for table user
bool UserManagementIF::deleteUser(QString username) const
{
    QSqlQuery query(getSqlQuery());
    int userID = getUserIDByUserName(username);
    int ret = query.exec(QString("DELETE FROM `user` WHERE `id`=%1").arg(userID));
    QString loginName = getDBLoginUserName(username);
    query.exec(QString("DROP USER '%1'@'%'").arg(loginName));
    query.exec(QString("DROP USER '%1'@'localhost'").arg(loginName));
    return ret;
}

//for table userrole
QSet<int> UserManagementIF::getRoleIDSetByUserID(int userID) const
{
    QSet<int> roleIDSet;
    roleIDSet.clear();
    QSqlQuery query(getSqlQuery());
    query.exec(QString("select roleID from userrole where userID = %1").arg(userID));
    while(query.next()) {
        roleIDSet.insert(query.record().value("roleID").toInt());
    }
    return roleIDSet;
}

//for table userrole
QSet<int> UserManagementIF::getUserIDSetByRoleID(int roleID) const
{
    QSet<int> userIDSet;
    userIDSet.clear();
    QSqlQuery query(getSqlQuery());
    query.exec(QString("select userID from userrole where roleID = %1").arg(roleID));
    while(query.next()) {
        userIDSet.insert(query.record().value("userID").toInt());
    }
    return userIDSet;
}

//for table userrole
bool UserManagementIF::addUserRoleByUserIDRoleID(int userID, int roleID) const
{
    QSqlQuery query(getSqlQuery());
    return query.exec(QString("INSERT INTO `userrole` (`userID`, `roleID`) VALUES (%1, %2)")
                      .arg(userID).arg(roleID));
}

//for table userschema
QSet<int> UserManagementIF::getSchemaIDSetByUserID(int userID) const
{
    if(getRoleIDSetByUserID(userID).contains(getRoleIDByRoleName("管理员"))) {
        return getAllSchemaID();
    }
    QSet<int> schemaIDSet;
    schemaIDSet.clear();
    QSqlQuery query(getSqlQuery());
    query.exec(QString("select schemaID from userschema where userID = %1").arg(userID));
    while(query.next()) {
        schemaIDSet.insert(query.record().value("schemaID").toInt());
    }
    return schemaIDSet;
}

//for table userschema
QSet<int> UserManagementIF::getUserIDSetBySchemaID(int schemaID) const
{
    QSet<int> userIDSet;
    userIDSet.clear();
    QSqlQuery query(getSqlQuery());
    query.exec(QString("select userID from userschema where schemaID = %1").arg(schemaID));
    while(query.next()) {
        userIDSet.insert(query.record().value("userID").toInt());
    }
    return userIDSet;
}

//for table userschema
bool UserManagementIF::addUserSchemaByUserIDSchemaID(int userID, int schemaID) const
{
    QSqlQuery query(getSqlQuery());
    return query.exec(QString("INSERT INTO `userschema` (`userID`, `schemaID`) VALUES (%1, %2)")
                      .arg(userID).arg(schemaID));
}

QStringList UserManagementIF::getDefaultSchema() const
{
    return defaultSchema;
}

QStringList UserManagementIF::getDefaultRole() const
{
    return defaultRole;
}

QStringList UserManagementIF::getDefaultUser() const
{
    return defaultUser;
}

///////////////////////////////////////////////////////////////////////////////
bool UserManagementIF::createUserManagementTables() const
{
    QSqlQuery query(getSqlQuery());
    if(!query.exec("use invoicingschema")) {
        if(!query.exec("CREATE SCHEMA `invoicingschema` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci")) {
            return false;
        }
    }
    setDataBaseName("invoicingschema");
    if(!query.exec("desc schemaname")) {
        if(!query.exec("CREATE TABLE `schemaname` \
                        (`id` INT UNSIGNED NOT NULL AUTO_INCREMENT ,\
                         `name` VARCHAR(45) NOT NULL ,\
                         PRIMARY KEY (`id`) )")) {
            query.exec("drop schema `invoicingschema`");
            return false;
        }
		//insert default schema Name
		int id = 1;
		foreach(QString schemaName, defaultSchema) {
			query.exec(QString("INSERT INTO `schemaname` (`id`, `name`) VALUES ( %1, '%2')")
					   .arg(id++).arg(schemaName));
		}
    }
    if(!query.exec("desc role")) {
        if(!query.exec("CREATE TABLE `role` \
                        (`id` INT UNSIGNED NOT NULL AUTO_INCREMENT ,\
                         `name` VARCHAR(45) NOT NULL ,\
                         PRIMARY KEY (`id`) )")) {
            query.exec("drop schema `invoicingschema`");
            return false;
        }
	    //insert default role Name
		int id = 1;
		foreach(QString roleName, defaultRole) {
			query.exec(QString("INSERT INTO `role` (`id`, `name`) VALUES (%1, '%2')")
					   .arg(id++).arg(roleName));
		}
    }
    if(!query.exec("desc user")) {
        if(!query.exec("CREATE TABLE `user` \
                        (`id` INT UNSIGNED NOT NULL AUTO_INCREMENT ,\
                         `name` VARCHAR(45) NOT NULL ,\
                         PRIMARY KEY (`id`) )")) {
            query.exec("drop schema `invoicingschema`");
            return false;
        }
    }
    if(!query.exec("desc userschema")) {
        if(!query.exec("CREATE TABLE `userschema` \
                       (`id` INT UNSIGNED NOT NULL AUTO_INCREMENT ,\
                        `userID` INT UNSIGNED NOT NULL ,\
                        FOREIGN KEY (`userID`) REFERENCES user(`id`) ,\
                        `schemaID` INT UNSIGNED NOT NULL ,\
                        FOREIGN KEY (`schemaID`) REFERENCES schemaname(`id`) ,\
                        PRIMARY KEY (`id`) )")) {
            query.exec("drop schema `invoicingschema`");
            return false;
        }
    }
    if(!query.exec("desc userrole")) {
        if(!query.exec("CREATE TABLE `userrole` \
                        (`id` INT UNSIGNED NOT NULL AUTO_INCREMENT ,\
                         `userID` INT UNSIGNED NOT NULL ,\
                         FOREIGN KEY (`userID`) REFERENCES user(`id`) ,\
                         `roleID` INT UNSIGNED NOT NULL ,\
                         FOREIGN KEY (`roleID`) REFERENCES role(`id`) ,\
                         PRIMARY KEY (`id`) )")) {
            query.exec("drop schema `invoicingschema`");
            return false;
        }
		//insert default user & userrole
		int id = 1;
		foreach(QString user, defaultUser) {
			query.exec(QString("INSERT INTO `user` (`id`, `name`) VALUES (%1, '%2')")
					   .arg(id).arg(user));
			query.exec(QString("INSERT INTO `userrole` (`id`, `userID`, `roleID`) VALUES (%1, %2, %3)")
					   .arg(id).arg(id).arg(getRoleIDByRoleName("管理员")));
			id++;
		}
    }

    return true;
}

QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(UserManagementIF, UserManagementIF)
QT_END_NAMESPACE
