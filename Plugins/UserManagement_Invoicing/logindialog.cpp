#include <QtGui>
#if QT_VERSION >= 0x050000
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
#endif

#include <QChar>
#include <QHostInfo>
#include <QHostAddress>
#include "logindialog.h"
#include "usermanagement_interface.h"

LoginDialog::LoginDialog(UserManagementInterface *userManagementInterface, QDialog *parent) :
    QDialog(parent, Qt::WindowStaysOnTopHint),
    userNameComboBox(NULL),
    rememberUsername(NULL),
    passwordLineEdit(NULL),
    rememberPassword(NULL),
    ipAddressLineEdit(NULL),
    rememberIPAddress(NULL),
    domainNameLineEdit(NULL),
    rememberDomainName(NULL),
    authenticateButton(NULL),
    dbSchemaLabel(NULL),
    dbSchemaComboBox(NULL),
    loginButton(NULL),
    cancelButton(NULL),
    loginButtonBox(NULL),
    reAuthenticate(false),
    userManagementInterface(userManagementInterface)
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
    connect(userNameComboBox, SIGNAL(editTextChanged(const QString &))
            , this, SLOT(populateRegSetting()));
    rememberUsername = new QCheckBox(this);
    rememberUsername->setToolTip(tr("Remember Username"));
    mainLayout->addWidget(userNameLabel, 0, 0, Qt::AlignRight);
    mainLayout->addWidget(userNameComboBox, 0, 1);
    mainLayout->addWidget(rememberUsername, 0, 2);

    QLabel *passwordLabel = new QLabel(tr("Pass Word: "),this);
    QRegExp pwRegExp("([a-zA-Z0-9]*)");
    passwordLineEdit = new QLineEdit(this);
    QValidator *pwValidator = new QRegExpValidator(pwRegExp, passwordLineEdit);
    passwordLineEdit->setValidator(pwValidator);
    passwordLineEdit->setEchoMode(QLineEdit::Password);
    passwordLineEdit->setToolTip(tr("<b>alphabet</b> & <b>number</b> only"));
    rememberPassword = new QCheckBox(this);
    rememberPassword->setToolTip(tr("Remember Password"));
    mainLayout->addWidget(passwordLabel, 1,0, Qt::AlignRight);
    mainLayout->addWidget(passwordLineEdit, 1, 1);
    mainLayout->addWidget(rememberPassword, 1, 2);

    QLabel *ipAddressLabel = new QLabel(tr("DB IP Address: "),this);
    QRegExp ipRegExp("(0*(2(5[0-5]|[0-4]\\d)|1?\\d{1,2})(\\.0*(2(5[0-5]|[0-4]\\d)|1?\\d{1,2})){3})");
    ipAddressLineEdit = new QLineEdit(this);
    QValidator *ipValidator = new QRegExpValidator(ipRegExp, ipAddressLineEdit);
    ipAddressLineEdit->setValidator(ipValidator);
    ipAddressLineEdit->setInputMask("000.000.000.000;_");
    rememberIPAddress = new QCheckBox(this);
    rememberIPAddress->setToolTip(tr("Remember IP Address"));
    mainLayout->addWidget(ipAddressLabel, 2,0, Qt::AlignRight);
    mainLayout->addWidget(ipAddressLineEdit, 2, 1);
    mainLayout->addWidget(rememberIPAddress, 2, 2);
    //hide the ipaddress, use domain name
    ipAddressLabel->hide();
    ipAddressLineEdit->hide();
    rememberIPAddress->hide();

    QLabel *domainNameLabel = new QLabel(tr("DB Domain Name: "),this);
    domainNameLineEdit = new QLineEdit(this);
    rememberDomainName = new QCheckBox(this);
    rememberDomainName->setToolTip(tr("Remember Domain Name"));
    mainLayout->addWidget(domainNameLabel, 3,0, Qt::AlignRight);
    mainLayout->addWidget(domainNameLineEdit, 3, 1);
    mainLayout->addWidget(rememberDomainName, 3, 2);
    //connect(domainNameLineEdit, SIGNAL(textChanged(const QString)), this, SLOT(updateIPAddress()));

    populateRegSetting();

//    authenticateButton = new QPushButton(tr("Authenticate"), this);
//    connect(authenticateButton, SIGNAL(clicked()), this, SLOT(onAuthenticate()));
//    mainLayout->addWidget(authenticateButton, 3, 1, 1, 2, Qt::AlignRight);
//    authenticateButton->setAutoDefault(false);
//    authenticateButton->setDefault(true);

//    dbSchemaLabel = new QLabel(tr("DB Schema: "),this);
//    dbSchemaLabel->setEnabled(false);
//    dbSchemaComboBox = new QComboBox(this);
//    dbSchemaComboBox->setEnabled(false);
//    mainLayout->addWidget(dbSchemaLabel, 4,0, Qt::AlignRight);
//    mainLayout->addWidget(dbSchemaComboBox, 4, 1, 1, 2);


    loginButton = new QPushButton(tr("Login"), this);
    connect(loginButton, SIGNAL(clicked()), this, SLOT(onLogin()));
    //connect(loginButton, SIGNAL(clicked()), this, SLOT(accept()));
    cancelButton = new QPushButton(tr("Cancel"), this);
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    loginButtonBox = new QDialogButtonBox(Qt::Horizontal, this);
    loginButtonBox->addButton(loginButton, QDialogButtonBox::ActionRole);
    loginButtonBox->addButton(cancelButton, QDialogButtonBox::ActionRole);
    //loginButtonBox->setEnabled(false);
    mainLayout->addWidget(loginButtonBox, 5, 1, 1, 2, Qt::AlignRight);
    mainLayout->setColumnStretch(1, 10);
    //loginButton->setAutoDefault(false);
    //loginButton->setDefault(false);

    mainLayout->setSizeConstraint(QLayout::SetFixedSize);

    setWindowTitle(tr("Login"));
    //QPixmap image(":/images/floodfill.png");
    setWindowIcon(QIcon(":/Icon/login_icon.png"));
}

void LoginDialog::updateIPAddress()
{
    QString domainname = domainNameLineEdit->text();
    QString ipaddressFromDomainname;
    QHostInfo host = QHostInfo::fromName(domainname);
    if (!host.addresses().isEmpty()) {
         QHostAddress address = host.addresses().first();
         ipaddressFromDomainname = address.toString();
         // use the first IP address
     }
    ipAddressLineEdit->setText(ipaddressFromDomainname);
}

void LoginDialog::enableLogin(QSet<QString> dbSchemas)
{
    enableAuthenticate(false);

    saveRegSetting();

    dbSchemaComboBox->clear();
    dbSchemaComboBox->insertItems(0, dbSchemas.toList());
    dbSchemaLabel->setEnabled(true);
    dbSchemaComboBox->setEnabled(true);
    loginButtonBox->setEnabled(true);
}

void LoginDialog::disableLogin()
{
    dbSchemaComboBox->clear();
    dbSchemaLabel->setEnabled(false);
    dbSchemaComboBox->setEnabled(false);
    loginButtonBox->setEnabled(false);
}

void LoginDialog::enableAuthenticate(bool toggle)
{
    userNameComboBox->setEnabled(toggle);
    rememberUsername->setEnabled(toggle);
    passwordLineEdit->setEnabled(toggle);
    rememberPassword->setEnabled(toggle);
    ipAddressLineEdit->setEnabled(toggle);
    rememberIPAddress->setEnabled(toggle);
    if(toggle) {
        authenticateButton->setText(tr("Authenticate"));
        authenticateButton->setDefault(true);
        loginButton->setDefault(false);
    }
    else {
        authenticateButton->setText(tr("Re-Authenticate"));
        authenticateButton->setDefault(false);
        loginButton->setDefault(true);
    }
    reAuthenticate = !toggle;

    disableLogin();
}

void LoginDialog::onAuthenticate()
{
    QString username = userNameComboBox->currentText();
    QString password = passwordLineEdit->text();
    QString ipaddress = ipAddressLineEdit->text();
    int ad0 = ipaddress.split(".").at(0).toInt();
    int ad1 = ipaddress.split(".").at(1).toInt();
    int ad2 = ipaddress.split(".").at(2).toInt();
    int ad3 = ipaddress.split(".").at(3).toInt();
    ipaddress = QString("%1.%2.%3.%4").arg(ad0).arg(ad1).arg(ad2).arg(ad3);
    qDebug()<<ipaddress;
    if(!reAuthenticate) {
        if (!userManagementInterface->openDatabase(username, password, ipaddress)) {
            QMessageBox::critical(this, tr("Database Authenticate Error"),
                                  tr("Check your <b>Username</b>, <b>Password</b> & DB <b>IP</b>"));
        }
        else {
            int userID = userManagementInterface->getUserIDByUserName(username);
            QSet<int> schemaIDSet = userManagementInterface->getSchemaIDSetByUserID(userID);
            if(schemaIDSet.count()==0) {
                if(userManagementInterface->isAdmin(username)) {
                    QMessageBox::information(this, tr("Setup"),
                                             tr("Haven't create DB Schema. Pls create the DB Schema first!"));
                    if(createInvoicingSchema()) {
                        enableLogin(userManagementInterface->getAllSchemaName());
                    }
                }
                else {
                    QMessageBox::information(this, tr("Setup"),
                                             tr("Haven't DB Schema access. Pls contact admin!"));
                }
            }
            else {
                QSet<QString> schemaNameSet;
                foreach(int schemaID, schemaIDSet) {
                    schemaNameSet<<userManagementInterface->getSchemaNameBySchemaID(schemaID);
                }
                enableLogin(schemaNameSet);
            }
        }
    }
    else {
        //re-Authenticate
        enableAuthenticate(true);
    }
}

bool LoginDialog::createInvoicingSchema()
{
    bool ret = false;
    userManagementInterface->addSchema("未指定");
    userManagementInterface->addUser("未指定", "test");
    QString dbSchema = QInputDialog::getText(0, tr("Create DB Schema"), tr("DB Schema Name: "),
                                             QLineEdit::Normal, "", &ret);
    dbSchema.simplified();
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
        setting.setValue("root/ipAddress", "127.0.0.1");
        setting.setValue("root/rememberIPAddress", Qt::Unchecked);
        setting.setValue("root/domainName", "");
        setting.setValue("root/rememberDomainName", Qt::Unchecked);
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
#if QT_VERSION < 0x050000
        setting.setValue("passWord", passwordLineEdit->text().toAscii().toBase64());
#else
        setting.setValue("passWord", passwordLineEdit->text().toLatin1().toBase64());
#endif
        setting.setValue("rememberPassword", Qt::Checked);
    }
    else {
        setting.setValue("passWord", "");
        setting.setValue("rememberPassword", Qt::Unchecked);
    }

    if(rememberIPAddress->isChecked()) {
        setting.setValue("ipAddress", ipAddressLineEdit->text());
        setting.setValue("rememberIPAddress", Qt::Checked);
    }
    else {
        setting.setValue("ipAddress", "");
        setting.setValue("rememberIPAddress", Qt::Unchecked);
    }

    if(rememberDomainName->isChecked()) {
        setting.setValue("domainName", domainNameLineEdit->text());
        setting.setValue("rememberDomainName", Qt::Checked);
    }
    else {
        setting.setValue("domainName", "");
        setting.setValue("rememberDomainName", Qt::Unchecked);
    }
    setting.endGroup();
}

void LoginDialog::populateRegSetting()
{
    QSettings setting("BenYin", "LoginWindow");
    if(setting.childGroups().contains(userNameComboBox->currentText())) {
        rememberUsername->setCheckState((Qt::CheckState)setting
                                        .value(QString("%1/rememberUsername").arg(userNameComboBox->currentText())).toInt());
#if QT_VERSION < 0x050000
        passwordLineEdit->setText(QByteArray::fromBase64(setting.value(QString("%1/passWord")
                                                                       .arg(userNameComboBox->currentText())).toString().toAscii()));
#else
        passwordLineEdit->setText(QByteArray::fromBase64(setting.value(QString("%1/passWord")
                                                                       .arg(userNameComboBox->currentText())).toString().toLatin1()));
#endif
        rememberPassword->setCheckState((Qt::CheckState)setting
                                        .value(QString("%1/rememberPassword").arg(userNameComboBox->currentText())).toInt());
        ipAddressLineEdit->setText(setting.value(QString("%1/ipAddress").arg(userNameComboBox->currentText())).toString());
        rememberIPAddress->setCheckState((Qt::CheckState)setting
                                         .value(QString("%1/rememberIPAddress").arg(userNameComboBox->currentText())).toInt());
        domainNameLineEdit->setText(setting.value(QString("%1/domainName").arg(userNameComboBox->currentText())).toString());
        rememberDomainName->setCheckState((Qt::CheckState)setting
                                         .value(QString("%1/rememberDomainName").arg(userNameComboBox->currentText())).toInt());
    }
    else {
        rememberUsername->setCheckState(Qt::Checked);
        passwordLineEdit->setText("");
        rememberPassword->setCheckState(Qt::Unchecked);
        ipAddressLineEdit->setText("127.0.0.1");
        rememberIPAddress->setCheckState(Qt::Checked);
        domainNameLineEdit->setText("");
        rememberDomainName->setCheckState(Qt::Checked);
    }
}

void LoginDialog::onLogin()
{
    QString username = userNameComboBox->currentText();
    QString password = passwordLineEdit->text();
    QString ipaddress = ipAddressLineEdit->text();
    QString domainname = domainNameLineEdit->text();
    QString ipaddressFromDomainname;
    QHostInfo host = QHostInfo::fromName(domainname);
    foreach(QHostAddress address, host.addresses())
    {
        if(address.toIPv4Address()!=0)
        {
            ipaddressFromDomainname = address.toString();
            break;
        }
    }

    //QMessageBox::information(this, tr("Login"), ipaddressFromDomainname);
    int ad0 = ipaddressFromDomainname.split(".").at(0).toInt();
    int ad1 = ipaddressFromDomainname.split(".").at(1).toInt();
    int ad2 = ipaddressFromDomainname.split(".").at(2).toInt();
    int ad3 = ipaddressFromDomainname.split(".").at(3).toInt();
    ipaddress = QString("%1.%2.%3.%4").arg(ad0).arg(ad1).arg(ad2).arg(ad3);
    if (!userManagementInterface->openDatabase(username, password, ipaddressFromDomainname)) {
        QMessageBox::critical(this, tr("Database Authenticate Error"),
                              tr("Check your <b>Username</b>, <b>Password</b> & DB <b>IP</b>"));
        return;
//        if (!userManagementInterface->openDatabase(username, password, ipaddressFromDomainname)) {
//        }
    }
    else {
        int userID = userManagementInterface->getUserIDByUserName(username);
        QSet<int> schemaIDSet = userManagementInterface->getSchemaIDSetByUserID(userID);
        if(schemaIDSet.count()==0) {
            if(userManagementInterface->isAdmin(username)) {
                QMessageBox::information(this, tr("Setup"),
                                         tr("Haven't create DB Schema. Pls create the DB Schema first!"));
                createInvoicingSchema();
            }
            else if(!userManagementInterface->isStatistic(username)){
                QMessageBox::information(this, tr("Setup"),
                                         tr("Haven't DB Schema access. Pls contact admin!"));
                return;
            }
        }
    }
    saveRegSetting();
    userManagementInterface->notifyAllObserver();
    accept();
}
