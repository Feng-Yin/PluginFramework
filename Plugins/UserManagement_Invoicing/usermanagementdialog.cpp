#include <QtGui>
#include <QtSql>
#include <limits>
#include "usermanagementdialog.h"
#include "usermanagement_invoicing.h"
#include "usermanagement_interface.h"

UserManagementDialog::UserManagementDialog(UserManagementInterface *userManagementInterface, QWidget *parent) :
    QWidget(parent),
    userPanel(NULL),
    userModel(NULL),
    userView(NULL),
    userLabel(NULL),
    addUserButton(NULL),
    delUserButton(NULL),
    changeUserPasswordButton(NULL),
    rolePanel(NULL),
    roleModel(NULL),
    roleView(NULL),
    roleLabel(NULL),
    addRoleButton(NULL),
    delRoleButton(NULL),
    schemaPanel(NULL),
    schemaModel(NULL),
    schemaView(NULL),
    schemaLabel(NULL),
    addSchemaButton(NULL),
    delSchemaButton(NULL),
    userRolePanel(NULL),
    userRoleModel(NULL),
    userRoleView(NULL),
    userRoleLabel(NULL),
    addUserRoleButton(NULL),
    delUserRoleButton(NULL),
    userSchemaPanel(NULL),
    userSchemaModel(NULL),
    userSchemaView(NULL),
    userSchemaLabel(NULL),
    addUserSchemaButton(NULL),
    delUserSchemaButton(NULL),
    userAccessPanel(NULL),
    userAccessTypeLabel(NULL),
    userAccessComboBox(NULL),
    addUserAccessButton(NULL),
    userManagementInterface(userManagementInterface)
{
    init();
}

void UserManagementDialog::init()
{
    createUserPanel();
    createRolePanel();
    createSchemaPanel();
    createUserRolePanel();
    createUserSchemaPanel();

    QHBoxLayout *userHLayout = new QHBoxLayout;
    userHLayout->addWidget(userPanel);

    QVBoxLayout *roleVLayout = new QVBoxLayout;
    roleVLayout->addWidget(rolePanel);
    roleVLayout->addWidget(userRolePanel);

    QVBoxLayout *schemaVLayout = new QVBoxLayout;
    schemaVLayout->addWidget(schemaPanel);
    schemaVLayout->addWidget(userSchemaPanel);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(userHLayout);
    mainLayout->addLayout(roleVLayout);
    mainLayout->addLayout(schemaVLayout);
    //mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    setLayout(mainLayout);

    setWindowTitle(tr("User Management"));
    setWindowIcon(QIcon(":/Icon/usermanagement_icon.png"));
    setWindowIconText(tr("User Management"));

    userView->setCurrentIndex(userModel->index(0, 0));
    roleView->setCurrentIndex(roleModel->index(0, 0));
    schemaView->setCurrentIndex(schemaModel->index(0, 0));
    userRoleView->setCurrentIndex(userRoleModel->index(0, 0));
    userSchemaView->setCurrentIndex(userSchemaModel->index(0, 0));
    selectedUserChanged();

//    QTimer *timer = new QTimer(this);
//    connect(timer, SIGNAL(timeout()), this, SLOT(updateDBTableModel()));
//    timer->setInterval(1000);
//    timer->start(1000);
}

void UserManagementDialog::createUserPanel()
{
    userPanel = new QWidget;
    QSqlDatabase db(userManagementInterface->getDatabase());
    userModel = new QSqlRelationalTableModel(this, db);
    userModel->setTable("user");
    userModel->setSort(UserID, Qt::AscendingOrder);
    userModel->setHeaderData(UserID, Qt::Horizontal, tr("ID"));
    userModel->setHeaderData(UserName, Qt::Horizontal, tr("User Name"));
    userModel->select();

    userView = new QTableView;
    userView->setModel(userModel);
    userView->setItemDelegate(new QSqlRelationalDelegate(this));
    userView->setSelectionMode(QAbstractItemView::SingleSelection);
    userView->setSelectionBehavior(QAbstractItemView::SelectRows);
    userView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    userView->setColumnHidden(UserID, true);
    userView->resizeColumnsToContents();
    userView->horizontalHeader()->setStretchLastSection(true);

    connect(userView, SIGNAL(clicked(const QModelIndex &)),
                this, SLOT(selectedUserChanged()));

    userLabel = new QLabel(tr("User"));
    QString username = userManagementInterface->getCurrentUserName();
    userLabel->setText(tr("User:(%1)").arg(username));
    userLabel->setBuddy(userView);
    addUserButton = new QPushButton(this);
    addUserButton->setAutoDefault(false);
    addUserButton->setIcon(QIcon(":/Icon/add_icon.png"));
    connect(addUserButton, SIGNAL(clicked()), this, SLOT(addUser()));
    delUserButton = new QPushButton(this);
    delUserButton->setAutoDefault(false);
    delUserButton->setIcon(QIcon(":/Icon/delete_icon.png"));
    connect(delUserButton, SIGNAL(clicked()), this, SLOT(deleteUser()));
    changeUserPasswordButton = new QPushButton(this);
    changeUserPasswordButton->setAutoDefault(false);
    changeUserPasswordButton->setIcon(QIcon(":/Icon/changepassword_icon.png"));
    connect(changeUserPasswordButton, SIGNAL(clicked()), this, SLOT(changeUserPassword()));
    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->addWidget(userLabel);
    hlayout->addWidget(addUserButton);
    hlayout->addWidget(delUserButton);
    hlayout->addWidget(changeUserPasswordButton);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(hlayout);
    layout->addWidget(userView);
    userPanel->setLayout(layout);
}

void UserManagementDialog::createRolePanel()
{
    rolePanel = new QWidget();
    QSqlDatabase db(userManagementInterface->getDatabase());
    roleModel = new QSqlRelationalTableModel(this, db);
    roleModel->setTable("role");
    roleModel->setSort(RoleID, Qt::AscendingOrder);
    roleModel->setHeaderData(RoleID, Qt::Horizontal, tr("ID"));
    roleModel->setHeaderData(RoleName, Qt::Horizontal, tr("Role"));
    roleModel->select();

    roleView = new QTableView;
    roleView->setModel(roleModel);
    roleView->setItemDelegate(new QSqlRelationalDelegate(this));
    roleView->setSelectionMode(QAbstractItemView::SingleSelection);
    roleView->setSelectionBehavior(QAbstractItemView::SelectRows);
    roleView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    roleView->setColumnHidden(RoleID, true);
    roleView->resizeColumnsToContents();
    roleView->horizontalHeader()->setStretchLastSection(true);

    roleLabel = new QLabel(tr("Role"));
    roleLabel->setBuddy(roleView);
    addRoleButton = new QPushButton(this);
    addRoleButton->setAutoDefault(false);
    addRoleButton->setIcon(QIcon(":/Icon/add_icon.png"));
    connect(addRoleButton, SIGNAL(clicked()), this, SLOT(addRole()));
    addRoleButton->setEnabled(false);
    delRoleButton = new QPushButton(this);
    delRoleButton->setAutoDefault(false);
    delRoleButton->setIcon(QIcon(":/Icon/delete_icon.png"));
    connect(delRoleButton, SIGNAL(clicked()), this, SLOT(deleteRole()));
    delRoleButton->setEnabled(false);
    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->addWidget(roleLabel);
    hlayout->addWidget(addRoleButton);
    hlayout->addWidget(delRoleButton);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(hlayout);
    layout->addWidget(roleView);
    rolePanel->setLayout(layout);
}

void UserManagementDialog::createSchemaPanel()
{
    schemaPanel = new QWidget();
    QSqlDatabase db(userManagementInterface->getDatabase());
    schemaModel = new QSqlRelationalTableModel(this, db);
    schemaModel->setTable("schemaname");
    schemaModel->setSort(SchemaID, Qt::AscendingOrder);
    schemaModel->setHeaderData(SchemaID, Qt::Horizontal, tr("ID"));
    schemaModel->setHeaderData(SchemaName, Qt::Horizontal, tr("Schema"));
    schemaModel->select();

    schemaView = new QTableView;
    schemaView->setModel(schemaModel);
    schemaView->setItemDelegate(new QSqlRelationalDelegate(this));
    schemaView->setSelectionMode(QAbstractItemView::SingleSelection);
    schemaView->setSelectionBehavior(QAbstractItemView::SelectRows);
    schemaView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    schemaView->setColumnHidden(SchemaID, true);
    schemaView->resizeColumnsToContents();
    schemaView->horizontalHeader()->setStretchLastSection(true);

    schemaLabel = new QLabel(tr("Schema"));
    schemaLabel->setBuddy(schemaView);
    addSchemaButton = new QPushButton(this);
    addSchemaButton->setAutoDefault(false);
    addSchemaButton->setIcon(QIcon(":/Icon/add_icon.png"));
    connect(addSchemaButton, SIGNAL(clicked()), this, SLOT(addSchema()));
    delSchemaButton = new QPushButton(this);
    delSchemaButton->setAutoDefault(false);
    delSchemaButton->setIcon(QIcon(":/Icon/delete_icon.png"));
    connect(delSchemaButton, SIGNAL(clicked()), this, SLOT(deleteSchema()));
    //delSchemaButton->setEnabled(false);
    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->addWidget(schemaLabel);
    hlayout->addWidget(addSchemaButton);
    hlayout->addWidget(delSchemaButton);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(hlayout);
    layout->addWidget(schemaView);
    schemaPanel->setLayout(layout);
}

void UserManagementDialog::createUserRolePanel()
{
    userRolePanel = new QWidget;
    QSqlDatabase db(userManagementInterface->getDatabase());
    userRoleModel = new QSqlRelationalTableModel(this, db);
    userRoleModel->setTable("userrole");
    userRoleModel->setRelation(UserRoleUserID, QSqlRelation("user", "id", "name"));
    userRoleModel->setRelation(UserRoleRoleID, QSqlRelation("role", "id", "name"));
    userRoleModel->setSort(UserRoleID, Qt::AscendingOrder);
    userRoleModel->setHeaderData(UserRoleID, Qt::Horizontal, tr("ID"));
    userRoleModel->setHeaderData(UserRoleUserID, Qt::Horizontal, tr("User Name"));
    userRoleModel->setHeaderData(UserRoleRoleID, Qt::Horizontal, tr("Role"));
    userRoleModel->select();

    userRoleView = new QTableView;
    userRoleView->setModel(userRoleModel);
    userRoleView->setItemDelegate(new QSqlRelationalDelegate(this));
    userRoleView->setSelectionMode(QAbstractItemView::SingleSelection);
    userRoleView->setSelectionBehavior(QAbstractItemView::SelectRows);
    userRoleView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    userRoleView->setColumnHidden(UserRoleID, true);
    userRoleView->setColumnHidden(UserRoleUserID, true);
    userRoleView->resizeColumnsToContents();
    userRoleView->horizontalHeader()->setStretchLastSection(true);

    userRoleLabel = new QLabel(tr("User-Role"));
    userRoleLabel->setBuddy(userRoleView);
    addUserRoleButton = new QPushButton(this);
    addUserRoleButton->setAutoDefault(false);
    addUserRoleButton->setIcon(QIcon(":/Icon/add_icon.png"));
    connect(addUserRoleButton, SIGNAL(clicked()), this, SLOT(addUserRole()));
    delUserRoleButton = new QPushButton(this);
    delUserRoleButton->setAutoDefault(false);
    delUserRoleButton->setIcon(QIcon(":/Icon/delete_icon.png"));
    connect(delUserRoleButton, SIGNAL(clicked()), this, SLOT(deleteUserRole()));
    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->addWidget(userRoleLabel);
    hlayout->addWidget(addUserRoleButton);
    hlayout->addWidget(delUserRoleButton);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(hlayout);
    layout->addWidget(userRoleView);
    userRolePanel->setLayout(layout);
}

void UserManagementDialog::createUserSchemaPanel()
{
    userSchemaPanel = new QWidget;
    QSqlDatabase db(userManagementInterface->getDatabase());
    userSchemaModel = new QSqlRelationalTableModel(this, db);
    userSchemaModel->setTable("userschema");
    userSchemaModel->setRelation(UserSchemaUserID, QSqlRelation("user", "id", "name"));
    userSchemaModel->setRelation(UserSchemaSchemaID, QSqlRelation("schemaname", "id", "name"));
    userSchemaModel->setSort(UserSchemaID, Qt::AscendingOrder);
    userSchemaModel->setHeaderData(UserSchemaID, Qt::Horizontal, tr("ID"));
    userSchemaModel->setHeaderData(UserSchemaUserID, Qt::Horizontal, tr("User Name"));
    userSchemaModel->setHeaderData(UserSchemaSchemaID, Qt::Horizontal, tr("Schema"));
    userSchemaModel->select();

    userSchemaView = new QTableView;
    userSchemaView->setModel(userSchemaModel);
    userSchemaView->setItemDelegate(new QSqlRelationalDelegate(this));
    userSchemaView->setSelectionMode(QAbstractItemView::SingleSelection);
    userSchemaView->setSelectionBehavior(QAbstractItemView::SelectRows);
    userSchemaView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    userSchemaView->setColumnHidden(UserSchemaID, true);
    userSchemaView->setColumnHidden(UserSchemaUserID, true);
    userSchemaView->resizeColumnsToContents();
    userSchemaView->horizontalHeader()->setStretchLastSection(true);

    userSchemaLabel = new QLabel(tr("User-Schema"));
    userSchemaLabel->setBuddy(userSchemaView);
    addUserSchemaButton = new QPushButton(this);
    addUserSchemaButton->setAutoDefault(false);
    addUserSchemaButton->setIcon(QIcon(":/Icon/add_icon.png"));
    connect(addUserSchemaButton, SIGNAL(clicked()), this, SLOT(addUserSchema()));
    delUserSchemaButton = new QPushButton(this);
    delUserSchemaButton->setAutoDefault(false);
    delUserSchemaButton->setIcon(QIcon(":/Icon/delete_icon.png"));
    connect(delUserSchemaButton, SIGNAL(clicked()), this, SLOT(deleteUserSchema()));
    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->addWidget(userSchemaLabel);
    hlayout->addWidget(addUserSchemaButton);
    hlayout->addWidget(delUserSchemaButton);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(hlayout);
    layout->addWidget(userSchemaView);
    userSchemaPanel->setLayout(layout);
}

void UserManagementDialog::createUserAccessPanel()
{
    userAccessPanel = new QDialog();
    userAccessTypeLabel = new QLabel();
    userAccessComboBox = new QComboBox();
    addUserAccessButton = new QPushButton(tr("Add"));
    connect(addUserAccessButton, SIGNAL(clicked()), this, SLOT(addUserAccess()));
    userAccessPanel->setWindowTitle(tr("Add User Role and Schema Access"));
    QHBoxLayout *hlayout = new QHBoxLayout();
    hlayout->addWidget(userAccessTypeLabel);
    hlayout->addWidget(userAccessComboBox);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(hlayout);
    mainLayout->addWidget(addUserAccessButton);
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    userAccessPanel->setLayout(mainLayout);
}

void UserManagementDialog::selectedUserChanged()
{
    QModelIndex index = userView->currentIndex();
    int row = 0;
    if (index.isValid()) {
        row = index.row();
    }

    QSqlRecord record = userModel->record(row);
    int userID = record.value("id").toInt();
    QString userName = record.value("name").toString();
    userRoleModel->setFilter(QString("userID = %1").arg(userID));
    userRoleLabel->setText(tr("Role Assign to User: %1")
                       .arg(userName));
    userSchemaModel->setFilter(QString("userID = %1").arg(userID));
    userSchemaLabel->setText(tr("Schema Assign to User: %1")
                       .arg(userName));
}

void UserManagementDialog::addUser()
{
    updateDBTableModel();
    bool okUserName = false;
    QString username;
    do {
        username = QInputDialog::getText(0, tr("Create User"), tr("User Name: "),
                                         QLineEdit::Normal, "", &okUserName);
        if(!okUserName) {
            //QMessageBox::critical(0, tr("Create User Error"), tr("Input UserName error !"));
            return;
        }
        username.simplified();
    }while(!checkUserName(username));

    bool okPassword1 = false;
    QString passWord1;
    bool okPassword2 = false;
    QString passWord2;
    do {
        do {
            passWord1 = QInputDialog::getText(0, tr("Create User"),
                                              QString(tr("Password for user '%1': ")).arg(username),
                                              QLineEdit::Password, "", &okPassword1);
            if(!okPassword1) {
                //QMessageBox::critical(0, tr("Create User Error"), tr("Input Password error !"));
                return;
            }
            if(passWord1.isEmpty()) {
                QMessageBox::critical(0, tr("Create User Error"), tr("Password can't be empty !"));
            }
            else if(!UserManagementInterface::checkPassword(passWord1)) {
                QMessageBox::critical(0, tr("Create User Error"), tr("Password illegal !"));
            }
        }while(passWord1.isEmpty()||!UserManagementInterface::checkPassword(passWord1));

        passWord2 = QInputDialog::getText(0, tr("Create User"),
                                          QString(tr("Re-confirm Password for user '%1': ")).arg(username),
                                          QLineEdit::Password, "", &okPassword2);

        if(!okPassword2) {
            //QMessageBox::critical(0, tr("Create User Error"), tr("Re-confirm Password error !"));
            return;
        }

        if(passWord1.compare(passWord2)!=0) {
            QMessageBox::critical(0, tr("Create User Error"),
                                  tr("The two entries of the new password don't match , Retry !"));
        }
    }while(passWord1.compare(passWord2)!=0);

    userManagementInterface->addUser(username, passWord1);
    userModel->select();
    userView->selectRow(userModel->rowCount()-1);
    selectedUserChanged();

    QSettings setting("BenYin", "LoginWindow");
    setting.setValue(QString("%1/userName").arg(username), "");
    setting.setValue(QString("%1/rememberUsername").arg(username), Qt::Checked);
    setting.setValue(QString("%1/passWord").arg(username), "");
    setting.setValue(QString("%1/rememberPassword").arg(username), Qt::Unchecked);
    setting.setValue(QString("%1/ipAddress").arg(username), "127.0.0.1");
    setting.setValue(QString("%1/rememberIPAddress").arg(username), Qt::Unchecked);
}

void UserManagementDialog::deleteUser()
{
    QModelIndex index = userView->currentIndex();
    if(!index.isValid()) {
        return;
    }
    QString username = userModel->record(index.row()).value("name").toString();
    if(userManagementInterface->getDefaultUser().contains(username)) {
        QMessageBox::critical(this, tr("Delete User Error"),
                              tr("The default user '%1' can't be delete !").arg(username));
        return;
    }

    if(userManagementInterface->deleteUser(username)) {
        userModel->select();
        if(index.row()<userModel->rowCount()) {
            userView->selectRow(index.row());
        }
        else {
            userView->selectRow(userModel->rowCount()-1);
        }
        selectedUserChanged();
        QSettings setting("BenYin", "LoginWindow");
        setting.remove(username);
    }
    else {
        QMessageBox::critical(this, tr("Delete User Error"),
                              tr("Delete user %1 error! it's in using").arg(username));
    }
}

void UserManagementDialog::addRole()
{
    updateDBTableModel();
}

void UserManagementDialog::deleteRole()
{
}

void UserManagementDialog::addSchema()
{
    updateDBTableModel();
    bool ok = false;
    QString dbSchema = QInputDialog::getText(0, tr("Create DB Schema"), tr("DB Schema Name: "),
                                             QLineEdit::Normal, "", &ok);
    dbSchema.simplified();
    if(ok && !dbSchema.isEmpty()) {
        userManagementInterface->addSchema(dbSchema);
        schemaModel->select();
        schemaView->selectRow(schemaModel->rowCount()-1);
    }
}

void UserManagementDialog::deleteSchema()
{
    QModelIndex index = schemaView->currentIndex();
    if(!index.isValid()) {
        return;
    }
    QSqlRecord record = schemaModel->record(index.row());
    QString schema = record.value("name").toString();
    if(userManagementInterface->deleteSchema(schema)) {
        schemaModel->select();
        if(index.row()<schemaModel->rowCount()) {
            schemaView->selectRow(index.row());
        }
        else {
            schemaView->selectRow(schemaModel->rowCount()-1);
        }
    }
    else {
        QSqlRecord record = schemaModel->record(index.row());
        QString schema = record.value("name").toString();
        QMessageBox::critical(this, tr("Delete Schema Error"),
                              tr("Delete schema %1 error! it's in using").arg(schema));
    }
}

void UserManagementDialog::addUserRole()
{
    updateDBTableModel();
    QString user = userModel->record(userView->currentIndex().row()).value("name").toString();
    if(userManagementInterface->getDefaultUser().contains(user)) {
        QMessageBox::critical(this, tr("Add User Role Error"),
                              tr("The role of default user '%1' can't be changed").arg(user));
        return;
    }
    if(userManagementInterface->isAdmin(user)) {
        QMessageBox::critical(this, tr("Add User Role Error"),
                                 tr("The admin user '%1' has all role access").arg(user));
        return;
    }
    addUserAccessDialog();
}

void UserManagementDialog::deleteUserRole()
{
    if(!userRoleView->currentIndex().isValid()) {
        return;
    }

    QString user = userModel->record(userView->currentIndex().row()).value("name").toString();
    if(userManagementInterface->getDefaultUser().contains(user)) {
        QMessageBox::critical(this, tr("Delete User Role Error"),
                              tr("The role of default user '%1' can't be changed").arg(user));
        return;
    }

    int row = userRoleView->currentIndex().row();
    userRoleModel->removeRow(row);
    if(row < userRoleModel->rowCount()) {
        userRoleView->selectRow(row);
    }
    else {
        userRoleView->selectRow(userRoleModel->rowCount()-1);
    }
}

void UserManagementDialog::addUserSchema()
{
    updateDBTableModel();
    QString user = userModel->record(userView->currentIndex().row()).value("name").toString();
    if(userManagementInterface->getDefaultUser().contains(user)) {
        QMessageBox::critical(this, tr("Add User Schema Error"),
                              tr("The schema of default user '%1' can't be changed").arg(user));
        return;
    }
    if(userManagementInterface->isAdmin(user)) {
        QMessageBox::critical(this, tr("Add User Schema Error"),
                              tr("The admin user '%1' has all schema access").arg(user));
        return;
    }
    addUserAccessDialog();
}

void UserManagementDialog::deleteUserSchema()
{
    if(!userSchemaView->currentIndex().isValid()) {
        return;
    }

    QString user = userModel->record(userView->currentIndex().row()).value("name").toString();
    if(userManagementInterface->getDefaultUser().contains(user)) {
        QMessageBox::critical(this, tr("Delete User Schema Error"),
                              tr("The schema of default user '%1' can't be changed").arg(user));
        return;
    }
    int row = userSchemaView->currentIndex().row();
    userSchemaModel->removeRow(row);
    if(row < userSchemaModel->rowCount()) {
        userSchemaView->selectRow(row);
    }
    else {
        userSchemaView->selectRow(userSchemaModel->rowCount()-1);
    }
}

void UserManagementDialog::changeUserPassword()
{
    QModelIndex index = userView->currentIndex();
    QSqlRecord record = userModel->record(index.row());
    QString username = record.value("name").toString();
    if(!username.isEmpty()) {
        emit(trigerChangePassword(username));
    }
}

void UserManagementDialog::addUserAccess()
{
    QSet<QString> schemaSet = userManagementInterface->getAllSchemaName();
    QString access = userAccessComboBox->currentText();
    QString user = userModel->record(userView->currentIndex().row()).value("name").toString();
    int userID = userModel->record(userView->currentIndex().row()).value("id").toInt();
    if(schemaSet.contains(access))
    {
        //add Schema access
        int schemaID = userManagementInterface->getSchemaIDBySchemaName(access);
        QSet<int> schemaIDSet = userManagementInterface->getSchemaIDSetByUserID(userID);
        if(schemaIDSet.contains(schemaID)) {
            QMessageBox::critical(this, tr("Add User Schema Error"),
                                  tr("The user '%1' has schema '%2' access already").arg(user).arg(access));
        }
        else {
            userManagementInterface->addUserSchemaByUserIDSchemaID(userID, schemaID);
        }
    }
    else {
        //add Role access
        int roleID = userManagementInterface->getRoleIDByRoleName(access);
        QSet<int> roleIDSet = userManagementInterface->getRoleIDSetByUserID(userID);
        if(userManagementInterface->isAdmin(user)) {
            QMessageBox::critical(this, tr("Add User Role Error"),
                                  tr("The admin user '%1' has all role access").arg(user));
        }
        else if(roleIDSet.contains(roleID)) {
            QMessageBox::critical(this, tr("Add User Role Error"),
                                  tr("The user '%1' has role '%2' access already").arg(user).arg(access));
        }
        else {
            userManagementInterface->addUserRoleByUserIDRoleID(userID, roleID);
        }
    }
    selectedUserChanged();
}

void UserManagementDialog::updateDBTableModel()
{
    QString username = userManagementInterface->getCurrentUserName();
    userLabel->setText(tr("User:(%1)").arg(username));
    QModelIndex index = userView->currentIndex();
    userModel->select();
    if(index.isValid()) {
        userView->setCurrentIndex(index);
    }

    index = roleView->currentIndex();
    roleModel->select();
    if(index.isValid()) {
        roleView->setCurrentIndex(index);
    }

    index = schemaView->currentIndex();
    schemaModel->select();
    if(index.isValid()) {
        schemaView->setCurrentIndex(index);
    }

    index = userRoleView->currentIndex();
    userRoleModel->select();
    if(index.isValid()) {
        userRoleView->setCurrentIndex(index);
    };

    index = userSchemaView->currentIndex();
    userSchemaModel->select();
    if(index.isValid()) {
        userSchemaView->setCurrentIndex(index);
    }
}

void UserManagementDialog::addUserAccessDialog()
{
    QString user = userModel->record(userView->currentIndex().row()).value("name").toString();
    if(!userAccessPanel) {
        createUserAccessPanel();
    }
    QPushButton *sender = qobject_cast<QPushButton *>(QObject::sender());
    if(sender == addUserRoleButton) {
        userAccessPanel->setWindowTitle(tr("Add Role"));
        userAccessTypeLabel->setText(QString(tr("Add role for user '%1' :")).arg(user));
        userAccessComboBox->setModel(roleModel);
        userAccessComboBox->setModelColumn(1);
    }
    else if(sender == addUserSchemaButton) {
        userAccessPanel->setWindowTitle(tr("Add Schema"));
        userAccessTypeLabel->setText(QString(tr("Add schema for user '%1' :")).arg(user));
        userAccessComboBox->setModel(schemaModel);
        userAccessComboBox->setModelColumn(1);
    }
    userAccessPanel->exec();
}

bool UserManagementDialog::checkUserName(QString username)
{
    if(username.isEmpty()) {
        QMessageBox::critical(0, tr("Username Error"), tr("User can't be empty, Try another !"));
        return false;
    }
    //check if the user name has exist
    if(userManagementInterface->getUserIDByUserName(username)) {
        QMessageBox::critical(0, tr("Username Error"), tr("User has existed, Try another !"));
        return false;
    }
    else {
        QChar NUL(0);
        QChar DEL(127);
        int length = 0;
        for (QString::iterator i=username.begin();i!=username.end(); ++i) {
            if(*i>=NUL&&*i<=DEL) {
                length += 1;
            }
            else {
                length += 5;
            }
        }
        QString convertedName = UserManagementInterface::getDBLoginUserName(username);
        length = convertedName.length();
        //qDebug()<<length;
        if(length>16) {
            QMessageBox::critical(0, tr("Username Error"), tr("User name too long, Try another !"));
            return false;
        }
        else {
            return true;
        }
    }
}

