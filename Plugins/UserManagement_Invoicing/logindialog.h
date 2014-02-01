#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QSet>

QT_BEGIN_NAMESPACE
class QLineEdit;
class QCheckBox;
class QComboBox;
class QPushButton;
class QLabel;
class QDialogButtonBox;
class QStringList;
class QString;
class QChar;
QT_END_NAMESPACE

class UserManagementInterface;

class LoginDialog : public QDialog
{
    Q_OBJECT
public:
    explicit LoginDialog(UserManagementInterface *userManagementInterface, QDialog *parent = 0);
    
signals:
    void login(QString dbSchema);
    
public slots:

private slots:
    void onAuthenticate();
    bool createInvoicingSchema();
    void onLogin();
    void init();
    void disableLogin();
    void enableAuthenticate(bool toggle);
    QStringList initRegSetting();
    void saveRegSetting();
    void populateRegSetting();
    void enableLogin(QSet<QString> dbSchemas);
    void updateIPAddress();

private:


private:
    QComboBox *userNameComboBox;
    QCheckBox *rememberUsername;
    QLineEdit *passwordLineEdit;
    QCheckBox *rememberPassword;
    QLineEdit *ipAddressLineEdit;
    QCheckBox *rememberIPAddress;
    QLineEdit *domainNameLineEdit;
    QCheckBox *rememberDomainName;
    QPushButton *authenticateButton;
    QLabel *dbSchemaLabel;
    QComboBox *dbSchemaComboBox;
    QPushButton *loginButton;
    QPushButton *cancelButton;
    QDialogButtonBox *loginButtonBox;
    bool reAuthenticate;
    UserManagementInterface *userManagementInterface;
};

#endif // LOGINDIALOG_H
