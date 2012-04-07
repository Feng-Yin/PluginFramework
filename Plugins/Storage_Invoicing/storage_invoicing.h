#ifndef STORAGE_INVOICING_H
#define STORAGE_INVOICING_H

#include <QObject>
#include <plugin_interface.h>
#include <usermanagement_interface.h>
#include "Storage_Invoicing_global.h"

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

class STORAGE_INVOICINGSHARED_EXPORT Storage_Invoicing :
        public QObject, public PluginInterface, public UserChangeNotifyInterface
{
    Q_OBJECT
    Q_INTERFACES(PluginInterface UserChangeNotifyInterface)
public:
    Storage_Invoicing();

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

    virtual void userChanged() const;

private:
    void initMainWidget();
    void createPurchasePanel();
    void createStoragePanel();
    void updatePurchaseFilter() const;
    void updateStorageFilter() const;

private slots:
    void inStorage();
    void outStorage();
    void updateProductinfo();
    void productUpdated();
    void updateDBTableModel() const;
    void onFilter();

private:
    MainWindow *parentWindow;
    UserManagementInterface *userManagementInterface;
    ProductManagementInterface *productManagementInterface;

    QWidget *mainWidget;

    QWidget *purchasePanel;
    QSqlRelationalTableModel *purchaseModel;
    QTableView *purchaseView;
    QPushButton *inStoragePushButton;

    QWidget *storagePanel;
    QSqlRelationalTableModel *storageModel;
    QTableView *storageView;
    QPushButton *outStoragePushButton;

    UpdateProductDialog *updateProductDialog;

    QLineEdit *serialNumberLineEdit;
    QPushButton *filterPushButton;
};

#endif // STORAGE_INVOICING_H
