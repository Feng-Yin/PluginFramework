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
class QProgressBar;
QT_END_NAMESPACE

class StorageUpdateProductDialog;
class UserManagementInterface;
class ProductManagementInterface;

class STORAGE_INVOICINGSHARED_EXPORT Storage_Invoicing :
        public QObject, public PluginInterface, public UserChangeNotifyInterface
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "Storage_Invoicing")
#endif
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
    virtual void update() { updateDBTableModel(); }

    virtual void userChanged();

private:
    void initMainWidget();
    void createPurchasePanel();
    void createStoragePanel();
    void updatePurchaseFilter();
    void updateStorageFilter();
    void hidePurchasePrice();

private slots:
    void returnToVendor();
    void deleteProduct();
    void inStorage();
    void allInStorage();
    void outStorage();
    void allOutStorage();
    void updateProductinfo();
    void productUpdated();
    void updateDBTableModel();
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
    QPushButton *inAllStoragePushButton;
    QPushButton *returnToVendorPushButton;
    QPushButton *deleteProductPushButton;

    QWidget *storagePanel;
    QSqlRelationalTableModel *storageModel;
    QTableView *storageView;
    QPushButton *outStoragePushButton;
    QPushButton *outAllStoragePushButton;

    StorageUpdateProductDialog *storageUpdateProductDialog;

    QLineEdit *serialNumberLineEdit;
    QPushButton *filterPushButton;

    QProgressBar *bar;

    QString purchageTimeStamp;
};

#endif // STORAGE_INVOICING_H
