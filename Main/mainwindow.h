#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#if QT_VERSION < 0x050000
#include <QMainWindow>
#else
#include <QtWidgets>
#endif
#include <QMap>
#include <Qset>
#include <QVector>

QT_BEGIN_NAMESPACE
class QMenu;
class QAction;
class QToolBox;
class QStackedWidget;
class QTableWidget;
class QDialog;
class QLabel;
class QLineEdit;
class QSplashScreen;
QT_END_NAMESPACE

class PluginInterface;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static MainWindow* getInstance();
    static void release();
    static QString calActiveCode(QString machineCode);
    static QString getHDLogicalID();
    static bool isRegistration();
    void updateUserNameTitle(QString userName);
    virtual PluginInterface* getPlugin(QString pluginName);

private:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    
signals:
    
public slots:

private slots:
    void about();
    void widgetChange();
    void pluginDialog();
    void update();
    void updateAll();
    void registerSoftware();

private:
    void createToolBox();
    void createActions();
    void createMenus();
    void createPluginToolBars();
    void createPluginToolBox(QWidget *toolbox, PluginInterface *plugin);
    void loadPlugins();
    PluginInterface* loadPlugin(QString filename);
    void updateCurrentUserInfo();

private:

    static MainWindow* instance;

    QMenu *fileMenu;
    QMenu *aboutMenu;

    QAction *exitAction;
    QAction *aboutAction;
    QAction *pluginAction;
    QAction *updateAction;
    QAction *registerAction;

    QToolBox *toolBox;
    QStackedWidget *stackedWidget;

    QMap<QString, PluginInterface*> pluginMap;
    QVector<PluginInterface*> pluginVector;
    PluginInterface *currentPlugin;
    QSet<QString> deferLoadingFile;
    QTableWidget *pluginTable;
    QDialog *containerDialog;
    QDialog *registerDialog;
    QLabel *machineCodeLabel;
    QLineEdit *machineCodeLineEdit;
    QLabel *registerCodeLabel;
    QLineEdit *registerCodeLineEdit;

    QSplashScreen *splash;
};

#endif // MAINWINDOW_H
