#ifndef PURCHASE_INVOICING_H
#define PURCHASE_INVOICING_H

#include <QObject>
#include <plugin_interface.h>
#include <usermanagement_interface.h>
#include "Purchase_Invoicing_global.h"


QT_BEGIN_NAMESPACE
class QWidget;
class QSqlRelationalTableModel;
class QTableView;
class QString;
class QLabel;
class QPushButton;
class QLineEdit;
class QProgressBar;
QT_END_NAMESPACE

class AddProductDialog;
class UpdateProductDialog;
class UserManagementInterface;
class ProductManagementInterface;

class PURCHASE_INVOICINGSHARED_EXPORT Purchase_Invoicing :
        public QObject, public PluginInterface, public UserChangeNotifyInterface
{
    Q_OBJECT
    Q_INTERFACES(PluginInterface UserChangeNotifyInterface)
public:
    Purchase_Invoicing();

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
    void updatePurchaseFilter() const;


private slots:
    void addProduct();
    bool addProduct(QStringList &product);
    void addProduct(QMap<QString, QList<QStringList> > &productsMap);
    void productAdded();
    void delProduct();
    void emptyProducts();
    void importProducts();
    void commitProduct();
    void commitAllProducts();
    void updateDBTableModel() const;
    void onFilter();
    void updateProductinfo();

signals:

private:

    QWidget *mainWidget;

    QPushButton *addProductButton;
    QPushButton *delProductButton;
    QPushButton *emptyProductsButton;
    QPushButton *importProductsButton;
    QPushButton *commitProductsButton;
    QPushButton *commitAllProductsButton;
    QPushButton *printProductsButton;

    QWidget *purchasePanel;
    QSqlRelationalTableModel *purchaseModel;
    QTableView *purchaseView;

    QProgressBar *bar;

    MainWindow *parentWindow;
    UserManagementInterface *userManagementInterface;
    ProductManagementInterface *productManagementInterface;

    AddProductDialog *addProductDialog;
    UpdateProductDialog *updateProductDialog;

    QLineEdit *serialNumberLineEdit;
    QPushButton *filterPushButton;
};

#endif // PURCHASE_INVOICING_H
