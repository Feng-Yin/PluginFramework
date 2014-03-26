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
    filterPushButton(NULL)
{
    QDir qmdir(":/Translations");
    foreach (QString fileName, qmdir.entryList(QDir::Files)) {
        //qDebug()<<QFileInfo(fileName).baseName();
        QTranslator *qtTranslator = new QTranslator(this);
        qtTranslator->load(QFileInfo(fileName).baseName(), ":/Translations");
        QApplication::instance()->installTranslator(qtTranslator);
    }
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
    connect(importProductsButton, SIGNAL(clicked()), this, SLOT(importProducts()));

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
            for(int i=1; i<count+1; i++) {
                QAxObject* sheet = sheets->querySubObject( "Item( int )", i );
                //////////////////////////////////////////////////////////////////
                QAxObject* usedrange = sheet->querySubObject("UsedRange");
                QAxObject* rows = usedrange->querySubObject("Rows");
                int intRows = rows->property("Count").toInt();
                totalRows += (intRows - 1);
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
                    for(int row=1; ; row++) {
                        QStringList data;
                        data.append(name);
                        for(int col=1; col<9; col++) {
                            QAxObject* cell = sheet->querySubObject( "Cells( int, int )", row+1, col );
                            QVariant value = cell->dynamicCall( "Value()" );
                            if(value.toString().isEmpty() && col==1 && row>=intRows) {
                                done = true;
                                break;
                            } else {
                                data.append(value.toString());
                            }
                        }
                        if(done) {
                            break;
                        } else {
                            //products.append(data);
                            int ret = addProduct(data);
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

QT_BEGIN_NAMESPACE
#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(Purchase_Invoicing, Purchase_Invoicing)
#endif
QT_END_NAMESPACE
