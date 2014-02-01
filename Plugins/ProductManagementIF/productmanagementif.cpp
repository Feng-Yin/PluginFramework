#include <QtSql>
#include <QApplication>
#include "mainwindow.h"
#include <usermanagement_interface.h>
#include <productmanagement_interface.h>
#include "productmanagementif.h"

const QStringList ProductManagementInterface::defaultProductType
                                        = QStringList() <<"其它"<<"手机"<<"手机卡"<<"手机配件"<<"平板电脑";
const QStringList ProductManagementInterface::defaultBrandName
                                        = QStringList() <<"其它"<<"诺基亚"<<"摩托罗拉"<<"三星"<<"HTC"<<"苹果";
const QStringList ProductManagementInterface::defaultProductModel = QStringList() <<"其它";
const QStringList ProductManagementInterface::defaultProdcutStatus = QStringList() <<"已删除"<<"待进货"<<"录入"<<"待入库"<<"已入库"<<"已下单"<<"已销售";
const QStringList ProductManagementInterface::defaultVendorInfo = QStringList() <<"其它";
const QStringList ProductManagementInterface::defaultColor = QStringList() <<"其它";
const QStringList ProductManagementInterface::replacementStatus = QStringList() <<"否"<<"是";

ProductManagementIF::ProductManagementIF():
    userManagementInterface(NULL)
{
    QDir qmdir(":/Translations");
    foreach (QString fileName, qmdir.entryList(QDir::Files)) {
        //qDebug()<<QFileInfo(fileName).baseName();
        QTranslator *qtTranslator = new QTranslator(this);
        qtTranslator->load(QFileInfo(fileName).baseName(), ":/Translations");
        QApplication::instance()->installTranslator(qtTranslator);
    }
}

ProductManagementIF::~ProductManagementIF()
{
}

QWidget* ProductManagementIF::getMainWidget() const
{
    return NULL;
}

QAction* ProductManagementIF::getAction() const
{
    return NULL;
}

QMenu* ProductManagementIF::getMenu() const
{
    return NULL;
}

QToolBar* ProductManagementIF::getToolBar() const
{
    return NULL;
}

bool ProductManagementIF::init(MainWindow *parent)
{
    PluginInterface *plugin = parent->getPlugin("UserManagementIF");
    userManagementInterface = dynamic_cast<UserManagementInterface *>(plugin);
    if(!userManagementInterface) {
        return false;
    }
    if(!createProductManagementTables()) {
        return false;
    }

    return true;
}

bool ProductManagementIF::deInit()
{
    return true;
}

QString ProductManagementIF::moduleName() const
{
    return this->metaObject()->className();
}

QString ProductManagementIF::moduleDescription() const
{
    return tr("Product Management Interface Implement");
}

QSet<QString> ProductManagementIF::getAccessRoleNameSet() const
{
    return QSet<QString>();
}

QSet<QString> ProductManagementIF::getDependencySet() const
{
    return QSet<QString>()<<"UserManagementIF"<<"UserManagement_Invoicing";
}

//for table productstatus
int ProductManagementIF::getStatusIDByStatusName(QString status) const
{
    QSqlQuery query(userManagementInterface->getSqlQuery());
    query.exec(QString("select id from productstatus where status = '%1'").arg(status));
    if(query.first()) {
        return query.record().value("id").toInt();
    }
    return 0;
}

//for table replacementstatus
int ProductManagementIF::getReplacementStatusIDByReplacementStatusName(QString status) const
{
    QSqlQuery query(userManagementInterface->getSqlQuery());
    query.exec(QString("select id from replacementstatus where status = '%1'").arg(status));
    if(query.first()) {
        return query.record().value("id").toInt();
    }
    return 0;
}


//for table product
QSet<int> ProductManagementIF::getProductIDSetByUserIDStatusID(int userID, int statusID) const
{
    QSqlQuery query(userManagementInterface->getSqlQuery());
    QSet<int> IDList;
    query.exec(QString("select id from products where productStatusID = %1 and operatorUserID = %2")
               .arg(statusID).arg(userID));
    while (query.next()) {
        IDList<<query.record().value("id").toInt();
    }
    return IDList;
}

//for table product
bool ProductManagementIF::deleteProductByProductID(int id) const
{
    QSqlQuery query(userManagementInterface->getSqlQuery());
    return query.exec(QString("delete from products where id = %1").arg(id));
}

//for table product
bool ProductManagementIF::addProductByDetail(QString serialNumber, int productTypeID, int brandNameID,
                                    int productModelID, int schemaNameID, int quantity, QString unit,
                                    QString oldPurchasePrice, QString purchasePrice, QString sellingPrice,
                                    int operatorUserID, int responserUserID, int productStatusID,
                                    QString comments) const
{
    QString addProductSQL = "INSERT INTO `products` (`serialNumber`, `productTypeID`, `brandNameID`, `productModelID`, `schemaNameID`, `quantity`, `unit`, `operatorUserID`, `responserUserID`, `productStatusID`";
    if(!oldPurchasePrice.isEmpty()) {
        addProductSQL += ", `oldPurchasePrice`";
    }
    if(!purchasePrice.isEmpty()) {
        addProductSQL += ", `purchasePrice`";
    }
    if(!sellingPrice.isEmpty()) {
        addProductSQL += ", `sellingPrice`";
    }
    if(!comments.isEmpty()) {
        addProductSQL += ", `comments`";
    }
    addProductSQL += QString(") VALUES ( '%1', %2, %3, %4, %5, %6, '%7', %8, %9, %10")
            .arg(serialNumber).arg(productTypeID).arg(brandNameID).arg(productModelID)
            .arg(schemaNameID).arg(quantity).arg(unit).arg(operatorUserID)
            .arg(responserUserID).arg(productStatusID);

    if(!oldPurchasePrice.isEmpty()) {
        addProductSQL += QString(", '%1'").arg(oldPurchasePrice);
    }
    if(!purchasePrice.isEmpty()) {
        addProductSQL += QString(", '%1'").arg(purchasePrice);
    }
    if(!sellingPrice.isEmpty()) {
        addProductSQL += QString(", '%1'").arg(sellingPrice);
    }
    if(!comments.isEmpty()) {
        addProductSQL += QString(", '%1'").arg(comments);
    }
    addProductSQL +=  ")";

    //qDebug()<<addProductSQL;
    QSqlQuery query(userManagementInterface->getSqlQuery());
    bool ret = query.exec(addProductSQL);
    query.exec(QString("select id from products where serialNumber='%1'").arg(serialNumber));
    if(query.first()) {
        int id = query.record().value(ProductID).toInt();
        addProductinfoByProductID(id);
    }
    return ret;
}

bool ProductManagementIF::addProductByDetail(QString serialNumber, int productTypeID, int brandNameID,
                                int productModelID, int colorID, int vendorID, int schemaNameID,
                                int quantity, QString unit, QString oldPurchasePrice, QString purchasePrice,
                                QString sellingPrice, int operatorUserID, int responserUserID, int sellerID,
                                QString barInfo, int productStatusID, int replacementStatusID, QString time,
                                QString comments) const
{
    QSqlQuery query(userManagementInterface->getSqlQuery());
    bool ret = false;
    //bool ret = query.exec(QString("select quantity, timeStamp from products where serialNumber='%1'").arg(serialNumber));
    //if(query.first()) {
    if(0) {
        QSqlRecord record = query.record();
        int newquatity = record.value("quantity").toInt() + quantity;
        QString time = record.value("timeStamp").toString();
        time.replace("T", " ", Qt::CaseInsensitive);
        qDebug()<<"old one: "<<record;
        qDebug()<<QString("UPDATE products SET quantity = %1, timestamp = '%2' where serialNumber='%3'").arg(newquatity).arg(time).arg(serialNumber);
        ret = query.exec(QString("UPDATE products SET quantity = %1, timestamp = '%2' where serialNumber='%3'").arg(newquatity).arg(time).arg(serialNumber));
    } else {
        QString addProductSQL = "INSERT INTO `products` (`serialNumber`, `productTypeID`, `brandNameID`, `productModelID`, `colorID`, `vendorID`, `schemaNameID`, `quantity`, `unit`, `oldPurchasePrice`, `purchasePrice`, `sellingPrice`, `operatorUserID`, `responserUserID`, `sellerID`, `barInfo`, `productStatusID`, `replacementStatusID`, `timeStamp`, `comments`";
        addProductSQL += QString(") VALUES ( '%1', %2, %3, %4, %5, %6, '%7', %8, '%9', '%10', '%11', '%12', %13, %14, %15, '%16', %17, %18, '%19', '%20')")
                .arg(serialNumber).arg(productTypeID).arg(brandNameID).arg(productModelID).arg(colorID).arg(vendorID).arg(schemaNameID)
                .arg(quantity).arg(unit).arg(oldPurchasePrice).arg(purchasePrice).arg(sellingPrice).arg(operatorUserID).arg(responserUserID)
                .arg(sellerID).arg(barInfo).arg(productStatusID).arg(replacementStatusID).arg(time).arg(comments);
        qDebug()<<"new one: "<<addProductSQL;
        ret = query.exec(addProductSQL);
        query.exec(QString("select id from products where serialNumber='%1'").arg(serialNumber));
        if(query.first()) {
            int id = query.record().value(ProductID).toInt();
            addProductinfoByProductID(id);
        }
    }
    return ret;
}

//for table product
bool ProductManagementIF::updateProductDetailByProductID(int id, QString serialNumber, int productTypeID,
                                                         int brandNameID, int productModelID, int schemaNameID,
                                                         int quantity, QString unit, QString oldPurchasePrice,
                                                         QString purchasePrice, QString sellingPrice,
                                                         int operatorUserID, int responserUserID,
                                                         int productStatusID, QString comments) const
{
    QSqlQuery query(userManagementInterface->getSqlQuery());
    query.exec(QString("select * from products where id=%1").arg(id));
    if(query.first()) {
        QSqlRecord record = query.record();
        bool different = false;
        while(1) {
            if(record.value(ProductTypeID).toInt()!=productTypeID) {
                different = true;
                break;
            }
            if(record.value(BrandNameID).toInt()!=brandNameID) {
                different = true;
                break;
            }
            if(record.value(ProductModelID).toInt()!=productModelID) {
                different = true;
                break;
            }
            if(record.value(SchemaNameID).toInt()!=schemaNameID) {
                different = true;
                break;
            }
            if(record.value(Quantity).toInt()!=quantity) {
                different = true;
                break;
            }
            if(record.value(Unit).toString().compare(unit)) {
                different = true;
                break;
            }
            if(record.value(OldPurchasePrice).toString().compare(oldPurchasePrice)) {
                different = true;
                break;
            }
            if(record.value(PurchasePrice).toString().compare(purchasePrice)) {
                different = true;
                break;
            }
            if(record.value(SellingPrice).toString().compare(sellingPrice)) {
                different = true;
                break;
            }
            if(record.value(OperatorUserID).toInt()!=operatorUserID) {
                different = true;
                break;
            }
            if(record.value(ResponserUserID).toInt()!=responserUserID) {
                different = true;
                break;
            }
            if(record.value(ProductStatusID).toInt()!=productStatusID) {
                different = true;
                break;
            }
            if(record.value(Comments).toString().compare(comments)) {
                different = true;
                break;
            }

            break;
        }
        if(different) {
            bool ret = query.exec(QString("UPDATE products SET productTypeID = %1, brandNameID = %2, productModelID = %3, schemaNameID = %4, quantity = %5, unit = '%6', operatorUserID = %7, responserUserID = %8, productStatusID = %9, oldPurchasePrice = '%10', purchasePrice = '%11', sellingPrice = '%12', comments = '%13' WHERE id=%14;")
                              .arg(productTypeID).arg(brandNameID).arg(productModelID).arg(schemaNameID).arg(quantity)
                              .arg(unit).arg(operatorUserID).arg(responserUserID).arg(productStatusID).arg(oldPurchasePrice)
                              .arg(purchasePrice).arg(sellingPrice).arg(comments).arg(id));
//            qDebug()<<QString("UPDATE products SET productTypeID = %1, brandNameID = %2, productModelID = %3, schemaNameID = %4, quantity = %5, unit = '%6', operatorUserID = %7, responserUserID = %8, productStatusID = %9, oldPurchasePrice = '%10', purchasePrice = '%11', sellingPrice = '%12', comments = '%13' WHERE id=%14;")
//                      .arg(productTypeID).arg(brandNameID).arg(productModelID).arg(schemaNameID).arg(quantity)
//                      .arg(unit).arg(operatorUserID).arg(responserUserID).arg(productStatusID).arg(oldPurchasePrice)
//                      .arg(purchasePrice).arg(sellingPrice).arg(comments).arg(id);
            addProductinfoByProductID(id);
            return ret;
        }
    }
    return false;
}

//for table product
bool ProductManagementIF::updateProductDetailByProductID(int id, QString serialNumber, int productTypeID, int brandNameID,
                                int productModelID, int colorID, int vendorID, int schemaNameID,
                                int quantity, QString unit, QString oldPurchasePrice, QString purchasePrice,
                                QString sellingPrice, int operatorUserID, int responserUserID, int sellerID,
                                QString barInfo, int productStatusID, int replacementStatusID, QString time,
                                QString comments) const
{
    QSqlQuery query(userManagementInterface->getSqlQuery());
    query.exec(QString("select * from products where id=%1").arg(id));
    if(query.first()) {
        QSqlRecord record = query.record();
        bool different = false;
        while(1) {
            if(record.value(SerialNumber).toString().compare(serialNumber)) {
                different = true;
                break;
            }
            if(record.value(ProductTypeID).toInt()!=productTypeID) {
                different = true;
                break;
            }
            if(record.value(BrandNameID).toInt()!=brandNameID) {
                different = true;
                break;
            }
            if(record.value(ProductModelID).toInt()!=productModelID) {
                different = true;
                break;
            }
            if(record.value(ColorID).toInt()!=colorID) {
                different = true;
                break;
            }
            if(record.value(VendorID).toInt()!=vendorID) {
                different = true;
                break;
            }
            if(record.value(SchemaNameID).toInt()!=schemaNameID) {
                different = true;
                break;
            }
            if(record.value(Quantity).toInt()!=quantity) {
                different = true;
                break;
            }
            if(record.value(Unit).toString().compare(unit)) {
                different = true;
                break;
            }
            if(record.value(OldPurchasePrice).toString().compare(oldPurchasePrice)) {
                different = true;
                break;
            }
            if(record.value(PurchasePrice).toString().compare(purchasePrice)) {
                different = true;
                break;
            }
            if(record.value(SellingPrice).toString().compare(sellingPrice)) {
                different = true;
                break;
            }
            if(record.value(OperatorUserID).toInt()!=operatorUserID) {
                different = true;
                break;
            }
            if(record.value(ResponserUserID).toInt()!=responserUserID) {
                different = true;
                break;
            }
            if(record.value(SellerID).toInt()!=sellerID) {
                different = true;
                break;
            }
            if(record.value(BarInfo).toString().compare(barInfo)) {
                different = true;
                break;
            }
            if(record.value(ProductStatusID).toInt()!=productStatusID) {
                different = true;
                break;
            }
            if(record.value(ReplacementStatusID).toInt()!=replacementStatusID) {
                different = true;
                break;
            }
            if(record.value(TimeStamp).toString().compare(time)) {
                different = true;
                break;
            }
            if(record.value(Comments).toString().compare(comments)) {
                different = true;
                break;
            }

            break;
        }
        if(different) {
            bool ret = query.exec(QString("UPDATE products SET productTypeID = %1, brandNameID = %2, productModelID = %3, colorID = %4, vendorID = %5, schemaNameID = %6, quantity = %7, unit = '%8', oldPurchasePrice = '%9', purchasePrice = '%10', sellingPrice = '%11', operatorUserID = %12, responserUserID = %13, sellerID = %14, barInfo = '%15', productStatusID = %16, replacementStatusID = %17, timeStamp = '%18', comments = \"%19\", serialNumber = '%20' WHERE id = %21;")
                                  .arg(productTypeID).arg(brandNameID).arg(productModelID).arg(colorID).arg(vendorID).arg(schemaNameID)
                                  .arg(quantity).arg(unit).arg(oldPurchasePrice).arg(purchasePrice).arg(sellingPrice).arg(operatorUserID).arg(responserUserID)
                                  .arg(sellerID).arg(barInfo).arg(productStatusID).arg(replacementStatusID).arg(time).arg(comments).arg(serialNumber).arg(id));
            qDebug()<<QString("UPDATE products SET productTypeID = %1, brandNameID = %2, productModelID = %3, colorID = %4, vendorID = %5, schemaNameID = %6, quantity = %7, unit = '%8', oldPurchasePrice = '%9', purchasePrice = '%10', sellingPrice = '%11', operatorUserID = %12, responserUserID = %13, sellerID = %14, barInfo = '%15', productStatusID = %16, replacementStatusID = %17, timeStamp = '%18', comments = '%19', serialNumber = '%20' WHERE id = %21;")
            .arg(productTypeID).arg(brandNameID).arg(productModelID).arg(colorID).arg(vendorID).arg(schemaNameID)
            .arg(quantity).arg(unit).arg(oldPurchasePrice).arg(purchasePrice).arg(sellingPrice).arg(operatorUserID).arg(responserUserID)
            .arg(sellerID).arg(barInfo).arg(productStatusID).arg(replacementStatusID).arg(time).arg(comments).arg(serialNumber).arg(id);
            addProductinfoByProductID(id);
            return ret;
        }
    }
    return false;
}

//for table product
int ProductManagementIF::getResponserUserIDByProductID(int id) const
{
    QSqlQuery query(userManagementInterface->getSqlQuery());
    query.exec(QString("select responserUserID from products where id=%1").arg(id));
    if(query.first()) {
        return query.value(0).toInt();
    }
    return 0;
}

//for table product
bool ProductManagementIF::updateStatusIDByProductID(int productID, int statusID) const
{
    QSqlQuery query(userManagementInterface->getSqlQuery());
    query.exec(QString("select productStatusID from products where id=%1").arg(productID));
    if(query.first()) {
        QSqlRecord record = query.record();
        bool different = false;
        while(1) {
            if(record.value(ProductStatusID).toInt()!=statusID) {
                different = true;
                break;
            }
            break;
        }
        if(different) {
            int userID = userManagementInterface->getUserIDByUserName(userManagementInterface->getCurrentUserName());
            bool ret = query.exec(QString("UPDATE products SET productStatusID=%1,operatorUserID = %2, responserUserID = %2 WHERE id=%3;").arg(statusID).arg(userID).arg(productID));
            addProductinfoByProductID(productID);
            return ret;
        }
    }
    return false;
}

int ProductManagementIF::getProductCountByQuery(QString queryString) const
{
    QSqlQuery query(userManagementInterface->getSqlQuery());
    query.exec(queryString);
    return query.size();
}

bool ProductManagementIF::isModelOutdate(QSqlRelationalTableModel* model, QString &timeStamp)
{
    QString queryString = model->query().lastQuery();
    int selectIndex = queryString.indexOf("select", 0, Qt::CaseInsensitive);
    int fromIndex = queryString.indexOf("from", 0, Qt::CaseInsensitive);
    queryString.insert(fromIndex, ") ");
    queryString.insert(selectIndex+QString("select").length(), " count(");
    QRegExp reg1("count\\(.*\\)");
    reg1.setMinimal(true);
    queryString.replace(reg1, "count(*)");
    QRegExp reg2("order by.*");
    reg2.setCaseSensitivity(Qt::CaseInsensitive);
    queryString.remove(reg2);
    QSqlQuery query(userManagementInterface->getSqlQuery());
    query.exec(queryString);
    int rowInDB = 0;
    if(query.first()) {
        //qDebug()<<query.value(0).toInt();
        rowInDB = query.value(0).toInt();
    }
    int rowInModel = model->rowCount();
    //qDebug()<<rowInDB<<" "<<rowInModel;
    bool outDate = rowInDB!=rowInModel;
    queryString.replace(reg1, "max(timeStamp)");
    if(timeStamp.isEmpty()) {
        query.exec(queryString);
        if(query.first()) {
            timeStamp = query.value(0).toString();
            timeStamp.replace("T", " ", Qt::CaseInsensitive);
        }
        return outDate;
    }

    if(!outDate) {
        query.exec(queryString);
        if(query.first()) {
            QString dbtime = query.value(0).toString();
            dbtime.replace("T", " ", Qt::CaseInsensitive);
            QDateTime dbT = QDateTime::fromString(dbtime, "yyyy-MM-dd hh:mm:ss");
            timeStamp.replace("T", " ", Qt::CaseInsensitive);
            QDateTime modelT = QDateTime::fromString(timeStamp, "yyyy-MM-dd hh:mm:ss");
            outDate = dbT!=modelT;
            if(outDate) {
                timeStamp = dbtime;
            }
        }
    }

    return outDate;
}

QMap<QString, SalesResult> ProductManagementIF::getSalesResultByTimeRange(int year, int month, int range) const
{
    QMap<QString, SalesResult> data;
    data.clear();
    QString timeRange = QString("and p1.timeStamp between '%1-%2-1' and ").arg(year).arg(month);
    int endMonth = month + range - 1;
    int endYear = year;
    while(endMonth>12) {
        endMonth -= 12;
        endYear++;
    }
    timeRange += QString("'%1-%2-31'").arg(endYear).arg(endMonth);
    QString queryString = QString("select u1.name, sum(p1.sellingprice), sum(p1.quantity) from user u1, productsinfo p1 where p1.productstatusID = %1 and u1.id = p1.sellerID %2 group by u1.name")
            .arg(getStatusIDByStatusName("已销售")).arg(timeRange);
    qDebug()<<queryString;
    QSqlQuery query(userManagementInterface->getSqlQuery());
    query.exec(queryString);
    while(query.next()) {
        QString key = query.value(0).toString();
        SalesResult result;
        result.volume = query.value(1).toDouble();
        result.quantity = query.value(2).toDouble();
        data.insert(key, result);
    }
    return data;
}

//for table productsinfo
bool ProductManagementIF::addProductinfoByProductID(int productID) const
{
    QSqlQuery query(userManagementInterface->getSqlQuery());
    QString filedsString = " `serialNumber`, `productTypeID`, `brandNameID`, `productModelID`, `colorID`, `vendorID`, `schemaNameID`, `quantity`, `unit`, `oldPurchasePrice`, `purchasePrice`, `sellingPrice`, `operatorUserID`, `responserUserID`, `sellerID`, `barInfo`, `productStatusID`, `replacementStatusID`, `timeStamp`, `comments`";
    return query.exec(QString("insert into productsinfo ( %1 ) (select %1 from products where id=%2)").arg(filedsString).arg(productID));
}

//for table producttype
int ProductManagementIF::getTypeIDByTypeName(QString productType) const
{
    QSqlQuery query(userManagementInterface->getSqlQuery());
    query.exec(QString("select id from producttype where name = '%1'").arg(productType));
    if(query.first()) {
        return query.record().value("id").toInt();
    }
    return 0;
}

//for table producttype
QString ProductManagementIF::getTypeNameByTypeID(int typeID) const
{
    QSqlQuery query(userManagementInterface->getSqlQuery());
    query.exec(QString("select name from producttype where id = %1").arg(typeID));
    if(query.first()) {
        return query.record().value("name").toString();
    }
    return "";
}

//for table producttype
bool ProductManagementIF::addProductType(QString productType) const
{
    QSqlQuery query(userManagementInterface->getSqlQuery());
    return query.exec(QString("INSERT INTO `producttype` (`name`) VALUES ('%1')")
                      .arg(productType));
}

//for table brandname
int ProductManagementIF::getBrandIDByBrandName(QString brandName) const
{
    QSqlQuery query(userManagementInterface->getSqlQuery());
    query.exec(QString("select id from brandname where name = '%1'").arg(brandName));
    if(query.first()) {
        return query.record().value("id").toInt();
    }
    return 0;
}

//for table brandname
QString ProductManagementIF::getBrandNameByBrandID(int brandID) const
{
    QSqlQuery query(userManagementInterface->getSqlQuery());
    query.exec(QString("select name from brandname where id = %1").arg(brandID));
    if(query.first()) {
        return query.record().value("name").toString();
    }
    return "";
}

//for table brandname
bool ProductManagementIF::addBrandName(QString brandName) const
{
    QSqlQuery query(userManagementInterface->getSqlQuery());
    return query.exec(QString("INSERT INTO `brandname` (`name`) VALUES ('%1')")
               .arg(brandName));
}

//for table vendorinfo
int ProductManagementIF::getVendorIDByVendorName(QString vendorName) const
{
    QSqlQuery query(userManagementInterface->getSqlQuery());
    query.exec(QString("select id from vendorinfo where name = '%1'").arg(vendorName));
    if(query.first()) {
        return query.record().value("id").toInt();
    }
    return 0;
}

//for table vendorinfo
bool ProductManagementIF::addVendorName(QString vendorName) const
{
    QSqlQuery query(userManagementInterface->getSqlQuery());
    return query.exec(QString("INSERT INTO `vendorinfo` (`name`) VALUES ('%1')")
               .arg(vendorName));
}

//for table colorinfo
int ProductManagementIF::getColorIDByColorName(QString colorName) const
{
    QSqlQuery query(userManagementInterface->getSqlQuery());
    query.exec(QString("select id from colorinfo where color = '%1'").arg(colorName));
    if(query.first()) {
        return query.record().value("id").toInt();
    }
    return 0;
}

//for table colorinfo
bool ProductManagementIF::addColorName(QString colorName) const
{
    QSqlQuery query(userManagementInterface->getSqlQuery());
    return query.exec(QString("INSERT INTO `colorinfo` (`color`) VALUES ('%1')")
               .arg(colorName));
}

//for table productmodel
int ProductManagementIF::getModelIDByTypeIDBrandIDModelName(int productTypeID,
                                                            int brandNameID, QString productModel) const
{
    QSqlQuery query(userManagementInterface->getSqlQuery());
    query.exec(QString("select id from productmodel where productTypeID = %1 and brandNameID = %2 and model = '%3'")
            .arg(productTypeID).arg(brandNameID).arg(productModel));
    if(query.first()) {
        return query.record().value("id").toInt();
    }
    return 0;
}

//for table productmodel
QString ProductManagementIF::getModelNameByModelID(int modelID) const
{
    QSqlQuery query(userManagementInterface->getSqlQuery());
    query.exec(QString("select model from productmodel where id = %1").arg(modelID));
    if(query.first()) {
        return query.record().value("model").toString();
    }
    return "";
}

//for table productmodel
bool ProductManagementIF::addProductModel(int productTypeID, int brandNameID, QString productModel) const
{
    QSqlQuery query(userManagementInterface->getSqlQuery());
    return query.exec(QString("INSERT INTO `productmodel` (`productTypeID`, `brandNameID`, `model`) VALUES (%1, %2, '%3')")
               .arg(productTypeID).arg(brandNameID).arg(productModel));
}

//for table productmodel
QSet<int> ProductManagementIF::getBrandNameIDSetbyProductTypeID(int productTypeID) const
{
    QSqlQuery query(userManagementInterface->getSqlQuery());
    QSet<int> IDList;
    query.exec(QString("select DISTINCT brandNameID from productmodel where productTypeID = %1")
               .arg(productTypeID));
    while (query.next()) {
        IDList<<query.record().value("brandNameID").toInt();
    }
    return IDList;
}

//help function
bool ProductManagementIF::checkSerialNumber(QString serialNumber) const
{
    QSqlQuery query(userManagementInterface->getSqlQuery());
//    qDebug()<<QString("select id from products where serialNumber = '%1'").arg(serialNumber);
    query.exec(QString("select id from products where serialNumber = '%1'").arg(serialNumber));
    return query.first();
}

bool ProductManagementIF::dropAllTables() const
{
    QSqlQuery query(userManagementInterface->getSqlQuery());
    query.exec("drop schema `producttype`");
    query.exec("drop schema `brandname`");
    query.exec("drop schema `productmodel`");
    query.exec("drop schema `productstatus`");
    query.exec("drop schema `vendorinfo`");
    query.exec("drop schema `defaultcolor`");
    query.exec("drop schema `repalcementstatus`");
    query.exec("drop schema `products`");
    query.exec("drop schema `productsinfo`");
    return true;
}

///////////////////////////////////////////////////////////////////////////////////
bool ProductManagementIF::createProductManagementTables() const
{
    QSqlQuery query(userManagementInterface->getSqlQuery());
    if(!query.exec("desc producttype")) {
        if(!query.exec("CREATE TABLE `producttype` \
                        (`id` INT UNSIGNED NOT NULL AUTO_INCREMENT ,\
                         `name` VARCHAR(45) NOT NULL ,\
                         PRIMARY KEY (`id`) )")) {
            dropAllTables();
            return false;
        }
        //insert default ProductType
        int id = 1;
        foreach(QString type, defaultProductType) {
                       query.exec(QString("INSERT INTO `producttype` (`id`, `name`) VALUES (%1, '%2')")
                                  .arg(id++).arg(type));
        }
    }
    if(!query.exec("desc brandname")) {
        if(!query.exec("CREATE TABLE `brandname` \
                        (`id` INT UNSIGNED NOT NULL AUTO_INCREMENT ,\
                         `name` VARCHAR(45) NOT NULL ,\
                         PRIMARY KEY (`id`) )")) {
            dropAllTables();
            return false;
        }
        //insert default BrandName
        int id = 1;
        foreach(QString brand, defaultBrandName) {
                       query.exec(QString("INSERT INTO `brandname` (`id`, `name`) VALUES (%1, '%2')")
                                  .arg(id++).arg(brand));
        }
    }
    if(!query.exec("desc productmodel")) {
        if(!query.exec("CREATE TABLE `productmodel` \
                        (`id` INT UNSIGNED NOT NULL AUTO_INCREMENT ,\
                         `productTypeID` INT UNSIGNED NOT NULL DEFAULT 1 ,\
                         FOREIGN KEY (`productTypeID`) REFERENCES producttype(`id`) ,\
                         `brandNameID` INT UNSIGNED NOT NULL DEFAULT 1 ,\
                         FOREIGN KEY (`brandNameID`) REFERENCES brandname(`id`) ,\
                         `model` VARCHAR(45) NOT NULL ,\
                         PRIMARY KEY (`id`) )")) {
            dropAllTables();
            return false;
        }
        //insert default ProductModel
        int id = 1;
        foreach(QString model, defaultProductModel) {
                       query.exec(QString("INSERT INTO `productmodel` (`id`, `model`) VALUES (%1, '%2')")
                                  .arg(id++).arg(model));
        }
    }
    if(!query.exec("desc productstatus")) {
        if(!query.exec("CREATE TABLE `productstatus` \
                       (`id` INT UNSIGNED NOT NULL AUTO_INCREMENT ,\
                        `status` VARCHAR(45) NOT NULL ,\
                        PRIMARY KEY (`id`) )")) {
            dropAllTables();
            return false;
        }
        //insert default ProdcutStatus
        int id = 1;
        foreach(QString status, defaultProdcutStatus) {
                       query.exec(QString("INSERT INTO `productstatus` (`id`, `status`) VALUES (%1, '%2')")
                                  .arg(id++).arg(status));
        }
    }
    if(!query.exec("desc vendorinfo")) {
        if(!query.exec("CREATE TABLE `vendorinfo` \
                       (`id` INT UNSIGNED NOT NULL AUTO_INCREMENT ,\
                        `name` VARCHAR(45) NOT NULL ,\
                        PRIMARY KEY (`id`) )")) {
            dropAllTables();
            return false;
        }
        //insert default vendorinfo
        int id = 1;
        foreach(QString name, defaultVendorInfo) {
                       query.exec(QString("INSERT INTO `vendorinfo` (`id`, `name`) VALUES (%1, '%2')")
                                  .arg(id++).arg(name));
        }
    }
    if(!query.exec("desc colorinfo")) {
        if(!query.exec("CREATE TABLE `colorinfo` \
                       (`id` INT UNSIGNED NOT NULL AUTO_INCREMENT ,\
                        `color` VARCHAR(45) NOT NULL ,\
                        PRIMARY KEY (`id`) )")) {
            dropAllTables();
            return false;
        }
        //insert default colorinfo
        int id = 1;
        foreach(QString name, defaultColor) {
                       query.exec(QString("INSERT INTO `colorinfo` (`id`, `color`) VALUES (%1, '%2')")
                                  .arg(id++).arg(name));
        }
    }
    if(!query.exec("desc replacementstatus")) {
        if(!query.exec("CREATE TABLE `replacementstatus` \
                       (`id` INT UNSIGNED NOT NULL AUTO_INCREMENT ,\
                        `status` VARCHAR(45) NOT NULL ,\
                        PRIMARY KEY (`id`) )")) {
           dropAllTables();
           return false;
        }
        //insert default replacementstatus
        int id = 1;
        foreach(QString status, replacementStatus) {
            query.exec(QString("INSERT INTO `replacementstatus` (`id`, `status`) VALUES (%1, '%2')")
                       .arg(id++).arg(status));
        }
    }
    if(!query.exec("desc products")) {
        if(!query.exec("CREATE TABLE `products` \
                        (`id` INT UNSIGNED NOT NULL AUTO_INCREMENT ,\
                         `serialNumber` VARCHAR(45) NOT NULL ,\
                         `productTypeID` INT UNSIGNED NOT NULL DEFAULT 1 ,\
                         FOREIGN KEY (`productTypeID`) REFERENCES producttype(`id`) ,\
                         `brandNameID` INT UNSIGNED NOT NULL DEFAULT 1 ,\
                         FOREIGN KEY (`brandNameID`) REFERENCES brandname(`id`) ,\
                         `productModelID` INT UNSIGNED NOT NULL DEFAULT 1 ,\
                         FOREIGN KEY (`productModelID`) REFERENCES productmodel(`id`) ,\
                         `colorID` INT UNSIGNED NOT NULL DEFAULT 1 ,\
                         FOREIGN KEY (`colorID`) REFERENCES colorinfo(`id`) ,\
                         `vendorID` INT UNSIGNED NOT NULL DEFAULT 1 ,\
                         FOREIGN KEY (`vendorID`) REFERENCES vendorinfo(`id`) ,\
                         `schemaNameID` INT UNSIGNED NOT NULL DEFAULT 1 ,\
                         FOREIGN KEY (`schemaNameID`) REFERENCES schemaname(`id`) ,\
                         `quantity` INT UNSIGNED NOT NULL DEFAULT 1 ,\
                         `unit` VARCHAR(45) NOT NULL ,\
                         `oldPurchasePrice` VARCHAR(45) NULL ,\
                         `purchasePrice` VARCHAR(45) NULL ,\
                         `sellingPrice` VARCHAR(45) NULL ,\
                         `operatorUserID` INT UNSIGNED NOT NULL ,\
                         FOREIGN KEY (`operatorUserID`) REFERENCES user(`id`) ,\
                         `responserUserID` INT UNSIGNED NOT NULL ,\
                         FOREIGN KEY (`responserUserID`) REFERENCES user(`id`) ,\
                         `sellerID` INT UNSIGNED ,\
                         FOREIGN KEY (`sellerID`) REFERENCES user(`id`) ,\
                         `barInfo` VARCHAR(45) NULL ,\
                         `productStatusID` INT UNSIGNED NOT NULL DEFAULT 1 ,\
                         FOREIGN KEY (`productStatusID`) REFERENCES productstatus(`id`) ,\
                         `replacementStatusID` INT UNSIGNED NOT NULL DEFAULT 1 ,\
                         FOREIGN KEY (`replacementStatusID`) REFERENCES replacementstatus(`id`) ,\
                         `timeStamp` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,\
                         `comments` VARCHAR(4096) NULL ,\
                         PRIMARY KEY (`id`) )")) {
            dropAllTables();
            return false;
        }
    }
	if(!query.exec("desc productsinfo")) {
        if(!query.exec("CREATE TABLE `productsinfo` \
                        (`id` INT UNSIGNED NOT NULL AUTO_INCREMENT ,\
                         `serialNumber` VARCHAR(45) NOT NULL ,\
                         `productTypeID` INT UNSIGNED NOT NULL DEFAULT 1 ,\
                         FOREIGN KEY (`productTypeID`) REFERENCES producttype(`id`) ,\
                         `brandNameID` INT UNSIGNED NOT NULL DEFAULT 1 ,\
                         FOREIGN KEY (`brandNameID`) REFERENCES brandname(`id`) ,\
                         `productModelID` INT UNSIGNED NOT NULL DEFAULT 1 ,\
                         FOREIGN KEY (`productModelID`) REFERENCES productmodel(`id`) ,\
                         `colorID` INT UNSIGNED NOT NULL DEFAULT 1 ,\
                         FOREIGN KEY (`colorID`) REFERENCES colorinfo(`id`) ,\
                         `vendorID` INT UNSIGNED NOT NULL DEFAULT 1 ,\
                         FOREIGN KEY (`vendorID`) REFERENCES vendorinfo(`id`) ,\
                         `schemaNameID` INT UNSIGNED NOT NULL DEFAULT 1 ,\
                         FOREIGN KEY (`schemaNameID`) REFERENCES schemaname(`id`) ,\
                         `quantity` INT UNSIGNED NOT NULL DEFAULT 1 ,\
                         `unit` VARCHAR(45) NOT NULL ,\
                         `oldPurchasePrice` VARCHAR(45) NULL ,\
                         `purchasePrice` VARCHAR(45) NULL ,\
                         `sellingPrice` VARCHAR(45) NULL ,\
                         `operatorUserID` INT UNSIGNED NOT NULL ,\
                         FOREIGN KEY (`operatorUserID`) REFERENCES user(`id`) ,\
                         `responserUserID` INT UNSIGNED NOT NULL ,\
                         FOREIGN KEY (`responserUserID`) REFERENCES user(`id`) ,\
                         `sellerID` INT UNSIGNED ,\
                         FOREIGN KEY (`sellerID`) REFERENCES user(`id`) ,\
                         `barInfo` VARCHAR(45) NULL ,\
                         `productStatusID` INT UNSIGNED NOT NULL DEFAULT 1 ,\
                         FOREIGN KEY (`productStatusID`) REFERENCES productstatus(`id`) ,\
                         `replacementStatusID` INT UNSIGNED NOT NULL DEFAULT 1 ,\
                         FOREIGN KEY (`replacementStatusID`) REFERENCES replacementstatus(`id`) ,\
                         `timeStamp` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,\
                         `comments` VARCHAR(4096) NULL ,\
                         PRIMARY KEY (`id`) )")) {
            dropAllTables();
            return false;
        }
	}
    return true;
}

QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(ProductManagementIF, ProductManagementIF)
QT_END_NAMESPACE
