#include <QtGui>
#include <QtSql>
#include "storage_invoicing.h"
#include "mainwindow.h"
#include "usermanagement_interface.h"
#include "productmanagement_interface.h"
#include "storageupdateproductdialog.h"

Storage_Invoicing::Storage_Invoicing():
    parentWindow(NULL),
    userManagementInterface(NULL),
    productManagementInterface(NULL),
    mainWidget(NULL),
    purchasePanel(NULL),
    purchaseModel(NULL),
    purchaseView(NULL),
    inStoragePushButton(NULL),
    inAllStoragePushButton(NULL),
	returnToVendorPushButton(NULL),
	deleteProductPushButton(NULL),
    storagePanel(NULL),
    storageModel(NULL),
    storageView(NULL),
    outStoragePushButton(NULL),
    outAllStoragePushButton(NULL),
    storageUpdateProductDialog(NULL),
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

QWidget* Storage_Invoicing::getMainWidget() const
{
    return mainWidget;
}

QAction* Storage_Invoicing::getAction() const
{
    return NULL;
}

QMenu* Storage_Invoicing::getMenu() const
{
    return NULL;
}

QToolBar* Storage_Invoicing::getToolBar() const
{
    return NULL;
}

bool Storage_Invoicing::init(MainWindow *parent)
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

bool Storage_Invoicing::deInit()
{
    if(userManagementInterface) {
        userManagementInterface->deregisteObserver(this);
    }
    return true;
}


void Storage_Invoicing::userChanged()
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
    updateStorageFilter();
    updateDBTableModel();
}

void Storage_Invoicing::hidePurchasePrice()
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
    storageView->setColumnHidden(OldPurchasePrice, hide);
    storageView->setColumnHidden(PurchasePrice, hide);
    returnToVendorPushButton->setEnabled(!hide);
    deleteProductPushButton->setEnabled(!hide);

    if(!storageUpdateProductDialog) {
        storageUpdateProductDialog = new StorageUpdateProductDialog(userManagementInterface,
                                                      productManagementInterface);
        connect(storageUpdateProductDialog, SIGNAL(productUpdated()), this, SLOT(productUpdated()));
    }
    storageUpdateProductDialog->hidePurchasePrice(hide);
}

QString Storage_Invoicing::moduleName() const
{
    return this->metaObject()->className();
}

QString Storage_Invoicing::moduleDescription() const
{
    return tr("Storage Function");
}

QSet<QString> Storage_Invoicing::getAccessRoleNameSet() const
{
    return QSet<QString>()<<"管理员"<<"库管";
}

QSet<QString> Storage_Invoicing::getDependencySet() const
{
    return QSet<QString>()<<"UserManagementIF"<<"ProductManagementIF";
}

void Storage_Invoicing::initMainWidget()
{
    mainWidget = new QWidget();
    mainWidget->setWindowIcon(QIcon(":/Icon/storage_icon.png"));
    mainWidget->setWindowIconText(tr("Storage"));

    createPurchasePanel();
    createStoragePanel();
    inStoragePushButton = new QPushButton();
    connect(inStoragePushButton, SIGNAL(clicked()), this, SLOT(inStorage()));
    inStoragePushButton->setToolTip(tr("In Storage"));
    inStoragePushButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    inStoragePushButton->setIcon(QIcon(":/Icon/down_icon.png"));
    inStoragePushButton->setIconSize(inStoragePushButton->sizeHint());
    inAllStoragePushButton = new QPushButton();
    connect(inAllStoragePushButton, SIGNAL(clicked()), this, SLOT(allInStorage()));
    inAllStoragePushButton->setToolTip(tr("All In Storage"));
    inAllStoragePushButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    inAllStoragePushButton->setIcon(QIcon(":/Icon/downall_icon.png"));
    inAllStoragePushButton->setIconSize(inAllStoragePushButton->sizeHint());
	returnToVendorPushButton = new QPushButton();
    connect(returnToVendorPushButton, SIGNAL(clicked()), this, SLOT(returnToVendor()));
    returnToVendorPushButton->setToolTip(tr("Return to Vendor"));
    returnToVendorPushButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    returnToVendorPushButton->setIcon(QIcon(":/Icon/return_icon.png"));
    returnToVendorPushButton->setIconSize(returnToVendorPushButton->sizeHint());
    deleteProductPushButton = new QPushButton();
    connect(deleteProductPushButton, SIGNAL(clicked()), this, SLOT(deleteProduct()));
    deleteProductPushButton->setToolTip(tr("Delete The Product"));
    deleteProductPushButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    deleteProductPushButton->setIcon(QIcon(":/Icon/deleteproduct_icon.png"));
    deleteProductPushButton->setIconSize(deleteProductPushButton->sizeHint());

    outStoragePushButton = new QPushButton();
    connect(outStoragePushButton, SIGNAL(clicked()), this, SLOT(outStorage()));
    outStoragePushButton->setToolTip(tr("Out Storage"));
    outStoragePushButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    outStoragePushButton->setIcon(QIcon(":/Icon/up_icon.png"));
    outStoragePushButton->setIconSize(outStoragePushButton->sizeHint());
    outAllStoragePushButton = new QPushButton();
    connect(outAllStoragePushButton, SIGNAL(clicked()), this, SLOT(allOutStorage()));
    outAllStoragePushButton->setToolTip(tr("All Out Storage"));
    outAllStoragePushButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    outAllStoragePushButton->setIcon(QIcon(":/Icon/upall_icon.png"));
    outAllStoragePushButton->setIconSize(outAllStoragePushButton->sizeHint());

    QLabel *purchaseLabel = new QLabel(tr("Waiting For Storage :"));
    QLabel *storageLabel = new QLabel(tr("Storaged :"));
    QVBoxLayout *purchaseVLayout = new QVBoxLayout();
    purchaseVLayout->addWidget(purchaseLabel);
    purchaseVLayout->addWidget(purchasePanel, 10);
    QVBoxLayout *storageVLayout = new QVBoxLayout();
    storageVLayout->addWidget(storageLabel);
    storageVLayout->addWidget(storagePanel, 10);
    QHBoxLayout *purchaseHLayout = new QHBoxLayout();
    purchaseHLayout->addLayout(purchaseVLayout, 10);
    QVBoxLayout *inStorageVLayout = new QVBoxLayout();
    inStorageVLayout->addWidget(inStoragePushButton);
    inStorageVLayout->addWidget(inAllStoragePushButton);
    inStorageVLayout->addWidget(returnToVendorPushButton);
    inStorageVLayout->addWidget(deleteProductPushButton);
    purchaseHLayout->addLayout(inStorageVLayout);


    QHBoxLayout *storageHLayout = new QHBoxLayout();
    storageHLayout->addLayout(storageVLayout, 10);
    QVBoxLayout *outStorageVLayout = new QVBoxLayout();
    outStorageVLayout->addWidget(outStoragePushButton);
    outStorageVLayout->addWidget(outAllStoragePushButton);
    storageHLayout->addLayout(outStorageVLayout);


    QWidget *purchageWidget = new QWidget();
    purchageWidget->setLayout(purchaseHLayout);
    QWidget *storageWidget = new QWidget();
    storageWidget->setLayout(storageHLayout);

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


    QSplitter *splitter = new QSplitter();
    splitter->setOrientation(Qt::Vertical);
    splitter->addWidget(filterWidget);
    splitter->addWidget(purchageWidget);
    splitter->addWidget(storageWidget);
    bar = new QProgressBar();
    bar->setFormat("%v/%m");
    splitter->addWidget(bar);
    splitter->setCollapsible(splitter->indexOf(filterWidget), false);
    splitter->setCollapsible(splitter->indexOf(purchageWidget), true);
    splitter->setCollapsible(splitter->indexOf(purchageWidget), true);
    splitter->setCollapsible(splitter->indexOf(bar), false);

    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addWidget(splitter);
    mainWidget->setLayout(mainLayout);
}


void Storage_Invoicing::createPurchasePanel()
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

void Storage_Invoicing::createStoragePanel()
{
    storagePanel = new QWidget();
    QSqlDatabase db(userManagementInterface->getDatabase());
    storageModel = new QSqlRelationalTableModel(this, db);
    storageModel->setTable("products");
    storageModel->setRelation(ProductTypeID, QSqlRelation("producttype", "id", "name"));
    storageModel->setRelation(BrandNameID, QSqlRelation("brandname", "id", "name"));
    storageModel->setRelation(ProductModelID, QSqlRelation("productmodel", "id", "model"));
    storageModel->setRelation(ColorID, QSqlRelation("colorinfo", "id", "color"));
    storageModel->setRelation(VendorID, QSqlRelation("vendorinfo", "id", "name"));
    storageModel->setRelation(SchemaNameID, QSqlRelation("schemaname", "id", "name"));
    storageModel->setRelation(OperatorUserID, QSqlRelation("user", "id", "name"));
    storageModel->setRelation(ResponserUserID, QSqlRelation("user", "id", "name"));
    storageModel->setRelation(SellerID, QSqlRelation("user", "id", "name"));
    storageModel->setRelation(ProductStatusID, QSqlRelation("productstatus", "id", "status"));
    storageModel->setRelation(ReplacementStatusID, QSqlRelation("replacementstatus", "id", "status"));
    storageModel->setSort(TimeStamp, Qt::AscendingOrder);

    storageModel->setHeaderData(ProductID, Qt::Horizontal, tr("ID"));
    storageModel->setHeaderData(SerialNumber, Qt::Horizontal, tr("Serial Number"));
    storageModel->setHeaderData(ProductTypeID, Qt::Horizontal, tr("Product Type"));
    storageModel->setHeaderData(BrandNameID, Qt::Horizontal, tr("Brand Name"));
    storageModel->setHeaderData(ProductModelID, Qt::Horizontal, tr("Model Name"));
    storageModel->setHeaderData(ColorID, Qt::Horizontal, tr("Color"));
    storageModel->setHeaderData(VendorID, Qt::Horizontal, tr("Vendor"));
    storageModel->setHeaderData(SchemaNameID, Qt::Horizontal, tr("Schema Name"));
    storageModel->setHeaderData(Quantity, Qt::Horizontal, tr("Quantity"));
    storageModel->setHeaderData(Unit, Qt::Horizontal, tr("Unit"));
    storageModel->setHeaderData(OldPurchasePrice, Qt::Horizontal, tr("Old Purchase Price"));
    storageModel->setHeaderData(PurchasePrice, Qt::Horizontal, tr("Purchase Price"));
    storageModel->setHeaderData(SellingPrice, Qt::Horizontal, tr("Selling Price"));
    storageModel->setHeaderData(OperatorUserID, Qt::Horizontal, tr("Operator"));
    storageModel->setHeaderData(ResponserUserID, Qt::Horizontal, tr("Responser"));
    storageModel->setHeaderData(SellerID, Qt::Horizontal, tr("Seller"));
    storageModel->setHeaderData(BarInfo, Qt::Horizontal, tr("BarInfo"));
    storageModel->setHeaderData(ProductStatusID, Qt::Horizontal, tr("Product Status"));
    storageModel->setHeaderData(ReplacementStatusID, Qt::Horizontal, tr("Replacement Status"));
    storageModel->setHeaderData(TimeStamp, Qt::Horizontal, tr("TimeStamp"));
    storageModel->setHeaderData(Comments, Qt::Horizontal, tr("Comments"));


    updateStorageFilter();
    storageModel->select();

    storageView = new QTableView;
    storageView->setModel(storageModel);
    storageView->setItemDelegate(new QSqlRelationalDelegate(this));
    storageView->setSelectionMode(QAbstractItemView::SingleSelection);
    storageView->setSelectionBehavior(QAbstractItemView::SelectRows);
    storageView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    storageView->resizeColumnsToContents();
    storageView->horizontalHeader()->setStretchLastSection(true);
    storageView->setColumnHidden(ProductID, true);
    storageView->horizontalHeader()->setVisible(true);
    connect(storageView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(updateProductinfo()));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(storageView);
    storagePanel->setLayout(layout);
}

void Storage_Invoicing::returnToVendor()
{
    QModelIndex purchaseIndex = purchaseView->currentIndex();
    if(purchaseIndex.isValid()) {
        QSqlRecord record = purchaseModel->record(purchaseIndex.row());
        int productID = record.value(ProductID).toInt();
        int statusID = productManagementInterface->getStatusIDByStatusName("已退货");
        productManagementInterface->updateStatusIDByProductID(productID, statusID);
        purchaseModel->select();
        storageModel->select();

        if(purchaseIndex.row()<purchaseModel->rowCount()) {
            purchaseView->selectRow(purchaseIndex.row());
        }
        else {
            purchaseView->selectRow(purchaseModel->rowCount()-1);
        }
        //purchaseView->resizeColumnsToContents();
        //purchaseView->resizeRowsToContents();
        storageView->resizeColumnsToContents();
        //storageView->resizeRowsToContents();
        purchaseView->setFocus();
    }
}

void Storage_Invoicing::deleteProduct()
{
    QModelIndex purchaseIndex = purchaseView->currentIndex();
    if(purchaseIndex.isValid()) {
        QSqlRecord record = purchaseModel->record(purchaseIndex.row());
        int productID = record.value(ProductID).toInt();
        int statusID = productManagementInterface->getStatusIDByStatusName("已删除");
        productManagementInterface->updateStatusIDByProductID(productID, statusID);
        purchaseModel->select();
        storageModel->select();

        if(purchaseIndex.row()<purchaseModel->rowCount()) {
            purchaseView->selectRow(purchaseIndex.row());
        }
        else {
            purchaseView->selectRow(purchaseModel->rowCount()-1);
        }
        //purchaseView->resizeColumnsToContents();
        //purchaseView->resizeRowsToContents();
        storageView->resizeColumnsToContents();
        //storageView->resizeRowsToContents();
        purchaseView->setFocus();
    }
}

void Storage_Invoicing::inStorage()
{
    QModelIndex purchaseIndex = purchaseView->currentIndex();
    if(purchaseIndex.isValid()) {
        QSqlRecord record = purchaseModel->record(purchaseIndex.row());
        int productID = record.value(ProductID).toInt();
        int statusID = productManagementInterface->getStatusIDByStatusName("已入库");
        productManagementInterface->updateStatusIDByProductID(productID, statusID);
        purchaseModel->select();
        storageModel->select();

        if(purchaseIndex.row()<purchaseModel->rowCount()) {
            purchaseView->selectRow(purchaseIndex.row());
        }
        else {
            purchaseView->selectRow(purchaseModel->rowCount()-1);
        }
        //purchaseView->resizeColumnsToContents();
        //purchaseView->resizeRowsToContents();
        storageView->resizeColumnsToContents();
        //storageView->resizeRowsToContents();
        purchaseView->setFocus();
    }
}

void Storage_Invoicing::allInStorage()
{
    int statusID = productManagementInterface->getStatusIDByStatusName("待入库");
    int userID = userManagementInterface->getUserIDByUserName(
                userManagementInterface->getCurrentUserName());
    int deleteID = productManagementInterface->getStatusIDByStatusName("已入库");
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
    productUpdated();
}

void Storage_Invoicing::outStorage()
{
    QModelIndex storageIndex = storageView->currentIndex();
    if(storageIndex.isValid()) {
        QSqlRecord record = storageModel->record(storageIndex.row());
        int productID = record.value(ProductID).toInt();
        int statusID = productManagementInterface->getStatusIDByStatusName("待入库");
        productManagementInterface->updateStatusIDByProductID(productID, statusID);
        purchaseModel->select();
        storageModel->select();

        if(storageIndex.row()<storageModel->rowCount()) {
            storageView->selectRow(storageIndex.row());
        }
        else {
            storageView->selectRow(storageModel->rowCount()-1);
        }
        purchaseView->resizeColumnsToContents();
        //purchaseView->resizeRowsToContents();
        //storageView->resizeColumnsToContents();
        //storageView->resizeRowsToContents();
        storageView->setFocus();
    }
}

void Storage_Invoicing::allOutStorage()
{
    int statusID = productManagementInterface->getStatusIDByStatusName("已入库");
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
    productUpdated();
}

void Storage_Invoicing::updateProductinfo()
{
    if(!storageUpdateProductDialog) {
        storageUpdateProductDialog = new StorageUpdateProductDialog(userManagementInterface,
                                                      productManagementInterface);
        connect(storageUpdateProductDialog, SIGNAL(productUpdated()), this, SLOT(productUpdated()));
    }
    storageUpdateProductDialog->updateDBTableModel();
    if(QObject::sender()==purchaseView) {
        QModelIndex storageIndex = purchaseView->currentIndex();
        QSqlRecord record = purchaseModel->record(storageIndex.row());
        storageUpdateProductDialog->updateRecord(record);
        storageUpdateProductDialog->serialNumberEditable(
                    userManagementInterface->isAdmin(userManagementInterface->getCurrentUserName()));
        storageUpdateProductDialog->exec();
        storageUpdateProductDialog->serialNumberEditable(false);
    }
    else {
        QModelIndex storageIndex = storageView->currentIndex();
        QSqlRecord record = storageModel->record(storageIndex.row());
        storageUpdateProductDialog->updateRecord(record);
        storageUpdateProductDialog->exec();
    }
}

void Storage_Invoicing::productUpdated()
{
    purchaseModel->select();
    purchaseView->resizeColumnsToContents();
    storageModel->select();
    storageView->resizeColumnsToContents();
}

void Storage_Invoicing::updateDBTableModel()
{
    qDebug()<<"Storage_Invoicing::updateDBTableModel()";
    if(productManagementInterface->isModelOutdate(purchaseModel, purchageTimeStamp)) {
        QModelIndex purchaseIndex = purchaseView->currentIndex();
        static QModelIndex purchaseOutViewPortindex;
        purchaseModel->select();
        purchaseView->resizeColumnsToContents();
        if(purchaseIndex.isValid()) {
            int rowPosition = purchaseView->rowViewportPosition(purchaseIndex.row());
            if(rowPosition>=0 && rowPosition<purchaseView->height()) {
                purchaseView->setCurrentIndex(purchaseIndex);
            }
            else {
                purchaseOutViewPortindex = purchaseIndex;
            }
        }
        else if(purchaseOutViewPortindex.isValid()) {
            int rowPosition = purchaseView->rowViewportPosition(purchaseOutViewPortindex.row());
            if(rowPosition>=0 && rowPosition<purchaseView->height()) {
                purchaseView->setCurrentIndex(purchaseOutViewPortindex);
            }
        }
        purchaseView->resizeColumnsToContents();
    }

    if(productManagementInterface->isModelOutdate(storageModel, timeStamp)) {
        QModelIndex storageIndex = storageView->currentIndex();
        static QModelIndex storageOutViewPortindex;
        storageModel->select();
        storageView->resizeColumnsToContents();
        if(storageIndex.isValid()) {
            int rowPosition = storageView->rowViewportPosition(storageIndex.row());
            if(rowPosition>=0 && rowPosition<storageView->height()) {
                storageView->setCurrentIndex(storageIndex);
            }
            else {
                storageOutViewPortindex = storageIndex;
            }
        }
        else if(storageOutViewPortindex.isValid()) {
            int rowPosition = storageView->rowViewportPosition(storageOutViewPortindex.row());
            if(rowPosition>=0 && rowPosition<storageView->height()) {
                storageView->setCurrentIndex(storageOutViewPortindex);
            }
        }
        storageView->resizeColumnsToContents();
    }
}

void Storage_Invoicing::onFilter()
{
    static QString oldPurchaseFilter = purchaseModel->filter();
    static QString oldStorageFilter = storageModel->filter();

    QString serialNumber = serialNumberLineEdit->text().simplified();
    if(!serialNumber.isEmpty()) {
        QString newPurchageFilter = oldPurchaseFilter + QString(" and serialNumber like '%%1%'").arg(serialNumber);
        purchaseModel->setFilter(newPurchageFilter);
        QString newStorageFilter = oldStorageFilter + QString(" and serialNumber like '%%1%'").arg(serialNumber);
        storageModel->setFilter(newStorageFilter);
    }
    else {
        purchaseModel->setFilter(oldPurchaseFilter);
        storageModel->setFilter(oldStorageFilter);
    }
    purchaseModel->select();
    storageModel->select();
}

void Storage_Invoicing::updatePurchaseFilter()
{
    int statusID = productManagementInterface->getStatusIDByStatusName("待入库");
    int userID = userManagementInterface->getUserIDByUserName(
                userManagementInterface->getCurrentUserName());
    QSet<int> schemaSet = userManagementInterface->getSchemaIDSetByUserID(userID);
    QString filter = QString("productStatusID = %1").arg(statusID);

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

void Storage_Invoicing::updateStorageFilter()
{
    int statusID = productManagementInterface->getStatusIDByStatusName("已入库");
    int userID = userManagementInterface->getUserIDByUserName(
                userManagementInterface->getCurrentUserName());
    QSet<int> schemaSet = userManagementInterface->getSchemaIDSetByUserID(userID);
    QString filter = QString("productStatusID = %1").arg(statusID);

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

    storageModel->setFilter(filter);
}

QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(Storage_Invoicing, Storage_Invoicing)
QT_END_NAMESPACE
