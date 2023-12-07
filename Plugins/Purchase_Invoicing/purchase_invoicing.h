#pragma once

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
class QDialog;
class QComboBox;
class QCheckBox;
QT_END_NAMESPACE

class AddProductDialog;
class UpdateProductDialog;
class UserManagementInterface;
class ProductManagementInterface;

class PURCHASE_INVOICINGSHARED_EXPORT Purchase_Invoicing :
        public QObject, public PluginInterface, public UserChangeNotifyInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "Purchase_Invoicing")
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
    virtual void update() { updateDBTableModel(); }

    virtual void userChanged();

private:
    void initMainWidget();
    void createPurchasePanel();
    void updatePurchaseFilter();
    void hidePurchasePrice();
    void initImportConfigFile();


private slots:
    void addProduct();
    bool addProduct(QStringList &product);
    void addProduct(QMap<QString, QList<QStringList> > &productsMap);
    bool addProduct(QMap<int, int> &columnsMap, QMap<int, QString> &columnsValue);
    void productAdded();
    void delProduct();
    void emptyProducts();
    void importProducts();
    void configImporting();
    void commitProduct();
    void commitAllProducts();
    void updateDBTableModel();
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

    QDialog *importConfigDialog;
    QComboBox *serialNumberComboBox;
    QComboBox *brandNameComboBox;
    QComboBox *productModelComboBox;
    QComboBox *colorComboBox;
    QComboBox *vendorComboBox;
    QComboBox *schemaNameComboBox;
    QComboBox *quantityComboBox;
    QComboBox *unitComboBox;
    QComboBox *oldPurchasePriceComboBox;
    QComboBox *purchasePriceComboBox;
    QComboBox *sellingPriceComboBox;
    QComboBox *operatorUserComboBox;
    QComboBox *responserUserComboBox;
    QComboBox *sellerComboBox;
    QComboBox *barInfoComboBox;
    QComboBox *productStatusComboBox;
    QComboBox *replacementStatusComboBox;
    QComboBox *timeStampComboBox;
    QComboBox *commentsComboBox;
    QCheckBox *skip1stRowCheckBox;
};
