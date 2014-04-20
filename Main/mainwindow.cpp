#include <QtGui>
#include <typeinfo>
#include <windows.h>
#include "mainwindow.h"
#include "plugin_interface.h"
#include "usermanagement_interface.h"

MainWindow* MainWindow::instance = NULL;
const char *indexProperty = "widgetIndex";
const char *pluginProperty = "plugin";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    fileMenu(NULL),
    aboutMenu(NULL),
    exitAction(NULL),
    aboutAction(NULL),
    pluginAction(NULL),
    updateAction(NULL),
    registerAction(NULL),
    toolBox(NULL),
    stackedWidget(NULL),
    currentPlugin(NULL),
    pluginTable(NULL),
    containerDialog(NULL),
    registerDialog(NULL),
    machineCodeLabel(NULL),
    machineCodeLineEdit(NULL),
    registerCodeLabel(NULL),
    registerCodeLineEdit(NULL),
    splash(NULL)

{
    QDir qmdir(":/Translations");
    foreach (QString fileName, qmdir.entryList(QDir::Files)) {
        //qDebug()<<QFileInfo(fileName).baseName();
        QTranslator *qtTranslator = new QTranslator(this);
        qtTranslator->load(QFileInfo(fileName).baseName(), ":/Translations");
        QApplication::instance()->installTranslator(qtTranslator);
    }

    if(!splash)
    {
        QPixmap pixmap(":/Icon/invoicing_icon.png");
        splash = new QSplashScreen(pixmap, Qt::WindowStaysOnTopHint);
    }
    splash->show();

    splash->showMessage(tr("createActions..."));
    createActions();
    splash->showMessage(tr("createMenus..."));
    createMenus();

    if(isRegistration())
    {
        splash->showMessage(tr("createToolBox..."));
        createToolBox();

        QHBoxLayout *layout = new QHBoxLayout;
        layout->addWidget(toolBox);

        QFrame *line = new QFrame(this);
        line->setAttribute(Qt::WA_MouseNoMask);
        line->setFrameStyle(QFrame::Sunken);
        line->setFrameShape(QFrame::VLine);
        layout->addWidget(line);

        stackedWidget = new QStackedWidget();
        layout->addWidget(stackedWidget);

        QWidget *widget = new QWidget;
        widget->setLayout(layout);

        setCentralWidget(widget);
        setWindowTitle(tr("PluginFramework"));
        setWindowIcon(QIcon(":/Icon/plugin_icon.png"));
        setUnifiedTitleAndToolBarOnMac(true);
        setCursor(Qt::BusyCursor);

        splash->showMessage(tr("loadPlugins..."));
        loadPlugins();
        splash->showMessage(tr("finalize initialization..."));
        currentPlugin = getPlugin("UserManagement_Invoicing");
        //Set current user
        updateCurrentUserInfo();
        //end of Set current user

        unsetCursor();

        QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(update()));
        timer->setInterval(1000*10);
        timer->start();
    }
    splash->finish(this);
}

MainWindow::~MainWindow()
{
//    QList<PluginInterface *> pluginList = pluginMap.values();
//    foreach(PluginInterface *plugin, pluginList)
//    {
//        plugin->deInit();
//    }
    while(!pluginVector.empty()) {
        pluginVector.last()->deInit();
        pluginVector.pop_back();
    }
}

void MainWindow::createToolBox()
{
    toolBox = new QToolBox;
    toolBox->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Ignored));
}

void MainWindow::createActions()
{
    exitAction = new QAction(tr("&Exit"), this);
    //exitAction->setShortcuts(QKeySequence::Quit);
    exitAction->setShortcut(QString("Ctrl+E"));
    exitAction->setStatusTip(tr("Quit"));
    exitAction->setIcon(QIcon(":/Icon/quit_icon.png"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    aboutAction = new QAction(tr("A&bout"), this);
    aboutAction->setShortcut(QString("Ctrl+B"));
    aboutAction->setIcon(QIcon(":/Icon/about_icon.png"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

    pluginAction = new QAction(tr("P&lugin List"), this);
    pluginAction->setShortcut(QString("Ctrl+P"));
    pluginAction->setIcon(QIcon(":/Icon/plugin_icon.png"));
    connect(pluginAction, SIGNAL(triggered()), this, SLOT(pluginDialog()));

    updateAction = new QAction(tr("&Refresh DB"), this);
    updateAction->setShortcut(QString("Ctrl+R"));
    updateAction->setIcon(QIcon(":/Icon/update_icon.png"));
    connect(updateAction, SIGNAL(triggered()), this, SLOT(updateAll()));

    registerAction = new QAction(tr("Reg&ister Software"), this);
    registerAction->setShortcut(QString("Ctrl+G"));
    registerAction->setIcon(QIcon(":/Icon/register_icon.png"));
    connect(registerAction, SIGNAL(triggered()), this, SLOT(registerSoftware()));
    if(isRegistration())
    {
        registerAction->setEnabled(false);
    }
    else
    {
        updateAction->setEnabled(false);
    }
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(exitAction);

    aboutMenu = menuBar()->addMenu(tr("&Help"));
    aboutMenu->addAction(pluginAction);
    aboutMenu->addAction(registerAction);
    aboutMenu->addAction(aboutAction);

    QToolBar *toolbar = new QToolBar(this);
    toolbar->addAction(registerAction);
    toolbar->addAction(updateAction);
    toolbar->addAction(pluginAction);
    toolbar->addAction(exitAction);
    addToolBar(toolbar);
}

void MainWindow::createPluginToolBars()
{
    foreach(PluginInterface *plugin, pluginMap) {
        if(plugin->getToolBar()) {
            addToolBar(plugin->getToolBar());
        }
    }
}

void MainWindow::createPluginToolBox(QWidget *toolbox, PluginInterface *plugin)
{
    static int row = 0;
    static int column = 0;
    QGridLayout *layout = qobject_cast<QGridLayout *>(toolbox->layout());

    QToolButton *toolButton = new QToolButton(this);
    toolButton->setIcon(plugin->getMainWidget()->windowIcon());
    toolButton->setIconSize(QSize(50, 50));
    toolButton->setCheckable(true);

    QGridLayout *toolButtonLayout = new QGridLayout;
    toolButtonLayout->addWidget(toolButton, 0, 0, Qt::AlignHCenter);
    toolButtonLayout->addWidget(new QLabel(plugin->getMainWidget()->windowIconText()),
                                1, 0, Qt::AlignHCenter);
    toolButtonLayout->setSizeConstraint(QLayout::SetFixedSize);
    QWidget *toolButtonWidget = new QWidget;
    toolButtonWidget->setLayout(toolButtonLayout);

    layout->addWidget(toolButtonWidget, row, column++, Qt::AlignHCenter);
    if(column>1) {
        column = 0;
        row++;
    }
    layout->setRowStretch(layout->rowCount(), 10);
    layout->setColumnStretch(layout->columnCount(), 10);
    int index = stackedWidget->addWidget(plugin->getMainWidget());
    toolButton->setProperty(indexProperty, QVariant(index));
    toolButton->setProperty(pluginProperty, QVariant::fromValue(plugin));
    if(index==0) {
        toolButton->setChecked(true);
    }

    connect(toolButton, SIGNAL(clicked()), this, SLOT(widgetChange()));
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Invoicing"),
                       tr("<b>Invoicing</b> use the plugin framework"));
}

void MainWindow::widgetChange()
{
    QObject *sender = QObject::sender();
    if(sender) {
        QToolButton *toolButton = qobject_cast<QToolButton *>(sender);
        int index = toolButton->property(indexProperty).toInt();
        currentPlugin = toolButton->property(pluginProperty).value<PluginInterface*>();
        QList<QToolButton *> toolButtonList = sender->parent()->parent()
                ->findChildren<QToolButton *>();
        foreach(QToolButton *tb, toolButtonList) {
            if(sender!=tb) {
                tb->setChecked(false);
            }
            else {
                tb->setChecked(true);
            }
        }
        qApp->processEvents();
        update();
        stackedWidget->setCurrentIndex(index);
    }
}

void MainWindow::loadPlugins()
{
    QDir pluginsDir = QDir(qApp->applicationDirPath());
    pluginsDir.cd("plugins");

    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        //qDebug()<<pluginsDir.absoluteFilePath(fileName);
        loadPlugin(pluginsDir.absoluteFilePath(fileName));
    }

    int count = deferLoadingFile.count();
    while(deferLoadingFile.count()) {
        foreach(QString fileName, deferLoadingFile) {
            if(loadPlugin(fileName)) {
                deferLoadingFile.remove(fileName);
            }
        }
        if(count==deferLoadingFile.count()) {
            //qDebug()<<deferLoadingFile;
            break;
        }
        count=deferLoadingFile.count();
    }

    foreach (QString dirName, pluginsDir.entryList(QDir::Dirs)) {
        qDebug()<<dirName;
        if(dirName.compare(".")!=0 && dirName.compare("..")!=0) {
            QWidget *toolbox = new QWidget(this);
            QGridLayout *layout = new QGridLayout;
            toolbox->setLayout(layout);
            toolBox->addItem(toolbox, dirName);
            pluginsDir.cd(dirName);
            foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
                //qDebug()<<pluginsDir.absoluteFilePath(fileName);
                PluginInterface *plugin = loadPlugin(pluginsDir.absoluteFilePath(fileName));
                if(plugin&&plugin->getMainWidget()) {
                    qDebug()<<"createPluginToolBox "<<toolbox;
                    createPluginToolBox(toolbox, plugin);
                }
            }
            count = deferLoadingFile.count();
            while(deferLoadingFile.count()) {
                foreach(QString fileName, deferLoadingFile) {
                    PluginInterface *plugin = loadPlugin(fileName);
                    if(plugin) {
                        deferLoadingFile.remove(fileName);
                    }
                    if(plugin&&plugin->getMainWidget()) {
                        createPluginToolBox(toolbox, plugin);
                    }
                }
                if(count==deferLoadingFile.count()) {
                    //qDebug()<<deferLoadingFile;
                    break;
                }
                count=deferLoadingFile.count();
            }
            toolBox->setMinimumWidth(toolbox->sizeHint().width());
            pluginsDir.cd("..");
        }
    }
    createPluginToolBars();
}

PluginInterface* MainWindow::loadPlugin(QString filename)
{
    QPluginLoader loader(filename);
    PluginInterface *plugin = qobject_cast<PluginInterface *>(loader.instance());
    if (plugin) {
        QSet<QString> dependency(plugin->getDependencySet());
        QSet<QString> loaded(pluginMap.keys().toSet());
        if(loaded.contains(dependency)) {
            qDebug()<<"load "<<filename;
            //splash->raise();
            splash->showMessage(tr("loading ")+QFileInfo(filename).baseName()+" ...");
            qApp->processEvents();
            pluginMap.insert(QFileInfo(filename).baseName(), plugin);
            pluginVector.push_back(plugin);
            splash->showMessage(tr("initialize ")+QFileInfo(filename).baseName()+" ...");
            qApp->processEvents();
            //splash->lower();
            if(!plugin->init(this)) {
                QMessageBox::critical(this, tr("Init Error"),
                                      tr("plugin ") + filename + tr(" init error!"));
                exit(1);
            }
            return plugin;
        }
        else {
            qDebug()<<"defer load "<<filename;
            deferLoadingFile.insert(filename);
        }
    }
    return NULL;
}

MainWindow* MainWindow::getInstance()
{
    if(!instance) {
        instance = new MainWindow();
    }
    return instance;
}

void MainWindow::release()
{
    if(!instance) {
        delete instance;
        instance = NULL;
    }
}

void MainWindow::updateUserNameTitle(QString userName)
{
    setWindowTitle(QString(tr("Invoicing : %1")).arg(userName));
}

PluginInterface* MainWindow::getPlugin(QString pluginName)
{
    return pluginMap.value(pluginName);
}

void MainWindow::pluginDialog()
{
    if(!pluginTable) {
        pluginTable = new QTableWidget(0, 2, this);
        pluginTable->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Plugin Name")));
        pluginTable->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Plugin Description")));

        QMapIterator<QString, PluginInterface*> iplugin(pluginMap);
        int row = 0;
        while (iplugin.hasNext()) {
            iplugin.next();
            QTableWidgetItem *name = new QTableWidgetItem(iplugin.value()->moduleName());
            QTableWidgetItem *description = new QTableWidgetItem(iplugin.value()->moduleDescription());
            pluginTable->insertRow(pluginTable->rowCount());
            pluginTable->setItem(row, 0, name);
            pluginTable->setItem(row++, 1, description);
        }
        pluginTable->resizeColumnsToContents();
    }
    if(!containerDialog) {
        containerDialog = new QDialog(this);
        containerDialog->setWindowTitle(tr("Plugins List"));
        containerDialog->setWindowIcon(QIcon(":/Icon/plugin_icon.png"));
        QHBoxLayout *containerLayout = new QHBoxLayout();
        containerLayout->addWidget(pluginTable);
        containerDialog->setLayout(containerLayout);
        containerDialog->resize(QSize((pluginTable->columnWidth(0)+pluginTable->columnWidth(1)
                                       +50),
                                      pluginTable->rowHeight(1)*(pluginTable->rowCount()+2)));
    }

    containerDialog->exec();
}

void MainWindow::update()
{
//    static int i = 0;
//    i = i % pluginVector.size();
//    pluginVector.at(i++)->update();
//    qDebug()<<"currentPlugin"<<endl;
//    qDebug()<<currentPlugin<<endl;
    PluginInterface *plugin = getPlugin("UserManagementIF");
    UserManagementInterface * userManagementInterface = dynamic_cast<UserManagementInterface *>(plugin);
    updateCurrentUserInfo();
    userManagementInterface->getDatabase();
    if(currentPlugin) {
        setCursor(Qt::BusyCursor);
        currentPlugin->update();
        unsetCursor();
    }
}

void MainWindow::updateAll()
{
//    setCursor(Qt::BusyCursor);
//    foreach(PluginInterface* plugin, pluginVector) {
//        plugin->update();
//    }
//    unsetCursor();
    update();
}

void MainWindow::registerSoftware()
{
    if(!registerDialog)
    {
        registerDialog = new QDialog(this);
        registerDialog->setWindowTitle(tr("Register Softeware"));
        registerDialog->setWindowIcon(QIcon(":/Icon/register_icon.png"));

        machineCodeLabel = new QLabel(tr("Machine Code: "));
        machineCodeLineEdit = new QLineEdit();
        machineCodeLineEdit->setText(getHDLogicalID());
        machineCodeLineEdit->setReadOnly(true);

        registerCodeLabel = new QLabel(tr("Register Code: "));
        registerCodeLineEdit = new QLineEdit();

        QDialogButtonBox *buttons = new QDialogButtonBox((QDialogButtonBox::Ok | QDialogButtonBox::Cancel),
                                                         Qt::Horizontal,
                                                         registerDialog);
        connect(buttons, SIGNAL(accepted()), registerDialog, SLOT(accept()));
        connect(buttons, SIGNAL(rejected()), registerDialog, SLOT(reject()));


        QHBoxLayout *machineCodeLayout = new QHBoxLayout();
        machineCodeLayout->addWidget(machineCodeLabel);
        machineCodeLayout->addWidget(machineCodeLineEdit);

        QHBoxLayout *registerCodeLayout = new QHBoxLayout();
        registerCodeLayout->addWidget(registerCodeLabel);
        registerCodeLayout->addWidget(registerCodeLineEdit);

        QVBoxLayout *containerLayout = new QVBoxLayout();

        containerLayout->addLayout(machineCodeLayout);
        containerLayout->addLayout(registerCodeLayout);
        containerLayout->addWidget(buttons);

        registerDialog->setLayout(containerLayout);
        registerDialog->resize(registerDialog->sizeHint().width()*2, registerDialog->sizeHint().height());
    }
    if(registerDialog->exec()==QDialog::Accepted)
    {
        if(registerCodeLineEdit->text() == calActiveCode(machineCodeLineEdit->text()))
        {
            QSettings setting("BenYin", "Registration");
            setting.setValue(QString("Registration"), registerCodeLineEdit->text());
            QMessageBox::information(0, tr("Register Softeware"), tr("Register Successful. Please restart the program"));
            exit(0);
        }
        else
        {
            QMessageBox::critical(0, tr("Register Softeware"), tr("Invalid Register Code !"));
        }
    }
}

void MainWindow::updateCurrentUserInfo()
{
    PluginInterface *plugin = getPlugin("UserManagementIF");
    UserManagementInterface * userManagementInterface = dynamic_cast<UserManagementInterface *>(plugin);
    QSet<int> roleset = userManagementInterface->getRoleIDSetByUserID(
                userManagementInterface->getUserIDByUserName(userManagementInterface->getCurrentUserName()));
    QString roles;
    foreach(int i, roleset) {
//        qDebug()<<i<<endl;
//        qDebug()<<userManagementInterface->getRoleNameByRoleID(i)<<endl;
        roles += userManagementInterface->getRoleNameByRoleID(i);
        roles += " ";
    }
    setWindowTitle(tr("Invocing System. Current User: %1 <--> Roles: %2")
                   .arg(userManagementInterface->getCurrentUserName()).arg(roles));
}

bool MainWindow::isRegistration()
{
    QSettings setting("BenYin", "Registration");
    bool ret = setting.value(QString("Registration")).toString()=="13980575406";
    if(!ret)
    {
        return setting.value(QString("Registration")).toString()==calActiveCode(getHDLogicalID());
    }
    return ret;
}

QString MainWindow::getHDLogicalID()
{
    DWORD VolumeSerialNumber;
    GetVolumeInformation(L"C:\\",NULL,0,&VolumeSerialNumber,NULL,NULL,NULL,0);
    return calActiveCode(QString("%1").arg(QString::number(VolumeSerialNumber,16).toUpper()));
}

QString MainWindow::calActiveCode(QString machineCode)
{
    const QString parm1("!@#");
    const QString parm2("$%^");
    const QString parm3("&*(");
    const QString parm4(")_+");

    QString seed = parm1 + machineCode + parm2 + parm3 + parm4;

    QCryptographicHash sha1(QCryptographicHash::Sha1);

#if QT_VERSION < 0x050000
    QByteArray datagram(seed.toAscii());
#else
    QByteArray datagram(seed.toLatin1());
#endif
    const char* tempConstChar = datagram.data();
    sha1.addData(tempConstChar);
    QString  activeCode=sha1.result().toHex();

    //QClipboard *board = QApplication::clipboard();
    //board->setText(activeCode);

    return activeCode;
}
