#ifndef PLUGIN_INTERFACE_H
#define PLUGIN_INTERFACE_H

#include <QtPlugin>
#include <QSet>
#include <QMetaType>

QT_BEGIN_NAMESPACE
class QWidget;
class QAction;
class QMenu;
class QToolBar;
QT_END_NAMESPACE

class MainWindow;

class PluginInterface
{
public:
    virtual ~PluginInterface() {}

    virtual QWidget* getMainWidget() const = 0;
    virtual QAction* getAction() const = 0;
    virtual QMenu* getMenu() const = 0;
    virtual QToolBar* getToolBar() const = 0;
    virtual bool init(MainWindow *parent) = 0;
    virtual bool deInit() = 0;
    virtual QString moduleName() const = 0;
    virtual QString moduleDescription() const = 0;
    virtual QSet<QString> getAccessRoleNameSet() const = 0;
    virtual QSet<QString> getDependencySet() const = 0;
    virtual void update() = 0;

protected:
    QString timeStamp;
};

Q_DECLARE_METATYPE(PluginInterface*)

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(PluginInterface, "com.emma.PluginInterface/1.0")
QT_END_NAMESPACE

#endif // PLUGIN_INTERFACE_H
