#ifndef USERMANAGEMENTDIALOG_H
#define USERMANAGEMENTDIALOG_H

#include <QDialog>
#include <QModelIndex>
#include <QSqlRecord>

QT_BEGIN_NAMESPACE
class QWidget;
class QSqlRelationalTableModel;
class QTableView;
class QString;
class QLabel;
class QPushButton;
class QComboBox;
class QDialog;
QT_END_NAMESPACE

class UserManagementInterface;

class UserManagementDialog : public QWidget
{
    Q_OBJECT
public:
    explicit UserManagementDialog(UserManagementInterface *userManagementInterface, QWidget *parent = 0);
    void update() { updateDBTableModel(); }
    
signals:
    void trigerChangePassword(QString username);
    
public slots:

private slots:
    void selectedUserChanged();

    void addUser();
    void deleteUser();
    void addRole();
    void deleteRole();
    void addSchema();
    void deleteSchema();
    void addUserRole();
    void deleteUserRole();
    void addUserSchema();
    void deleteUserSchema();
    void changeUserPassword();
    void cleanLoginInfo();
    void addUserAccess();
    void updateDBTableModel();

private:
    void init();

    void createRolePanel();
    void createSchemaPanel();
    void createUserPanel();
    void createUserRolePanel();
    void createUserSchemaPanel();
    void createUserAccessPanel();
    void addUserAccessDialog();

    bool checkUserName(QString username);

private:

    QWidget *userPanel;
    QSqlRelationalTableModel *userModel;
    QTableView *userView;
    QLabel *userLabel;
    QPushButton *addUserButton;
    QPushButton *delUserButton;
    QPushButton *changeUserPasswordButton;
    QPushButton *cleanLoginInfoButton;

    QWidget *rolePanel;
    QSqlRelationalTableModel *roleModel;
    QTableView *roleView;
    QLabel *roleLabel;
    QPushButton *addRoleButton;
    QPushButton *delRoleButton;

    QWidget *schemaPanel;
    QSqlRelationalTableModel *schemaModel;
    QTableView *schemaView;
    QLabel *schemaLabel;
    QPushButton *addSchemaButton;
    QPushButton *delSchemaButton;

    QWidget *userRolePanel;
    QSqlRelationalTableModel *userRoleModel;
    QTableView *userRoleView;
    QLabel *userRoleLabel;
    QPushButton *addUserRoleButton;
    QPushButton *delUserRoleButton;

    QWidget *userSchemaPanel;
    QSqlRelationalTableModel *userSchemaModel;
    QTableView *userSchemaView;
    QLabel *userSchemaLabel;
    QPushButton *addUserSchemaButton;
    QPushButton *delUserSchemaButton;

    QDialog *userAccessPanel;
    QLabel *userAccessTypeLabel;
    QComboBox *userAccessComboBox;
    QPushButton *addUserAccessButton;

    UserManagementInterface *userManagementInterface;
};

#endif // USERMANAGEMENTDIALOG_H
