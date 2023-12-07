#include <QtGui>
#include <QtSql>

#include "barchart.h"
#include "exportexcelobject.h"
#include "helper.h"
#include "mainwindow.h"
#include "productmanagement_interface.h"
#include "statistic_invoicing.h"
#include "statisticupdateproductdialog.h"
#include "usermanagement_interface.h"

const char *sortTypeProperty = "SortType";

Statistic_Invoicing::Statistic_Invoicing():
    parentWindow(NULL),
    userManagementInterface(NULL),
    productManagementInterface(NULL),
    mainWidget(NULL),
    allProductsPanel(NULL),
    allProductsModel(NULL),
    allProductsView(NULL),
    unsellProductsPanel(NULL),
    unsellProductsModel(NULL),
    unsellProductsView(NULL),
    filterPanel(NULL),
    unsellProductsFilterPanel(NULL),
    allProductsFilterPanel(NULL),
    salesStatisticContainer(NULL),
    salesStatisticPlot(NULL),
    sortByVolumeAscRadioButton(NULL),
    sortByVolumeDesRadioButton(NULL),
    sortByQuantityAscRadioButton(NULL),
    sortByQuantityDesRadioButton(NULL),
    time(NULL),
    timeRange(NULL),
    peopleRange(NULL),
    updateStatisticPushButton(NULL),
    printStatisticPushButton(NULL),
    serialNumberAllProductsLabel(NULL),
    serialNumberAllProductsLineEdit(NULL),
    serialNumberAllProductsCheckBox(NULL),
    productTypeAllProductsLabel(NULL),
    productTypeAllProductsComboBox(NULL),
    productTypeAllProductsModel(NULL),
    productTypeAllProductsCheckBox(NULL),
    brandNameAllProductsLabel(NULL),
    brandNameAllProductsComboBox(NULL),
    brandNameAllProductsModel(NULL),
    brandNameAllProductsCheckBox(NULL),
    productModelAllProductsLabel(NULL),
    productModelAllProductsComboBox(NULL),
    productModelAllProductsModel(NULL),
    productModelAllProductsCheckBox(NULL),
    productColorAllProductsLabel(NULL),
    productColorAllProductsComboBox(NULL),
    productColorAllProductsModel(NULL),
    productColorAllProductsCheckBox(NULL),
    productVendorAllProductsLabel(NULL),
    productVendorAllProductsComboBox(NULL),
    productVendorAllProductsModel(NULL),
    productVendorAllProductsCheckBox(NULL),
    replacementInfoAllProductsLabel(NULL),
    replacementInfoAllProductsComboBox(NULL),
    replacementInfoAllProductsModel(NULL),
    replacementInfoAllProductsCheckBox(NULL),
    barInfoAllProductsLabel(NULL),
    barInfoAllProductsLineEdit(NULL),
    barInfoAllProductsCheckBox(NULL),
    sellerAllProductsLabel(NULL),
    sellerAllProductsComboBox(NULL),
    sellerAllProductsCheckBox(NULL),
    productStatusAllProductsLabel(NULL),
    productStatusAllProductsComboBox(NULL),
    productStatusAllProductsModel(NULL),
    productStatusAllProductsCheckBox(NULL),
    quantityAllProductsLabel(NULL),
    quantityAllProductsSpinBox(NULL),
    quantityAllProductsCheckBox(NULL),
    unitAllProductsLabel(NULL),
    unitAllProductsLineEdit(NULL),
    unitAllProductsCheckBox(NULL),
    schemaAllProductsLabel(NULL),
    schemaAllProductsComboBox(NULL),
    userSchemaAllProductsModel(NULL),
    schemaAllProductsCheckBox(NULL),
    oldPurchasePriceAllProductsLabel(NULL),
    oldPurchasePriceAllProductsLineEdit(NULL),
    oldPurchasePriceAllProductsCheckBox(NULL),
    purchasePriceAllProductsLabel(NULL),
    purchasePriceAllProductsLineEdit(NULL),
    purchasePriceAllProductsCheckBox(NULL),
    sellingPriceAllProductsLabel(NULL),
    sellingPriceAllProductsLineEdit(NULL),
    sellingPriceAllProductsCheckBox(NULL),
    commentsAllProductsLabel(NULL),
    commentsAllProductsLineEdit(NULL),
    commentsAllProductsCheckBox(NULL),
    startTimeAllProductsLabel(NULL),
    startTimeAllProductsDateTimeEdit(NULL),
    startTimeAllProductsCheckBox(NULL),
    endTimeAllProductsLabel(NULL),
    endTimeAllProductsDateTimeEdit(NULL),
    endTimeAllProductsCheckBox(NULL),
    allProductsFilterButton(NULL),
    exportAllProducts2ExcelButton(NULL),
    allProductsSummaryLabel(NULL),
    allProductsSummaryLineEdit(NULL),
    serialNumberUnsellProductsLabel(NULL),
    serialNumberUnsellProductsLineEdit(NULL),
    serialNumberUnsellProductsCheckBox(NULL),
    productTypeUnsellProductsLabel(NULL),
    productTypeUnsellProductsComboBox(NULL),
    productTypeUnsellProductsModel(NULL),
    productTypeUnsellProductsCheckBox(NULL),
    brandNameUnsellProductsLabel(NULL),
    brandNameUnsellProductsComboBox(NULL),
    brandNameUnsellProductsModel(NULL),
    brandNameUnsellProductsCheckBox(NULL),
    productModelUnsellProductsLabel(NULL),
    productModelUnsellProductsComboBox(NULL),
    productModelUnsellProductsModel(NULL),
    productModelUnsellProductsCheckBox(NULL),
    productColorUnsellProductsLabel(NULL),
    productColorUnsellProductsComboBox(NULL),
    productColorUnsellProductsModel(NULL),
    productColorUnsellProductsCheckBox(NULL),
    productVendorUnsellProductsLabel(NULL),
    productVendorUnsellProductsComboBox(NULL),
    productVendorUnsellProductsModel(NULL),
    productVendorUnsellProductsCheckBox(NULL),
    replacementInfoUnsellProductsLabel(NULL),
    replacementInfoUnsellProductsComboBox(NULL),
    replacementInfoUnsellProductsModel(NULL),
    replacementInfoUnsellProductsCheckBox(NULL),
    barInfoUnsellProductsLabel(NULL),
    barInfoUnsellProductsLineEdit(NULL),
    barInfoUnsellProductsCheckBox(NULL),
    sellerUnsellProductsLabel(NULL),
    sellerUnsellProductsComboBox(NULL),
    sellerUnsellProductsCheckBox(NULL),
    productStatusUnsellProductsLabel(NULL),
    productStatusUnsellProductsComboBox(NULL),
    productStatusUnsellProductsModel(NULL),
    productStatusUnsellProductsCheckBox(NULL),
    quantityUnsellProductsLabel(NULL),
    quantityUnsellProductsSpinBox(NULL),
    quantityUnsellProductsCheckBox(NULL),
    unitUnsellProductsLabel(NULL),
    unitUnsellProductsLineEdit(NULL),
    unitUnsellProductsCheckBox(NULL),
    schemaUnsellProductsLabel(NULL),
    schemaUnsellProductsComboBox(NULL),
    userSchemaUnsellProductsModel(NULL),
    schemaUnsellProductsCheckBox(NULL),
    oldPurchasePriceUnsellProductsLabel(NULL),
    oldPurchasePriceUnsellProductsLineEdit(NULL),
    oldPurchasePriceUnsellProductsCheckBox(NULL),
    purchasePriceUnsellProductsLabel(NULL),
    purchasePriceUnsellProductsLineEdit(NULL),
    purchasePriceUnsellProductsCheckBox(NULL),
    sellingPriceUnsellProductsLabel(NULL),
    sellingPriceUnsellProductsLineEdit(NULL),
    sellingPriceUnsellProductsCheckBox(NULL),
    commentsUnsellProductsLabel(NULL),
    commentsUnsellProductsLineEdit(NULL),
    commentsUnsellProductsCheckBox(NULL),
    startTimeUnsellProductsLabel(NULL),
    startTimeUnsellProductsDateTimeEdit(NULL),
    startTimeUnsellProductsCheckBox(NULL),
    endTimeUnsellProductsLabel(NULL),
    endTimeUnsellProductsDateTimeEdit(NULL),
    endTimeUnsellProductsCheckBox(NULL),
    unsellProductsFilterButton(NULL),
    exportUnsellProducts2ExcelButton(NULL),
    unsellProductsSummaryLabel(NULL),
    unsellProductsSummaryLineEdit(NULL),
    statisticUpdateAllProductDialog(NULL),
    statisticUpdateUnsellProductDialog(NULL),
    unsellProductProgressBar(NULL),
    allProductProgressBar(NULL)
{
    INSTALL_TRANSLATION;
}


QWidget* Statistic_Invoicing::getMainWidget() const
{
    return mainWidget;
}

QAction* Statistic_Invoicing::getAction() const
{
    return NULL;
}

QMenu* Statistic_Invoicing::getMenu() const
{
    return NULL;
}

QToolBar* Statistic_Invoicing::getToolBar() const
{
    return NULL;
}

bool Statistic_Invoicing::init(MainWindow *parent)
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

bool Statistic_Invoicing::deInit()
{
    if(userManagementInterface) {
        userManagementInterface->deregisteObserver(this);
    }
    return true;
}


void Statistic_Invoicing::userChanged()
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
}

QString Statistic_Invoicing::moduleName() const
{
    return this->metaObject()->className();
}

QString Statistic_Invoicing::moduleDescription() const
{
    return tr("Statistic Function");
}

QSet<QString> Statistic_Invoicing::getAccessRoleNameSet() const
{
    return QSet<QString>()<<"管理员"<<"审计";
}

QSet<QString> Statistic_Invoicing::getDependencySet() const
{
    return QSet<QString>()<<"UserManagementIF"<<"ProductManagementIF";
}

void Statistic_Invoicing::initMainWidget()
{
    mainWidget = new QWidget();
    mainWidget->setWindowIcon(QIcon(":/Icon/statistic_icon.png"));
    mainWidget->setWindowIconText(tr("Statistic"));

    createFilterPanel();

    QGridLayout *statisticFilterLayout = new QGridLayout(mainWidget);
    statisticFilterLayout->addWidget(filterPanel, 0, 0);
    //statisticFilterLayout->addWidget(statisticPanel, 1, 0);
    mainWidget->setLayout(statisticFilterLayout);
}

void Statistic_Invoicing::createAllProductsPanel()
{
    allProductsPanel = new QWidget();
    QSqlDatabase db(userManagementInterface->getDatabase());
    allProductsModel = new QSqlRelationalTableModel(NULL, db);
    allProductsModel->setTable("productsinfo");
    allProductsModel->setRelation(ProductTypeID, QSqlRelation("producttype", "id", "name"));
    allProductsModel->setRelation(BrandNameID, QSqlRelation("brandname", "id", "name"));
    allProductsModel->setRelation(ProductModelID, QSqlRelation("productmodel", "id", "model"));
    allProductsModel->setRelation(ColorID, QSqlRelation("colorinfo", "id", "color"));
    allProductsModel->setRelation(VendorID, QSqlRelation("vendorinfo", "id", "name"));
    allProductsModel->setRelation(SchemaNameID, QSqlRelation("schemaname", "id", "name"));
    allProductsModel->setRelation(OperatorUserID, QSqlRelation("user", "id", "name"));
    allProductsModel->setRelation(ResponserUserID, QSqlRelation("user", "id", "name"));
    allProductsModel->setRelation(SellerID, QSqlRelation("user", "id", "name"));
    allProductsModel->setRelation(ProductStatusID, QSqlRelation("productstatus", "id", "status"));
    allProductsModel->setRelation(ReplacementStatusID, QSqlRelation("replacementstatus", "id", "status"));
    allProductsModel->setSort(TimeStamp, Qt::AscendingOrder);

    allProductsModel->setHeaderData(ProductID, Qt::Horizontal, tr("ID"));
    allProductsModel->setHeaderData(SerialNumber, Qt::Horizontal, tr("Serial Number"));
    allProductsModel->setHeaderData(ProductTypeID, Qt::Horizontal, tr("Product Type"));
    allProductsModel->setHeaderData(BrandNameID, Qt::Horizontal, tr("Brand Name"));
    allProductsModel->setHeaderData(ProductModelID, Qt::Horizontal, tr("Model Name"));
    allProductsModel->setHeaderData(ColorID, Qt::Horizontal, tr("Color"));
    allProductsModel->setHeaderData(VendorID, Qt::Horizontal, tr("Vendor"));
    allProductsModel->setHeaderData(SchemaNameID, Qt::Horizontal, tr("Schema Name"));
    allProductsModel->setHeaderData(Quantity, Qt::Horizontal, tr("Quantity"));
    allProductsModel->setHeaderData(Unit, Qt::Horizontal, tr("Unit"));
    allProductsModel->setHeaderData(OldPurchasePrice, Qt::Horizontal, tr("Old Purchase Price"));
    allProductsModel->setHeaderData(PurchasePrice, Qt::Horizontal, tr("Purchase Price"));
    allProductsModel->setHeaderData(SellingPrice, Qt::Horizontal, tr("Selling Price"));
    allProductsModel->setHeaderData(OperatorUserID, Qt::Horizontal, tr("Operator"));
    allProductsModel->setHeaderData(ResponserUserID, Qt::Horizontal, tr("Responser"));
    allProductsModel->setHeaderData(SellerID, Qt::Horizontal, tr("Seller"));
    allProductsModel->setHeaderData(BarInfo, Qt::Horizontal, tr("BarInfo"));
    allProductsModel->setHeaderData(ProductStatusID, Qt::Horizontal, tr("Product Status"));
    allProductsModel->setHeaderData(ReplacementStatusID, Qt::Horizontal, tr("Replacement Status"));
    allProductsModel->setHeaderData(TimeStamp, Qt::Horizontal, tr("TimeStamp"));
    allProductsModel->setHeaderData(Comments, Qt::Horizontal, tr("Comments"));

    allProductsView = new QTableView;
    allProductsView->setModel(allProductsModel);
    allProductsView->setItemDelegate(new QSqlRelationalDelegate(this));
    allProductsView->setSelectionMode(QAbstractItemView::SingleSelection);
    allProductsView->setSelectionBehavior(QAbstractItemView::SelectRows);
    allProductsView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    allProductsView->resizeColumnsToContents();
    allProductsView->horizontalHeader()->setStretchLastSection(true);
    allProductsView->setColumnHidden(ProductID, true);
    allProductsView->horizontalHeader()->setVisible(true);
    allProductsView->resizeColumnsToContents();
    connect(allProductsView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(updateAllProductinfo()));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(allProductsView);
    allProductsPanel->setLayout(layout);

    productTypeAllProductsComboBox->setCurrentIndex(
                productTypeAllProductsComboBox->findText("其它"));

    brandNameAllProductsComboBox->setCurrentIndex(
                brandNameAllProductsComboBox->findText("其它"));

    productModelAllProductsComboBox->setCurrentIndex(
                productModelAllProductsComboBox->findText("其它"));

    productColorAllProductsComboBox->setCurrentIndex(
                productColorAllProductsComboBox->findText("其它"));

    productVendorAllProductsComboBox->setCurrentIndex(
                productVendorAllProductsComboBox->findText("其它"));

    replacementInfoAllProductsComboBox->setCurrentIndex(
                replacementInfoAllProductsComboBox->findText("否"));

    productStatusAllProductsComboBox->setCurrentIndex(
                productStatusAllProductsComboBox->findText("已删除"));

    sellerAllProductsComboBox->setCurrentIndex(
                sellerAllProductsComboBox->findText("未指定"));

    schemaAllProductsComboBox->setCurrentIndex(
                schemaAllProductsComboBox->findText("未指定"));
}

void Statistic_Invoicing::createUnsellProductsPanel()
{
    unsellProductsPanel = new QWidget();
    QSqlDatabase db(userManagementInterface->getDatabase());
    unsellProductsModel = new QSqlRelationalTableModel(NULL, db);
    unsellProductsModel->setTable("products");
    unsellProductsModel->setRelation(ProductTypeID, QSqlRelation("producttype", "id", "name"));
    unsellProductsModel->setRelation(BrandNameID, QSqlRelation("brandname", "id", "name"));
    unsellProductsModel->setRelation(ProductModelID, QSqlRelation("productmodel", "id", "model"));
    unsellProductsModel->setRelation(ColorID, QSqlRelation("colorinfo", "id", "color"));
    unsellProductsModel->setRelation(VendorID, QSqlRelation("vendorinfo", "id", "name"));
    unsellProductsModel->setRelation(SchemaNameID, QSqlRelation("schemaname", "id", "name"));
    unsellProductsModel->setRelation(OperatorUserID, QSqlRelation("user", "id", "name"));
    unsellProductsModel->setRelation(ResponserUserID, QSqlRelation("user", "id", "name"));
    unsellProductsModel->setRelation(SellerID, QSqlRelation("user", "id", "name"));
    unsellProductsModel->setRelation(ProductStatusID, QSqlRelation("productstatus", "id", "status"));
    unsellProductsModel->setRelation(ReplacementStatusID, QSqlRelation("replacementstatus", "id", "status"));
    unsellProductsModel->setSort(TimeStamp, Qt::AscendingOrder);

    unsellProductsModel->setHeaderData(ProductID, Qt::Horizontal, tr("ID"));
    unsellProductsModel->setHeaderData(SerialNumber, Qt::Horizontal, tr("Serial Number"));
    unsellProductsModel->setHeaderData(ProductTypeID, Qt::Horizontal, tr("Product Type"));
    unsellProductsModel->setHeaderData(BrandNameID, Qt::Horizontal, tr("Brand Name"));
    unsellProductsModel->setHeaderData(ProductModelID, Qt::Horizontal, tr("Model Name"));
    unsellProductsModel->setHeaderData(ColorID, Qt::Horizontal, tr("Color"));
    unsellProductsModel->setHeaderData(VendorID, Qt::Horizontal, tr("Vendor"));
    unsellProductsModel->setHeaderData(SchemaNameID, Qt::Horizontal, tr("Schema Name"));
    unsellProductsModel->setHeaderData(Quantity, Qt::Horizontal, tr("Quantity"));
    unsellProductsModel->setHeaderData(Unit, Qt::Horizontal, tr("Unit"));
    unsellProductsModel->setHeaderData(OldPurchasePrice, Qt::Horizontal, tr("Old Purchase Price"));
    unsellProductsModel->setHeaderData(PurchasePrice, Qt::Horizontal, tr("Purchase Price"));
    unsellProductsModel->setHeaderData(SellingPrice, Qt::Horizontal, tr("Selling Price"));
    unsellProductsModel->setHeaderData(OperatorUserID, Qt::Horizontal, tr("Operator"));
    unsellProductsModel->setHeaderData(ResponserUserID, Qt::Horizontal, tr("Responser"));
    unsellProductsModel->setHeaderData(SellerID, Qt::Horizontal, tr("Seller"));
    unsellProductsModel->setHeaderData(BarInfo, Qt::Horizontal, tr("BarInfo"));
    unsellProductsModel->setHeaderData(ProductStatusID, Qt::Horizontal, tr("Product Status"));
    unsellProductsModel->setHeaderData(ReplacementStatusID, Qt::Horizontal, tr("Replacement Status"));
    unsellProductsModel->setHeaderData(TimeStamp, Qt::Horizontal, tr("TimeStamp"));
    unsellProductsModel->setHeaderData(Comments, Qt::Horizontal, tr("Comments"));

    unsellProductsView = new QTableView;
    unsellProductsView->setModel(unsellProductsModel);
    unsellProductsView->setItemDelegate(new QSqlRelationalDelegate(this));
    unsellProductsView->setSelectionMode(QAbstractItemView::SingleSelection);
    unsellProductsView->setSelectionBehavior(QAbstractItemView::SelectRows);
    unsellProductsView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    unsellProductsView->resizeColumnsToContents();
    unsellProductsView->horizontalHeader()->setStretchLastSection(true);
    unsellProductsView->setColumnHidden(ProductID, true);
    unsellProductsView->horizontalHeader()->setVisible(true);
    unsellProductsView->resizeColumnsToContents();
    connect(unsellProductsView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(updateUnsellProductinfo()));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(unsellProductsView);
    unsellProductsPanel->setLayout(layout);
}

void Statistic_Invoicing::createUnsellProductsFilterPanel()
{
    unsellProductsFilterPanel = new QWidget();

    serialNumberUnsellProductsLabel = new QLabel(tr("Serial Number: "), unsellProductsFilterPanel);
    serialNumberUnsellProductsLineEdit = new QLineEdit(unsellProductsFilterPanel);
    serialNumberUnsellProductsLabel->setEnabled(false);
    serialNumberUnsellProductsLineEdit->setEnabled(false);
    serialNumberUnsellProductsCheckBox = new QCheckBox(unsellProductsFilterPanel);
    connect(serialNumberUnsellProductsCheckBox, SIGNAL(toggled(bool)), serialNumberUnsellProductsLabel, SLOT(setEnabled(bool)));
    connect(serialNumberUnsellProductsCheckBox, SIGNAL(toggled(bool)), serialNumberUnsellProductsLineEdit, SLOT(setEnabled(bool)));

    productTypeUnsellProductsLabel = new QLabel(tr("Product Type: "), unsellProductsFilterPanel);
    productTypeUnsellProductsLabel->setEnabled(false);
    productTypeUnsellProductsComboBox = new QComboBox(unsellProductsFilterPanel);
    productTypeUnsellProductsComboBox->setEnabled(false);
    productTypeUnsellProductsComboBox->setEditable(true);
    QSqlDatabase db(userManagementInterface->getDatabase());
    productTypeUnsellProductsModel = new QSqlRelationalTableModel(NULL, db);
    productTypeUnsellProductsModel->setTable("producttype");
    productTypeUnsellProductsModel->select();
    productTypeUnsellProductsComboBox->setModel(productTypeUnsellProductsModel);
    productTypeUnsellProductsComboBox->setModelColumn(1);
    productTypeUnsellProductsCheckBox = new QCheckBox(unsellProductsFilterPanel);
    connect(productTypeUnsellProductsCheckBox, SIGNAL(toggled(bool)), productTypeUnsellProductsLabel, SLOT(setEnabled(bool)));
    connect(productTypeUnsellProductsCheckBox, SIGNAL(toggled(bool)), productTypeUnsellProductsComboBox, SLOT(setEnabled(bool)));

    brandNameUnsellProductsLabel = new QLabel(tr("Brand Name: "), unsellProductsFilterPanel);
    brandNameUnsellProductsComboBox = new QComboBox(unsellProductsFilterPanel);
    brandNameUnsellProductsComboBox->setEditable(true);
    brandNameUnsellProductsLabel->setEnabled(false);
    brandNameUnsellProductsComboBox->setEnabled(false);
    brandNameUnsellProductsModel = new QSqlRelationalTableModel(NULL, db);
    brandNameUnsellProductsModel->setTable("brandname");
    brandNameUnsellProductsModel->select();
    brandNameUnsellProductsComboBox->setModel(brandNameUnsellProductsModel);
    brandNameUnsellProductsComboBox->setModelColumn(1);
    brandNameUnsellProductsCheckBox = new QCheckBox(unsellProductsFilterPanel);
    connect(brandNameUnsellProductsCheckBox, SIGNAL(toggled(bool)), brandNameUnsellProductsLabel, SLOT(setEnabled(bool)));
    connect(brandNameUnsellProductsCheckBox, SIGNAL(toggled(bool)), brandNameUnsellProductsComboBox, SLOT(setEnabled(bool)));

    productModelUnsellProductsLabel = new QLabel(tr("Product Model: "), unsellProductsFilterPanel);
    productModelUnsellProductsLabel->setEnabled(false);
    productModelUnsellProductsComboBox = new QComboBox(unsellProductsFilterPanel);
    productModelUnsellProductsComboBox->setEnabled(false);
    productModelUnsellProductsComboBox->setEditable(true);
    productModelUnsellProductsModel = new QSqlRelationalTableModel(NULL, db);
    productModelUnsellProductsModel->setTable("productmodel");
    productModelUnsellProductsModel->select();
    productModelUnsellProductsComboBox->setModel(productModelUnsellProductsModel);
    productModelUnsellProductsComboBox->setModelColumn(3);
    productModelUnsellProductsCheckBox = new QCheckBox(unsellProductsFilterPanel);
    connect(productModelUnsellProductsCheckBox, SIGNAL(toggled(bool)), productModelUnsellProductsLabel, SLOT(setEnabled(bool)));
    connect(productModelUnsellProductsCheckBox, SIGNAL(toggled(bool)), productModelUnsellProductsComboBox, SLOT(setEnabled(bool)));

    productColorUnsellProductsLabel = new QLabel(tr("Product Color: "), unsellProductsFilterPanel);
    productColorUnsellProductsLabel->setEnabled(false);
    productColorUnsellProductsComboBox = new QComboBox(unsellProductsFilterPanel);
    productColorUnsellProductsComboBox->setEnabled(false);
    productColorUnsellProductsComboBox->setEditable(true);
    productColorUnsellProductsModel = new QSqlRelationalTableModel(NULL, db);
    productColorUnsellProductsModel->setTable("colorinfo");
    productColorUnsellProductsModel->select();
    productColorUnsellProductsComboBox->setModel(productColorUnsellProductsModel);
    productColorUnsellProductsComboBox->setModelColumn(1);
    productColorUnsellProductsCheckBox = new QCheckBox(unsellProductsFilterPanel);
    connect(productColorUnsellProductsCheckBox, SIGNAL(toggled(bool)), productColorUnsellProductsLabel, SLOT(setEnabled(bool)));
    connect(productColorUnsellProductsCheckBox, SIGNAL(toggled(bool)), productColorUnsellProductsComboBox, SLOT(setEnabled(bool)));

    productVendorUnsellProductsLabel = new QLabel(tr("Vendor: "), unsellProductsFilterPanel);
    productVendorUnsellProductsLabel->setEnabled(false);
    productVendorUnsellProductsComboBox = new QComboBox(unsellProductsFilterPanel);
    productVendorUnsellProductsComboBox->setEnabled(false);
    productVendorUnsellProductsComboBox->setEditable(true);
    productVendorUnsellProductsModel = new QSqlRelationalTableModel(NULL, db);
    productVendorUnsellProductsModel->setTable("vendorinfo");
    productVendorUnsellProductsModel->select();
    productVendorUnsellProductsComboBox->setModel(productVendorUnsellProductsModel);
    productVendorUnsellProductsComboBox->setModelColumn(1);
    productVendorUnsellProductsCheckBox = new QCheckBox(unsellProductsFilterPanel);
    connect(productVendorUnsellProductsCheckBox, SIGNAL(toggled(bool)), productVendorUnsellProductsLabel, SLOT(setEnabled(bool)));
    connect(productVendorUnsellProductsCheckBox, SIGNAL(toggled(bool)), productVendorUnsellProductsComboBox, SLOT(setEnabled(bool)));

    replacementInfoUnsellProductsLabel = new QLabel(tr("Replacement Info: "), unsellProductsFilterPanel);
    replacementInfoUnsellProductsLabel->setEnabled(false);
    replacementInfoUnsellProductsComboBox = new QComboBox(unsellProductsFilterPanel);
    replacementInfoUnsellProductsComboBox->setEnabled(false);
    replacementInfoUnsellProductsComboBox->setEditable(false);
    replacementInfoUnsellProductsModel = new QSqlRelationalTableModel(NULL, db);
    replacementInfoUnsellProductsModel->setTable("replacementstatus");
    replacementInfoUnsellProductsModel->select();
    replacementInfoUnsellProductsComboBox->setModel(replacementInfoUnsellProductsModel);
    replacementInfoUnsellProductsComboBox->setModelColumn(1);
    replacementInfoUnsellProductsCheckBox = new QCheckBox(unsellProductsFilterPanel);
    connect(replacementInfoUnsellProductsCheckBox, SIGNAL(toggled(bool)), replacementInfoUnsellProductsLabel, SLOT(setEnabled(bool)));
    connect(replacementInfoUnsellProductsCheckBox, SIGNAL(toggled(bool)), replacementInfoUnsellProductsComboBox, SLOT(setEnabled(bool)));

    barInfoUnsellProductsLabel = new QLabel(tr("Bar Info: "), unsellProductsFilterPanel);
    barInfoUnsellProductsLineEdit = new QLineEdit(unsellProductsFilterPanel);
    barInfoUnsellProductsLabel->setEnabled(false);
    barInfoUnsellProductsLineEdit->setEnabled(false);
    barInfoUnsellProductsCheckBox = new QCheckBox(unsellProductsFilterPanel);
    connect(barInfoUnsellProductsCheckBox, SIGNAL(toggled(bool)), barInfoUnsellProductsLabel, SLOT(setEnabled(bool)));
    connect(barInfoUnsellProductsCheckBox, SIGNAL(toggled(bool)), barInfoUnsellProductsLineEdit, SLOT(setEnabled(bool)));

    sellerUnsellProductsLabel = new QLabel(tr("Seller: "), unsellProductsFilterPanel);
    sellerUnsellProductsComboBox = new QComboBox(unsellProductsFilterPanel);
    sellerUnsellProductsComboBox->setEditable(false);
    populateSellerNameComboBox(sellerUnsellProductsComboBox);
    sellerUnsellProductsLabel->setEnabled(false);
    sellerUnsellProductsComboBox->setEnabled(false);
    sellerUnsellProductsCheckBox = new QCheckBox(unsellProductsFilterPanel);
    connect(sellerUnsellProductsCheckBox, SIGNAL(toggled(bool)), sellerUnsellProductsLabel, SLOT(setEnabled(bool)));
    connect(sellerUnsellProductsCheckBox, SIGNAL(toggled(bool)), sellerUnsellProductsComboBox, SLOT(setEnabled(bool)));

    productStatusUnsellProductsLabel = new QLabel(tr("Product Status: "), unsellProductsFilterPanel);
    productStatusUnsellProductsComboBox = new QComboBox(unsellProductsFilterPanel);
    productStatusUnsellProductsLabel->setEnabled(false);
    productStatusUnsellProductsComboBox->setEnabled(false);
    productStatusUnsellProductsComboBox->setEditable(false);
    productStatusUnsellProductsModel = new QSqlRelationalTableModel(NULL, db);
    productStatusUnsellProductsModel->setTable("productstatus");
    productStatusUnsellProductsModel->select();
    productStatusUnsellProductsComboBox->setModel(productStatusUnsellProductsModel);
    productStatusUnsellProductsComboBox->setModelColumn(1);
    productStatusUnsellProductsCheckBox = new QCheckBox(unsellProductsFilterPanel);
    connect(productStatusUnsellProductsCheckBox, SIGNAL(toggled(bool)), productStatusUnsellProductsLabel, SLOT(setEnabled(bool)));
    connect(productStatusUnsellProductsCheckBox, SIGNAL(toggled(bool)), productStatusUnsellProductsComboBox, SLOT(setEnabled(bool)));

    quantityUnsellProductsLabel = new QLabel(tr("Quantity: "), unsellProductsFilterPanel);
    quantityUnsellProductsSpinBox = new QSpinBox(unsellProductsFilterPanel);
    quantityUnsellProductsSpinBox->setMinimum(1);
    quantityUnsellProductsSpinBox->setValue(1);
    quantityUnsellProductsLabel->setEnabled(false);
    quantityUnsellProductsSpinBox->setEnabled(false);
    quantityUnsellProductsCheckBox = new QCheckBox(unsellProductsFilterPanel);
    connect(quantityUnsellProductsCheckBox, SIGNAL(toggled(bool)), quantityUnsellProductsLabel, SLOT(setEnabled(bool)));
    connect(quantityUnsellProductsCheckBox, SIGNAL(toggled(bool)), quantityUnsellProductsSpinBox, SLOT(setEnabled(bool)));

    unitUnsellProductsLabel = new QLabel(tr("Unit: "), unsellProductsFilterPanel);
    unitUnsellProductsLineEdit = new QLineEdit(unsellProductsFilterPanel);
    unitUnsellProductsLabel->setEnabled(false);
    unitUnsellProductsLineEdit->setEnabled(false);
    unitUnsellProductsCheckBox = new QCheckBox(unsellProductsFilterPanel);
    connect(unitUnsellProductsCheckBox, SIGNAL(toggled(bool)), unitUnsellProductsLabel, SLOT(setEnabled(bool)));
    connect(unitUnsellProductsCheckBox, SIGNAL(toggled(bool)), unitUnsellProductsLineEdit, SLOT(setEnabled(bool)));

    schemaUnsellProductsLabel = new QLabel(tr("Schema: "), unsellProductsFilterPanel);
    schemaUnsellProductsComboBox = new QComboBox(unsellProductsFilterPanel);
    schemaUnsellProductsComboBox->setEditable(false);
    populateSchemaComboBox(schemaUnsellProductsComboBox);
    schemaUnsellProductsLabel->setEnabled(false);
    schemaUnsellProductsComboBox->setEnabled(false);
    schemaUnsellProductsCheckBox = new QCheckBox(unsellProductsFilterPanel);
    connect(schemaUnsellProductsCheckBox, SIGNAL(toggled(bool)), schemaUnsellProductsLabel, SLOT(setEnabled(bool)));
    connect(schemaUnsellProductsCheckBox, SIGNAL(toggled(bool)), schemaUnsellProductsComboBox, SLOT(setEnabled(bool)));

    oldPurchasePriceUnsellProductsLabel = new QLabel(tr("Old Purchase Price: "), unsellProductsFilterPanel);
    oldPurchasePriceUnsellProductsLineEdit = new QLineEdit(unsellProductsFilterPanel);
    oldPurchasePriceUnsellProductsLabel->setEnabled(false);
    oldPurchasePriceUnsellProductsLineEdit->setEnabled(false);
    oldPurchasePriceUnsellProductsCheckBox = new QCheckBox(unsellProductsFilterPanel);
    connect(oldPurchasePriceUnsellProductsCheckBox, SIGNAL(toggled(bool)), oldPurchasePriceUnsellProductsLabel, SLOT(setEnabled(bool)));
    connect(oldPurchasePriceUnsellProductsCheckBox, SIGNAL(toggled(bool)), oldPurchasePriceUnsellProductsLineEdit, SLOT(setEnabled(bool)));

    purchasePriceUnsellProductsLabel = new QLabel(tr("Purchase Price: "), unsellProductsFilterPanel);
    purchasePriceUnsellProductsLineEdit = new QLineEdit(unsellProductsFilterPanel);
    purchasePriceUnsellProductsLabel->setEnabled(false);
    purchasePriceUnsellProductsLineEdit->setEnabled(false);
    purchasePriceUnsellProductsCheckBox = new QCheckBox(unsellProductsFilterPanel);
    connect(purchasePriceUnsellProductsCheckBox, SIGNAL(toggled(bool)), purchasePriceUnsellProductsLabel, SLOT(setEnabled(bool)));
    connect(purchasePriceUnsellProductsCheckBox, SIGNAL(toggled(bool)), purchasePriceUnsellProductsLineEdit, SLOT(setEnabled(bool)));

    sellingPriceUnsellProductsLabel = new QLabel(tr("Selling Price: "), unsellProductsFilterPanel);
    sellingPriceUnsellProductsLineEdit = new QLineEdit(unsellProductsFilterPanel);
    sellingPriceUnsellProductsLabel->setEnabled(false);
    sellingPriceUnsellProductsLineEdit->setEnabled(false);
    sellingPriceUnsellProductsCheckBox = new QCheckBox(unsellProductsFilterPanel);
    connect(sellingPriceUnsellProductsCheckBox, SIGNAL(toggled(bool)), sellingPriceUnsellProductsLabel, SLOT(setEnabled(bool)));
    connect(sellingPriceUnsellProductsCheckBox, SIGNAL(toggled(bool)), sellingPriceUnsellProductsLineEdit, SLOT(setEnabled(bool)));

    commentsUnsellProductsLabel = new QLabel(tr("Comments: "), unsellProductsFilterPanel);
    commentsUnsellProductsLineEdit = new QLineEdit(unsellProductsFilterPanel);
    commentsUnsellProductsLabel->setEnabled(false);
    commentsUnsellProductsLineEdit->setEnabled(false);
    commentsUnsellProductsCheckBox = new QCheckBox(unsellProductsFilterPanel);
    connect(commentsUnsellProductsCheckBox, SIGNAL(toggled(bool)), commentsUnsellProductsLabel, SLOT(setEnabled(bool)));
    connect(commentsUnsellProductsCheckBox, SIGNAL(toggled(bool)), commentsUnsellProductsLineEdit, SLOT(setEnabled(bool)));

    startTimeUnsellProductsLabel = new QLabel(tr("Start Time: "), unsellProductsFilterPanel);
    startTimeUnsellProductsDateTimeEdit = new QDateTimeEdit();
    startTimeUnsellProductsDateTimeEdit->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
    startTimeUnsellProductsDateTimeEdit->setDateTime(QDateTime::currentDateTime());
    startTimeUnsellProductsDateTimeEdit->setCalendarPopup(true);
    startTimeUnsellProductsLabel->setEnabled(false);
    startTimeUnsellProductsDateTimeEdit->setEnabled(false);
    startTimeUnsellProductsCheckBox = new QCheckBox(unsellProductsFilterPanel);
    connect(startTimeUnsellProductsCheckBox, SIGNAL(toggled(bool)), startTimeUnsellProductsLabel, SLOT(setEnabled(bool)));
    connect(startTimeUnsellProductsCheckBox, SIGNAL(toggled(bool)), startTimeUnsellProductsDateTimeEdit, SLOT(setEnabled(bool)));

    endTimeUnsellProductsLabel = new QLabel(tr("End Time: "), unsellProductsFilterPanel);
    endTimeUnsellProductsDateTimeEdit = new QDateTimeEdit();
    endTimeUnsellProductsDateTimeEdit->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
    endTimeUnsellProductsDateTimeEdit->setDateTime(QDateTime::currentDateTime());
    endTimeUnsellProductsDateTimeEdit->setCalendarPopup(true);
    endTimeUnsellProductsLabel->setEnabled(false);
    endTimeUnsellProductsDateTimeEdit->setEnabled(false);
    endTimeUnsellProductsCheckBox = new QCheckBox(unsellProductsFilterPanel);
    connect(endTimeUnsellProductsCheckBox, SIGNAL(toggled(bool)), endTimeUnsellProductsLabel, SLOT(setEnabled(bool)));
    connect(endTimeUnsellProductsCheckBox, SIGNAL(toggled(bool)), endTimeUnsellProductsDateTimeEdit, SLOT(setEnabled(bool)));

    unsellProductsSummaryLabel = new QLabel(tr("Summary: "), unsellProductsFilterPanel);
    unsellProductsSummaryLineEdit = new QLineEdit(unsellProductsFilterPanel);
    unsellProductsSummaryLineEdit->setReadOnly(true);

    unsellProductsFilterButton = new QPushButton(tr("Filter Result"), unsellProductsFilterPanel);
    QFont f;
    f.setBold(true);
    unsellProductsFilterButton->setFont(f);
    connect(unsellProductsFilterButton, SIGNAL(clicked()), this, SLOT(onUnsellProductsFilter()));

    exportUnsellProducts2ExcelButton = new QPushButton(tr("Export2Excel"), unsellProductsFilterPanel);
    exportUnsellProducts2ExcelButton->setFont(f);
    connect(exportUnsellProducts2ExcelButton, SIGNAL(clicked()), this, SLOT(onExportUnsellProducts2Excel()));

    QGridLayout *unsellProductsFilterLayout = new QGridLayout(unsellProductsFilterPanel);
    unsellProductsFilterLayout->addWidget(serialNumberUnsellProductsLabel, 0, 0, Qt::AlignRight);
    unsellProductsFilterLayout->addWidget(serialNumberUnsellProductsLineEdit, 0, 1);
    unsellProductsFilterLayout->addWidget(serialNumberUnsellProductsCheckBox, 0, 2);
    QHBoxLayout *group = new QHBoxLayout();
    group->addWidget(unsellProductsFilterButton);
    group->addWidget(exportUnsellProducts2ExcelButton);
    unsellProductsFilterLayout->addLayout(group, 0, 20, 1, 3);

    unsellProductsFilterLayout->addWidget(productTypeUnsellProductsLabel, 10, 0, Qt::AlignRight);
    unsellProductsFilterLayout->addWidget(productTypeUnsellProductsComboBox, 10, 1);
    unsellProductsFilterLayout->addWidget(productTypeUnsellProductsCheckBox, 10, 2);
    unsellProductsFilterLayout->addWidget(brandNameUnsellProductsLabel, 10, 10, Qt::AlignRight);
    unsellProductsFilterLayout->addWidget(brandNameUnsellProductsComboBox, 10, 11);
    unsellProductsFilterLayout->addWidget(brandNameUnsellProductsCheckBox, 10, 12);
    unsellProductsFilterLayout->addWidget(productModelUnsellProductsLabel, 10, 20, Qt::AlignRight);
    unsellProductsFilterLayout->addWidget(productModelUnsellProductsComboBox, 10, 21);
    unsellProductsFilterLayout->addWidget(productModelUnsellProductsCheckBox, 10, 22);

    unsellProductsFilterLayout->addWidget(productColorUnsellProductsLabel, 15, 0, Qt::AlignRight);
    unsellProductsFilterLayout->addWidget(productColorUnsellProductsComboBox, 15, 1);
    unsellProductsFilterLayout->addWidget(productColorUnsellProductsCheckBox, 15, 2);
    unsellProductsFilterLayout->addWidget(productVendorUnsellProductsLabel, 15, 10, Qt::AlignRight);
    unsellProductsFilterLayout->addWidget(productVendorUnsellProductsComboBox, 15, 11);
    unsellProductsFilterLayout->addWidget(productVendorUnsellProductsCheckBox, 15, 12);
    unsellProductsFilterLayout->addWidget(replacementInfoUnsellProductsLabel, 15, 20, Qt::AlignRight);
    unsellProductsFilterLayout->addWidget(replacementInfoUnsellProductsComboBox, 15, 21);
    unsellProductsFilterLayout->addWidget(replacementInfoUnsellProductsCheckBox, 15, 22);

    unsellProductsFilterLayout->addWidget(quantityUnsellProductsLabel, 20, 0, Qt::AlignRight);
    unsellProductsFilterLayout->addWidget(quantityUnsellProductsSpinBox, 20, 1);
    unsellProductsFilterLayout->addWidget(quantityUnsellProductsCheckBox, 20, 2);
    unsellProductsFilterLayout->addWidget(unitUnsellProductsLabel, 20, 10, Qt::AlignRight);
    unsellProductsFilterLayout->addWidget(unitUnsellProductsLineEdit, 20, 11);
    unsellProductsFilterLayout->addWidget(unitUnsellProductsCheckBox, 20, 12);
    unsellProductsFilterLayout->addWidget(schemaUnsellProductsLabel, 20, 20, Qt::AlignRight);
    unsellProductsFilterLayout->addWidget(schemaUnsellProductsComboBox, 20, 21);
    unsellProductsFilterLayout->addWidget(schemaUnsellProductsCheckBox, 20, 22);

    unsellProductsFilterLayout->addWidget(barInfoUnsellProductsLabel, 23, 0, Qt::AlignRight);
    unsellProductsFilterLayout->addWidget(barInfoUnsellProductsLineEdit, 23, 1);
    unsellProductsFilterLayout->addWidget(barInfoUnsellProductsCheckBox, 23, 2);
    unsellProductsFilterLayout->addWidget(sellerUnsellProductsLabel, 23, 10, Qt::AlignRight);
    unsellProductsFilterLayout->addWidget(sellerUnsellProductsComboBox, 23, 11);
    unsellProductsFilterLayout->addWidget(sellerUnsellProductsCheckBox, 23, 12);
    unsellProductsFilterLayout->addWidget(productStatusUnsellProductsLabel, 23, 20, Qt::AlignRight);
    unsellProductsFilterLayout->addWidget(productStatusUnsellProductsComboBox, 23, 21);
    unsellProductsFilterLayout->addWidget(productStatusUnsellProductsCheckBox, 23, 22);

    unsellProductsFilterLayout->addWidget(oldPurchasePriceUnsellProductsLabel, 30, 0, Qt::AlignRight);
    unsellProductsFilterLayout->addWidget(oldPurchasePriceUnsellProductsLineEdit, 30, 1);
    unsellProductsFilterLayout->addWidget(oldPurchasePriceUnsellProductsCheckBox, 30, 2);
    unsellProductsFilterLayout->addWidget(purchasePriceUnsellProductsLabel, 30, 10, Qt::AlignRight);
    unsellProductsFilterLayout->addWidget(purchasePriceUnsellProductsLineEdit, 30, 11);
    unsellProductsFilterLayout->addWidget(purchasePriceUnsellProductsCheckBox, 30, 12);
    unsellProductsFilterLayout->addWidget(sellingPriceUnsellProductsLabel, 30, 20, Qt::AlignRight);
    unsellProductsFilterLayout->addWidget(sellingPriceUnsellProductsLineEdit, 30, 21);
    unsellProductsFilterLayout->addWidget(sellingPriceUnsellProductsCheckBox, 30, 22);

    unsellProductsFilterLayout->addWidget(commentsUnsellProductsLabel, 36, 0, Qt::AlignRight);
    unsellProductsFilterLayout->addWidget(commentsUnsellProductsLineEdit, 36, 1);
    unsellProductsFilterLayout->addWidget(commentsUnsellProductsCheckBox, 36, 2);
    unsellProductsFilterLayout->addWidget(startTimeUnsellProductsLabel, 36, 10, Qt::AlignRight);
    unsellProductsFilterLayout->addWidget(startTimeUnsellProductsDateTimeEdit, 36, 11);
    unsellProductsFilterLayout->addWidget(startTimeUnsellProductsCheckBox, 36, 12);
    unsellProductsFilterLayout->addWidget(endTimeUnsellProductsLabel, 36, 20, Qt::AlignRight);
    unsellProductsFilterLayout->addWidget(endTimeUnsellProductsDateTimeEdit, 36, 21);
    unsellProductsFilterLayout->addWidget(endTimeUnsellProductsCheckBox, 36, 22);

    unsellProductsFilterLayout->addWidget(unsellProductsSummaryLabel, 50, 0, Qt::AlignRight);
    unsellProductsFilterLayout->addWidget(unsellProductsSummaryLineEdit, 50, 1, 1, unsellProductsFilterLayout->columnCount());

    QFrame *line1 = new QFrame();
    line1->setAttribute(Qt::WA_MouseNoMask);
    line1->setFrameStyle(QFrame::Sunken);
    line1->setFrameShape(QFrame::HLine);
    unsellProductsFilterLayout->addWidget(line1, 5, 0, 1, unsellProductsFilterLayout->columnCount());

    QFrame *line2 = new QFrame();
    line2->setAttribute(Qt::WA_MouseNoMask);
    line2->setFrameStyle(QFrame::Sunken);
    line2->setFrameShape(QFrame::HLine);
    unsellProductsFilterLayout->addWidget(line2, 25, 0, 1, unsellProductsFilterLayout->columnCount());

    QFrame *line3 = new QFrame();
    line3->setAttribute(Qt::WA_MouseNoMask);
    line3->setFrameStyle(QFrame::Sunken);
    line3->setFrameShape(QFrame::HLine);
    unsellProductsFilterLayout->addWidget(line3, 35, 0, 1, unsellProductsFilterLayout->columnCount());

    QFrame *line4 = new QFrame();
    line4->setAttribute(Qt::WA_MouseNoMask);
    line4->setFrameStyle(QFrame::Sunken);
    line4->setFrameShape(QFrame::HLine);
    unsellProductsFilterLayout->addWidget(line4, 45, 0, 1, unsellProductsFilterLayout->columnCount());

    createUnsellProductsPanel();
    unsellProductsFilterLayout->addWidget(unsellProductsPanel, 55, 0, 1, unsellProductsFilterLayout->columnCount());

    unsellProductProgressBar = new QProgressBar();
    unsellProductProgressBar->setValue(0);
    unsellProductsFilterLayout->addWidget(unsellProductProgressBar, 60, 0, 1, unsellProductsFilterLayout->columnCount());

    unsellProductsFilterPanel->setLayout(unsellProductsFilterLayout);

    productTypeUnsellProductsComboBox->setCurrentIndex(
                productTypeUnsellProductsComboBox->findText("其它"));

    brandNameUnsellProductsComboBox->setCurrentIndex(
                brandNameUnsellProductsComboBox->findText("其它"));

    productModelUnsellProductsComboBox->setCurrentIndex(
                productModelUnsellProductsComboBox->findText("其它"));

    productColorUnsellProductsComboBox->setCurrentIndex(
                productColorUnsellProductsComboBox->findText("其它"));

    productVendorUnsellProductsComboBox->setCurrentIndex(
                productVendorUnsellProductsComboBox->findText("其它"));

    replacementInfoUnsellProductsComboBox->setCurrentIndex(
                replacementInfoUnsellProductsComboBox->findText("否"));

    productStatusUnsellProductsComboBox->setCurrentIndex(
                productStatusUnsellProductsComboBox->findText("已删除"));

    sellerUnsellProductsComboBox->setCurrentIndex(
                sellerUnsellProductsComboBox->findText("未指定"));

    schemaUnsellProductsComboBox->setCurrentIndex(
                schemaUnsellProductsComboBox->findText("未指定"));
}

void Statistic_Invoicing::createBarChartPanel()
{
    salesStatisticPlot = new BarChart();

    sortByVolumeAscRadioButton = new QRadioButton(tr("SortByVolumeAsc"));
    sortByVolumeDesRadioButton = new QRadioButton(tr("SortByVolumeDes"));
    sortByVolumeDesRadioButton->setChecked(true);
    sortByQuantityAscRadioButton = new QRadioButton(tr("SortByQuantityAsc"));
    sortByQuantityDesRadioButton = new QRadioButton(tr("SortByQuantityDes"));

    QHBoxLayout *sortTypeHLayout = new QHBoxLayout();
    sortTypeHLayout->addWidget(sortByVolumeAscRadioButton);
    sortTypeHLayout->addWidget(sortByVolumeDesRadioButton);
    sortTypeHLayout->addWidget(sortByQuantityAscRadioButton);
    sortTypeHLayout->addWidget(sortByQuantityDesRadioButton);
    QGroupBox *sortTypeGroup = new QGroupBox(tr("Sort Type"));
    sortTypeGroup->setLayout(sortTypeHLayout);

    QLabel *timeLabel = new QLabel(tr("Start Time :"));
    time = new QDateEdit();
    time->setDisplayFormat("yyyy-MM");
    time->setDateTime(QDateTime::currentDateTime());
    time->setCalendarPopup(true);
    QLabel *timeRangeLabel = new QLabel(tr("Month Range :"));
    timeRange = new QSpinBox();
    timeRange->setMinimum(1);
    QLabel *peopleRangeLabel = new QLabel(tr("People Range :"));
    peopleRange = new QSpinBox();
    peopleRange->setMinimum(1);
    peopleRange->setValue(10);

    QHBoxLayout *sortRangeHLayout = new QHBoxLayout();
    sortRangeHLayout->addWidget(timeLabel);
    sortRangeHLayout->addWidget(time);
    sortRangeHLayout->addWidget(timeRangeLabel);
    sortRangeHLayout->addWidget(timeRange);
    sortRangeHLayout->addWidget(peopleRangeLabel);
    sortRangeHLayout->addWidget(peopleRange);
    QGroupBox *sortRangeGroup = new QGroupBox(tr("Sort Range"));
    sortRangeGroup->setLayout(sortRangeHLayout);

    updateStatisticPushButton = new QPushButton(tr("Update"));
    updateStatisticPushButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    connect(updateStatisticPushButton, SIGNAL(clicked()), this, SLOT(updateSalesStatisticPlot()));
    printStatisticPushButton = new QPushButton(tr("Print"));
    printStatisticPushButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    connect(printStatisticPushButton, SIGNAL(clicked()), salesStatisticPlot, SLOT(printChart()));

    QHBoxLayout *sortHLayout = new QHBoxLayout();
    sortHLayout->addWidget(sortRangeGroup);
    sortHLayout->addWidget(sortTypeGroup);
    sortHLayout->addWidget(updateStatisticPushButton);
    sortHLayout->addWidget(printStatisticPushButton);

    QVBoxLayout *salesStatisticVLayout = new QVBoxLayout();
    //salesStatisticVLayout->addWidget(sortTypeGroup);
    salesStatisticVLayout->addLayout(sortHLayout);
    salesStatisticVLayout->addWidget(salesStatisticPlot);
    salesStatisticContainer = new QWidget();
    salesStatisticContainer->setLayout(salesStatisticVLayout);
}

void Statistic_Invoicing::createAllProductsFilterPanel()
{
    allProductsFilterPanel = new QWidget();

    serialNumberAllProductsLabel = new QLabel(tr("Serial Number: "), allProductsFilterPanel);
    serialNumberAllProductsLineEdit = new QLineEdit(allProductsFilterPanel);
    serialNumberAllProductsLabel->setEnabled(false);
    serialNumberAllProductsLineEdit->setEnabled(false);
    serialNumberAllProductsCheckBox = new QCheckBox(allProductsFilterPanel);
    connect(serialNumberAllProductsCheckBox, SIGNAL(toggled(bool)), serialNumberAllProductsLabel, SLOT(setEnabled(bool)));
    connect(serialNumberAllProductsCheckBox, SIGNAL(toggled(bool)), serialNumberAllProductsLineEdit, SLOT(setEnabled(bool)));

    productTypeAllProductsLabel = new QLabel(tr("Product Type: "), allProductsFilterPanel);
    productTypeAllProductsLabel->setEnabled(false);
    productTypeAllProductsComboBox = new QComboBox(allProductsFilterPanel);
    productTypeAllProductsComboBox->setEnabled(false);
    productTypeAllProductsComboBox->setEditable(true);
    QSqlDatabase db(userManagementInterface->getDatabase());
    productTypeAllProductsModel = new QSqlRelationalTableModel(NULL, db);
    productTypeAllProductsModel->setTable("producttype");
    productTypeAllProductsModel->select();
    productTypeAllProductsComboBox->setModel(productTypeAllProductsModel);
    productTypeAllProductsComboBox->setModelColumn(1);
    productTypeAllProductsCheckBox = new QCheckBox(allProductsFilterPanel);
    connect(productTypeAllProductsCheckBox, SIGNAL(toggled(bool)), productTypeAllProductsLabel, SLOT(setEnabled(bool)));
    connect(productTypeAllProductsCheckBox, SIGNAL(toggled(bool)), productTypeAllProductsComboBox, SLOT(setEnabled(bool)));

    brandNameAllProductsLabel = new QLabel(tr("Brand Name: "), allProductsFilterPanel);
    brandNameAllProductsComboBox = new QComboBox(allProductsFilterPanel);
    brandNameAllProductsComboBox->setEditable(true);
    brandNameAllProductsLabel->setEnabled(false);
    brandNameAllProductsComboBox->setEnabled(false);
    brandNameAllProductsModel = new QSqlRelationalTableModel(NULL, db);
    brandNameAllProductsModel->setTable("brandname");
    brandNameAllProductsModel->select();
    brandNameAllProductsComboBox->setModel(brandNameAllProductsModel);
    brandNameAllProductsComboBox->setModelColumn(1);
    brandNameAllProductsCheckBox = new QCheckBox(allProductsFilterPanel);
    connect(brandNameAllProductsCheckBox, SIGNAL(toggled(bool)), brandNameAllProductsLabel, SLOT(setEnabled(bool)));
    connect(brandNameAllProductsCheckBox, SIGNAL(toggled(bool)), brandNameAllProductsComboBox, SLOT(setEnabled(bool)));

    productModelAllProductsLabel = new QLabel(tr("Product Model: "), allProductsFilterPanel);
    productModelAllProductsLabel->setEnabled(false);
    productModelAllProductsComboBox = new QComboBox(allProductsFilterPanel);
    productModelAllProductsComboBox->setEnabled(false);
    productModelAllProductsComboBox->setEditable(true);
    productModelAllProductsModel = new QSqlRelationalTableModel(NULL, db);
    productModelAllProductsModel->setTable("productmodel");
    productModelAllProductsModel->select();
    productModelAllProductsComboBox->setModel(productModelAllProductsModel);
    productModelAllProductsComboBox->setModelColumn(3);
    productModelAllProductsCheckBox = new QCheckBox(allProductsFilterPanel);
    connect(productModelAllProductsCheckBox, SIGNAL(toggled(bool)), productModelAllProductsLabel, SLOT(setEnabled(bool)));
    connect(productModelAllProductsCheckBox, SIGNAL(toggled(bool)), productModelAllProductsComboBox, SLOT(setEnabled(bool)));

    productColorAllProductsLabel = new QLabel(tr("Product Color: "), allProductsFilterPanel);
    productColorAllProductsLabel->setEnabled(false);
    productColorAllProductsComboBox = new QComboBox(allProductsFilterPanel);
    productColorAllProductsComboBox->setEnabled(false);
    productColorAllProductsComboBox->setEditable(true);
    productColorAllProductsModel = new QSqlRelationalTableModel(NULL, db);
    productColorAllProductsModel->setTable("colorinfo");
    productColorAllProductsModel->select();
    productColorAllProductsComboBox->setModel(productColorAllProductsModel);
    productColorAllProductsComboBox->setModelColumn(1);
    productColorAllProductsCheckBox = new QCheckBox(allProductsFilterPanel);
    connect(productColorAllProductsCheckBox, SIGNAL(toggled(bool)), productColorAllProductsLabel, SLOT(setEnabled(bool)));
    connect(productColorAllProductsCheckBox, SIGNAL(toggled(bool)), productColorAllProductsComboBox, SLOT(setEnabled(bool)));

    productVendorAllProductsLabel = new QLabel(tr("Vendor: "), allProductsFilterPanel);
    productVendorAllProductsLabel->setEnabled(false);
    productVendorAllProductsComboBox = new QComboBox(allProductsFilterPanel);
    productVendorAllProductsComboBox->setEnabled(false);
    productVendorAllProductsComboBox->setEditable(true);
    productVendorAllProductsModel = new QSqlRelationalTableModel(NULL, db);
    productVendorAllProductsModel->setTable("vendorinfo");
    productVendorAllProductsModel->select();
    productVendorAllProductsComboBox->setModel(productVendorAllProductsModel);
    productVendorAllProductsComboBox->setModelColumn(1);
    productVendorAllProductsCheckBox = new QCheckBox(allProductsFilterPanel);
    connect(productVendorAllProductsCheckBox, SIGNAL(toggled(bool)), productVendorAllProductsLabel, SLOT(setEnabled(bool)));
    connect(productVendorAllProductsCheckBox, SIGNAL(toggled(bool)), productVendorAllProductsComboBox, SLOT(setEnabled(bool)));

    replacementInfoAllProductsLabel = new QLabel(tr("Replacement Info: "), allProductsFilterPanel);
    replacementInfoAllProductsLabel->setEnabled(false);
    replacementInfoAllProductsComboBox = new QComboBox(allProductsFilterPanel);
    replacementInfoAllProductsComboBox->setEnabled(false);
    replacementInfoAllProductsComboBox->setEditable(false);
    replacementInfoAllProductsModel = new QSqlRelationalTableModel(NULL, db);
    replacementInfoAllProductsModel->setTable("replacementstatus");
    replacementInfoAllProductsModel->select();
    replacementInfoAllProductsComboBox->setModel(replacementInfoAllProductsModel);
    replacementInfoAllProductsComboBox->setModelColumn(1);
    replacementInfoAllProductsCheckBox = new QCheckBox(allProductsFilterPanel);
    connect(replacementInfoAllProductsCheckBox, SIGNAL(toggled(bool)), replacementInfoAllProductsLabel, SLOT(setEnabled(bool)));
    connect(replacementInfoAllProductsCheckBox, SIGNAL(toggled(bool)), replacementInfoAllProductsComboBox, SLOT(setEnabled(bool)));

    barInfoAllProductsLabel = new QLabel(tr("Bar Info: "), allProductsFilterPanel);
    barInfoAllProductsLineEdit = new QLineEdit(allProductsFilterPanel);
    barInfoAllProductsLabel->setEnabled(false);
    barInfoAllProductsLineEdit->setEnabled(false);
    barInfoAllProductsCheckBox = new QCheckBox(allProductsFilterPanel);
    connect(barInfoAllProductsCheckBox, SIGNAL(toggled(bool)), barInfoAllProductsLabel, SLOT(setEnabled(bool)));
    connect(barInfoAllProductsCheckBox, SIGNAL(toggled(bool)), barInfoAllProductsLineEdit, SLOT(setEnabled(bool)));

    sellerAllProductsLabel = new QLabel(tr("Seller: "), allProductsFilterPanel);
    sellerAllProductsComboBox = new QComboBox(allProductsFilterPanel);
    sellerAllProductsComboBox->setEditable(false);
    populateSellerNameComboBox(sellerAllProductsComboBox);
    sellerAllProductsLabel->setEnabled(false);
    sellerAllProductsComboBox->setEnabled(false);
    sellerAllProductsCheckBox = new QCheckBox(allProductsFilterPanel);
    connect(sellerAllProductsCheckBox, SIGNAL(toggled(bool)), sellerAllProductsLabel, SLOT(setEnabled(bool)));
    connect(sellerAllProductsCheckBox, SIGNAL(toggled(bool)), sellerAllProductsComboBox, SLOT(setEnabled(bool)));

    productStatusAllProductsLabel = new QLabel(tr("Product Status: "), allProductsFilterPanel);
    productStatusAllProductsComboBox = new QComboBox(allProductsFilterPanel);
    productStatusAllProductsLabel->setEnabled(false);
    productStatusAllProductsComboBox->setEnabled(false);
    productStatusAllProductsComboBox->setEditable(false);
    productStatusAllProductsModel = new QSqlRelationalTableModel(NULL, db);
    productStatusAllProductsModel->setTable("productstatus");
    productStatusAllProductsModel->select();
    productStatusAllProductsComboBox->setModel(productStatusAllProductsModel);
    productStatusAllProductsComboBox->setModelColumn(1);
    productStatusAllProductsCheckBox = new QCheckBox(allProductsFilterPanel);
    connect(productStatusAllProductsCheckBox, SIGNAL(toggled(bool)), productStatusAllProductsLabel, SLOT(setEnabled(bool)));
    connect(productStatusAllProductsCheckBox, SIGNAL(toggled(bool)), productStatusAllProductsComboBox, SLOT(setEnabled(bool)));

    quantityAllProductsLabel = new QLabel(tr("Quantity: "), allProductsFilterPanel);
    quantityAllProductsSpinBox = new QSpinBox(allProductsFilterPanel);
    quantityAllProductsSpinBox->setMinimum(1);
    quantityAllProductsSpinBox->setValue(1);
    quantityAllProductsLabel->setEnabled(false);
    quantityAllProductsSpinBox->setEnabled(false);
    quantityAllProductsCheckBox = new QCheckBox(allProductsFilterPanel);
    connect(quantityAllProductsCheckBox, SIGNAL(toggled(bool)), quantityAllProductsLabel, SLOT(setEnabled(bool)));
    connect(quantityAllProductsCheckBox, SIGNAL(toggled(bool)), quantityAllProductsSpinBox, SLOT(setEnabled(bool)));

    unitAllProductsLabel = new QLabel(tr("Unit: "), allProductsFilterPanel);
    unitAllProductsLineEdit = new QLineEdit(allProductsFilterPanel);
    unitAllProductsLabel->setEnabled(false);
    unitAllProductsLineEdit->setEnabled(false);
    unitAllProductsCheckBox = new QCheckBox(allProductsFilterPanel);
    connect(unitAllProductsCheckBox, SIGNAL(toggled(bool)), unitAllProductsLabel, SLOT(setEnabled(bool)));
    connect(unitAllProductsCheckBox, SIGNAL(toggled(bool)), unitAllProductsLineEdit, SLOT(setEnabled(bool)));

    schemaAllProductsLabel = new QLabel(tr("Schema: "), allProductsFilterPanel);
    schemaAllProductsComboBox = new QComboBox(allProductsFilterPanel);
    schemaAllProductsComboBox->setEditable(false);
    populateSchemaComboBox(schemaAllProductsComboBox);
    schemaAllProductsLabel->setEnabled(false);
    schemaAllProductsComboBox->setEnabled(false);
    schemaAllProductsCheckBox = new QCheckBox(allProductsFilterPanel);
    connect(schemaAllProductsCheckBox, SIGNAL(toggled(bool)), schemaAllProductsLabel, SLOT(setEnabled(bool)));
    connect(schemaAllProductsCheckBox, SIGNAL(toggled(bool)), schemaAllProductsComboBox, SLOT(setEnabled(bool)));

    oldPurchasePriceAllProductsLabel = new QLabel(tr("Old Purchase Price: "), allProductsFilterPanel);
    oldPurchasePriceAllProductsLineEdit = new QLineEdit(allProductsFilterPanel);
    oldPurchasePriceAllProductsLabel->setEnabled(false);
    oldPurchasePriceAllProductsLineEdit->setEnabled(false);
    oldPurchasePriceAllProductsCheckBox = new QCheckBox(allProductsFilterPanel);
    connect(oldPurchasePriceAllProductsCheckBox, SIGNAL(toggled(bool)), oldPurchasePriceAllProductsLabel, SLOT(setEnabled(bool)));
    connect(oldPurchasePriceAllProductsCheckBox, SIGNAL(toggled(bool)), oldPurchasePriceAllProductsLineEdit, SLOT(setEnabled(bool)));

    purchasePriceAllProductsLabel = new QLabel(tr("Purchase Price: "), allProductsFilterPanel);
    purchasePriceAllProductsLineEdit = new QLineEdit(allProductsFilterPanel);
    purchasePriceAllProductsLabel->setEnabled(false);
    purchasePriceAllProductsLineEdit->setEnabled(false);
    purchasePriceAllProductsCheckBox = new QCheckBox(allProductsFilterPanel);
    connect(purchasePriceAllProductsCheckBox, SIGNAL(toggled(bool)), purchasePriceAllProductsLabel, SLOT(setEnabled(bool)));
    connect(purchasePriceAllProductsCheckBox, SIGNAL(toggled(bool)), purchasePriceAllProductsLineEdit, SLOT(setEnabled(bool)));

    sellingPriceAllProductsLabel = new QLabel(tr("Selling Price: "), allProductsFilterPanel);
    sellingPriceAllProductsLineEdit = new QLineEdit(allProductsFilterPanel);
    sellingPriceAllProductsLabel->setEnabled(false);
    sellingPriceAllProductsLineEdit->setEnabled(false);
    sellingPriceAllProductsCheckBox = new QCheckBox(allProductsFilterPanel);
    connect(sellingPriceAllProductsCheckBox, SIGNAL(toggled(bool)), sellingPriceAllProductsLabel, SLOT(setEnabled(bool)));
    connect(sellingPriceAllProductsCheckBox, SIGNAL(toggled(bool)), sellingPriceAllProductsLineEdit, SLOT(setEnabled(bool)));

    commentsAllProductsLabel = new QLabel(tr("Comments: "), allProductsFilterPanel);
    commentsAllProductsLineEdit = new QLineEdit(allProductsFilterPanel);
    commentsAllProductsLabel->setEnabled(false);
    commentsAllProductsLineEdit->setEnabled(false);
    commentsAllProductsCheckBox = new QCheckBox(allProductsFilterPanel);
    connect(commentsAllProductsCheckBox, SIGNAL(toggled(bool)), commentsAllProductsLabel, SLOT(setEnabled(bool)));
    connect(commentsAllProductsCheckBox, SIGNAL(toggled(bool)), commentsAllProductsLineEdit, SLOT(setEnabled(bool)));

    startTimeAllProductsLabel = new QLabel(tr("Start Time: "), allProductsFilterPanel);
    startTimeAllProductsDateTimeEdit = new QDateTimeEdit();
    startTimeAllProductsDateTimeEdit->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
    startTimeAllProductsDateTimeEdit->setDateTime(QDateTime::currentDateTime());
    startTimeAllProductsDateTimeEdit->setCalendarPopup(true);
    startTimeAllProductsLabel->setEnabled(false);
    startTimeAllProductsDateTimeEdit->setEnabled(false);
    startTimeAllProductsCheckBox = new QCheckBox(allProductsFilterPanel);
    connect(startTimeAllProductsCheckBox, SIGNAL(toggled(bool)), startTimeAllProductsLabel, SLOT(setEnabled(bool)));
    connect(startTimeAllProductsCheckBox, SIGNAL(toggled(bool)), startTimeAllProductsDateTimeEdit, SLOT(setEnabled(bool)));

    endTimeAllProductsLabel = new QLabel(tr("End Time: "), allProductsFilterPanel);
    endTimeAllProductsDateTimeEdit = new QDateTimeEdit();
    endTimeAllProductsDateTimeEdit->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
    endTimeAllProductsDateTimeEdit->setDateTime(QDateTime::currentDateTime());
    endTimeAllProductsDateTimeEdit->setCalendarPopup(true);
    endTimeAllProductsLabel->setEnabled(false);
    endTimeAllProductsDateTimeEdit->setEnabled(false);
    endTimeAllProductsCheckBox = new QCheckBox(allProductsFilterPanel);
    connect(endTimeAllProductsCheckBox, SIGNAL(toggled(bool)), endTimeAllProductsLabel, SLOT(setEnabled(bool)));
    connect(endTimeAllProductsCheckBox, SIGNAL(toggled(bool)), endTimeAllProductsDateTimeEdit, SLOT(setEnabled(bool)));

    allProductsSummaryLabel = new QLabel(tr("Summary: "), allProductsFilterPanel);
    allProductsSummaryLineEdit = new QLineEdit(allProductsFilterPanel);
    allProductsSummaryLineEdit->setReadOnly(true);

    allProductsFilterButton = new QPushButton(tr("Filter Result"), allProductsFilterPanel);
    QFont f;
    f.setBold(true);
    allProductsFilterButton->setFont(f);
    connect(allProductsFilterButton, SIGNAL(clicked()), this, SLOT(onAllProductsFilter()));

    exportAllProducts2ExcelButton = new QPushButton(tr("Export2Excel"), allProductsFilterPanel);
    exportAllProducts2ExcelButton->setFont(f);
    connect(exportAllProducts2ExcelButton, SIGNAL(clicked()), this, SLOT(onExportAllProducts2Excel()));

    QGridLayout *allProductsFilterLayout = new QGridLayout(allProductsFilterPanel);
    allProductsFilterLayout->addWidget(serialNumberAllProductsLabel, 0, 0, Qt::AlignRight);
    allProductsFilterLayout->addWidget(serialNumberAllProductsLineEdit, 0, 1);
    allProductsFilterLayout->addWidget(serialNumberAllProductsCheckBox, 0, 2);
    QHBoxLayout *group = new QHBoxLayout();
    group->addWidget(allProductsFilterButton);
    group->addWidget(exportAllProducts2ExcelButton);
    allProductsFilterLayout->addLayout(group, 0, 20, 1, 3);

    allProductsFilterLayout->addWidget(productTypeAllProductsLabel, 10, 0, Qt::AlignRight);
    allProductsFilterLayout->addWidget(productTypeAllProductsComboBox, 10, 1);
    allProductsFilterLayout->addWidget(productTypeAllProductsCheckBox, 10, 2);
    allProductsFilterLayout->addWidget(brandNameAllProductsLabel, 10, 10, Qt::AlignRight);
    allProductsFilterLayout->addWidget(brandNameAllProductsComboBox, 10, 11);
    allProductsFilterLayout->addWidget(brandNameAllProductsCheckBox, 10, 12);
    allProductsFilterLayout->addWidget(productModelAllProductsLabel, 10, 20, Qt::AlignRight);
    allProductsFilterLayout->addWidget(productModelAllProductsComboBox, 10, 21);
    allProductsFilterLayout->addWidget(productModelAllProductsCheckBox, 10, 22);

    allProductsFilterLayout->addWidget(productColorAllProductsLabel, 15, 0, Qt::AlignRight);
    allProductsFilterLayout->addWidget(productColorAllProductsComboBox, 15, 1);
    allProductsFilterLayout->addWidget(productColorAllProductsCheckBox, 15, 2);
    allProductsFilterLayout->addWidget(productVendorAllProductsLabel, 15, 10, Qt::AlignRight);
    allProductsFilterLayout->addWidget(productVendorAllProductsComboBox, 15, 11);
    allProductsFilterLayout->addWidget(productVendorAllProductsCheckBox, 15, 12);
    allProductsFilterLayout->addWidget(replacementInfoAllProductsLabel, 15, 20, Qt::AlignRight);
    allProductsFilterLayout->addWidget(replacementInfoAllProductsComboBox, 15, 21);
    allProductsFilterLayout->addWidget(replacementInfoAllProductsCheckBox, 15, 22);

    allProductsFilterLayout->addWidget(quantityAllProductsLabel, 20, 0, Qt::AlignRight);
    allProductsFilterLayout->addWidget(quantityAllProductsSpinBox, 20, 1);
    allProductsFilterLayout->addWidget(quantityAllProductsCheckBox, 20, 2);
    allProductsFilterLayout->addWidget(unitAllProductsLabel, 20, 10, Qt::AlignRight);
    allProductsFilterLayout->addWidget(unitAllProductsLineEdit, 20, 11);
    allProductsFilterLayout->addWidget(unitAllProductsCheckBox, 20, 12);
    allProductsFilterLayout->addWidget(schemaAllProductsLabel, 20, 20, Qt::AlignRight);
    allProductsFilterLayout->addWidget(schemaAllProductsComboBox, 20, 21);
    allProductsFilterLayout->addWidget(schemaAllProductsCheckBox, 20, 22);

    allProductsFilterLayout->addWidget(barInfoAllProductsLabel, 23, 0, Qt::AlignRight);
    allProductsFilterLayout->addWidget(barInfoAllProductsLineEdit, 23, 1);
    allProductsFilterLayout->addWidget(barInfoAllProductsCheckBox, 23, 2);
    allProductsFilterLayout->addWidget(sellerAllProductsLabel, 23, 10, Qt::AlignRight);
    allProductsFilterLayout->addWidget(sellerAllProductsComboBox, 23, 11);
    allProductsFilterLayout->addWidget(sellerAllProductsCheckBox, 23, 12);
    allProductsFilterLayout->addWidget(productStatusAllProductsLabel, 23, 20, Qt::AlignRight);
    allProductsFilterLayout->addWidget(productStatusAllProductsComboBox, 23, 21);
    allProductsFilterLayout->addWidget(productStatusAllProductsCheckBox, 23, 22);

    allProductsFilterLayout->addWidget(oldPurchasePriceAllProductsLabel, 30, 0, Qt::AlignRight);
    allProductsFilterLayout->addWidget(oldPurchasePriceAllProductsLineEdit, 30, 1);
    allProductsFilterLayout->addWidget(oldPurchasePriceAllProductsCheckBox, 30, 2);
    allProductsFilterLayout->addWidget(purchasePriceAllProductsLabel, 30, 10, Qt::AlignRight);
    allProductsFilterLayout->addWidget(purchasePriceAllProductsLineEdit, 30, 11);
    allProductsFilterLayout->addWidget(purchasePriceAllProductsCheckBox, 30, 12);
    allProductsFilterLayout->addWidget(sellingPriceAllProductsLabel, 30, 20, Qt::AlignRight);
    allProductsFilterLayout->addWidget(sellingPriceAllProductsLineEdit, 30, 21);
    allProductsFilterLayout->addWidget(sellingPriceAllProductsCheckBox, 30, 22);

    allProductsFilterLayout->addWidget(commentsAllProductsLabel, 36, 0, Qt::AlignRight);
    allProductsFilterLayout->addWidget(commentsAllProductsLineEdit, 36, 1);
    allProductsFilterLayout->addWidget(commentsAllProductsCheckBox, 36, 2);
    allProductsFilterLayout->addWidget(startTimeAllProductsLabel, 36, 10, Qt::AlignRight);
    allProductsFilterLayout->addWidget(startTimeAllProductsDateTimeEdit, 36, 11);
    allProductsFilterLayout->addWidget(startTimeAllProductsCheckBox, 36, 12);
    allProductsFilterLayout->addWidget(endTimeAllProductsLabel, 36, 20, Qt::AlignRight);
    allProductsFilterLayout->addWidget(endTimeAllProductsDateTimeEdit, 36, 21);
    allProductsFilterLayout->addWidget(endTimeAllProductsCheckBox, 36, 22);

    allProductsFilterLayout->addWidget(allProductsSummaryLabel, 50, 0, Qt::AlignRight);
    allProductsFilterLayout->addWidget(allProductsSummaryLineEdit, 50, 1, 1, allProductsFilterLayout->columnCount());

    QFrame *line1 = new QFrame();
    line1->setAttribute(Qt::WA_MouseNoMask);
    line1->setFrameStyle(QFrame::Sunken);
    line1->setFrameShape(QFrame::HLine);
    allProductsFilterLayout->addWidget(line1, 5, 0, 1, allProductsFilterLayout->columnCount());

    QFrame *line2 = new QFrame();
    line2->setAttribute(Qt::WA_MouseNoMask);
    line2->setFrameStyle(QFrame::Sunken);
    line2->setFrameShape(QFrame::HLine);
    allProductsFilterLayout->addWidget(line2, 25, 0, 1, allProductsFilterLayout->columnCount());

    QFrame *line3 = new QFrame();
    line3->setAttribute(Qt::WA_MouseNoMask);
    line3->setFrameStyle(QFrame::Sunken);
    line3->setFrameShape(QFrame::HLine);
    allProductsFilterLayout->addWidget(line3, 35, 0, 1, allProductsFilterLayout->columnCount());

    QFrame *line4 = new QFrame();
    line4->setAttribute(Qt::WA_MouseNoMask);
    line4->setFrameStyle(QFrame::Sunken);
    line4->setFrameShape(QFrame::HLine);
    allProductsFilterLayout->addWidget(line4, 45, 0, 1, allProductsFilterLayout->columnCount());

    createAllProductsPanel();
    allProductsFilterLayout->addWidget(allProductsPanel, 55, 0, 1, allProductsFilterLayout->columnCount());

    allProductProgressBar = new QProgressBar();
    allProductProgressBar->setValue(0);
    allProductsFilterLayout->addWidget(allProductProgressBar, 60, 0, 1, allProductsFilterLayout->columnCount());

    allProductsFilterPanel->setLayout(allProductsFilterLayout);
}

void Statistic_Invoicing::createFilterPanel()
{
    filterPanel = new QTabWidget();
    createUnsellProductsFilterPanel();
    createAllProductsFilterPanel();
    createBarChartPanel();

    filterPanel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

    filterPanel->addTab(unsellProductsFilterPanel, tr("Unsell Products Query"));
    filterPanel->addTab(allProductsFilterPanel, tr("All Products Info Query"));
    filterPanel->addTab(salesStatisticContainer, tr("Sales Statistic"));
}

void Statistic_Invoicing::updateDBTableModel()
{
    QString productTypeUnsell = productTypeUnsellProductsComboBox->currentText();
    QString brandNameUnsell = brandNameUnsellProductsComboBox->currentText();
    QString productModelUnsell = productModelUnsellProductsComboBox->currentText();
    QString productColorUnsell = productColorUnsellProductsComboBox->currentText();
    QString productVendorUnsell = productVendorUnsellProductsComboBox->currentText();
    QString replacementInfoUnsell = replacementInfoUnsellProductsComboBox->currentText();
    QString productStatusUnsell = productStatusUnsellProductsComboBox->currentText();
    QString sellerUnsell = sellerUnsellProductsComboBox->currentText();
    QString schemaUnsell = schemaUnsellProductsComboBox->currentText();
    QString productTypeAll = productTypeAllProductsComboBox->currentText();
    QString brandNameAll = brandNameAllProductsComboBox->currentText();
    QString productModelAll = productModelAllProductsComboBox->currentText();
    QString productColorAll = productColorAllProductsComboBox->currentText();
    QString productVendorAll = productVendorAllProductsComboBox->currentText();
    QString replacementInfoAll = replacementInfoAllProductsComboBox->currentText();
    QString productStatusAll = productStatusAllProductsComboBox->currentText();
    QString sellerAll = sellerAllProductsComboBox->currentText();
    QString schemaAll = schemaAllProductsComboBox->currentText();

    productTypeUnsellProductsModel->select();
    brandNameUnsellProductsModel->select();
    productModelUnsellProductsModel->select();
    productColorUnsellProductsModel->select();
    productVendorUnsellProductsModel->select();
    replacementInfoUnsellProductsModel->select();
    productStatusUnsellProductsModel->select();
    productTypeAllProductsModel->select();
    brandNameAllProductsModel->select();
    productModelAllProductsModel->select();
    productColorAllProductsModel->select();
    productVendorAllProductsModel->select();
    replacementInfoAllProductsModel->select();
    productStatusAllProductsModel->select();
    populateSellerNameComboBox(sellerUnsellProductsComboBox);
    populateSchemaComboBox(schemaUnsellProductsComboBox);
    populateSellerNameComboBox(sellerAllProductsComboBox);
    populateSchemaComboBox(schemaAllProductsComboBox);

    if(!productTypeUnsell.simplified().isEmpty()) {
        productTypeUnsellProductsComboBox->setCurrentIndex(
                    productTypeUnsellProductsComboBox->findText(productTypeUnsell));
    }
    if(!brandNameUnsell.simplified().isEmpty()) {
        brandNameUnsellProductsComboBox->setCurrentIndex(
                    brandNameUnsellProductsComboBox->findText(brandNameUnsell));
    }
    if(!productModelUnsell.simplified().isEmpty()) {
        productModelUnsellProductsComboBox->setCurrentIndex(
                    productModelUnsellProductsComboBox->findText(productModelUnsell));
    }
    if(!productColorUnsell.simplified().isEmpty()) {
        productColorUnsellProductsComboBox->setCurrentIndex(
                    productColorUnsellProductsComboBox->findText(productColorUnsell));
    }
    if(!productVendorUnsell.simplified().isEmpty()) {
        productVendorUnsellProductsComboBox->setCurrentIndex(
                    productVendorUnsellProductsComboBox->findText(productVendorUnsell));
    }
    if(!replacementInfoUnsell.simplified().isEmpty()) {
        replacementInfoUnsellProductsComboBox->setCurrentIndex(
                    replacementInfoUnsellProductsComboBox->findText(replacementInfoUnsell));
    }
    if(!productStatusUnsell.simplified().isEmpty()) {
        productStatusUnsellProductsComboBox->setCurrentIndex(
                    productStatusUnsellProductsComboBox->findText(productStatusUnsell));
    }
    if(!sellerUnsell.simplified().isEmpty()) {
        sellerUnsellProductsComboBox->setCurrentIndex(
                    sellerUnsellProductsComboBox->findText(sellerUnsell));
    }
    if(!schemaUnsell.simplified().isEmpty()) {
        schemaUnsellProductsComboBox->setCurrentIndex(
                    schemaUnsellProductsComboBox->findText(schemaUnsell));
    }
    if(!productTypeAll.simplified().isEmpty()) {
        productTypeAllProductsComboBox->setCurrentIndex(
                    productTypeAllProductsComboBox->findText(productTypeAll));
    }
    if(!brandNameAll.simplified().isEmpty()) {
        brandNameAllProductsComboBox->setCurrentIndex(
                    brandNameAllProductsComboBox->findText(brandNameAll));
    }
    if(!productModelAll.simplified().isEmpty()) {
        productModelAllProductsComboBox->setCurrentIndex(
                    productModelAllProductsComboBox->findText(productModelAll));
    }
    if(!productColorAll.simplified().isEmpty()) {
        productColorAllProductsComboBox->setCurrentIndex(
                    productColorAllProductsComboBox->findText(productColorAll));
    }
    if(!productVendorAll.simplified().isEmpty()) {
        productVendorAllProductsComboBox->setCurrentIndex(
                    productVendorAllProductsComboBox->findText(productVendorAll));
    }
    if(!replacementInfoAll.simplified().isEmpty()) {
        replacementInfoAllProductsComboBox->setCurrentIndex(
                    replacementInfoAllProductsComboBox->findText(replacementInfoAll));
    }
    if(!productStatusAll.simplified().isEmpty()) {
        productStatusAllProductsComboBox->setCurrentIndex(
                    productStatusAllProductsComboBox->findText(productStatusAll));
    }
    if(!sellerAll.simplified().isEmpty()) {
        sellerAllProductsComboBox->setCurrentIndex(
                    sellerAllProductsComboBox->findText(sellerAll));
    }
    if(!schemaAll.simplified().isEmpty()) {
        schemaAllProductsComboBox->setCurrentIndex(
                    schemaAllProductsComboBox->findText(schemaAll));
    }
}

void Statistic_Invoicing::hidePurchasePrice()
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
    unsellProductsView->setColumnHidden(OldPurchasePrice, hide);
    unsellProductsView->setColumnHidden(PurchasePrice, hide);
    allProductsView->setColumnHidden(OldPurchasePrice, hide);
    allProductsView->setColumnHidden(PurchasePrice, hide);

    if(!statisticUpdateUnsellProductDialog) {
        statisticUpdateUnsellProductDialog = new StatisticUpdateProductDialog(userManagementInterface,
                                                      productManagementInterface);
    }
    statisticUpdateUnsellProductDialog->hidePurchasePrice(hide);
    if(!statisticUpdateAllProductDialog) {
        statisticUpdateAllProductDialog = new StatisticUpdateProductDialog(userManagementInterface,
                                                      productManagementInterface);
    }
    statisticUpdateAllProductDialog->hidePurchasePrice(hide);

    if(hide) {
        oldPurchasePriceAllProductsLabel->hide();
        oldPurchasePriceAllProductsLineEdit->hide();
        oldPurchasePriceAllProductsCheckBox->hide();
        purchasePriceAllProductsLabel->hide();
        purchasePriceAllProductsLineEdit->hide();
        purchasePriceAllProductsCheckBox->hide();
        oldPurchasePriceUnsellProductsLabel->hide();
        oldPurchasePriceUnsellProductsLineEdit->hide();
        oldPurchasePriceUnsellProductsCheckBox->hide();
        purchasePriceUnsellProductsLabel->hide();
        purchasePriceUnsellProductsLineEdit->hide();
        purchasePriceUnsellProductsCheckBox->hide();
    }
}

void Statistic_Invoicing::setProductModelFilter()
{
    //we want to make all model avaliable, so no filter on table productmodel
    productModelAllProductsModel->select();
}

void Statistic_Invoicing::populateSchemaComboBox(QComboBox *schemaComboBox) const
{
    QSet<QString> schemaNameSet = userManagementInterface->getAllSchemaName();;

    schemaComboBox->clear();
    schemaComboBox->addItems(schemaNameSet.values());
}

void Statistic_Invoicing::onAllProductsFilter()
{
    allProductsSummaryLineEdit->clear();

    QString allProductsFilter("");
    if(serialNumberAllProductsCheckBox->isChecked()) {
        QString serialNumber = serialNumberAllProductsLineEdit->text().simplified();
        if(!serialNumber.isEmpty()) {
            QString filter = QString("serialNumber like '%%1%'").arg(serialNumber);
            if(allProductsFilter.isEmpty()) {
                allProductsFilter += filter;
            }
            else {
                allProductsFilter +=  " and " + filter;
            }
        }
    }

    if(productTypeAllProductsCheckBox->isChecked()) {
        QString productType = productTypeAllProductsComboBox->currentText().simplified();
        //Qt bug, refer to
        //qt.onyou.ch/2010/06/20/filtering-relational-tables-in-qsqlrelationaltablemodel/
        QString filter = QString("relTblAl_%1.name = '%2'").arg(ProductTypeID).arg(productType);
        if(allProductsFilter.isEmpty()) {
            allProductsFilter += filter;
        }
        else {
            allProductsFilter +=  " and " + filter;
        }
    }

    if(brandNameAllProductsCheckBox->isChecked()) {
        QString brandName = brandNameAllProductsComboBox->currentText().simplified();
        QString filter = QString("relTblAl_%1.name = '%2'").arg(BrandNameID).arg(brandName);
        if(allProductsFilter.isEmpty()) {
            allProductsFilter += filter;
        }
        else {
            allProductsFilter += " and " + filter;
        }
    }

    if(productModelAllProductsCheckBox->isChecked()) {
        QString productModel = productModelAllProductsComboBox->currentText().simplified();
        QString filter = QString("relTblAl_%1.model = '%2'").arg(ProductModelID).arg(productModel);
        if(allProductsFilter.isEmpty()) {
            allProductsFilter += filter;
        }
        else {
            allProductsFilter += " and " + filter;
        }
    }

    if(productColorAllProductsCheckBox->isChecked()) {
        QString productColor = productColorAllProductsComboBox->currentText().simplified();
        QString filter = QString("relTblAl_%1.color = '%2'").arg(ColorID).arg(productColor);
        if(allProductsFilter.isEmpty()) {
            allProductsFilter += filter;
        }
        else {
            allProductsFilter += " and " + filter;
        }
    }

    if(productVendorAllProductsCheckBox->isChecked()) {
        QString productVendor = productVendorAllProductsComboBox->currentText().simplified();
        QString filter = QString("relTblAl_%1.name = '%2'").arg(VendorID).arg(productVendor);
        if(allProductsFilter.isEmpty()) {
            allProductsFilter += filter;
        }
        else {
            allProductsFilter += " and " + filter;
        }
    }

    if(replacementInfoAllProductsCheckBox->isChecked()) {
        QString replacmentInof = replacementInfoAllProductsComboBox->currentText().simplified();
        QString filter = QString("relTblAl_%1.status = '%2'").arg(ReplacementStatusID).arg(replacmentInof);
        if(allProductsFilter.isEmpty()) {
            allProductsFilter += filter;
        }
        else {
            allProductsFilter += " and " + filter;
        }
    }

    if(quantityAllProductsCheckBox->isChecked()) {
        int quantity = quantityAllProductsSpinBox->value();
        QString filter = QString("quantity = %1").arg(quantity);
        if(allProductsFilter.isEmpty()) {
            allProductsFilter += filter;
        }
        else {
            allProductsFilter +=  " and " + filter;
        }
    }

    if(unitAllProductsCheckBox->isChecked()) {
        QString unit = unitAllProductsLineEdit->text().simplified();
        if(!unit.isEmpty()) {
            QString filter = QString("unit = '%1'").arg(unit);
            if(allProductsFilter.isEmpty()) {
                allProductsFilter += filter;
            }
            else {
                allProductsFilter +=  " and " + filter;
            }
        }
    }

    if(schemaAllProductsCheckBox->isChecked()) {
        QString schema = schemaAllProductsComboBox->currentText().simplified();
        QString filter = QString("relTblAl_%1.name = '%2'").arg(SchemaNameID).arg(schema);
        if(allProductsFilter.isEmpty()) {
            allProductsFilter += filter;
        }
        else {
            allProductsFilter += " and " + filter;
        }
    }

    if(barInfoAllProductsCheckBox->isChecked()) {
        QString barInfo = barInfoAllProductsLineEdit->text().simplified();
        if(!barInfo.isEmpty()) {
            QString filter = QString("barInfo like '%%1%'").arg(barInfo);
            if(allProductsFilter.isEmpty()) {
                allProductsFilter += filter;
            }
            else {
                allProductsFilter +=  " and " + filter;
            }
        }
    }

    if(sellerAllProductsCheckBox->isChecked()) {
        QString seller = sellerAllProductsComboBox->currentText().simplified();
        QString filter = QString("relTblAl_%1.name = '%2'").arg(SellerID).arg(seller);
        if(allProductsFilter.isEmpty()) {
            allProductsFilter += filter;
        }
        else {
            allProductsFilter += " and " + filter;
        }
    }

    if(productStatusAllProductsCheckBox->isChecked()) {
        QString productStatus = productStatusAllProductsComboBox->currentText().simplified();
        QString filter = QString("relTblAl_%1.status = '%2'").arg(ProductStatusID).arg(productStatus);
        if(allProductsFilter.isEmpty()) {
            allProductsFilter += filter;
        }
        else {
            allProductsFilter += " and " + filter;
        }
    }

    if(oldPurchasePriceAllProductsCheckBox->isChecked()) {
        QString oldPurchasePrice = oldPurchasePriceAllProductsLineEdit->text().simplified();
        if(!oldPurchasePrice.isEmpty()) {
            QString filter = QString("oldPurchasePrice = '%1'").arg(oldPurchasePrice);
            if(allProductsFilter.isEmpty()) {
                allProductsFilter += filter;
            }
            else {
                allProductsFilter +=  " and " + filter;
            }
        }
    }

    if(purchasePriceAllProductsCheckBox->isChecked()) {
        QString purchasePrice = purchasePriceAllProductsLineEdit->text().simplified();
        if(!purchasePrice.isEmpty()) {
            QString filter = QString("purchasePrice = '%1'").arg(purchasePrice);
            if(allProductsFilter.isEmpty()) {
                allProductsFilter += filter;
            }
            else {
                allProductsFilter +=  " and " + filter;
            }
        }
    }


    if(sellingPriceAllProductsCheckBox->isChecked()) {
        QString sellingPrice = sellingPriceAllProductsLineEdit->text().simplified();
        if(!sellingPrice.isEmpty()) {
            QString filter = QString("sellingPrice = '%1'").arg(sellingPrice);
            if(allProductsFilter.isEmpty()) {
                allProductsFilter += filter;
            }
            else {
                allProductsFilter +=  " and " + filter;
            }
        }
    }

    if(commentsAllProductsCheckBox->isChecked()) {
        QString comments = commentsAllProductsLineEdit->text().simplified();
        if(!comments.isEmpty()) {
            QString filter = QString("comments like '%%1%'").arg(comments);
            if(allProductsFilter.isEmpty()) {
                allProductsFilter += filter;
            }
            else {
                allProductsFilter +=  " and " + filter;
            }
        }
    }

    if(startTimeAllProductsCheckBox->isChecked()) {
        QDateTime startTime = startTimeAllProductsDateTimeEdit->dateTime();
        QString start = startTime.toString("yyyy-MM-dd hh:mm:ss");
        if(!start.isEmpty()) {
            QString filter = QString("timeStamp >= '%1'").arg(start);
            if(allProductsFilter.isEmpty()) {
                allProductsFilter += filter;
            }
            else {
                allProductsFilter +=  " and " + filter;
            }
        }
    }

    if(endTimeAllProductsCheckBox->isChecked()) {
        QDateTime endTime = endTimeAllProductsDateTimeEdit->dateTime();
        QString end = endTime.toString("yyyy-MM-dd hh:mm:ss");
        if(!end.isEmpty()) {
            QString filter = QString("timeStamp <= '%1'").arg(end);
            if(allProductsFilter.isEmpty()) {
                allProductsFilter += filter;
            }
            else {
                allProductsFilter +=  " and " + filter;
            }
        }
    }

    mainWidget->setCursor(Qt::BusyCursor);
    allProductsModel->setFilter(allProductsFilter);
    allProductsModel->select();
//    qDebug()<<statisticModel->filter();
//    qDebug()<<statisticModel->query().lastQuery();
    allProductsView->resizeColumnsToContents();

    int rows = allProductsModel->rowCount();
    int totalQuantity = 0;
    double totalSelling = 0;
    QSqlRecord record;
    for(int i=0; i<rows; i++) {
        record = allProductsModel->record(i);
        totalQuantity += record.value(Quantity).toInt();
        QString selling = record.value(SellingPrice).toString().simplified();
        //qDebug()<<selling<<selling.toULongLong();
        if(!selling.isEmpty()) {
            totalSelling += selling.toDouble();
        }
    }
    allProductsSummaryLineEdit->setText(QString(tr("Numbers of Total Products: "))
                                        + QString("%1  ").arg(totalQuantity)
                                        + QString(tr("Numbers of Total Selling: "))
                                        + QString("%1").arg((qulonglong)totalSelling));

    mainWidget->unsetCursor();
}

void Statistic_Invoicing::onUnsellProductsFilter()
{
    unsellProductsSummaryLineEdit->clear();

    QString unsellProductsFilter("");
    if(serialNumberUnsellProductsCheckBox->isChecked()) {
        QString serialNumber = serialNumberUnsellProductsLineEdit->text().simplified();
        if(!serialNumber.isEmpty()) {
            QString filter = QString("serialNumber like '%%1%'").arg(serialNumber);
            if(unsellProductsFilter.isEmpty()) {
                unsellProductsFilter += filter;
            }
            else {
                unsellProductsFilter +=  " and " + filter;
            }
        }
    }

    if(productTypeUnsellProductsCheckBox->isChecked()) {
        QString productType = productTypeUnsellProductsComboBox->currentText().simplified();
        //Qt bug, refer to
        //qt.onyou.ch/2010/06/20/filtering-relational-tables-in-qsqlrelationaltablemodel/
        QString filter = QString("relTblAl_%1.name = '%2'").arg(ProductTypeID).arg(productType);
        if(unsellProductsFilter.isEmpty()) {
            unsellProductsFilter += filter;
        }
        else {
            unsellProductsFilter +=  " and " + filter;
        }
    }

    if(brandNameUnsellProductsCheckBox->isChecked()) {
        QString brandName = brandNameUnsellProductsComboBox->currentText().simplified();
        QString filter = QString("relTblAl_%1.name = '%2'").arg(BrandNameID).arg(brandName);
        if(unsellProductsFilter.isEmpty()) {
            unsellProductsFilter += filter;
        }
        else {
            unsellProductsFilter += " and " + filter;
        }
    }

    if(productModelUnsellProductsCheckBox->isChecked()) {
        QString productModel = productModelUnsellProductsComboBox->currentText().simplified();
        QString filter = QString("relTblAl_%1.model = '%2'").arg(ProductModelID).arg(productModel);
        if(unsellProductsFilter.isEmpty()) {
            unsellProductsFilter += filter;
        }
        else {
            unsellProductsFilter += " and " + filter;
        }
    }

    if(productColorUnsellProductsCheckBox->isChecked()) {
        QString productColor = productColorUnsellProductsComboBox->currentText().simplified();
        QString filter = QString("relTblAl_%1.color = '%2'").arg(ColorID).arg(productColor);
        if(unsellProductsFilter.isEmpty()) {
            unsellProductsFilter += filter;
        }
        else {
            unsellProductsFilter += " and " + filter;
        }
    }

    if(productVendorUnsellProductsCheckBox->isChecked()) {
        QString productVendor = productVendorUnsellProductsComboBox->currentText().simplified();
        QString filter = QString("relTblAl_%1.name = '%2'").arg(VendorID).arg(productVendor);
        if(unsellProductsFilter.isEmpty()) {
            unsellProductsFilter += filter;
        }
        else {
            unsellProductsFilter += " and " + filter;
        }
    }

    if(replacementInfoUnsellProductsCheckBox->isChecked()) {
        QString replacmentInof = replacementInfoUnsellProductsComboBox->currentText().simplified();
        QString filter = QString("relTblAl_%1.status = '%2'").arg(ReplacementStatusID).arg(replacmentInof);
        if(unsellProductsFilter.isEmpty()) {
            unsellProductsFilter += filter;
        }
        else {
            unsellProductsFilter += " and " + filter;
        }
    }

    if(quantityUnsellProductsCheckBox->isChecked()) {
        int quantity = quantityUnsellProductsSpinBox->value();
        QString filter = QString("quantity = %1").arg(quantity);
        if(unsellProductsFilter.isEmpty()) {
            unsellProductsFilter += filter;
        }
        else {
            unsellProductsFilter +=  " and " + filter;
        }
    }

    if(unitUnsellProductsCheckBox->isChecked()) {
        QString unit = unitUnsellProductsLineEdit->text().simplified();
        if(!unit.isEmpty()) {
            QString filter = QString("unit = '%1'").arg(unit);
            if(unsellProductsFilter.isEmpty()) {
                unsellProductsFilter += filter;
            }
            else {
                unsellProductsFilter +=  " and " + filter;
            }
        }
    }

    if(schemaUnsellProductsCheckBox->isChecked()) {
        QString schema = schemaUnsellProductsComboBox->currentText().simplified();
        QString filter = QString("relTblAl_%1.name = '%2'").arg(SchemaNameID).arg(schema);
        if(unsellProductsFilter.isEmpty()) {
            unsellProductsFilter += filter;
        }
        else {
            unsellProductsFilter += " and " + filter;
        }
    }

    if(barInfoUnsellProductsCheckBox->isChecked()) {
        QString barInfo = barInfoUnsellProductsLineEdit->text().simplified();
        if(!barInfo.isEmpty()) {
            QString filter = QString("barInfo like '%%1%'").arg(barInfo);
            if(unsellProductsFilter.isEmpty()) {
                unsellProductsFilter += filter;
            }
            else {
                unsellProductsFilter +=  " and " + filter;
            }
        }
    }

    if(sellerUnsellProductsCheckBox->isChecked()) {
        QString seller = sellerUnsellProductsComboBox->currentText().simplified();
        QString filter = QString("relTblAl_%1.name = '%2'").arg(SellerID).arg(seller);
        if(unsellProductsFilter.isEmpty()) {
            unsellProductsFilter += filter;
        }
        else {
            unsellProductsFilter += " and " + filter;
        }
    }

    if(productStatusUnsellProductsCheckBox->isChecked()) {
        QString productStatus = productStatusUnsellProductsComboBox->currentText().simplified();
        QString filter = QString("relTblAl_%1.status = '%2'").arg(ProductStatusID).arg(productStatus);
        if(unsellProductsFilter.isEmpty()) {
            unsellProductsFilter += filter;
        }
        else {
            unsellProductsFilter += " and " + filter;
        }
    }

    if(oldPurchasePriceUnsellProductsCheckBox->isChecked()) {
        QString oldPurchasePrice = oldPurchasePriceUnsellProductsLineEdit->text().simplified();
        if(!oldPurchasePrice.isEmpty()) {
            QString filter = QString("oldPurchasePrice = '%1'").arg(oldPurchasePrice);
            if(unsellProductsFilter.isEmpty()) {
                unsellProductsFilter += filter;
            }
            else {
                unsellProductsFilter +=  " and " + filter;
            }
        }
    }

    if(purchasePriceUnsellProductsCheckBox->isChecked()) {
        QString purchasePrice = purchasePriceUnsellProductsLineEdit->text().simplified();
        if(!purchasePrice.isEmpty()) {
            QString filter = QString("purchasePrice = '%1'").arg(purchasePrice);
            if(unsellProductsFilter.isEmpty()) {
                unsellProductsFilter += filter;
            }
            else {
                unsellProductsFilter +=  " and " + filter;
            }
        }
    }


    if(sellingPriceUnsellProductsCheckBox->isChecked()) {
        QString sellingPrice = sellingPriceUnsellProductsLineEdit->text().simplified();
        if(!sellingPrice.isEmpty()) {
            QString filter = QString("sellingPrice = '%1'").arg(sellingPrice);
            if(unsellProductsFilter.isEmpty()) {
                unsellProductsFilter += filter;
            }
            else {
                unsellProductsFilter +=  " and " + filter;
            }
        }
    }

    if(commentsUnsellProductsCheckBox->isChecked()) {
        QString comments = commentsUnsellProductsLineEdit->text().simplified();
        if(!comments.isEmpty()) {
            QString filter = QString("comments like '%%1%'").arg(comments);
            if(unsellProductsFilter.isEmpty()) {
                unsellProductsFilter += filter;
            }
            else {
                unsellProductsFilter +=  " and " + filter;
            }
        }
    }

    if(startTimeUnsellProductsCheckBox->isChecked()) {
        QDateTime startTime = startTimeUnsellProductsDateTimeEdit->dateTime();
        QString start = startTime.toString("yyyy-MM-dd hh:mm:ss");
        if(!start.isEmpty()) {
            QString filter = QString("timeStamp >= '%1'").arg(start);
            if(unsellProductsFilter.isEmpty()) {
                unsellProductsFilter += filter;
            }
            else {
                unsellProductsFilter +=  " and " + filter;
            }
        }
    }

    if(endTimeUnsellProductsCheckBox->isChecked()) {
        QDateTime endTime = endTimeUnsellProductsDateTimeEdit->dateTime();
        QString end = endTime.toString("yyyy-MM-dd hh:mm:ss");
        if(!end.isEmpty()) {
            QString filter = QString("timeStamp <= '%1'").arg(end);
            if(unsellProductsFilter.isEmpty()) {
                unsellProductsFilter += filter;
            }
            else {
                unsellProductsFilter +=  " and " + filter;
            }
        }
    }

    mainWidget->setCursor(Qt::BusyCursor);
    unsellProductsModel->setFilter(unsellProductsFilter);
    unsellProductsModel->select();
//    qDebug()<<statisticModel->filter();
//    qDebug()<<statisticModel->query().lastQuery();
    unsellProductsView->resizeColumnsToContents();

    int rows = unsellProductsModel->rowCount();
    int totalQuantity = 0;
    double totalSelling = 0;
    QSqlRecord record;
    for(int i=0; i<rows; i++) {
        record = unsellProductsModel->record(i);
        totalQuantity += record.value(Quantity).toInt();
        QString selling = record.value(SellingPrice).toString().simplified();
        //qDebug()<<selling<<selling.toULongLong();
        if(!selling.isEmpty()) {
            totalSelling += selling.toDouble();
        }
    }
    unsellProductsSummaryLineEdit->setText(QString(tr("Numbers of Total Products: "))
                                           + QString("%1  ").arg(totalQuantity)
                                           + QString(tr("Numbers of Total Selling: "))
                                           + QString("%1").arg((qulonglong)totalSelling));

    mainWidget->unsetCursor();

}

void Statistic_Invoicing::onExportUnsellProducts2Excel()
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

    QString fileName = QFileDialog::getSaveFileName(0,
                                                    tr("Export to Excel"),
                                                    qApp->applicationDirPath(),
                                                    tr("Excel Files (*.xls)"));

    ExportExcelObject obj(fileName, tr("UnsellProducts"), unsellProductsView);
    //obj.addField(1, tr("ID"), "int");
    obj.addField(1, tr("Serial Number"), "char(255)");
    obj.addField(2, tr("Product Type"), "char(255)");
    obj.addField(3, tr("Brand Name"), "char(255)");
    obj.addField(4, tr("Model Name"), "char(255)");
    obj.addField(5, tr("Color"), "char(255)");
    obj.addField(6, tr("Vendor"), "char(255)");
    obj.addField(7, tr("Schema Name"), "char(255)");
    obj.addField(8, tr("Quantity"), "char(255)");
    obj.addField(9, tr("Unit"), "char(255)");
    if(!hide)
    {
        obj.addField(10, tr("Old Purchase Price"), "char(255)");
        obj.addField(11, tr("Purchase Price"), "char(255)");
    }
    obj.addField(12, tr("Selling Price"), "char(255)");
    obj.addField(13, tr("Operator"), "char(255)");
    obj.addField(14, tr("Seller"), "char(255)");
    obj.addField(15, tr("Responser"), "char(255)");
    obj.addField(16, tr("BarInfo"), "char(255)");
    obj.addField(17, tr("Product Status"), "char(255)");
    obj.addField(18, tr("Replacement Status"), "char(255)");
    obj.addField(19, tr("TimeStamp"), "char(255)");
    obj.addField(20, tr("Comments"), "char(255)");
    unsellProductProgressBar->setValue(0);
    unsellProductProgressBar->setMaximum(unsellProductsView->model()->rowCount());
    connect(&obj, SIGNAL(exportedRowCount(int)), unsellProductProgressBar, SLOT(setValue(int)));
    int retVal = obj.export2Excel();
    if(retVal>0)
    {
        //QMessageBox::information(0, tr("Export To Excel"), tr("Done!"));
    }
    else
    {
        //QMessageBox::critical(0, tr("Export To Excel"), tr("Error!"));
    }
}

void Statistic_Invoicing::onExportAllProducts2Excel()
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

    QString fileName = QFileDialog::getSaveFileName(0, tr("Export to Excel"), qApp->applicationDirPath (),
                                                    tr("Excel Files (*.xls)"));

    ExportExcelObject obj(fileName, tr("AllProducts"), allProductsView);
    //obj.addField(1, tr("ID"), "int");
    obj.addField(1, tr("Serial Number"), "char(255)");
    obj.addField(2, tr("Product Type"), "char(255)");
    obj.addField(3, tr("Brand Name"), "char(255)");
    obj.addField(4, tr("Model Name"), "char(255)");
    obj.addField(5, tr("Color"), "char(255)");
    obj.addField(6, tr("Vendor"), "char(255)");
    obj.addField(7, tr("Schema Name"), "char(255)");
    obj.addField(8, tr("Quantity"), "char(255)");
    obj.addField(9, tr("Unit"), "char(255)");
    if(!hide)
    {
        obj.addField(10, tr("Old Purchase Price"), "char(255)");
        obj.addField(11, tr("Purchase Price"), "char(255)");
    }
    obj.addField(12, tr("Selling Price"), "char(255)");
    obj.addField(13, tr("Operator"), "char(255)");
    obj.addField(14, tr("Seller"), "char(255)");
    obj.addField(15, tr("Responser"), "char(255)");
    obj.addField(16, tr("BarInfo"), "char(255)");
    obj.addField(17, tr("Product Status"), "char(255)");
    obj.addField(18, tr("Replacement Status"), "char(255)");
    obj.addField(19, tr("TimeStamp"), "char(255)");
    obj.addField(20, tr("Comments"), "char(255)");
    allProductProgressBar->setValue(0);
    allProductProgressBar->setMaximum(allProductsView->model()->rowCount());
    connect(&obj, SIGNAL(exportedRowCount(int)), allProductProgressBar, SLOT(setValue(int)));
    int retVal = obj.export2Excel();
    if(retVal>0)
    {
        //QMessageBox::information(0, tr("Export To Excel"), tr("Done!"));
    }
    else
    {
        //QMessageBox::critical(0, tr("Export To Excel"), tr("Error!"));
    }
}

void Statistic_Invoicing::updateSalesStatisticPlot()
{
    int year = time->date().year();
    int month = time->date().month();
    int monthRange = timeRange->value();
    QMap<QString, SalesResult> data =
            productManagementInterface->getSalesResultByTimeRange(year, month, monthRange);

    if(!data.isEmpty()) {
        int sortType = SORTBYVOLUMEDESCEND;
        if(sortByVolumeAscRadioButton->isChecked()) {
            sortType = SORTBYVOLUMEASCEND;
        }
        else if(sortByVolumeDesRadioButton->isChecked()) {
            sortType = SORTBYVOLUMEDESCEND;
        }
        else if(sortByQuantityAscRadioButton->isChecked()) {
            sortType = SORTBYQUANTITYASCEND;
        }
        else if(sortByQuantityDesRadioButton->isChecked()) {
            sortType = SORTBYQUANTITYDESCEND;
        }
        SortArg sArg;
        sArg.sortType = sortType;
        sArg.peopleRange = peopleRange->value();

        if(salesStatisticPlot) {
            salesStatisticPlot->updatePlot(data, sArg);
        }
    }
}

void Statistic_Invoicing::populateSellerNameComboBox(QComboBox *sellerNameComboBox) const
{
    int sellerRoleID = userManagementInterface->getRoleIDByRoleName("销售");
    int adminRoleID = userManagementInterface->getRoleIDByRoleName("管理员");
    QSet<int> sellers = userManagementInterface->getUserIDSetByRoleID(sellerRoleID);
    QSet<int> admins = userManagementInterface->getUserIDSetByRoleID(adminRoleID);
    sellerNameComboBox->clear();
    //sellerComboBox->addItem(userManagementInterface->getUserNameByUserID(1));
    sellerNameComboBox->addItem("未指定");
    foreach(int admin, admins) {
        sellerNameComboBox->addItem(userManagementInterface->getUserNameByUserID(admin));
    }
    foreach(int seller, sellers) {
        sellerNameComboBox->addItem(userManagementInterface->getUserNameByUserID(seller));
    }
}

void Statistic_Invoicing::updateAllProductinfo()
{
    if(!statisticUpdateAllProductDialog) {
        statisticUpdateAllProductDialog = new StatisticUpdateProductDialog(userManagementInterface,
                                                      productManagementInterface);
    }
    statisticUpdateAllProductDialog->updateDBTableModel();
    QModelIndex storageIndex = allProductsView->currentIndex();
    QSqlRecord record = allProductsModel->record(storageIndex.row());
    statisticUpdateAllProductDialog->updateRecord(record);
    statisticUpdateAllProductDialog->exec();
}

void Statistic_Invoicing::updateUnsellProductinfo()
{
    if(!statisticUpdateUnsellProductDialog) {
        statisticUpdateUnsellProductDialog = new StatisticUpdateProductDialog(userManagementInterface,
                                                      productManagementInterface);
    }
    statisticUpdateUnsellProductDialog->updateDBTableModel();
    QModelIndex storageIndex = unsellProductsView->currentIndex();
    QSqlRecord record = unsellProductsModel->record(storageIndex.row());
    statisticUpdateUnsellProductDialog->updateRecord(record);
    statisticUpdateUnsellProductDialog->exec();
}
