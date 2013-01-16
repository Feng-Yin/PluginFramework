#ifndef PRODUCTMANAGEMENTIF_H
#define PRODUCTMANAGEMENTIF_H

#include <QObject>
#include <QMap>
#include <plugin_interface.h>
#include <productmanagement_interface.h>
#include "ProductManagementIF_global.h"

class UserManagementInterface;

class PRODUCTMANAGEMENTIFSHARED_EXPORT ProductManagementIF  :
        public QObject, public PluginInterface, public ProductManagementInterface
{
    Q_OBJECT
    Q_INTERFACES(PluginInterface ProductManagementInterface)
public:
    ProductManagementIF();
    ~ProductManagementIF();

    //PluginInterface
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
    virtual void update() const {}

    //for table producttype
    virtual int getTypeIDByTypeName(QString productType) const;
    virtual QString getTypeNameByTypeID(int typeID) const;
    virtual bool addProductType(QString productType) const;
    //for table brandname
    virtual int getBrandIDByBrandName(QString brandName) const;
    virtual QString getBrandNameByBrandID(int brandID) const;
    virtual bool addBrandName(QString brandName) const;
    //for table vendorinfo
    virtual int getVendorIDByVendorName(QString vendorName) const;
    virtual bool addVendorName(QString vendorName) const;
    //for table colorinfo
    virtual int getColorIDByColorName(QString colorName) const;
    virtual bool addColorName(QString colorName) const;
    //for table productmodel
    virtual QString getModelNameByModelID(int modelID) const;
    virtual bool addProductModel(int productTypeID, int brandNameID, QString productModel) const;
    virtual QSet<int> getBrandNameIDSetbyProductTypeID(int productTypeID) const;
    //for table productstatus
    virtual int getStatusIDByStatusName(QString status) const;
    //for table replacementstatus
    virtual int getReplacementStatusIDByReplacementStatusName(QString status) const;
    //for table products
    virtual QSet<int> getProductIDSetByUserIDStatusID(int userID, int statusID) const;
    virtual int getModelIDByTypeIDBrandIDModelName(int productTypeID,
                                                   int brandNameID,
                                                   QString productModel) const;
    virtual bool deleteProductByProductID(int id) const;
    virtual bool addProductByDetail(QString serialNumber, int productTypeID, int brandNameID,
                                    int productModelID, int schemaNameID, int quantity, QString unit,
                                    QString oldPurchasePrice, QString purchasePrice, QString sellingPrice,
                                    int operatorUserID, int responserUserID, int productStatusID,
                                    QString comments) const;
    virtual bool addProductByDetail(QString serialNumber, int productTypeID, int brandNameID,
                                    int productModelID, int colorID, int vendorID, int schemaNameID,
                                    int quantity, QString unit, QString oldPurchasePrice, QString purchasePrice,
                                    QString sellingPrice, int operatorUserID, int responserUserID, int sellerID,
                                    QString barInfo, int productStatusID, int replacementStatusID, QString time,
                                    QString comments) const;
    virtual bool updateProductDetailByProductID(int id, QString serialNumber, int productTypeID, int brandNameID,
                                    int productModelID, int schemaNameID, int quantity, QString unit,
                                    QString oldPurchasePrice, QString purchasePrice, QString sellingPrice,
                                    int operatorUserID, int responserUserID, int productStatusID,
                                    QString comments) const;
    virtual bool updateProductDetailByProductID(int id, QString serialNumber, int productTypeID, int brandNameID,
                                    int productModelID, int colorID, int vendorID, int schemaNameID,
                                    int quantity, QString unit, QString oldPurchasePrice, QString purchasePrice,
                                    QString sellingPrice, int operatorUserID, int responserUserID, int sellerID,
                                    QString barInfo, int productStatusID, int replacementStatusID, QString time,
                                    QString comments) const;
    virtual int getResponserUserIDByProductID(int id) const;
    virtual bool updateStatusIDByProductID(int productID, int statusID) const;
    virtual int getProductCountByQuery(QString queryString) const;
    virtual bool isModelOutdate(QSqlRelationalTableModel* model) const;
    virtual QMap<QString, SalesResult> getSalesResultByTimeRange(int year, int month, int range = 1) const;

    //help function
    virtual bool checkSerialNumber(QString serialNumber) const;


private:
    bool createProductManagementTables() const;
    bool dropAllTables() const;

    //for table productsinfo
    virtual bool addProductinfoByProductID(int productID) const;

private:
    UserManagementInterface *userManagementInterface;
};

#endif // PRODUCTMANAGEMENTIF_H
