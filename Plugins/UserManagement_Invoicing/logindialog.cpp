#include <QtGui>
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


#include <QChar>
#include <QHostInfo>
#include <QHostAddress>
#include "logindialog.h"
#include "usermanagement_interface.h"

LoginDialog::LoginDialog(UserManagementInterface *userManagementInterface, QDialog *parent)
    : QDialog(parent, Qt::WindowStaysOnTopHint)
    , userNameComboBox(NULL)
    , rememberUsername(NULL)
    , passwordLineEdit(NULL)
    , rememberPassword(NULL)
    , domainNameLineEdit(NULL)
    , rememberDomainName(NULL)
    , portSpinBox(NULL)
    , rememberPort(NULL)
    , loginButton(NULL)
    , cancelButton(NULL)
    , loginButtonBox(NULL)
    , userManagementInterface(userManagementInterface)
{
    init();
}

void LoginDialog::init()
{
    QGridLayout *mainLayout = new QGridLayout(this);

    QLabel *userNameLabel = new QLabel(tr("User Name: "),this);
    userNameComboBox = new QComboBox(this);
    userNameComboBox->setEditable(true);
    userNameComboBox->clear();
    userNameComboBox->addItems(initRegSetting());
    connect(userNameComboBox, SIGNAL(editTextChanged(QString)), this, SLOT(populateRegSetting()));
    rememberUsername = new QCheckBox(this);
    rememberUsername->setToolTip(tr("Remember Username"));
    mainLayout->addWidget(userNameLabel, 0, 0, Qt::AlignRight);
    mainLayout->addWidget(userNameComboBox, 0, 1);
    mainLayout->addWidget(rememberUsername, 0, 2);

    QLabel *passwordLabel = new QLabel(tr("Pass Word: "),this);
    QRegularExpression pwRegExp("([a-zA-Z0-9]*)");
    passwordLineEdit = new QLineEdit(this);
    QValidator *pwValidator = new QRegularExpressionValidator(pwRegExp, passwordLineEdit);
    passwordLineEdit->setValidator(pwValidator);
    passwordLineEdit->setEchoMode(QLineEdit::Password);
    passwordLineEdit->setToolTip(tr("<b>alphabet</b> & <b>number</b> only"));
    rememberPassword = new QCheckBox(this);
    rememberPassword->setToolTip(tr("Remember Password"));
    mainLayout->addWidget(passwordLabel, 1,0, Qt::AlignRight);
    mainLayout->addWidget(passwordLineEdit, 1, 1);
    mainLayout->addWidget(rememberPassword, 1, 2);

    QLabel *domainNameLabel = new QLabel(tr("DB Domain Name: "),this);
    domainNameLineEdit = new QLineEdit(this);
    rememberDomainName = new QCheckBox(this);
    rememberDomainName->setToolTip(tr("Remember Domain Name"));
    mainLayout->addWidget(domainNameLabel, 3,0, Qt::AlignRight);
    mainLayout->addWidget(domainNameLineEdit, 3, 1);
    mainLayout->addWidget(rememberDomainName, 3, 2);

    QLabel *portLabel = new QLabel(tr("DB port: "), this);
    portSpinBox = new QSpinBox(this);
    portSpinBox->setRange(0, 65535);
    portSpinBox->setValue(3306);
    rememberPort = new QCheckBox(this);
    rememberPort->setToolTip(tr("Remember DB Port"));
    mainLayout->addWidget(portLabel, 4, 0, Qt::AlignRight);
    mainLayout->addWidget(portSpinBox, 4, 1);
    mainLayout->addWidget(rememberPort, 4, 2);

    populateRegSetting();

    loginButton = new QPushButton(tr("Login"), this);
    connect(loginButton, SIGNAL(clicked()), this, SLOT(onLogin()));
    cancelButton = new QPushButton(tr("Cancel"), this);
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    loginButtonBox = new QDialogButtonBox(Qt::Horizontal, this);
    loginButtonBox->addButton(loginButton, QDialogButtonBox::ActionRole);
    loginButtonBox->addButton(cancelButton, QDialogButtonBox::ActionRole);
    mainLayout->addWidget(loginButtonBox, 5, 1, 1, 2, Qt::AlignRight);
    mainLayout->setColumnStretch(1, 10);

    mainLayout->setSizeConstraint(QLayout::SetFixedSize);

    setWindowTitle(tr("Login"));
    setWindowIcon(QIcon(":/Icon/login_icon.png"));
}

bool LoginDialog::createInvoicingSchema()
{
    bool ret = false;
    userManagementInterface->addSchema("未指定");
    userManagementInterface->addUser("未指定", "test");
    QString dbSchema = QInputDialog::getText(this, tr("Create DB Schema"), tr("DB Schema Name: "),
                                             QLineEdit::Normal, "", &ret);
    if(ret && !dbSchema.isEmpty()) {
        userManagementInterface->addSchema(dbSchema);
        ret = true;
    }
    else {
        ret = false;
    }
    return ret;
}

QStringList LoginDialog::initRegSetting()
{
    QSettings setting("BenYin", "LoginWindow");
    QStringList userList = setting.childGroups();
    if (!userList.count()) {
        setting.setValue("root/userName", "");
        setting.setValue("root/rememberUsername", Qt::Checked);
        setting.setValue("root/passWord", "");
        setting.setValue("root/rememberPassword", Qt::Unchecked);
        setting.setValue("root/domainName", "");
        setting.setValue("root/rememberDomainName", Qt::Unchecked);
        setting.setValue("root/port", "");
        setting.setValue("root/rememberPort", Qt::Unchecked);
    }
    return setting.childGroups();;
}

void LoginDialog::saveRegSetting()
{
    QSettings setting("BenYin", "LoginWindow");
    if(!setting.childGroups().contains(userNameComboBox->currentText())) {
        userNameComboBox->addItem(userNameComboBox->currentText());
    }

    setting.beginGroup(userNameComboBox->currentText());
    if(rememberUsername->isChecked()) {
        setting.setValue("userName", userNameComboBox->currentText());
        setting.setValue("rememberUsername", Qt::Checked);
    }
    else {
        setting.setValue("userName", "");
        setting.setValue("rememberUsername", Qt::Unchecked);
    }

    if(rememberPassword->isChecked()) {
        setting.setValue("passWord", passwordLineEdit->text().toLatin1().toBase64());
        setting.setValue("rememberPassword", Qt::Checked);
    }
    else {
        setting.setValue("passWord", "");
        setting.setValue("rememberPassword", Qt::Unchecked);
    }

    if(rememberDomainName->isChecked()) {
        setting.setValue("domainName", domainNameLineEdit->text());
        setting.setValue("rememberDomainName", Qt::Checked);
    }
    else {
        setting.setValue("domainName", "");
        setting.setValue("rememberDomainName", Qt::Unchecked);
    }

    if (rememberPort->isChecked()) {
        setting.setValue("port", portSpinBox->value());
        setting.setValue("rememberPort", Qt::Checked);
    } else {
        setting.setValue("port", "");
        setting.setValue("rememberPort", Qt::Unchecked);
    }
    setting.endGroup();
}

void LoginDialog::populateRegSetting()
{
    QSettings setting("BenYin", "LoginWindow");
    if(setting.childGroups().contains(userNameComboBox->currentText())) {
        rememberUsername->setCheckState((Qt::CheckState)setting
                                        .value(QString("%1/rememberUsername").arg(userNameComboBox->currentText())).toInt());
        passwordLineEdit->setText(QByteArray::fromBase64(setting.value(QString("%1/passWord")
                                                                       .arg(userNameComboBox->currentText())).toString().toLatin1()));
        rememberPassword->setCheckState((Qt::CheckState)setting
                                        .value(QString("%1/rememberPassword").arg(userNameComboBox->currentText())).toInt());
        domainNameLineEdit->setText(setting.value(QString("%1/domainName").arg(userNameComboBox->currentText())).toString());
        rememberDomainName->setCheckState((Qt::CheckState)setting
                                         .value(QString("%1/rememberDomainName").arg(userNameComboBox->currentText())).toInt());
        portSpinBox->setValue(
            setting.value(QString("%1/port").arg(userNameComboBox->currentText())).toInt());
        rememberPort->setCheckState(
            (Qt::CheckState) setting
                .value(QString("%1/rememberPort").arg(userNameComboBox->currentText()))
                .toInt());
    }
    else {
        rememberUsername->setCheckState(Qt::Checked);
        passwordLineEdit->setText("");
        rememberPassword->setCheckState(Qt::Unchecked);
        domainNameLineEdit->setText("");
        rememberDomainName->setCheckState(Qt::Checked);
        portSpinBox->setValue(3306);
        rememberPort->setCheckState(Qt::Checked);
    }
}

void LoginDialog::onLogin()
{
    QString username = userNameComboBox->currentText();
    QString password = passwordLineEdit->text();
    QString domainname = domainNameLineEdit->text();
    int port = portSpinBox->value();
    if (!userManagementInterface->openDatabase(username, password, domainname, port)) {
        QMessageBox::critical(this,
                              tr("Database Authenticate Error"),
                              tr("Check your <b>Username</b>, <b>Password</b> & DB <b>IP</b>"));
        return;
    }
    saveRegSetting();
    int userID = userManagementInterface->getUserIDByUserName(username);
    QSet<int> schemaIDSet = userManagementInterface->getSchemaIDSetByUserID(userID);
    if (schemaIDSet.count() == 0) {
        if (userManagementInterface->isAdmin(username)) {
            QMessageBox::information(
                this, tr("Setup"), tr("Haven't create DB Schema. Pls create the DB Schema first!"));
            createInvoicingSchema();
        } else if (!userManagementInterface->isAuditor(username)) {
            QMessageBox::information(this,
                                     tr("Setup"),
                                     tr("Haven't DB Schema access. Pls contact admin!"));
            return;
        }
    }
    userManagementInterface->notifyAllObserver();
    accept();
}
