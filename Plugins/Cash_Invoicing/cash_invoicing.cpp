#include <QtGui>
#include <QtSql>
#include "cash_invoicing.h"
#include "mainwindow.h"
#include "usermanagement_interface.h"
#include "productmanagement_interface.h"
#include "cashupdateproductdialog.h"

Cash_Invoicing::Cash_Invoicing():
    parentWindow(NULL),
    userManagementInterface(NULL),
    productManagementInterface(NULL),
    mainWidget(NULL),
    cashUpdateProductDialog(NULL),
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


QWidget* Cash_Invoicing::getMainWidget() const
{
    return mainWidget;
}

QAction* Cash_Invoicing::getAction() const
{
    return NULL;
}

QMenu* Cash_Invoicing::getMenu() const
{
    return NULL;
}

QToolBar* Cash_Invoicing::getToolBar() const
{
    return NULL;
}

bool Cash_Invoicing::init(MainWindow *parent)
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

bool Cash_Invoicing::deInit()
{
    if(userManagementInterface) {
        userManagementInterface->deregisteObserver(this);
    }
    return true;
}


void Cash_Invoicing::userChanged()
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
    updateCashFilter();
    updateDBTableModel();
}

void Cash_Invoicing::hidePurchasePrice()
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
    cashView->setColumnHidden(OldPurchasePrice, hide);
    cashView->setColumnHidden(PurchasePrice, hide);

    if(!cashUpdateProductDialog) {
        cashUpdateProductDialog = new CashUpdateProductDialog(userManagementInterface,
                                                      productManagementInterface);
        connect(cashUpdateProductDialog, SIGNAL(productUpdated()), this, SLOT(productUpdated()));
    }
    cashUpdateProductDialog->hidePurchasePrice(hide);
}

QString Cash_Invoicing::moduleName() const
{
    return this->metaObject()->className();
}

QString Cash_Invoicing::moduleDescription() const
{
    return tr("Cash Function");
}

QSet<QString> Cash_Invoicing::getAccessRoleNameSet() const
{
    return QSet<QString>()<<"管理员"<<"收银员";
}

QSet<QString> Cash_Invoicing::getDependencySet() const
{
    return QSet<QString>()<<"UserManagementIF"<<"ProductManagementIF";
}

void Cash_Invoicing::initMainWidget()
{
    mainWidget = new QWidget();
    mainWidget->setWindowIcon(QIcon(":/Icon/cash_icon.png"));
    mainWidget->setWindowIconText(tr("Cash"));

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

    createCashPanel();

    QLabel *cashLabel = new QLabel(tr("Waiting For Cash :"));
    QVBoxLayout *cashVLayout = new QVBoxLayout();
    cashVLayout->addWidget(filterWidget);
    cashVLayout->addWidget(cashLabel);
    cashVLayout->addWidget(cashPanel, 10);

    QWidget *cashWidget = new QWidget();
    cashWidget->setLayout(cashVLayout);

    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addWidget(cashWidget);
    mainWidget->setLayout(mainLayout);
}

void Cash_Invoicing::createCashPanel()
{
    cashPanel = new QWidget();
    QSqlDatabase db(userManagementInterface->getDatabase());
    cashModel = new QSqlRelationalTableModel(this, db);
    cashModel->setTable("products");
    cashModel->setRelation(ProductTypeID, QSqlRelation("producttype", "id", "name"));
    cashModel->setRelation(BrandNameID, QSqlRelation("brandname", "id", "name"));
    cashModel->setRelation(ProductModelID, QSqlRelation("productmodel", "id", "model"));
    cashModel->setRelation(ColorID, QSqlRelation("colorinfo", "id", "color"));
    cashModel->setRelation(VendorID, QSqlRelation("vendorinfo", "id", "name"));
    cashModel->setRelation(SchemaNameID, QSqlRelation("schemaname", "id", "name"));
    cashModel->setRelation(OperatorUserID, QSqlRelation("user", "id", "name"));
    cashModel->setRelation(ResponserUserID, QSqlRelation("user", "id", "name"));
    cashModel->setRelation(SellerID, QSqlRelation("user", "id", "name"));
    cashModel->setRelation(ProductStatusID, QSqlRelation("productstatus", "id", "status"));
    cashModel->setRelation(ReplacementStatusID, QSqlRelation("replacementstatus", "id", "status"));
    cashModel->setSort(TimeStamp, Qt::AscendingOrder);

    cashModel->setHeaderData(ProductID, Qt::Horizontal, tr("ID"));
    cashModel->setHeaderData(SerialNumber, Qt::Horizontal, tr("Serial Number"));
    cashModel->setHeaderData(ProductTypeID, Qt::Horizontal, tr("Product Type"));
    cashModel->setHeaderData(BrandNameID, Qt::Horizontal, tr("Brand Name"));
    cashModel->setHeaderData(ProductModelID, Qt::Horizontal, tr("Model Name"));
    cashModel->setHeaderData(ColorID, Qt::Horizontal, tr("Color"));
    cashModel->setHeaderData(VendorID, Qt::Horizontal, tr("Vendor"));
    cashModel->setHeaderData(SchemaNameID, Qt::Horizontal, tr("Schema Name"));
    cashModel->setHeaderData(Quantity, Qt::Horizontal, tr("Quantity"));
    cashModel->setHeaderData(Unit, Qt::Horizontal, tr("Unit"));
    cashModel->setHeaderData(OldPurchasePrice, Qt::Horizontal, tr("Old Purchase Price"));
    cashModel->setHeaderData(PurchasePrice, Qt::Horizontal, tr("Purchase Price"));
    cashModel->setHeaderData(SellingPrice, Qt::Horizontal, tr("Selling Price"));
    cashModel->setHeaderData(OperatorUserID, Qt::Horizontal, tr("Operator"));
    cashModel->setHeaderData(ResponserUserID, Qt::Horizontal, tr("Responser"));
    cashModel->setHeaderData(SellerID, Qt::Horizontal, tr("Seller"));
    cashModel->setHeaderData(BarInfo, Qt::Horizontal, tr("BarInfo"));
    cashModel->setHeaderData(ProductStatusID, Qt::Horizontal, tr("Product Status"));
    cashModel->setHeaderData(ReplacementStatusID, Qt::Horizontal, tr("Replacement Status"));
    cashModel->setHeaderData(TimeStamp, Qt::Horizontal, tr("TimeStamp"));
    cashModel->setHeaderData(Comments, Qt::Horizontal, tr("Comments"));

    updateCashFilter();
    cashModel->select();

    cashView = new QTableView;
    cashView->setModel(cashModel);
    cashView->setItemDelegate(new QSqlRelationalDelegate(this));
    cashView->setSelectionMode(QAbstractItemView::SingleSelection);
    cashView->setSelectionBehavior(QAbstractItemView::SelectRows);
    cashView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    cashView->resizeColumnsToContents();
    cashView->horizontalHeader()->setStretchLastSection(true);
    cashView->setColumnHidden(ProductID, true);
    cashView->horizontalHeader()->setVisible(true);
    connect(cashView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(updateProductinfo()));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(cashView);
    cashPanel->setLayout(layout);
}

void Cash_Invoicing::onFilter()
{
    static QString oldCashFilter = cashModel->filter();

    QString serialNumber = serialNumberLineEdit->text().simplified();
    if(!serialNumber.isEmpty()) {
        QString newCashFilter = oldCashFilter + QString(" and serialNumber like '%%1%'").arg(serialNumber);
        cashModel->setFilter(newCashFilter);
    }
    else {
        cashModel->setFilter(oldCashFilter);
    }
    cashModel->select();
}

void Cash_Invoicing::updateDBTableModel()
{
    if(productManagementInterface->isModelOutdate(cashModel, timeStamp)) {
        QModelIndex index = cashView->currentIndex();
        static QModelIndex outViewPortindex;
        cashModel->select();
        cashView->resizeColumnsToContents();
        if(index.isValid()) {
            int rowPosition = cashView->rowViewportPosition(index.row());
            if(rowPosition>0 && rowPosition<cashView->height()) {
                cashView->setCurrentIndex(index);
            }
            else {
                outViewPortindex = index;
            }
        }
        else if(outViewPortindex.isValid()) {
            int rowPosition = cashView->rowViewportPosition(outViewPortindex.row());
            if(rowPosition>=0 && rowPosition<cashView->height()) {
                cashView->setCurrentIndex(outViewPortindex);
            }
        }
        cashView->resizeColumnsToContents();
    }
}

void Cash_Invoicing::updateCashFilter()
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

    cashModel->setFilter(filter);
}

void Cash_Invoicing::updateProductinfo()
{
    if(!cashUpdateProductDialog) {
        cashUpdateProductDialog = new CashUpdateProductDialog(userManagementInterface,
                                                      productManagementInterface);
        connect(cashUpdateProductDialog, SIGNAL(productUpdated()), this, SLOT(productUpdated()));
    }
    cashUpdateProductDialog->updateDBTableModel();
    QModelIndex storageIndex = cashView->currentIndex();
    QSqlRecord record = cashModel->record(storageIndex.row());
    cashUpdateProductDialog->updateRecord(record);
    cashUpdateProductDialog->exec();
}

void Cash_Invoicing::productUpdated()
{
    cashModel->select();
    cashView->resizeColumnsToContents();
}

QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(Cash_Invoicing, Cash_Invoicing)
QT_END_NAMESPACE
