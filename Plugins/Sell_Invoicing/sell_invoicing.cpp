#include <QtGui>
#include <QtSql>
#include "sell_invoicing.h"
#include "mainwindow.h"
#include "usermanagement_interface.h"
#include "productmanagement_interface.h"
#include "updateproductdialog.h"

Sell_Invoicing::Sell_Invoicing():
    parentWindow(NULL),
    userManagementInterface(NULL),
    productManagementInterface(NULL),
    mainWidget(NULL),
    storagePanel(NULL),
    storageModel(NULL),
    storageView(NULL),
    submitOrderPushButton(NULL),
    orderPanel(NULL),
    orderModel(NULL),
    orderView(NULL),
    backToStoragePushButton(NULL),
    updateStorageProductDialog(NULL),
    updateOrderProductDialog(NULL),
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

QWidget* Sell_Invoicing::getMainWidget() const
{
    return mainWidget;
}

QAction* Sell_Invoicing::getAction() const
{
    return NULL;
}

QMenu* Sell_Invoicing::getMenu() const
{
    return NULL;
}

QToolBar* Sell_Invoicing::getToolBar() const
{
    return NULL;
}

bool Sell_Invoicing::init(MainWindow *parent)
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

bool Sell_Invoicing::deInit()
{
    if(userManagementInterface) {
        userManagementInterface->deregisteObserver(this);
    }
    return true;
}


void Sell_Invoicing::userChanged()
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
    updateStorageFilter();
    updateOrderFilter();
    updateDBTableModel();
}

void Sell_Invoicing::hidePurchasePrice()
{
    bool hide = true;
    QSet<int> roleset = userManagementInterface->getRoleIDSetByUserID(
                userManagementInterface->getUserIDByUserName(
                    userManagementInterface->getCurrentUserName()));
    int adminRoleID = userManagementInterface->getRoleIDByRoleName("管理员");
    int storagerRoleID = userManagementInterface->getRoleIDByRoleName("库管");
    foreach(int i, roleset) {
        if(i == adminRoleID || i == storagerRoleID) {
            hide = false;
        }
    }
    orderView->setColumnHidden(OldPurchasePrice, hide);
    orderView->setColumnHidden(PurchasePrice, hide);
    storageView->setColumnHidden(OldPurchasePrice, hide);
    storageView->setColumnHidden(PurchasePrice, hide);

    if(!updateOrderProductDialog) {
        updateOrderProductDialog = new UpdateProductDialog(userManagementInterface,
                                                      productManagementInterface);
    }
    updateOrderProductDialog->hidePurchasePrice(hide);
    if(!updateStorageProductDialog) {
        updateStorageProductDialog = new UpdateProductDialog(userManagementInterface,
                                                      productManagementInterface);
        connect(updateStorageProductDialog, SIGNAL(productUpdated()), this, SLOT(productUpdated()));
    }
    updateStorageProductDialog->hidePurchasePrice(hide);
}

QString Sell_Invoicing::moduleName() const
{
    return this->metaObject()->className();
}

QString Sell_Invoicing::moduleDescription() const
{
    return tr("Sell Function");
}

QSet<QString> Sell_Invoicing::getAccessRoleNameSet() const
{
    return QSet<QString>()<<"管理员"<<"销售";
}

QSet<QString> Sell_Invoicing::getDependencySet() const
{
    return QSet<QString>()<<"UserManagementIF"<<"ProductManagementIF";
}

void Sell_Invoicing::initMainWidget()
{
    mainWidget = new QWidget();
    mainWidget->setWindowIcon(QIcon(":/Icon/sell_icon.png"));
    mainWidget->setWindowIconText(tr("Sell"));

    createOrderPanel();
    createStoragePanel();
    submitOrderPushButton = new QPushButton();
    connect(submitOrderPushButton, SIGNAL(clicked()), this, SLOT(submitOrder()));
    submitOrderPushButton->setToolTip(tr("Submit Order"));
    submitOrderPushButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    submitOrderPushButton->setIcon(QIcon(":/Icon/down_icon.png"));
    submitOrderPushButton->setIconSize(submitOrderPushButton->sizeHint());

    backToStoragePushButton = new QPushButton();
    connect(backToStoragePushButton, SIGNAL(clicked()), this, SLOT(backToStorage()));
    backToStoragePushButton->setToolTip(tr("Back To Storage"));
    backToStoragePushButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    backToStoragePushButton->setIcon(QIcon(":/Icon/up_icon.png"));
    backToStoragePushButton->setIconSize(backToStoragePushButton->sizeHint());

    QLabel *storageLabel = new QLabel(tr("Available For Order :"));
    QLabel *orderedLabel = new QLabel(tr("Ordered :"));
    QVBoxLayout *storageVLayout = new QVBoxLayout();
    storageVLayout->addWidget(storageLabel);
    storageVLayout->addWidget(storagePanel, 10);
    QVBoxLayout *orderedVLayout = new QVBoxLayout();
    orderedVLayout->addWidget(orderedLabel);
    orderedVLayout->addWidget(orderPanel, 10);
    QHBoxLayout *storageHLayout = new QHBoxLayout();
    storageHLayout->addLayout(storageVLayout, 10);
    storageHLayout->addWidget(submitOrderPushButton);
    QHBoxLayout *orderedHLayout = new QHBoxLayout();
    orderedHLayout->addLayout(orderedVLayout, 10);
    orderedHLayout->addWidget(backToStoragePushButton);

    QWidget *storageWidget = new QWidget();
    storageWidget->setLayout(storageHLayout);
    QWidget *orderedWidget = new QWidget();
    orderedWidget->setLayout(orderedHLayout);

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
    splitter->addWidget(storageWidget);
    splitter->addWidget(orderedWidget);
    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addWidget(splitter);
    mainWidget->setLayout(mainLayout);
}

void Sell_Invoicing::createOrderPanel()
{
    orderPanel = new QWidget();
    QSqlDatabase db(userManagementInterface->getDatabase());
    orderModel = new QSqlRelationalTableModel(this, db);
    orderModel->setTable("products");
    orderModel->setRelation(ProductTypeID, QSqlRelation("producttype", "id", "name"));
    orderModel->setRelation(BrandNameID, QSqlRelation("brandname", "id", "name"));
    orderModel->setRelation(ProductModelID, QSqlRelation("productmodel", "id", "model"));
    orderModel->setRelation(ColorID, QSqlRelation("colorinfo", "id", "color"));
    orderModel->setRelation(VendorID, QSqlRelation("vendorinfo", "id", "name"));
    orderModel->setRelation(SchemaNameID, QSqlRelation("schemaname", "id", "name"));
    orderModel->setRelation(OperatorUserID, QSqlRelation("user", "id", "name"));
    orderModel->setRelation(ResponserUserID, QSqlRelation("user", "id", "name"));
    orderModel->setRelation(SellerID, QSqlRelation("user", "id", "name"));
    orderModel->setRelation(ProductStatusID, QSqlRelation("productstatus", "id", "status"));
    orderModel->setRelation(ReplacementStatusID, QSqlRelation("replacementstatus", "id", "status"));
    orderModel->setSort(TimeStamp, Qt::AscendingOrder);

    orderModel->setHeaderData(ProductID, Qt::Horizontal, tr("ID"));
    orderModel->setHeaderData(SerialNumber, Qt::Horizontal, tr("Serial Number"));
    orderModel->setHeaderData(ProductTypeID, Qt::Horizontal, tr("Product Type"));
    orderModel->setHeaderData(BrandNameID, Qt::Horizontal, tr("Brand Name"));
    orderModel->setHeaderData(ProductModelID, Qt::Horizontal, tr("Model Name"));
    orderModel->setHeaderData(ColorID, Qt::Horizontal, tr("Color"));
    orderModel->setHeaderData(VendorID, Qt::Horizontal, tr("Vendor"));
    orderModel->setHeaderData(SchemaNameID, Qt::Horizontal, tr("Schema Name"));
    orderModel->setHeaderData(Quantity, Qt::Horizontal, tr("Quantity"));
    orderModel->setHeaderData(Unit, Qt::Horizontal, tr("Unit"));
    orderModel->setHeaderData(OldPurchasePrice, Qt::Horizontal, tr("Old Purchase Price"));
    orderModel->setHeaderData(PurchasePrice, Qt::Horizontal, tr("Purchase Price"));
    orderModel->setHeaderData(SellingPrice, Qt::Horizontal, tr("Selling Price"));
    orderModel->setHeaderData(OperatorUserID, Qt::Horizontal, tr("Operator"));
    orderModel->setHeaderData(ResponserUserID, Qt::Horizontal, tr("Responser"));
    orderModel->setHeaderData(SellerID, Qt::Horizontal, tr("Seller"));
    orderModel->setHeaderData(BarInfo, Qt::Horizontal, tr("BarInfo"));
    orderModel->setHeaderData(ProductStatusID, Qt::Horizontal, tr("Product Status"));
    orderModel->setHeaderData(ReplacementStatusID, Qt::Horizontal, tr("Replacement Status"));
    orderModel->setHeaderData(TimeStamp, Qt::Horizontal, tr("TimeStamp"));
    orderModel->setHeaderData(Comments, Qt::Horizontal, tr("Comments"));

    updateOrderFilter();
    orderModel->select();

    orderView = new QTableView;
    orderView->setModel(orderModel);
    orderView->setItemDelegate(new QSqlRelationalDelegate(this));
    orderView->setSelectionMode(QAbstractItemView::SingleSelection);
    orderView->setSelectionBehavior(QAbstractItemView::SelectRows);
    orderView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    orderView->resizeColumnsToContents();
    orderView->horizontalHeader()->setStretchLastSection(true);
    orderView->setColumnHidden(ProductID, true);
    orderView->horizontalHeader()->setVisible(true);
    connect(orderView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(updateProductInfoOrder()));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(orderView);
    orderPanel->setLayout(layout);
}

void Sell_Invoicing::createStoragePanel()
{
    storagePanel = new QWidget();
    QSqlDatabase db(userManagementInterface->getDatabase());
    storageModel = new QSqlRelationalTableModel(this, db);
    storageModel->setTable("products");
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

void Sell_Invoicing::submitOrder()
{
    QModelIndex storageIndex = storageView->currentIndex();
    if(storageIndex.isValid()) {
        if(updateProductinfo()==QDialog::Accepted) {
            QSqlRecord record = storageModel->record(storageIndex.row());
            int productID = record.value(ProductID).toInt();
            int statusID = productManagementInterface->getStatusIDByStatusName("已下单");
            productManagementInterface->updateStatusIDByProductID(productID, statusID);
            orderModel->select();
            storageModel->select();

            if(storageIndex.row()<storageModel->rowCount()) {
                storageView->selectRow(storageIndex.row());
            }
            else {
                storageView->selectRow(storageModel->rowCount()-1);
            }
            orderView->resizeColumnsToContents();
            storageView->setFocus();
        }
    }
}

void Sell_Invoicing::backToStorage()
{
    QModelIndex purchaseIndex = orderView->currentIndex();
    if(purchaseIndex.isValid()) {
        QSqlRecord record = orderModel->record(purchaseIndex.row());
        int productID = record.value(ProductID).toInt();
        int statusID = productManagementInterface->getStatusIDByStatusName("已入库");
        productManagementInterface->updateStatusIDByProductID(productID, statusID);
        orderModel->select();
        storageModel->select();

        if(purchaseIndex.row()<orderModel->rowCount()) {
            orderView->selectRow(purchaseIndex.row());
        }
        else {
            orderView->selectRow(orderModel->rowCount()-1);
        }
        storageView->resizeColumnsToContents();
        orderView->setFocus();
    }
}

void Sell_Invoicing::updateDBTableModel()
{
    if(productManagementInterface->isModelOutdate(storageModel, storageTimeStamp)) {
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

    if(productManagementInterface->isModelOutdate(orderModel, timeStamp)) {
        QModelIndex orderIndex = orderView->currentIndex();
        static QModelIndex orderOutViewPortindex;
        orderModel->select();
        orderView->resizeColumnsToContents();
        if(orderIndex.isValid()) {
            int rowPosition = orderView->rowViewportPosition(orderIndex.row());
            if(rowPosition>=0 && rowPosition<orderView->height()) {
                orderView->setCurrentIndex(orderIndex);
            }
            else {
                orderOutViewPortindex = orderIndex;
            }
        }
        else if(orderOutViewPortindex.isValid()) {
            int rowPosition = orderView->rowViewportPosition(orderOutViewPortindex.row());
            if(rowPosition>=0 && rowPosition<orderView->height()) {
                orderView->setCurrentIndex(orderOutViewPortindex);
            }
        }
        orderView->resizeColumnsToContents();
    }
}

int Sell_Invoicing::updateProductInfoOrder()
{
    if(!updateOrderProductDialog) {
        updateOrderProductDialog = new UpdateProductDialog(userManagementInterface,
                                                      productManagementInterface);
        //connect(updateStorageProductDialog, SIGNAL(productUpdated()), this, SLOT(productUpdated()));
    }
    updateOrderProductDialog->hideForOrderInfo();
    updateOrderProductDialog->updateDBTableModel();
    QModelIndex storageIndex = orderView->currentIndex();
    QSqlRecord record = orderModel->record(storageIndex.row());
    updateOrderProductDialog->updateRecord(record);
    return updateOrderProductDialog->exec();
}

int Sell_Invoicing::updateProductinfo()
{
    if(!updateStorageProductDialog) {
        updateStorageProductDialog = new UpdateProductDialog(userManagementInterface,
                                                      productManagementInterface);
        connect(updateStorageProductDialog, SIGNAL(productUpdated()), this, SLOT(productUpdated()));
    }
    updateStorageProductDialog->updateDBTableModel();
    QModelIndex storageIndex = storageView->currentIndex();
    QSqlRecord record = storageModel->record(storageIndex.row());
    updateStorageProductDialog->updateRecord(record);
    return updateStorageProductDialog->exec();
}

void Sell_Invoicing::productUpdated()
{
    storageModel->select();
    storageView->resizeColumnsToContents();
    orderModel->select();
    orderView->resizeColumnsToContents();
    updateStorageProductDialog->hide();
}

void Sell_Invoicing::onFilter()
{
    static QString oldStoreageFilter = storageModel->filter();
    static QString oldOrderFilter = orderModel->filter();

    QString serialNumber = serialNumberLineEdit->text().simplified();
    if(!serialNumber.isEmpty()) {
        QString newStorageFilter = oldStoreageFilter + QString(" and serialNumber like '%%1%'").arg(serialNumber);
        storageModel->setFilter(newStorageFilter);
        QString newOrderFilter = oldOrderFilter + QString(" and serialNumber like '%%1%'").arg(serialNumber);
        orderModel->setFilter(newOrderFilter);
    }
    else {
        storageModel->setFilter(oldStoreageFilter);
        orderModel->setFilter(oldOrderFilter);
    }
    storageModel->select();
    orderModel->select();
}

void Sell_Invoicing::updateStorageFilter()
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

void Sell_Invoicing::updateOrderFilter()
{
    int statusID = productManagementInterface->getStatusIDByStatusName("已下单");
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

    orderModel->setFilter(filter);
}


QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(Sell_Invoicing, Sell_Invoicing)
QT_END_NAMESPACE
