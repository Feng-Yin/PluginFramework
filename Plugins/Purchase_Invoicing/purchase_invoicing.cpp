#include <QtGui>
#include <QtSql>
#include "purchase_invoicing.h"
#include "mainwindow.h"
#include "usermanagement_interface.h"
#include "productmanagement_interface.h"
#include "addproductdialog.h"

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

    QGridLayout *mainLayout = new QGridLayout(mainWidget);
    mainLayout->addWidget(filterWidget, 0, 0, Qt::AlignTop);
    mainLayout->addLayout(hlayout, 1, 0, Qt::AlignTop);

    createPurchasePanel();
    mainLayout->addWidget(purchasePanel, 2, 0);
    mainWidget->setLayout(mainLayout);
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
//    timer->setInterval(1000);
//    timer->start(1000);

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
    purchaseModel->setRelation(SchemaNameID, QSqlRelation("schemaname", "id", "name"));
    purchaseModel->setRelation(OperatorUserID, QSqlRelation("user", "id", "name"));
    purchaseModel->setRelation(ResponserUserID, QSqlRelation("user", "id", "name"));
    purchaseModel->setRelation(ProductStatusID, QSqlRelation("productstatus", "id", "status"));
    purchaseModel->setSort(TimeStamp, Qt::AscendingOrder);

    purchaseModel->setHeaderData(ProductID, Qt::Horizontal, tr("ID"));
    purchaseModel->setHeaderData(SerialNumber, Qt::Horizontal, tr("Serial Number"));
    purchaseModel->setHeaderData(ProductTypeID, Qt::Horizontal, tr("Product Type"));
    purchaseModel->setHeaderData(BrandNameID, Qt::Horizontal, tr("Brand Name"));
    purchaseModel->setHeaderData(ProductModelID, Qt::Horizontal, tr("Model Name"));
    purchaseModel->setHeaderData(SchemaNameID, Qt::Horizontal, tr("Schema Name"));
    purchaseModel->setHeaderData(Quantity, Qt::Horizontal, tr("Quantity"));
    purchaseModel->setHeaderData(Unit, Qt::Horizontal, tr("Unit"));
    purchaseModel->setHeaderData(OldPurchasePrice, Qt::Horizontal, tr("Old Purchase Price"));
    purchaseModel->setHeaderData(PurchasePrice, Qt::Horizontal, tr("Purchase Price"));
    purchaseModel->setHeaderData(SellingPrice, Qt::Horizontal, tr("Selling Price"));
    purchaseModel->setHeaderData(OperatorUserID, Qt::Horizontal, tr("Operator"));
    purchaseModel->setHeaderData(ResponserUserID, Qt::Horizontal, tr("Responser"));
    purchaseModel->setHeaderData(ProductStatusID, Qt::Horizontal, tr("Product Status"));
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

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(purchaseView);
    purchasePanel->setLayout(layout);
}

void Purchase_Invoicing::userChanged() const
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
    updatePurchaseFilter();
    updateDBTableModel();
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
    foreach(int id, productIDSet) {
        productManagementInterface->updateStatusIDByProductID(id, deleteID);
        productManagementInterface->deleteProductByProductID(id);
    }
    purchaseModel->select();
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
    while(purchaseModel->rowCount()>0) {
        QSqlRecord record = purchaseModel->record(0);
        int productID = record.value(ProductID).toInt();
        int statusID = productManagementInterface->getStatusIDByStatusName("待入库");
        productManagementInterface->updateStatusIDByProductID(productID, statusID);
        purchaseModel->select();
    }
}

void Purchase_Invoicing::updateDBTableModel() const
{
    if(productManagementInterface->isModelOutdate(purchaseModel)) {
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

void Purchase_Invoicing::updatePurchaseFilter() const
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

QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(Purchase_Invoicing, Purchase_Invoicing)
QT_END_NAMESPACE
