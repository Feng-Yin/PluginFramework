#include <QtGui>
#include "mainwindow.h"
#include "plugin_interface.h"

MainWindow* MainWindow::instance = NULL;
const char *indexProperty = "widgetIndex";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    fileMenu(NULL),
    aboutMenu(NULL),
    exitAction(NULL),
    aboutAction(NULL),
    pluginAction(NULL),
    toolBox(NULL),
    stackedWidget(NULL),
    pluginTable(NULL),
    containerDialog(NULL)
{
    createActions();
    createToolBox();
    createMenus();

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

    loadPlugins();
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
    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcuts(QKeySequence::Quit);
    exitAction->setStatusTip(tr("Quit"));
    exitAction->setIcon(QIcon(":/Icon/quit_icon.png"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    aboutAction = new QAction(tr("A&bout"), this);
    aboutAction->setShortcut(tr("Ctrl+B"));
    aboutAction->setIcon(QIcon(":/Icon/about_icon.png"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

    pluginAction = new QAction(tr("P&lugin List"), this);
    pluginAction->setShortcut(tr("Ctrl+P"));
    pluginAction->setIcon(QIcon(":/Icon/plugin_icon.png"));
    connect(pluginAction, SIGNAL(triggered()), this, SLOT(pluginDialog()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(exitAction);

    aboutMenu = menuBar()->addMenu(tr("&Help"));
    aboutMenu->addAction(pluginAction);
    aboutMenu->addAction(aboutAction);

    QToolBar *toolbar = new QToolBar(this);
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
        stackedWidget->setCurrentIndex(index);
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
            pluginMap.insert(QFileInfo(filename).baseName(), plugin);
            pluginVector.push_back(plugin);
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
