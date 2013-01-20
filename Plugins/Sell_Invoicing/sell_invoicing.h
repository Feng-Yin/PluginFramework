#ifndef SELL_INVOICING_H
#define SELL_INVOICING_H

#include <QObject>
#include <plugin_interface.h>
#include <usermanagement_interface.h>
#include "Sell_Invoicing_global.h"

QT_BEGIN_NAMESPACE
class QSqlRelationalTableModel;
class QTableView;
class QPushButton;
class QModelIndex;
class QLineEdit;
QT_END_NAMESPACE

class UpdateProductDialog;
class UserManagementInterface;
class ProductManagementInterface;

class SELL_INVOICINGSHARED_EXPORT Sell_Invoicing :
        public QObject, public PluginInterface, public UserChangeNotifyInterface
{
    Q_OBJECT
    Q_INTERFACES(PluginInterface UserChangeNotifyInterface)
public:
    Sell_Invoicing();

    virtual QWidget* getMainWidget() const;
    virtual QAction* getAction() const;
    virtual QMenu* getMenu() const;
    virtual QToolBar* getToolBar() const;
    virtual bool init(MainWindow *parent);
    virtual bool deInit();
    virtual QString moduleName() const;
    virtual QString moduleDescription() const;
    virtual QSet<QString> getAccessRoleNameSet() const;
    virtual QSet<QString> getDependencySet() const;
    virtual void update() { updateDBTableModel(); }

    virtual void userChanged();

private:
    void initMainWidget();
    void createOrderPanel();
    void createStoragePanel();
    void updateStorageFilter();
    void updateOrderFilter();

private slots:
    int updateProductinfo();
    int updateProductInfoOrder();
    void submitOrder();
    void backToStorage();
    void updateDBTableModel();
    void productUpdated();
    void onFilter();

private:
    MainWindow *parentWindow;
    UserManagementInterface *userManagementInterface;
    ProductManagementInterface *productManagementInterface;

    QWidget *mainWidget;

    QWidget *storagePanel;
    QSqlRelationalTableModel *storageModel;
    QTableView *storageView;
    QPushButton *submitOrderPushButton;

    QWidget *orderPanel;
    QSqlRelationalTableModel *orderModel;
    QTableView *orderView;
    QPushButton *backToStoragePushButton;

    UpdateProductDialog *updateStorageProductDialog;
    UpdateProductDialog *updateOrderProductDialog;

    QLineEdit *serialNumberLineEdit;
    QPushButton *filterPushButton;

    QString storageTimeStamp;
};

#endif // SELL_INVOICING_H
