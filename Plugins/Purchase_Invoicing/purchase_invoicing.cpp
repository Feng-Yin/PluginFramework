#include <QtGui>
#include <QtSql>
#include <QAxObject>
#include "purchase_invoicing.h"
#include "mainwindow.h"
#include "usermanagement_interface.h"
#include "productmanagement_interface.h"
#include "addproductdialog.h"
#include "updateproductdialog.h"

Purchase_Invoicing::Purchase_Invoicing() :
    mainWidget(NULL),
    addProductButton(NULL),
    delProductButton(NULL),
    emptyProductsButton(NULL),
    importProductsButton(NULL),
    commitProductsButton(NULL),
    commitAllProductsButton(NULL),
    printProductsButton(NULL),
    purchasePanel(NULL),
    purchaseModel(NULL),
    purchaseView(NULL),
    parentWindow(NULL),
    userManagementInterface(NULL),
    productManagementInterface(NULL),
    addProductDialog(NULL),
    updateProductDialog(NULL),
    serialNumberLineEdit(NULL),
    filterPushButton(NULL),
    importConfigDialog(NULL),
    serialNumberComboBox(NULL),
    brandNameComboBox(NULL),
    productModelComboBox(NULL),
    colorComboBox(NULL),
    vendorComboBox(NULL),
    schemaNameComboBox(NULL),
    quantityComboBox(NULL),
    unitComboBox(NULL),
    oldPurchasePriceComboBox(NULL),
    purchasePriceComboBox(NULL),
    sellingPriceComboBox(NULL),
    operatorUserComboBox(NULL),
    responserUserComboBox(NULL),
    sellerComboBox(NULL),
    barInfoComboBox(NULL),
    productStatusComboBox(NULL),
    replacementStatusComboBox(NULL),
    timeStampComboBox(NULL),
    commentsComboBox(NULL),
    skip1stRowCheckBox(NULL)
{
    QDir qmdir(":/Translations");
    foreach (QString fileName, qmdir.entryList(QDir::Files)) {
        //qDebug()<<QFileInfo(fileName).baseName();
        QTranslator *qtTranslator = new QTranslator(this);
        qtTranslator->load(QFileInfo(fileName).baseName(), ":/Translations");
        QApplication::instance()->installTranslator(qtTranslator);
    }
    initImportConfigFile();
}

void Purchase_Invoicing::initMainWidget()
{
    mainWidget = new QWidget();
    mainWidget->setWindowIcon(QIcon(":/Icon/purchase_icon.png"));
    mainWidget->setWindowIconText(tr("Purchase"));

    addProductButton = new QPushButton();
    addProductButton->setIcon(QIcon(":/Icon/add_icon.png"));
    addProductButton->setIconSize(QSize(25, 25));
    addProductButton->setFlat(true);
    addProductButton->setToolTip(QString(tr("Add Products")));
    connect(addProductButton, SIGNAL(clicked()), this, SLOT(addProduct()));

    delProductButton = new QPushButton();
    delProductButton->setIcon(QIcon(":/Icon/delete_icon.png"));
    delProductButton->setIconSize(QSize(25, 25));
    delProductButton->setFlat(true);
    delProductButton->setToolTip(QString(tr("Delete Products")));
    connect(delProductButton, SIGNAL(clicked()), this, SLOT(delProduct()));

    emptyProductsButton = new QPushButton();
    emptyProductsButton->setIcon(QIcon(":/Icon/empty_icon.png"));
    emptyProductsButton->setIconSize(QSize(25, 25));
    emptyProductsButton->setFlat(true);
    emptyProductsButton->setToolTip(QString(tr("Empty Products")));
    connect(emptyProductsButton, SIGNAL(clicked()), this, SLOT(emptyProducts()));

    importProductsButton = new QPushButton();
    importProductsButton->setIcon(QIcon(":/Icon/import_icon.png"));
    importProductsButton->setIconSize(QSize(25, 25));
    importProductsButton->setFlat(true);
    importProductsButton->setToolTip(QString(tr("Import Products")));
    QMenu *menu = new QMenu();
    QAction *importAction = menu->addAction(QString(tr("Import Products")));
    QAction *configAction = menu->addAction(QString(tr("Config Importing")));

    importProductsButton->setMenu(menu);
    //connect(importProductsButton, SIGNAL(clicked()), this, SLOT(importProducts()));
    connect(importAction, SIGNAL(triggered()), this, SLOT(importProducts()));
    connect(configAction, SIGNAL(triggered()), this, SLOT(configImporting()));

    commitProductsButton = new QPushButton();
    commitProductsButton->setIcon(QIcon(":/Icon/commit_icon.png"));
    commitProductsButton->setIconSize(QSize(25, 25));
    commitProductsButton->setFlat(true);
    commitProductsButton->setToolTip(QString(tr("Commit Product")));
    connect(commitProductsButton, SIGNAL(clicked()), this, SLOT(commitProduct()));

    commitAllProductsButton = new QPushButton();
    commitAllProductsButton->setIcon(QIcon(":/Icon/commitall_icon.png"));
    commitAllProductsButton->setIconSize(QSize(25, 25));
    commitAllProductsButton->setFlat(true);
    commitAllProductsButton->setToolTip(QString(tr("Commit All Products")));
    connect(commitAllProductsButton, SIGNAL(clicked()), this, SLOT(commitAllProducts()));

    printProductsButton = new QPushButton();
    printProductsButton->setIcon(QIcon(":/Icon/print_icon.png"));
    printProductsButton->setIconSize(QSize(25, 25));
    printProductsButton->setFlat(true);
    printProductsButton->setToolTip(QString(tr("Print Products")));

    QFrame *line1 = new QFrame();
    line1->setAttribute(Qt::WA_MouseNoMask);
    line1->setFrameStyle(QFrame::Sunken);
    line1->setFrameShape(QFrame::VLine);
    QFrame *line2 = new QFrame();
    line2->setAttribute(Qt::WA_MouseNoMask);
    line2->setFrameStyle(QFrame::Sunken);
    line2->setFrameShape(QFrame::VLine);
    QFrame *line3 = new QFrame();
    line3->setAttribute(Qt::WA_MouseNoMask);
    line3->setFrameStyle(QFrame::Sunken);
    line3->setFrameShape(QFrame::VLine);
    QFrame *line4 = new QFrame();
    line4->setAttribute(Qt::WA_MouseNoMask);
    line4->setFrameStyle(QFrame::Sunken);
    line4->setFrameShape(QFrame::VLine);

    QFrame *line5 = new QFrame();
    line5->setAttribute(Qt::WA_MouseNoMask);
    line5->setFrameStyle(QFrame::Sunken);
    line5->setFrameShape(QFrame::VLine);

    QFrame *line6 = new QFrame();
    line6->setAttribute(Qt::WA_MouseNoMask);
    line6->setFrameStyle(QFrame::Sunken);
    line6->setFrameShape(QFrame::VLine);

    QHBoxLayout *hlayout = new QHBoxLayout();
    hlayout->addWidget(addProductButton);
    hlayout->addWidget(line1);
    hlayout->addWidget(delProductButton);
    hlayout->addWidget(line2);
    hlayout->addWidget(emptyProductsButton);
    hlayout->addWidget(line3);
    hlayout->addWidget(importProductsButton);
    hlayout->addWidget(line4);
    hlayout->addWidget(commitProductsButton);
    hlayout->addWidget(line5);
    hlayout->addWidget(commitAllProductsButton);
    hlayout->addWidget(line6);
    hlayout->addWidget(printProductsButton);

    QLabel *serialNumberLabel = new QLabel(tr("Serial Number : "));
    serialNumberLineEdit = new QLineEdit();
    filterPushButton = new QPushButton(tr("Filter"));
    connect(filterPushButton, SIGNAL(clicked()), this, SLOT(onFilter()));
    QHBoxLayout *filterLayout = new QHBoxLayout();
    filterLayout->addWidget(serialNumberLabel);
    filterLayout->addWidget(serialNumberLineEdit);
    filterLayout->addWidget(filterPushButton);
    QWidget *filterWidget = new QWidget();
    filterWidget->setLayout(filterLayout);

    bar = new QProgressBar();
    bar->setFormat("%v/%m");

    QGridLayout *mainLayout = new QGridLayout(mainWidget);
    mainLayout->addWidget(filterWidget, 0, 0, Qt::AlignTop);
    mainLayout->addLayout(hlayout, 1, 0, Qt::AlignTop);

    createPurchasePanel();
    mainLayout->addWidget(purchasePanel, 2, 0);
    mainLayout->addWidget(bar, 3, 0);
    mainWidget->setLayout(mainLayout);
    bar->hide();
}

QWidget* Purchase_Invoicing::getMainWidget() const
{
    return mainWidget;
}

QAction* Purchase_Invoicing::getAction() const
{
    return NULL;
}

QMenu* Purchase_Invoicing::getMenu() const
{
    return NULL;
}

QToolBar* Purchase_Invoicing::getToolBar() const
{
    return NULL;
}

bool Purchase_Invoicing::init(MainWindow *parent)
{
    parentWindow = parent;
    PluginInterface *plugin = parent->getPlugin("UserManagementIF");
    userManagementInterface = dynamic_cast<UserManagementInterface *>(plugin);
    if(!userManagementInterface) {
        return false;
    }
    else {
        userManagementInterface->registeObserver(this);
    }
    plugin = parent->getPlugin("ProductManagementIF");
    productManagementInterface = dynamic_cast<ProductManagementInterface *>(plugin);
    if(!productManagementInterface) {
        return false;
    }

    initMainWidget();
    userChanged();

//    QTimer *timer = new QTimer(this);
//    connect(timer, SIGNAL(timeout()), this, SLOT(updateDBTableModel()));
//    timer->setInterval(poll_interval);
//    timer->start(poll_interval);

    return true;
}

bool Purchase_Invoicing::deInit()
{
    if(userManagementInterface) {
        userManagementInterface->deregisteObserver(this);
    }
    return true;
}

void Purchase_Invoicing::createPurchasePanel()
{
    purchasePanel = new QWidget();
    QSqlDatabase db(userManagementInterface->getDatabase());
    purchaseModel = new QSqlRelationalTableModel(this, db);
    purchaseModel->setTable("products");
    purchaseModel->setRelation(ProductTypeID, QSqlRelation("producttype", "id", "name"));
    purchaseModel->setRelation(BrandNameID, QSqlRelation("brandname", "id", "name"));
    purchaseModel->setRelation(ProductModelID, QSqlRelation("productmodel", "id", "model"));
    purchaseModel->setRelation(ColorID, QSqlRelation("colorinfo", "id", "color"));
    purchaseModel->setRelation(VendorID, QSqlRelation("vendorinfo", "id", "name"));
    purchaseModel->setRelation(SchemaNameID, QSqlRelation("schemaname", "id", "name"));
    purchaseModel->setRelation(OperatorUserID, QSqlRelation("user", "id", "name"));
    purchaseModel->setRelation(ResponserUserID, QSqlRelation("user", "id", "name"));
    purchaseModel->setRelation(SellerID, QSqlRelation("user", "id", "name"));
    purchaseModel->setRelation(ProductStatusID, QSqlRelation("productstatus", "id", "status"));
    purchaseModel->setRelation(ReplacementStatusID, QSqlRelation("replacementstatus", "id", "status"));
    purchaseModel->setSort(TimeStamp, Qt::AscendingOrder);

    purchaseModel->setHeaderData(ProductID, Qt::Horizontal, tr("ID"));
    purchaseModel->setHeaderData(SerialNumber, Qt::Horizontal, tr("Serial Number"));
    purchaseModel->setHeaderData(ProductTypeID, Qt::Horizontal, tr("Product Type"));
    purchaseModel->setHeaderData(BrandNameID, Qt::Horizontal, tr("Brand Name"));
    purchaseModel->setHeaderData(ProductModelID, Qt::Horizontal, tr("Model Name"));
    purchaseModel->setHeaderData(ColorID, Qt::Horizontal, tr("Color"));
    purchaseModel->setHeaderData(VendorID, Qt::Horizontal, tr("Vendor"));
    purchaseModel->setHeaderData(SchemaNameID, Qt::Horizontal, tr("Schema Name"));
    purchaseModel->setHeaderData(Quantity, Qt::Horizontal, tr("Quantity"));
    purchaseModel->setHeaderData(Unit, Qt::Horizontal, tr("Unit"));
    purchaseModel->setHeaderData(OldPurchasePrice, Qt::Horizontal, tr("Old Purchase Price"));
    purchaseModel->setHeaderData(PurchasePrice, Qt::Horizontal, tr("Purchase Price"));
    purchaseModel->setHeaderData(SellingPrice, Qt::Horizontal, tr("Selling Price"));
    purchaseModel->setHeaderData(OperatorUserID, Qt::Horizontal, tr("Operator"));
    purchaseModel->setHeaderData(ResponserUserID, Qt::Horizontal, tr("Responser"));
    purchaseModel->setHeaderData(SellerID, Qt::Horizontal, tr("Seller"));
    purchaseModel->setHeaderData(BarInfo, Qt::Horizontal, tr("BarInfo"));
    purchaseModel->setHeaderData(ProductStatusID, Qt::Horizontal, tr("Product Status"));
    purchaseModel->setHeaderData(ReplacementStatusID, Qt::Horizontal, tr("Replacement Status"));
    purchaseModel->setHeaderData(TimeStamp, Qt::Horizontal, tr("TimeStamp"));
    purchaseModel->setHeaderData(Comments, Qt::Horizontal, tr("Comments"));

//    int statusID = productManagementInterface->getStatusIDByStatusName("录入");
//    int userID = userManagementInterface->getUserIDByUserName(
//                userManagementInterface->getCurrentUserName());
//    QSet<int> schemaSet = userManagementInterface->getSchemaIDSetByUserID(userID);
//    QString filter = QString("productStatusID = %1 and operatorUserID = %2")
//            .arg(statusID).arg(userID);

//    if(!schemaSet.isEmpty()) {
//        QString schemaFilter("");
//        foreach(int schemaID, schemaSet) {
//            if(schemaFilter.isEmpty()) {
//                schemaFilter += QString(" and (schemaNameID = %1 ").arg(schemaID);
//            }
//            else {
//                schemaFilter += QString(" or schemaNameID = %1 ").arg(schemaID);
//            }
//        }
//        schemaFilter += ")";
//        filter += schemaFilter;
//    }

//    purchaseModel->setFilter(filter);
    updatePurchaseFilter();
    purchaseModel->select();

    purchaseView = new QTableView;
    purchaseView->setModel(purchaseModel);
    purchaseView->setItemDelegate(new QSqlRelationalDelegate(this));
    purchaseView->setSelectionMode(QAbstractItemView::SingleSelection);
    purchaseView->setSelectionBehavior(QAbstractItemView::SelectRows);
    purchaseView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    purchaseView->resizeColumnsToContents();
    purchaseView->horizontalHeader()->setStretchLastSection(true);
    purchaseView->setColumnHidden(ProductID, true);
    purchaseView->horizontalHeader()->setVisible(true);
    connect(purchaseView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(updateProductinfo()));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(purchaseView);
    purchasePanel->setLayout(layout);
}

void Purchase_Invoicing::userChanged()
{
    if(userManagementInterface->checkAccess(getAccessRoleNameSet())) {
        if(mainWidget) {
            QObjectList children = mainWidget->children();
            foreach(QObject *object, children) {
                QWidget *widget = qobject_cast<QWidget *>(object);
                if(widget) {
                    widget->show();
                }
            }
        }
    }
    else {
        if(mainWidget) {
            QObjectList children = mainWidget->children();
            foreach(QObject *object, children) {
                QWidget *widget = qobject_cast<QWidget *>(object);
                if(widget) {
                    widget->hide();
                }
            }
        }
    }
    hidePurchasePrice();
    updatePurchaseFilter();
    updateDBTableModel();
}

void Purchase_Invoicing::hidePurchasePrice()
{
    bool hide = true;
    QSet<int> roleset = userManagementInterface->getRoleIDSetByUserID(
                userManagementInterface->getUserIDByUserName(
                    userManagementInterface->getCurrentUserName()));
    int adminRoleID = userManagementInterface->getRoleIDByRoleName("管理员");
    //int storagerRoleID = userManagementInterface->getRoleIDByRoleName("库管");
    foreach(int i, roleset) {
        //if(i == adminRoleID || i == storagerRoleID) {
        if(i == adminRoleID) {
            hide = false;
        }
    }
    purchaseView->setColumnHidden(OldPurchasePrice, hide);
    purchaseView->setColumnHidden(PurchasePrice, hide);

    //QMessageBox::critical(NULL, "test", "test1");
    if(!updateProductDialog) {
        updateProductDialog = new UpdateProductDialog(userManagementInterface,
                                                      productManagementInterface);
        connect(updateProductDialog, SIGNAL(productUpdated()), this, SLOT(productAdded()));
    }
    updateProductDialog->hidePurchasePrice(hide);
    if(!addProductDialog) {
        addProductDialog = new AddProductDialog(userManagementInterface, productManagementInterface);
        connect(addProductDialog, SIGNAL(productAdded()), this, SLOT(productAdded()));
    }
    addProductDialog->hidePurchasePrice(hide);
}

QString Purchase_Invoicing::moduleName() const
{
    return this->metaObject()->className();
}

QString Purchase_Invoicing::moduleDescription() const
{
    return tr("Purchase Function");
}

QSet<QString> Purchase_Invoicing::getAccessRoleNameSet() const
{
    return QSet<QString>()<<"管理员"<<"采购";
}

QSet<QString> Purchase_Invoicing::getDependencySet() const
{
    return QSet<QString>()<<"UserManagementIF"<<"ProductManagementIF";
}

void Purchase_Invoicing::addProduct()
{
    if(!addProductDialog) {
        addProductDialog = new AddProductDialog(userManagementInterface, productManagementInterface);
        connect(addProductDialog, SIGNAL(productAdded()), this, SLOT(productAdded()));
    }
    addProductDialog->updateDBTableModel();
    addProductDialog->exec();
}

bool Purchase_Invoicing::addProduct(QMap<int, int> &columnsMap, QMap<int, QString> &columnsValue)
{
    QMap<int, QString> allInfo;
    for(int i=0; i<MaxFieldID; i++)
    {
        allInfo[i] = "";
    }

    //0 is the product type
    int size = columnsValue.size();
    for(int i=0; i<size; i++)
    {
        allInfo[columnsMap[i]] = columnsValue[i];
        //QMessageBox::critical(NULL, "addProduct", QString("%1 %2").arg(columnsMap[i]).arg(columnsValue[i]));
    }

    QString serialNumber = allInfo[SerialNumber].simplified();
    QString productType = allInfo[ProductTypeID].simplified();

    QString brandName = allInfo[BrandNameID].simplified();
    QString productModel = allInfo[ProductModelID].simplified();
    QString color = allInfo[ColorID].simplified();
    QString vendor = allInfo[VendorID].simplified();
    QString schemaName = allInfo[SchemaNameID].simplified();
    QString quantity = allInfo[Quantity].simplified();
    QString unit = allInfo[Unit].simplified();
    QString oldPurchasePrice = allInfo[OldPurchasePrice].simplified();
    QString purchasePrice = allInfo[PurchasePrice].simplified();
    QString sellingPrice = allInfo[SellingPrice].simplified();
    QString operatorUser = allInfo[OperatorUserID].simplified();
    QString responserUser = allInfo[ResponserUserID].simplified();
    QString seller = allInfo[SellerID].simplified();
    QString barInfo = allInfo[BarInfo].simplified();
    QString productStatus = allInfo[ProductStatusID].simplified();
    QString replacementStatus = allInfo[ReplacementStatusID].simplified();
    QString timeStamp = allInfo[TimeStamp].simplified();
    QString comments = allInfo[Comments].simplified();

//    QMessageBox::critical(NULL, "configImporting", QString("%1 %2").arg(size).arg(sellingPrice));
//    QMessageBox::critical(NULL, "configImporting", QString("%1 %2").arg(size).arg(columnsValue[10]));
//    QMessageBox::critical(NULL, "configImporting", QString("%1 %2").arg(size).arg(columnsValue[10]));

    int productTypeID;
    if(productType.isEmpty()) {
        productTypeID = productManagementInterface->getTypeIDByTypeName("其它");
    } else {
        productTypeID = productManagementInterface->getTypeIDByTypeName(productType);
        if(!productTypeID) {
            productManagementInterface->addProductType(productType);
            productTypeID = productManagementInterface->getTypeIDByTypeName(productType);
        }
    }

    int brandNameID;
    if(brandName.isEmpty()) {
        brandNameID = productManagementInterface->getBrandIDByBrandName("其它");
    } else {
        brandNameID = productManagementInterface->getBrandIDByBrandName(brandName);
        if(!brandNameID) {
            productManagementInterface->addBrandName(brandName);
            brandNameID = productManagementInterface->getBrandIDByBrandName(brandName);
        }
    }

    int productModelID;
    if(productModel.isEmpty()) {
        productModelID = productManagementInterface->getModelIDByTypeIDBrandIDModelName(productTypeID, brandNameID, "其它");
        if(!productModelID) {
            productManagementInterface->addProductModel(productTypeID, brandNameID, "其它");
            productModelID = productManagementInterface->getModelIDByTypeIDBrandIDModelName(productTypeID, brandNameID, "其它");
        }
    } else {
        productModelID = productManagementInterface->getModelIDByTypeIDBrandIDModelName(productTypeID, brandNameID, productModel);
        if(!productModelID) {
            productManagementInterface->addProductModel(productTypeID, brandNameID, productModel);
            productModelID = productManagementInterface->getModelIDByTypeIDBrandIDModelName(productTypeID, brandNameID, productModel);
        }
    }

    int colorID;
    if(color.isEmpty()) {
        colorID = productManagementInterface->getColorIDByColorName("其它");
    } else {
        colorID = productManagementInterface->getColorIDByColorName(color);
        if(!colorID) {
            productManagementInterface->addColorName(color);
            colorID = productManagementInterface->getColorIDByColorName(color);
        }
    }

    int vendorID;
    if(vendor.isEmpty()) {
        vendorID = productManagementInterface->getVendorIDByVendorName("其它");
    } else {
        vendorID = productManagementInterface->getVendorIDByVendorName(vendor);
        if(!vendorID) {
            productManagementInterface->addVendorName(vendor);
            vendorID = productManagementInterface->getVendorIDByVendorName(vendor);
        }
    }

    int schemaID;
    if(schemaName.isEmpty()) {
        schemaID = userManagementInterface->getSchemaIDBySchemaName("未指定");
    } else {
        schemaID = userManagementInterface->getSchemaIDBySchemaName(schemaName);
        if(!schemaID) {
            userManagementInterface->addSchema(schemaName);
            schemaID = userManagementInterface->getSchemaIDBySchemaName(schemaName);
        }
    }

    int quantityNum = quantity.toInt();
    if(unit.isEmpty())
    {
        unit = "个";
    }
    int userID = userManagementInterface->getUserIDByUserName(userManagementInterface->getCurrentUserName());
    int sellerID = userManagementInterface->getUserIDByUserName("未指定");
    if(barInfo.isEmpty())
    {
        barInfo = "未指定";
    }
    int statusID = productManagementInterface->getStatusIDByStatusName("录入");

    int replacementStatusID;
    if(replacementStatus.isEmpty()) {
        replacementStatusID = productManagementInterface->getReplacementStatusIDByReplacementStatusName("否");
    } else {
        replacementStatusID = productManagementInterface->getReplacementStatusIDByReplacementStatusName(replacementStatus);
        if(!replacementStatusID) {
            replacementStatusID = productManagementInterface->getReplacementStatusIDByReplacementStatusName("否");
        }
    }

    QString timeStamp1("");
    if(!timeStamp.isEmpty()) {
        timeStamp.replace("T", " ", Qt::CaseInsensitive);
        QDateTime tmpTime = QDateTime::fromString(timeStamp, "yyyy-MM-dd hh:mm:ss");
        if(tmpTime.isValid()) {
            timeStamp1 = tmpTime.toString("yyyy-MM-dd hh:mm:ss");
        } else {
            QDateTime tmpTime = QDateTime::currentDateTime();
            timeStamp1 = tmpTime.toString("yyyy-MM-dd hh:mm:ss");
        }
    } else {
        QDateTime tmpTime = QDateTime::currentDateTime();
        timeStamp1 = tmpTime.toString("yyyy-MM-dd hh:mm:ss");
    }

    int ret = productManagementInterface->addProductByDetail(serialNumber, productTypeID, brandNameID, productModelID,
                                                             colorID, vendorID, schemaID, quantityNum, unit, oldPurchasePrice,
                                                             purchasePrice, sellingPrice, userID, userID, sellerID, barInfo,
                                                             statusID, replacementStatusID, timeStamp1, comments);
    if(!ret) {
        //QMessageBox::warning(0, "Error", QString("Insert %1 Error").arg(serialNumber));
    }
    return ret;
}

bool Purchase_Invoicing::addProduct(QStringList &product)
{
    //进货日期 供货商 品牌 机型 串号 颜色 数量 备注
    QString productType;
    QString time;
    QString vendorName;
    QString brandName;
    QString modelName;
    QString serialNumber;
    QString colorName;
    QString quatityString;
    QString comments;
    productType = product.at(0).simplified();
    time = product.at(1).simplified();
    vendorName = product.at(2).simplified();
    brandName = product.at(3).simplified();
    modelName = product.at(4).simplified();
    serialNumber = product.at(5).simplified();
    colorName = product.at(6).simplified();
    quatityString = product.at(7).simplified();
    comments = product.at(8).simplified();
    int productTypeID;
    if(productType.isEmpty()) {
        productTypeID = productManagementInterface->getTypeIDByTypeName("其它");
    } else {
        productTypeID = productManagementInterface->getTypeIDByTypeName(productType);
        if(!productTypeID) {
            productManagementInterface->addProductType(productType);
            productTypeID = productManagementInterface->getTypeIDByTypeName(productType);
        }
    }

    int vendorID;
    if(vendorName.isEmpty()) {
        vendorID = productManagementInterface->getVendorIDByVendorName("其它");
    } else {
        vendorID = productManagementInterface->getVendorIDByVendorName(vendorName);
        if(!vendorID) {
            productManagementInterface->addVendorName(vendorName);
            vendorID = productManagementInterface->getVendorIDByVendorName(vendorName);
        }
    }

    int brandNameID;
    if(brandName.isEmpty()) {
        brandNameID = productManagementInterface->getBrandIDByBrandName("其它");
    } else {
        brandNameID = productManagementInterface->getBrandIDByBrandName(brandName);
        if(!brandNameID) {
            productManagementInterface->addBrandName(brandName);
            brandNameID = productManagementInterface->getBrandIDByBrandName(brandName);
        }
    }
    int productModelID;
    if(modelName.isEmpty()) {
        productModelID = productManagementInterface->getModelIDByTypeIDBrandIDModelName(productTypeID, brandNameID, "其它");
        if(!productModelID) {
            productManagementInterface->addProductModel(productTypeID, brandNameID, "其它");
            productModelID = productManagementInterface->getModelIDByTypeIDBrandIDModelName(productTypeID, brandNameID, "其它");
        }
    } else {
        productModelID = productManagementInterface->getModelIDByTypeIDBrandIDModelName(productTypeID, brandNameID, modelName);
        if(!productModelID) {
            productManagementInterface->addProductModel(productTypeID, brandNameID, modelName);
            productModelID = productManagementInterface->getModelIDByTypeIDBrandIDModelName(productTypeID, brandNameID, modelName);
        }
    }

    int colorID;
    if(colorName.isEmpty()) {
        colorID = productManagementInterface->getColorIDByColorName("其它");
    } else {
        colorID = productManagementInterface->getColorIDByColorName(colorName);
        if(!colorID) {
            productManagementInterface->addColorName(colorName);
            colorID = productManagementInterface->getColorIDByColorName(colorName);
        }
    }

    int schemaID = userManagementInterface->getSchemaIDBySchemaName("未指定");
    //int schemaID = 1;
    int quantity = quatityString.toInt();
    int userID = userManagementInterface->getUserIDByUserName(userManagementInterface->getCurrentUserName());
    int sellerID = userManagementInterface->getUserIDByUserName("未指定");
    int statusID = productManagementInterface->getStatusIDByStatusName("录入");
    QString timeStamp("");
    if(!time.isEmpty()) {
        time.replace("T", " ", Qt::CaseInsensitive);
        QDateTime tmpTime = QDateTime::fromString(time, "yyyy-MM-dd hh:mm:ss");
        if(tmpTime.isValid()) {
            timeStamp = tmpTime.toString("yyyy-MM-dd hh:mm:ss");
        } else {
            QDateTime tmpTime = QDateTime::currentDateTime();
            timeStamp = tmpTime.toString("yyyy-MM-dd hh:mm:ss");
        }
    } else {
        QDateTime tmpTime = QDateTime::currentDateTime();
        timeStamp = tmpTime.toString("yyyy-MM-dd hh:mm:ss");
    }
    int replacementStatusID = productManagementInterface->getReplacementStatusIDByReplacementStatusName("否");
    int ret = productManagementInterface->addProductByDetail(serialNumber.simplified(), productTypeID, brandNameID,
                                                   productModelID, colorID, vendorID, schemaID, quantity, /*unit*/"个",
                                                   "", "", "", userID, userID, sellerID, "未指定", statusID, replacementStatusID,
                                                   timeStamp.simplified(), comments.simplified());
    if(!ret) {
        //QMessageBox::warning(0, "Error", QString("Insert %1 Error").arg(serialNumber));
    }
    return ret;
    //productAdded();
}

void Purchase_Invoicing::addProduct(QMap<QString, QList<QStringList> > &productsMap)
{
    foreach(QString productType, productsMap.keys()) {
        int productTypeID = productManagementInterface->getTypeIDByTypeName(productType);
        if(!productTypeID) {
            productManagementInterface->addProductType(productType);
            productTypeID = productManagementInterface->getTypeIDByTypeName(productType);
        }

        //进货日期 供货商 品牌 机型 串号 颜色 数量 备注
        QString time;
        QString vendorName;
        QString brandName;
        QString modelName;
        QString serialNumber;
        QString colorName;
        QString quatityString;
        QString comments;
        foreach(QStringList product, productsMap[productType]){
            //product>>time>>vendorName>>brandName>>modelName>>serialNumber>>colorName>>quatityString>>comments;
            product.pop_front();
            time = product.at(0);
            vendorName = product.at(1);
            brandName = product.at(2);
            modelName = product.at(3);
            serialNumber = product.at(4);
            colorName = product.at(5);
            quatityString = product.at(6);
            comments = product.at(7);
            int vendorID = productManagementInterface->getVendorIDByVendorName(vendorName);
            if(!vendorID) {
                productManagementInterface->addVendorName(vendorName);
            }
            vendorID = productManagementInterface->getVendorIDByVendorName(vendorName);
            int brandNameID = productManagementInterface->getBrandIDByBrandName(brandName);
            if(!brandNameID) {
                productManagementInterface->addBrandName(brandName);
            }
            brandNameID = productManagementInterface->getBrandIDByBrandName(brandName);
            int productModelID = productManagementInterface->getModelIDByTypeIDBrandIDModelName(productTypeID, brandNameID, modelName);
            if(!productModelID) {
                productManagementInterface->addProductModel(productTypeID, brandNameID, modelName);
            }
            productModelID = productManagementInterface->getModelIDByTypeIDBrandIDModelName(productTypeID, brandNameID, modelName);
            int colorID = productManagementInterface->getColorIDByColorName(colorName);
            if(!colorID) {
                productManagementInterface->addColorName(colorName);
            }
            colorID = productManagementInterface->getColorIDByColorName(colorName);

            //int schemaID = userManagementInterface->getSchemaIDBySchemaName("未指定");
            int schemaID = 1;
            int quantity = quatityString.toInt();
            int userID = userManagementInterface->getUserIDByUserName(userManagementInterface->getCurrentUserName());
            int statusID = productManagementInterface->getStatusIDByStatusName("录入");
            time.replace("T", " ", Qt::CaseInsensitive);
            QDateTime tmpTime = QDateTime::fromString(time, "yyyy-MM-dd hh:mm:ss");
            QString timeStamp("");
            if(tmpTime.isValid()) {
                timeStamp = tmpTime.toString("yyyy-MM-dd hh:mm:ss");
            }
            int replacementStatusID = productManagementInterface->getReplacementStatusIDByReplacementStatusName("否");
            productManagementInterface->addProductByDetail(serialNumber.simplified(), productTypeID, brandNameID,
                                                           productModelID, colorID, vendorID, schemaID, quantity, /*unit*/"个",
                                                           "", "", "", userID, userID, userID, "未指定", statusID, replacementStatusID,
                                                           timeStamp.simplified(), comments.simplified());
        }
    }
}

void Purchase_Invoicing::productAdded()
{
    purchaseModel->select();
    purchaseView->resizeColumnsToContents();
}

void Purchase_Invoicing::delProduct()
{
    QModelIndex index = purchaseView->currentIndex();
    QSqlRecord record = purchaseModel->record(index.row());
    int id = record.value(ProductID).toInt();
    int statusID = productManagementInterface->getStatusIDByStatusName("已删除");
    productManagementInterface->updateStatusIDByProductID(id, statusID);

    purchaseModel->removeRow(index.row());
    if(index.row()<purchaseModel->rowCount()) {
        purchaseView->selectRow(index.row());
    }
    else {
        purchaseView->selectRow(purchaseModel->rowCount()-1);
    }
}

void Purchase_Invoicing::emptyProducts()
{
    int statusID = productManagementInterface->getStatusIDByStatusName("录入");
    int userID = userManagementInterface->getUserIDByUserName(
                userManagementInterface->getCurrentUserName());
    int deleteID = productManagementInterface->getStatusIDByStatusName("已删除");
    QSet<int> productIDSet = productManagementInterface->getProductIDSetByUserIDStatusID(userID, statusID);
    bar->setRange(0, productIDSet.count());
    bar->setValue(0);
    int i = 0;
    mainWidget->setCursor(Qt::BusyCursor);
    foreach(int id, productIDSet) {
        productManagementInterface->updateStatusIDByProductID(id, deleteID);
        productManagementInterface->deleteProductByProductID(id);
        bar->setValue(++i);
        //qApp->processEvents();
    }
    mainWidget->unsetCursor();
    purchaseModel->select();
}

void Purchase_Invoicing::importProducts()
{
    QMap<int, int> filedsMap;
    QMap<int, int> columnsMap;
    QMap<int, QString> columnsValue;
    QSettings importConfig(qApp->applicationDirPath()+"/importconfig.ini", QSettings::IniFormat);
    for(int i=0; i<MaxFieldID; i++)
    {
        filedsMap[i] = importConfig.value("config/"+filedsName[i]).toInt();
        //QMessageBox::critical(NULL, "importProducts", QString("%1 %2").arg(i).arg(filedsMap[i]));
        if(filedsMap[i] != 0)
        {
            columnsMap[filedsMap[i]] = i;
        }
    }
    columnsMap[0] = ProductTypeID;
    int skip1stRow = importConfig.value("config/Skip1stRow").toBool()?1:0;
    QString fileName = QFileDialog::getOpenFileName(0, tr("Open Inventory"), ".", tr("Inventory files (*.xls *.xlsx)"));
    if (!fileName.isEmpty())
    {
        // load file into the excel ojbect
        QAxObject* excel = new QAxObject( "Excel.Application", 0 );
        QAxObject* workbooks = excel->querySubObject( "Workbooks" );
        QAxObject* workbook = workbooks->querySubObject( "Open(QString&)", fileName );
        try{
            QAxObject* sheets = workbook->querySubObject( "Worksheets" );
            //worksheets count
            int count=sheets->dynamicCall("Count()").toInt();
            int totalRows = 0;
            int rowCount = 0;
            int colsCount = 0;
            for(int i=1; i<count+1; i++) {
                QAxObject* sheet = sheets->querySubObject( "Item( int )", i );
                //////////////////////////////////////////////////////////////////
                QAxObject* usedrange = sheet->querySubObject("UsedRange");
                QAxObject* rows = usedrange->querySubObject("Rows");
                QAxObject* cols = usedrange->querySubObject("Columns");
                //QMessageBox::critical(NULL, "configImporting", QString("%1").arg(cols->property("Count").toInt()));
                int intRows = rows->property("Count").toInt();                
                totalRows += (intRows - 1);
                colsCount = cols->property("Count").toInt();
            }
            bar->setRange(0, totalRows);
            bar->setValue(0);
            bar->show();
            mainWidget->setCursor(Qt::BusyCursor);
            for(int i=1; i<count+1; i++) {
                QAxObject* sheet = sheets->querySubObject( "Item( int )", i );
                //////////////////////////////////////////////////////////////////
                QAxObject* usedrange = sheet->querySubObject("UsedRange");
                QAxObject* rows = usedrange->querySubObject("Rows");
//                QAxObject* columns = usedrange->querySubObject("Columns");
//                int intRowStart = usedrange->property("Row").toInt();
//                int intColStart = usedrange->property("Column").toInt();
//                int intCols = columns->property("Count").toInt();
                int intRows = rows->property("Count").toInt();
                QString name = sheet->dynamicCall("Name()").toString();
//                QMessageBox::about(0, "testing", QString("%1 %2 %3 %4 %5").arg(name).arg(intRowStart).arg(intColStart).arg(intRows).arg(intCols));
                //////////////////////////////////////////////////////////////////
                if(sheet) {
                    //进货日期 供货商 品牌 机型 串号 颜色 数量 备注
                    QList<QStringList> products;
                    bool done = false;
                    for(int row=skip1stRow; ; row++) {
                        QStringList data;
                        columnsValue.clear();
                        columnsValue[0] = name;
                        data.append(name);
                        for(int col=1; col<=colsCount; col++) {
                            QAxObject* cell = sheet->querySubObject( "Cells( int, int )", row+1, col );
                            QVariant value = cell->dynamicCall( "Value()" );
                            //QMessageBox::critical(NULL, "configImporting", QString("%1 %2").arg(col).arg(value.toString()));
                            if(value.toString().isEmpty() && col==1 && row>=intRows) {
                                done = true;
                                break;
                            } else {
                                data.append(value.toString());
                                columnsValue[col]=value.toString();
                            }
                        }
                        if(done) {
                            break;
                        } else {
                            //products.append(data);
                            //int ret = addProduct(data);
                            int ret = addProduct(columnsMap, columnsValue);
                            bar->setValue(++rowCount);
                            //bar->update();
                            //qApp->processEvents();
                            if(!ret) {
                                QMessageBox::warning(0, "Error", QString("Insert Error. Sheet:%1, Row: %2").arg(name).arg(row));
                            }
                        }
                    }
    //                foreach(QStringList product, products) {
    //                    QMessageBox::about(0, product.at(0), QString("进货日期:%1 供货商:%2 品牌:%3 机型:%4 串号:%5 颜色:%6 数量:%7 备注:%8")
    //                                       .arg(product.at(1)).arg(product.at(2)).arg(product.at(3)).arg(product.at(4))
    //                                       .arg(product.at(5)).arg(product.at(6)).arg(product.at(7)).arg(product.at(8)));
    //                }
    //                productsMap.insert(name, products);
                }
            }
            mainWidget->unsetCursor();
        }
        catch(...) {
            //bar->hide();
            QMessageBox::about(0, tr("Import"), tr("Error"));
            workbook->dynamicCall("Close()");
            excel->dynamicCall("Quit()");
            productAdded();
            return;
        }
        //bar->hide();
        workbook->dynamicCall("Close()");
        excel->dynamicCall("Quit()");
        productAdded();
        //addProduct(productsMap);
    }
}

void Purchase_Invoicing::configImporting()
{
    if(!importConfigDialog)
    {
        importConfigDialog = new QDialog();

        QStringList items;
        items.append("NULL");
        for(char a='A'; a<='Z'; a++)
        {
            items.append(QString(a));
        }

        QGridLayout *layout = new QGridLayout();

        QLabel *serialNumberLable = new QLabel(tr("serialNumber:"));
        serialNumberComboBox = new QComboBox();
        serialNumberComboBox->addItems(items);
        layout->addWidget(serialNumberLable, 1, 1, 1, 1, Qt::AlignRight);
        layout->addWidget(serialNumberComboBox, 1, 10, 1, 1, Qt::AlignLeft);

        //QMessageBox::critical(NULL, "configImporting", importConfig.value("config/SerialNumber").toString());

        QLabel *brandNameLabel = new QLabel(tr("brandName:"));
        brandNameComboBox = new QComboBox();
        brandNameComboBox->addItems(items);
        layout->addWidget(brandNameLabel, 1, 20, 1, 1, Qt::AlignRight);
        layout->addWidget(brandNameComboBox, 1, 30, 1, 1, Qt::AlignLeft);

        QLabel *productModelLabel = new QLabel(tr("productModel:"));
        productModelComboBox = new QComboBox();
        productModelComboBox->addItems(items);
        layout->addWidget(productModelLabel, 1, 40, 1, 1, Qt::AlignRight);
        layout->addWidget(productModelComboBox, 1, 50, 1, 1, Qt::AlignLeft);

        QLabel *colorLabel = new QLabel(tr("color:"));
        colorComboBox = new QComboBox();
        colorComboBox->addItems(items);
        layout->addWidget(colorLabel, 1, 60, 1, 1, Qt::AlignRight);
        layout->addWidget(colorComboBox, 1, 70, 1, 1, Qt::AlignLeft);

        QLabel *vendorLabel = new QLabel(tr("vendor:"));
        vendorComboBox = new QComboBox();
        vendorComboBox->addItems(items);
        layout->addWidget(vendorLabel, 1, 80, 1, 1, Qt::AlignRight);
        layout->addWidget(vendorComboBox, 1, 90, 1, 1, Qt::AlignLeft);

        QLabel *schemaNameLabel = new QLabel(tr("schemaName:"));
        schemaNameComboBox = new QComboBox();
        schemaNameComboBox->addItems(items);
        layout->addWidget(schemaNameLabel, 10, 1, 1, 1, Qt::AlignRight);
        layout->addWidget(schemaNameComboBox, 10, 10, 1, 1, Qt::AlignLeft);

        QLabel *quantityLabel = new QLabel(tr("quantity:"));
        quantityComboBox = new QComboBox();
        quantityComboBox->addItems(items);
        layout->addWidget(quantityLabel, 10, 20, 1, 1, Qt::AlignRight);
        layout->addWidget(quantityComboBox, 10, 30, 1, 1, Qt::AlignLeft);

        QLabel *unitLabel = new QLabel(tr("unit:"));
        unitComboBox = new QComboBox();
        unitComboBox->addItems(items);
        layout->addWidget(unitLabel, 10, 40, 1, 1, Qt::AlignRight);
        layout->addWidget(unitComboBox, 10, 50, 1, 1, Qt::AlignLeft);

        QLabel *oldPurchasePriceLabel = new QLabel(tr("oldPurchasePrice:"));
        oldPurchasePriceComboBox = new QComboBox();
        oldPurchasePriceComboBox->addItems(items);
        layout->addWidget(oldPurchasePriceLabel, 10, 60, 1, 1, Qt::AlignRight);
        layout->addWidget(oldPurchasePriceComboBox, 10, 70, 1, 1, Qt::AlignLeft);

        QLabel *purchasePriceLabel = new QLabel(tr("purchasePrice:"));
        purchasePriceComboBox = new QComboBox();
        purchasePriceComboBox->addItems(items);
        layout->addWidget(purchasePriceLabel, 10, 80, 1, 1, Qt::AlignRight);
        layout->addWidget(purchasePriceComboBox, 10, 90, 1, 1, Qt::AlignLeft);

        QLabel *sellingPriceLabel = new QLabel(tr("sellingPrice:"));
        sellingPriceComboBox = new QComboBox();
        sellingPriceComboBox->addItems(items);
        layout->addWidget(sellingPriceLabel, 20, 1, 1, 1, Qt::AlignRight);
        layout->addWidget(sellingPriceComboBox, 20, 10, 1, 1, Qt::AlignLeft);

        QLabel *operatorUserLabel = new QLabel(tr("operatorUser:"));
        operatorUserComboBox = new QComboBox();
        operatorUserComboBox->addItems(items);
        layout->addWidget(operatorUserLabel, 20, 20, 1, 1, Qt::AlignRight);
        layout->addWidget(operatorUserComboBox, 20, 30, 1, 1, Qt::AlignLeft);
        operatorUserLabel->setEnabled(false);
        operatorUserComboBox->setEnabled(false);

        QLabel *responserUserLabel = new QLabel(tr("responserUser:"));
        responserUserComboBox = new QComboBox();
        responserUserComboBox->addItems(items);
        layout->addWidget(responserUserLabel, 20, 40, 1, 1, Qt::AlignRight);
        layout->addWidget(responserUserComboBox, 20, 50, 1, 1, Qt::AlignLeft);
        responserUserLabel->setEnabled(false);
        responserUserComboBox->setEnabled(false);

        QLabel *sellerLabel = new QLabel(tr("seller:"));
        sellerComboBox = new QComboBox();
        sellerComboBox->addItems(items);
        layout->addWidget(sellerLabel, 20, 60, 1, 1, Qt::AlignRight);
        layout->addWidget(sellerComboBox, 20, 70, 1, 1, Qt::AlignLeft);
        sellerLabel->setEnabled(false);
        sellerComboBox->setEnabled(false);

        QLabel *barInfoLabel = new QLabel(tr("barInfo:"));
        barInfoComboBox = new QComboBox();
        barInfoComboBox->addItems(items);
        layout->addWidget(barInfoLabel, 20, 80, 1, 1, Qt::AlignRight);
        layout->addWidget(barInfoComboBox, 20, 90, 1, 1, Qt::AlignLeft);

        QLabel *productStatusLabel = new QLabel(tr("productStatus:"));
        productStatusComboBox = new QComboBox();
        productStatusComboBox->addItems(items);
        layout->addWidget(productStatusLabel, 30, 1, 1, 1, Qt::AlignRight);
        layout->addWidget(productStatusComboBox, 30, 10, 1, 1, Qt::AlignLeft);
        productStatusLabel->setEnabled(false);
        productStatusComboBox->setEnabled(false);

        QLabel *replacementStatusLabel = new QLabel(tr("replacementStatus:"));
        replacementStatusComboBox = new QComboBox();
        replacementStatusComboBox->addItems(items);
        layout->addWidget(replacementStatusLabel, 30, 20, 1, 1, Qt::AlignRight);
        layout->addWidget(replacementStatusComboBox, 30, 30, 1, 1, Qt::AlignLeft);

        QLabel *timeStampLabel = new QLabel(tr("timeStamp:"));
        timeStampComboBox = new QComboBox();
        timeStampComboBox->addItems(items);
        layout->addWidget(timeStampLabel, 30, 40, 1, 1, Qt::AlignRight);
        layout->addWidget(timeStampComboBox, 30, 50, 1, 1, Qt::AlignLeft);

        QLabel *commentsLabel = new QLabel(tr("comments:"));
        commentsComboBox = new QComboBox();
        commentsComboBox->addItems(items);
        layout->addWidget(commentsLabel, 30, 60, 1, 1, Qt::AlignRight);
        layout->addWidget(commentsComboBox, 30, 70, 1, 1, Qt::AlignLeft);

        QLabel *skip1stRowLabel = new QLabel(tr("skip1stRow:"));
        skip1stRowCheckBox = new QCheckBox();
        layout->addWidget(skip1stRowLabel, 30, 80, 1, 1, Qt::AlignRight);
        layout->addWidget(skip1stRowCheckBox, 30, 90, 1, 1, Qt::AlignLeft);

        QLabel *title = new QLabel(tr("config the colunm position of product info"));

        QDialogButtonBox *buttons = new QDialogButtonBox((QDialogButtonBox::Ok | QDialogButtonBox::Cancel),
                                                         Qt::Horizontal,
                                                         importConfigDialog);
        connect(buttons, SIGNAL(accepted()), importConfigDialog, SLOT(accept()));
        connect(buttons, SIGNAL(rejected()), importConfigDialog, SLOT(reject()));

        QVBoxLayout *vlayout = new QVBoxLayout();
        vlayout->addWidget(title, 0,  Qt::AlignCenter);
        vlayout->addLayout(layout);
        vlayout->addWidget(buttons, 0,  Qt::AlignCenter);

        importConfigDialog->setLayout(vlayout);
    }

    QSettings importConfig(qApp->applicationDirPath()+"/importconfig.ini", QSettings::IniFormat);
    serialNumberComboBox->setCurrentIndex(importConfig.value("config/SerialNumber").toInt());
    brandNameComboBox->setCurrentIndex(importConfig.value("config/BrandName").toInt());
    productModelComboBox->setCurrentIndex(importConfig.value("config/ProductModel").toInt());
    colorComboBox->setCurrentIndex(importConfig.value("config/Color").toInt());
    vendorComboBox->setCurrentIndex(importConfig.value("config/Vendor").toInt());
    schemaNameComboBox->setCurrentIndex(importConfig.value("config/SchemaName").toInt());
    quantityComboBox->setCurrentIndex(importConfig.value("config/Quantity").toInt());
    unitComboBox->setCurrentIndex(importConfig.value("config/Unit").toInt());
    oldPurchasePriceComboBox->setCurrentIndex(importConfig.value("config/OldPurchasePrice").toInt());
    purchasePriceComboBox->setCurrentIndex(importConfig.value("config/PurchasePrice").toInt());
    sellingPriceComboBox->setCurrentIndex(importConfig.value("config/SellingPrice").toInt());
    operatorUserComboBox->setCurrentIndex(importConfig.value("config/OperatorUser").toInt());
    responserUserComboBox->setCurrentIndex(importConfig.value("config/ResponserUser").toInt());
    sellerComboBox->setCurrentIndex(importConfig.value("config/Seller").toInt());
    barInfoComboBox->setCurrentIndex(importConfig.value("config/BarInfo").toInt());
    productStatusComboBox->setCurrentIndex(importConfig.value("config/ProductStatus").toInt());
    replacementStatusComboBox->setCurrentIndex(importConfig.value("config/ReplacementStatus").toInt());
    timeStampComboBox->setCurrentIndex(importConfig.value("config/TimeStamp").toInt());
    commentsComboBox->setCurrentIndex(importConfig.value("config/Comments").toInt());
    skip1stRowCheckBox->setChecked(importConfig.value("config/Skip1stRow").toBool());

    if(importConfigDialog->exec()==QDialog::Accepted)
    {
        importConfig.setValue("config/SerialNumber", serialNumberComboBox->currentIndex());
        importConfig.setValue("config/BrandName", brandNameComboBox->currentIndex());
        importConfig.setValue("config/ProductModel", productModelComboBox->currentIndex());
        importConfig.setValue("config/Color", colorComboBox->currentIndex());
        importConfig.setValue("config/Vendor", vendorComboBox->currentIndex());
        importConfig.setValue("config/SchemaName", schemaNameComboBox->currentIndex());
        importConfig.setValue("config/Quantity", quantityComboBox->currentIndex());
        importConfig.setValue("config/Unit", unitComboBox->currentIndex());
        importConfig.setValue("config/OldPurchasePrice", oldPurchasePriceComboBox->currentIndex());
        importConfig.setValue("config/PurchasePrice", purchasePriceComboBox->currentIndex());
        importConfig.setValue("config/SellingPrice", sellingPriceComboBox->currentIndex());
        importConfig.setValue("config/OperatorUser", operatorUserComboBox->currentIndex());
        importConfig.setValue("config/ResponserUser", responserUserComboBox->currentIndex());
        importConfig.setValue("config/Seller", sellerComboBox->currentIndex());
        importConfig.setValue("config/BarInfo", barInfoComboBox->currentIndex());
        importConfig.setValue("config/ProductStatus", productStatusComboBox->currentIndex());
        importConfig.setValue("config/ReplacementStatus", replacementStatusComboBox->currentIndex());
        importConfig.setValue("config/TimeStamp", timeStampComboBox->currentIndex());
        importConfig.setValue("config/Comments", commentsComboBox->currentIndex());
        importConfig.setValue("config/Skip1stRow", skip1stRowCheckBox->isChecked());
    }

    //QMessageBox::critical(NULL, "configImporting", "Hasn't been implemented!");
    return;

}

void Purchase_Invoicing::commitProduct()
{
    QModelIndex purchaseIndex = purchaseView->currentIndex();
    QSqlRecord record = purchaseModel->record(purchaseIndex.row());
    int productID = record.value(ProductID).toInt();
    int statusID = productManagementInterface->getStatusIDByStatusName("待入库");
    productManagementInterface->updateStatusIDByProductID(productID, statusID);
    purchaseModel->select();
    purchaseView->resizeColumnsToContents();
    if(purchaseIndex.row()<purchaseModel->rowCount()) {
        purchaseView->selectRow(purchaseIndex.row());
    }
    else {
        purchaseView->selectRow(purchaseModel->rowCount()-1);
    }
}

void Purchase_Invoicing::commitAllProducts()
{
    /*
    bar->setRange(0, purchaseModel->rowCount());
    bar->setValue(0);
    int i = 0;
    while(purchaseModel->rowCount()>0) {
        QSqlRecord record = purchaseModel->record(0);
        int productID = record.value(ProductID).toInt();
        int statusID = productManagementInterface->getStatusIDByStatusName("待入库");
        productManagementInterface->updateStatusIDByProductID(productID, statusID);
        bar->setValue(++i);
    }
    purchaseModel->select();
    */
    int statusID = productManagementInterface->getStatusIDByStatusName("录入");
    int userID = userManagementInterface->getUserIDByUserName(
                userManagementInterface->getCurrentUserName());
    int deleteID = productManagementInterface->getStatusIDByStatusName("待入库");
    QSet<int> productIDSet = productManagementInterface->getProductIDSetByUserIDStatusID(userID, statusID);
    bar->setRange(0, productIDSet.count());
    bar->setValue(0);
    int i = 0;
    mainWidget->setCursor(Qt::BusyCursor);
    foreach(int id, productIDSet) {
        productManagementInterface->updateStatusIDByProductID(id, deleteID);
        bar->setValue(++i);
        //bar->update();
        //qApp->processEvents();
    }
    mainWidget->unsetCursor();
    productAdded();
}

void Purchase_Invoicing::updateDBTableModel()
{
    if(productManagementInterface->isModelOutdate(purchaseModel, timeStamp)) {
        QModelIndex index = purchaseView->currentIndex();
        static QModelIndex outViewPortindex;
        purchaseModel->select();
        purchaseView->resizeColumnsToContents();
        if(index.isValid()) {
            int rowPosition = purchaseView->rowViewportPosition(index.row());
            if(rowPosition>=0 && rowPosition<purchaseView->height()) {
                purchaseView->setCurrentIndex(index);
            }
            else {
                outViewPortindex = index;
            }
        }
        else if(outViewPortindex.isValid()) {
            int rowPosition = purchaseView->rowViewportPosition(outViewPortindex.row());
            if(rowPosition>=0 && rowPosition<purchaseView->height()) {
                purchaseView->setCurrentIndex(outViewPortindex);
            }
        }
        purchaseView->resizeColumnsToContents();
    }
}

void Purchase_Invoicing::onFilter()
{
    static QString oldPurchaseFilter = purchaseModel->filter();

    QString serialNumber = serialNumberLineEdit->text().simplified();
    if(!serialNumber.isEmpty()) {
        QString newPurchageFilter = oldPurchaseFilter + QString(" and serialNumber like '%%1%'").arg(serialNumber);
        purchaseModel->setFilter(newPurchageFilter);
    }
    else {
        purchaseModel->setFilter(oldPurchaseFilter);
    }
    purchaseModel->select();
}

void Purchase_Invoicing::updatePurchaseFilter()
{
    int statusID = productManagementInterface->getStatusIDByStatusName("录入");
    int userID = userManagementInterface->getUserIDByUserName(
                userManagementInterface->getCurrentUserName());
    QSet<int> schemaSet = userManagementInterface->getSchemaIDSetByUserID(userID);
    QString filter = QString("productStatusID = %1 and operatorUserID = %2")
            .arg(statusID).arg(userID);

    if(!schemaSet.isEmpty()) {
        QString schemaFilter("");
        foreach(int schemaID, schemaSet) {
            if(schemaFilter.isEmpty()) {
                schemaFilter += QString(" and (schemaNameID = %1 ").arg(schemaID);
            }
            else {
                schemaFilter += QString(" or schemaNameID = %1 ").arg(schemaID);
            }
        }
        schemaFilter += ")";
        filter += schemaFilter;
    }

    purchaseModel->setFilter(filter);
}

void Purchase_Invoicing::updateProductinfo()
{
    if(!updateProductDialog) {
        updateProductDialog = new UpdateProductDialog(userManagementInterface,
                                                      productManagementInterface);
        connect(updateProductDialog, SIGNAL(productUpdated()), this, SLOT(productAdded()));
    }
    updateProductDialog->updateDBTableModel();
    QModelIndex storageIndex = purchaseView->currentIndex();
    QSqlRecord record = purchaseModel->record(storageIndex.row());
    updateProductDialog->updateRecord(record);
    updateProductDialog->exec();
}

void Purchase_Invoicing::initImportConfigFile()
{
    QFile file(qApp->applicationDirPath()+"/importconfig.ini");
    if(!file.exists())
    {
        QSettings importConfig(qApp->applicationDirPath()+"/importconfig.ini", QSettings::IniFormat);
        importConfig.setValue("config/SerialNumber", 5);
        importConfig.setValue("config/BrandName", 3 );
        importConfig.setValue("config/ProductModel", 4);
        importConfig.setValue("config/Color", 6);
        importConfig.setValue("config/Vendor", 2);
        importConfig.setValue("config/SchemaName", 0);
        importConfig.setValue("config/Quantity", 8);
        importConfig.setValue("config/Unit", 0);
        importConfig.setValue("config/OldPurchasePrice", 0);
        importConfig.setValue("config/PurchasePrice", 0);
        importConfig.setValue("config/SellingPrice", 0);
        importConfig.setValue("config/OperatorUser", 0);
        importConfig.setValue("config/ResponserUser", 0);
        importConfig.setValue("config/Seller", 0);
        importConfig.setValue("config/BarInfo", 7);
        importConfig.setValue("config/ProductStatus", 0);
        importConfig.setValue("config/ReplacementStatus", 0);
        importConfig.setValue("config/TimeStamp", 1);
        importConfig.setValue("config/Comments", 9);
        importConfig.setValue("config/Skip1stRow", true);
    }
}

QT_BEGIN_NAMESPACE
#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(Purchase_Invoicing, Purchase_Invoicing)
#endif
QT_END_NAMESPACE
