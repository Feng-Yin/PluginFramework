#ifndef PRODUCTMANAGEMENT_INTERFACE_H
#define PRODUCTMANAGEMENT_INTERFACE_H

#include <QtPlugin>
#include <QSet>

QT_BEGIN_NAMESPACE
class QString;
class QSqlRelationalTableModel;
QT_END_NAMESPACE

enum {
    ProductID = 0,
    SerialNumber = 1,
    ProductTypeID = 2,
    BrandNameID = 3,
    ProductModelID = 4,
    SchemaNameID = 5,
    Quantity = 6,
    Unit = 7,
    OldPurchasePrice = 8,
    PurchasePrice = 9,
    SellingPrice = 10,
    OperatorUserID = 11,
    ResponserUserID = 12,
    ProductStatusID = 13,
    TimeStamp = 14,
    Comments = 15
};

class ProductManagementInterface
{
public:
    virtual ~ProductManagementInterface() {}

    //for table producttype
    virtual int getTypeIDByTypeName(QString productType) const = 0;
    virtual QString getTypeNameByTypeID(int typeID) const = 0;
    virtual bool addProductType(QString productType) const = 0;
    //for table brandname
    virtual int getBrandIDByBrandName(QString brandName) const = 0;
    virtual QString getBrandNameByBrandID(int brandID) const = 0;
    virtual bool addBrandName(QString brandName) const = 0;
    //for table productmodel
    virtual QString getModelNameByModelID(int modelID) const = 0;
    virtual bool addProductModel(int productTypeID, int brandNameID, QString productModel) const = 0;
    //for table productstatus
    virtual int getStatusIDByStatusName(QString status) const = 0;
    //for table products
    virtual QSet<int> getProductIDSetByUserIDStatusID(int userID, int statusID) const = 0;
    virtual int getModelIDByTypeIDBrandIDModelName(int productTypeID,
                                                   int brandNameID,
                                                   QString productModel) const = 0;
    virtual bool deleteProductByProductID(int id) const = 0;
    virtual bool addProductByDetail(QString serialNumber, int productTypeID, int brandNameID,
                                    int productModelID, int schemaNameID, int quantity, QString unit,
                                    QString oldPurchasePrice, QString purchasePrice, QString sellingPrice,
                                    int operatorUserID, int responserUserID, int productStatusID,
                                    QString comments) const = 0;
    virtual bool updateProductDetailByProductID(int id, QString serialNumber, int productTypeID, int brandNameID,
                                       int productModelID, int schemaNameID, int quantity, QString unit,
                                       QString oldPurchasePrice, QString purchasePrice, QString sellingPrice,
                                       int operatorUserID, int responserUserID, int productStatusID,
                                       QString comments) const = 0;
    virtual bool updateStatusIDByProductID(int productID, int statusID) const = 0;

    virtual int getProductCountByQuery(QString queryString) const = 0;
    virtual bool isModelOutdate(QSqlRelationalTableModel* model) const = 0;

//    //for table productsinfo
//    virtual bool addProductinfoByProductID(int productID) const = 0;

    //help function
    virtual bool checkSerialNumber(QString serialNumber) const = 0;

    //pre define product info
    const static QStringList defaultProductType;
    const static QStringList defaultBrandName;
    const static QStringList defaultProductModel;
    const static QStringList defaultProdcutStatus;

};


QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(ProductManagementInterface, "com.emma.ProductManagementInterface/1.0")
QT_END_NAMESPACE
#endif // PRODUCTMANAGEMENT_INTERFACE_H
