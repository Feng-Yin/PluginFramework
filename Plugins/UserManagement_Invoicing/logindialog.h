#pragma once

#include <QDialog>
#include <QSet>

#include "qspinbox.h"

QT_BEGIN_NAMESPACE
class QLineEdit;
class QCheckBox;
class QComboBox;
class QPushButton;
class QLabel;
class QDialogButtonBox;
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
    bool createInvoicingSchema();
    void onLogin();
    void init();
    QStringList initRegSetting();
    void saveRegSetting();
    void populateRegSetting();

private:


private:
    QComboBox *userNameComboBox;
    QCheckBox *rememberUsername;
    QLineEdit *passwordLineEdit;
    QCheckBox *rememberPassword;
    QLineEdit *domainNameLineEdit;
    QCheckBox *rememberDomainName;
    QSpinBox *portSpinBox;
    QCheckBox *rememberPort;
    QPushButton *loginButton;
    QPushButton *cancelButton;
    QDialogButtonBox *loginButtonBox;
    UserManagementInterface *userManagementInterface;
};
