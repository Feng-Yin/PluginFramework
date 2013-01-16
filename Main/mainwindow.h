#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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
QT_END_NAMESPACE

class PluginInterface;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static MainWindow* getInstance();
    static void release();
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

private:
    void createToolBox();
    void createActions();
    void createMenus();
    void createPluginToolBars();
    void createPluginToolBox(QWidget *toolbox, PluginInterface *plugin);
    void loadPlugins();
    PluginInterface* loadPlugin(QString filename);

private:

    static MainWindow* instance;

    QMenu *fileMenu;
    QMenu *aboutMenu;

    QAction *exitAction;
    QAction *aboutAction;
    QAction *pluginAction;
    QAction *updateAction;

    QToolBox *toolBox;
    QStackedWidget *stackedWidget;

    QMap<QString, PluginInterface*> pluginMap;
    QVector<PluginInterface*> pluginVector;
    QSet<QString> deferLoadingFile;
    QTableWidget *pluginTable;
    QDialog *containerDialog;
};

#endif // MAINWINDOW_H
