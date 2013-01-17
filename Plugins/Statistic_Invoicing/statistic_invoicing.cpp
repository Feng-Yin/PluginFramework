#include <QtGui>
#include <QtSql>
#include "statistic_invoicing.h"
#include "mainwindow.h"
#include "usermanagement_interface.h"
#include "productmanagement_interface.h"
#include "barchart.h"

const char *sortTypeProperty = "SortType";

Statistic_Invoicing::Statistic_Invoicing():
    parentWindow(NULL),
    userManagementInterface(NULL),
    productManagementInterface(NULL),
    mainWidget(NULL),
    statisticPanel(NULL),
    statisticModel(NULL),
    statisticView(NULL),
    filterPanel(NULL),
    basicFilterPanel(NULL),
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
    serialNumberLabel(NULL),
    serialNumberLineEdit(NULL),
	serialNumberCheckBox(NULL),
    productTypeLabel(NULL),
    productTypeComboBox(NULL),
    productTypeModel(NULL),
	productTypeCheckBox(NULL),
    brandNameLabel(NULL),
    brandNameComboBox(NULL),
    brandNameModel(NULL),
	brandNameCheckBox(NULL),
    productModelLabel(NULL),
    productModelComboBox(NULL),
    productModelModel(NULL),
	productModelCheckBox(NULL),
    quantityLabel(NULL),
    quantitySpinBox(NULL),
    unitLabel(NULL),
    unitLineEdit(NULL),
    schemaLabel(NULL),
    schemaComboBox(NULL),
    userSchemaModel(NULL),
    schemaCheckBox(NULL),
    oldPurchasePriceLabel(NULL),
    oldPurchasePriceLineEdit(NULL),
	oldPurchasePriceCheckBox(NULL),
    purchasePriceLabel(NULL),
    purchasePriceLineEdit(NULL),
	purchasePriceCheckBox(NULL),
    sellingPriceLabel(NULL),
    sellingPriceLineEdit(NULL),
    sellingPriceCheckBox(NULL)
{
    QDir qmdir(":/Translations");
    foreach (QString fileName, qmdir.entryList(QDir::Files)) {
        //qDebug()<<QFileInfo(fileName).baseName();
        QTranslator *qtTranslator = new QTranslator(this);
        qtTranslator->load(QFileInfo(fileName).baseName(), ":/Translations");
        QApplication::instance()->installTranslator(qtTranslator);
    }
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


void Statistic_Invoicing::userChanged() const
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
    return QSet<QString>()<<"¹ÜÀíÔ±";
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

    createStatisticPanel();
    createFilterPanel();

    QGridLayout *mainLayout = new QGridLayout(mainWidget);
    mainLayout->addWidget(filterPanel, 0, 0);
    //mainLayout->addWidget(statisticPanel, 1, 0);
    mainWidget->setLayout(mainLayout);
}

void Statistic_Invoicing::createStatisticPanel()
{
    statisticPanel = new QWidget();
    QSqlDatabase db(userManagementInterface->getDatabase());
    statisticModel = new QSqlRelationalTableModel(this, db);
    statisticModel->setTable("productsinfo");
    statisticModel->setRelation(ProductTypeID, QSqlRelation("producttype", "id", "name"));
    statisticModel->setRelation(BrandNameID, QSqlRelation("brandname", "id", "name"));
    statisticModel->setRelation(ProductModelID, QSqlRelation("productmodel", "id", "model"));
    statisticModel->setRelation(SchemaNameID, QSqlRelation("schemaname", "id", "name"));
    statisticModel->setRelation(OperatorUserID, QSqlRelation("user", "id", "name"));
    statisticModel->setRelation(ResponserUserID, QSqlRelation("user", "id", "name"));
    statisticModel->setRelation(ProductStatusID, QSqlRelation("productstatus", "id", "status"));
    statisticModel->setSort(TimeStamp, Qt::AscendingOrder);

    statisticModel->setHeaderData(ProductID, Qt::Horizontal, tr("ID"));
    statisticModel->setHeaderData(SerialNumber, Qt::Horizontal, tr("Serial Number"));
    statisticModel->setHeaderData(ProductTypeID, Qt::Horizontal, tr("Product Type"));
    statisticModel->setHeaderData(BrandNameID, Qt::Horizontal, tr("Brand Name"));
    statisticModel->setHeaderData(ProductModelID, Qt::Horizontal, tr("Model Name"));
    statisticModel->setHeaderData(SchemaNameID, Qt::Horizontal, tr("Schema Name"));
    statisticModel->setHeaderData(Quantity, Qt::Horizontal, tr("Quantity"));
    statisticModel->setHeaderData(Unit, Qt::Horizontal, tr("Unit"));
    statisticModel->setHeaderData(OldPurchasePrice, Qt::Horizontal, tr("Old Purchase Price"));
    statisticModel->setHeaderData(PurchasePrice, Qt::Horizontal, tr("Purchase Price"));
    statisticModel->setHeaderData(SellingPrice, Qt::Horizontal, tr("Selling Price"));
    statisticModel->setHeaderData(OperatorUserID, Qt::Horizontal, tr("Operator"));
    statisticModel->setHeaderData(ResponserUserID, Qt::Horizontal, tr("Responser"));
    statisticModel->setHeaderData(ProductStatusID, Qt::Horizontal, tr("Product Status"));
    statisticModel->setHeaderData(TimeStamp, Qt::Horizontal, tr("TimeStamp"));
    statisticModel->setHeaderData(Comments, Qt::Horizontal, tr("Comments"));

    //statisticModel->select();

    statisticView = new QTableView;
    statisticView->setModel(statisticModel);
    statisticView->setItemDelegate(new QSqlRelationalDelegate(this));
    statisticView->setSelectionMode(QAbstractItemView::SingleSelection);
    statisticView->setSelectionBehavior(QAbstractItemView::SelectRows);
    //statisticView->setSelectionMode(QAbstractItemView::NoSelection);
    statisticView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    statisticView->resizeColumnsToContents();
    statisticView->horizontalHeader()->setStretchLastSection(true);
    statisticView->setColumnHidden(ProductID, true);
    statisticView->horizontalHeader()->setVisible(true);
    statisticView->resizeColumnsToContents();

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(statisticView);
    statisticPanel->setLayout(layout);
}

void Statistic_Invoicing::createFilterPanel()
{
    filterPanel = new QTabWidget();

    basicFilterPanel = new QWidget();
    serialNumberLabel = new QLabel(tr("Serial Number: "), basicFilterPanel);
    serialNumberLabel->setEnabled(false);
    serialNumberLineEdit = new QLineEdit(basicFilterPanel);
    serialNumberLineEdit->setEnabled(false);
    serialNumberCheckBox = new QCheckBox(basicFilterPanel);
    connect(serialNumberCheckBox, SIGNAL(toggled(bool)), serialNumberLabel, SLOT(setEnabled(bool)));
    connect(serialNumberCheckBox, SIGNAL(toggled(bool)), serialNumberLineEdit, SLOT(setEnabled(bool)));

    productTypeLabel = new QLabel(tr("Product Type: "), basicFilterPanel);
    productTypeLabel->setEnabled(false);
    productTypeComboBox = new QComboBox(basicFilterPanel);
    productTypeComboBox->setEnabled(false);
    productTypeComboBox->setEditable(false);
    QSqlDatabase db(userManagementInterface->getDatabase());
    productTypeModel = new QSqlRelationalTableModel(this, db);
    productTypeModel->setTable("producttype");
    productTypeModel->select();
    productTypeComboBox->setModel(productTypeModel);
    productTypeComboBox->setModelColumn(1);
//    connect(productTypeComboBox, SIGNAL(currentIndexChanged(const QString &))
//            , this, SLOT(setProductModelFilter()));
    productTypeCheckBox = new QCheckBox(basicFilterPanel);
    connect(productTypeCheckBox, SIGNAL(toggled(bool)), productTypeLabel, SLOT(setEnabled(bool)));
    connect(productTypeCheckBox, SIGNAL(toggled(bool)), productTypeComboBox, SLOT(setEnabled(bool)));

    brandNameLabel = new QLabel(tr("Brand Name: "), basicFilterPanel);
    brandNameLabel->setEnabled(false);
    brandNameComboBox = new QComboBox(basicFilterPanel);
    brandNameComboBox->setEnabled(false);
    brandNameComboBox->setEditable(false);
    brandNameModel = new QSqlRelationalTableModel(this, db);
    brandNameModel->setTable("brandname");
    brandNameModel->select();
    brandNameComboBox->setModel(brandNameModel);
    brandNameComboBox->setModelColumn(1);
    brandNameCheckBox = new QCheckBox(basicFilterPanel);
//    connect(brandNameComboBox, SIGNAL(currentIndexChanged(const QString &))
//            , this, SLOT(setProductModelFilter()));
    connect(brandNameCheckBox, SIGNAL(toggled(bool)), brandNameLabel, SLOT(setEnabled(bool)));
    connect(brandNameCheckBox, SIGNAL(toggled(bool)), brandNameComboBox, SLOT(setEnabled(bool)));

    productModelLabel = new QLabel(tr("Product Model: "), basicFilterPanel);
    productModelLabel->setEnabled(false);
    productModelComboBox = new QComboBox(basicFilterPanel);
    productModelComboBox->setEnabled(false);
    productModelComboBox->setEditable(false);
    productModelModel = new QSqlRelationalTableModel(this, db);
    productModelModel->setTable("productmodel");
    setProductModelFilter();
    productModelComboBox->setModel(productModelModel);
    productModelComboBox->setModelColumn(3);
    productModelCheckBox = new QCheckBox(basicFilterPanel);
    connect(productModelCheckBox, SIGNAL(toggled(bool)), productModelLabel, SLOT(setEnabled(bool)));
    connect(productModelCheckBox, SIGNAL(toggled(bool)), productModelComboBox, SLOT(setEnabled(bool)));

    quantityLabel = new QLabel(tr("Quantity: "), basicFilterPanel);
    quantityLabel->setEnabled(false);
    quantitySpinBox = new QSpinBox(basicFilterPanel);
    quantitySpinBox->setEnabled(false);
    quantitySpinBox->setMinimum(1);
    quantitySpinBox->setValue(1);
    quantityCheckBox = new QCheckBox(basicFilterPanel);
    connect(quantityCheckBox, SIGNAL(toggled(bool)), quantityLabel, SLOT(setEnabled(bool)));
    connect(quantityCheckBox, SIGNAL(toggled(bool)), quantitySpinBox, SLOT(setEnabled(bool)));

    unitLabel = new QLabel(tr("Unit: "), basicFilterPanel);
    unitLabel->setEnabled(false);
    unitLineEdit = new QLineEdit(basicFilterPanel);
    unitLineEdit->setEnabled(false);
    unitCheckBox = new QCheckBox(basicFilterPanel);
    connect(unitCheckBox, SIGNAL(toggled(bool)), unitLabel, SLOT(setEnabled(bool)));
    connect(unitCheckBox, SIGNAL(toggled(bool)), unitLineEdit, SLOT(setEnabled(bool)));

    schemaLabel = new QLabel(tr("Schema: "), basicFilterPanel);
    schemaLabel->setEnabled(false);
    schemaComboBox = new QComboBox(basicFilterPanel);
    schemaComboBox->setEnabled(false);
    schemaComboBox->setEditable(false);
    populateSchemaComboBox();
    schemaCheckBox = new QCheckBox(basicFilterPanel);
    connect(schemaCheckBox, SIGNAL(toggled(bool)), schemaLabel, SLOT(setEnabled(bool)));
    connect(schemaCheckBox, SIGNAL(toggled(bool)), schemaComboBox, SLOT(setEnabled(bool)));

    oldPurchasePriceLabel = new QLabel(tr("Old Purchase Price: "), basicFilterPanel);
    oldPurchasePriceLabel->setEnabled(false);
    oldPurchasePriceLineEdit = new QLineEdit(basicFilterPanel);
    oldPurchasePriceLineEdit->setEnabled(false);
    oldPurchasePriceCheckBox = new QCheckBox(basicFilterPanel);
    connect(oldPurchasePriceCheckBox, SIGNAL(toggled(bool)), oldPurchasePriceLabel, SLOT(setEnabled(bool)));
    connect(oldPurchasePriceCheckBox, SIGNAL(toggled(bool)), oldPurchasePriceLineEdit, SLOT(setEnabled(bool)));

    purchasePriceLabel = new QLabel(tr("Purchase Price: "), basicFilterPanel);
    purchasePriceLabel->setEnabled(false);
    purchasePriceLineEdit = new QLineEdit(basicFilterPanel);
    purchasePriceLineEdit->setEnabled(false);
    purchasePriceCheckBox = new QCheckBox(basicFilterPanel);
    connect(purchasePriceCheckBox, SIGNAL(toggled(bool)), purchasePriceLabel, SLOT(setEnabled(bool)));
    connect(purchasePriceCheckBox, SIGNAL(toggled(bool)), purchasePriceLineEdit, SLOT(setEnabled(bool)));

    sellingPriceLabel = new QLabel(tr("Selling Price: "), basicFilterPanel);
    sellingPriceLabel->setEnabled(false);
    sellingPriceLineEdit = new QLineEdit(basicFilterPanel);
    sellingPriceLineEdit->setEnabled(false);
    sellingPriceCheckBox = new QCheckBox(basicFilterPanel);
    connect(sellingPriceCheckBox, SIGNAL(toggled(bool)), sellingPriceLabel, SLOT(setEnabled(bool)));
    connect(sellingPriceCheckBox, SIGNAL(toggled(bool)), sellingPriceLineEdit, SLOT(setEnabled(bool)));

    basicFilterButton = new QPushButton(tr("Filter Result"), basicFilterPanel);
    QFont f;
    f.setBold(true);
    basicFilterButton->setFont(f);
    connect(basicFilterButton, SIGNAL(clicked()), this, SLOT(onBasicFilter()));

    QGridLayout *basicFilterLayout = new QGridLayout(basicFilterPanel);
    basicFilterLayout->addWidget(serialNumberLabel, 0, 0, Qt::AlignRight);
    basicFilterLayout->addWidget(serialNumberLineEdit, 0, 1);
    basicFilterLayout->addWidget(serialNumberCheckBox, 0, 2);

    basicFilterLayout->addWidget(productTypeLabel, 10, 0, Qt::AlignRight);
    basicFilterLayout->addWidget(productTypeComboBox, 10, 1);
    basicFilterLayout->addWidget(productTypeCheckBox, 10, 2);
    basicFilterLayout->addWidget(brandNameLabel, 10, 10, Qt::AlignRight);
    basicFilterLayout->addWidget(brandNameComboBox, 10, 11);
    basicFilterLayout->addWidget(brandNameCheckBox, 10, 12);
    basicFilterLayout->addWidget(productModelLabel, 10, 20, Qt::AlignRight);
    basicFilterLayout->addWidget(productModelComboBox, 10, 21);
    basicFilterLayout->addWidget(productModelCheckBox, 10, 22);

    basicFilterLayout->addWidget(quantityLabel, 20, 0, Qt::AlignRight);
    basicFilterLayout->addWidget(quantitySpinBox, 20, 1);
    basicFilterLayout->addWidget(quantityCheckBox, 20, 2);
    basicFilterLayout->addWidget(unitLabel, 20, 10, Qt::AlignRight);
    basicFilterLayout->addWidget(unitLineEdit, 20, 11);
    basicFilterLayout->addWidget(unitCheckBox, 20, 12);
    basicFilterLayout->addWidget(schemaLabel, 20, 20, Qt::AlignRight);
    basicFilterLayout->addWidget(schemaComboBox, 20, 21);
    basicFilterLayout->addWidget(schemaCheckBox, 20, 22);

    basicFilterLayout->addWidget(oldPurchasePriceLabel, 30, 0, Qt::AlignRight);
    basicFilterLayout->addWidget(oldPurchasePriceLineEdit, 30, 1);
    basicFilterLayout->addWidget(oldPurchasePriceCheckBox, 30, 2);
    basicFilterLayout->addWidget(purchasePriceLabel, 30, 10, Qt::AlignRight);
    basicFilterLayout->addWidget(purchasePriceLineEdit, 30, 11);
    basicFilterLayout->addWidget(purchasePriceCheckBox, 30, 12);
    basicFilterLayout->addWidget(sellingPriceLabel, 30, 20, Qt::AlignRight);
    basicFilterLayout->addWidget(sellingPriceLineEdit, 30, 21);
    basicFilterLayout->addWidget(sellingPriceCheckBox, 30, 22);

    basicFilterLayout->addWidget(basicFilterButton, 0, 20, 1, 3);

    QFrame *line1 = new QFrame();
    line1->setAttribute(Qt::WA_MouseNoMask);
    line1->setFrameStyle(QFrame::Sunken);
    line1->setFrameShape(QFrame::HLine);
    basicFilterLayout->addWidget(line1, 5, 0, 1, basicFilterLayout->columnCount());
    QFrame *line2 = new QFrame();
    line2->setAttribute(Qt::WA_MouseNoMask);
    line2->setFrameStyle(QFrame::Sunken);
    line2->setFrameShape(QFrame::HLine);
    basicFilterLayout->addWidget(line2, 25, 0, 1, basicFilterLayout->columnCount());
    QFrame *line3 = new QFrame();
    line3->setAttribute(Qt::WA_MouseNoMask);
    line3->setFrameStyle(QFrame::Sunken);
    line3->setFrameShape(QFrame::HLine);
    basicFilterLayout->addWidget(line3, 35, 0, 1, basicFilterLayout->columnCount());

    basicFilterLayout->addWidget(statisticPanel, 45, 0, 1, basicFilterLayout->columnCount());

    basicFilterPanel->setLayout(basicFilterLayout);
    filterPanel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

    filterPanel->addTab(basicFilterPanel, tr("Basci Filter"));

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
    QWidget *salesStatisticContainer = new QWidget();
    salesStatisticContainer->setLayout(salesStatisticVLayout);

    filterPanel->addTab(salesStatisticContainer, tr("Sales Statistic"));
}

void Statistic_Invoicing::updateDBTableModel() const
{
    if(productManagementInterface->isModelOutdate(statisticModel)) {
        QModelIndex index = statisticView->currentIndex();
        static QModelIndex outViewPortindex;
        statisticModel->select();
        statisticView->resizeColumnsToContents();
        if(index.isValid()) {
            int rowPosition = statisticView->rowViewportPosition(index.row());
            if(rowPosition>=0 && rowPosition<statisticView->height()) {
                statisticView->setCurrentIndex(index);
            }
            else {
                outViewPortindex = index;
            }
        }
        else if(outViewPortindex.isValid()) {
            int rowPosition = statisticView->rowViewportPosition(outViewPortindex.row());
            if(rowPosition>=0 && rowPosition<statisticView->height()) {
                statisticView->setCurrentIndex(outViewPortindex);
            }
        }
        statisticView->resizeColumnsToContents();
    }
}

void Statistic_Invoicing::setProductModelFilter()
{
    //we want to make all model avaliable, so no filter on table productmodel
    productModelModel->select();
}

void Statistic_Invoicing::populateSchemaComboBox() const
{
    QSet<QString> schemaNameSet = userManagementInterface->getAllSchemaName();;

    schemaComboBox->clear();
    schemaComboBox->addItems(schemaNameSet.toList());
}

void Statistic_Invoicing::onBasicFilter()
{
    QString basicFilter("");
    if(serialNumberCheckBox->isChecked()) {
        QString serialNumber = serialNumberLineEdit->text().simplified();
        if(!serialNumber.isEmpty()) {
            QString filter = QString("serialNumber = '%1'").arg(serialNumber);
            if(basicFilter.isEmpty()) {
                basicFilter += filter;
            }
            else {
                basicFilter +=  " and " + filter;
            }
        }
    }

    if(productTypeCheckBox->isChecked()) {
        QString productType = productTypeComboBox->currentText().simplified();
        //Qt bug, refer to
        //qt.onyou.ch/2010/06/20/filtering-relational-tables-in-qsqlrelationaltablemodel/
        QString filter = QString("relTblAl_2.name = '%1'").arg(productType);
        if(basicFilter.isEmpty()) {
            basicFilter += filter;
        }
        else {
            basicFilter +=  " and " + filter;
        }
    }

    if(brandNameCheckBox->isChecked()) {
        QString brandName = brandNameComboBox->currentText().simplified();
        QString filter = QString("relTblAl_3.name = '%1'").arg(brandName);
        if(basicFilter.isEmpty()) {
            basicFilter += filter;
        }
        else {
            basicFilter += " and " + filter;
        }
    }

    if(productModelCheckBox->isChecked()) {
        QString productModel = productModelComboBox->currentText().simplified();
        QString filter = QString("relTblAl_4.model = '%1'").arg(productModel);
        if(basicFilter.isEmpty()) {
            basicFilter += filter;
        }
        else {
            basicFilter += " and " + filter;
        }
    }

    if(quantityCheckBox->isChecked()) {
        int quantity = quantitySpinBox->value();
        QString filter = QString("quantity = %1").arg(quantity);
        if(basicFilter.isEmpty()) {
            basicFilter += filter;
        }
        else {
            basicFilter +=  " and " + filter;
        }
    }

    if(unitCheckBox->isChecked()) {
        QString unit = unitLineEdit->text().simplified();
        if(!unit.isEmpty()) {
            QString filter = QString("unit = '%1'").arg(unit);
            if(basicFilter.isEmpty()) {
                basicFilter += filter;
            }
            else {
                basicFilter +=  " and " + filter;
            }
        }
    }

    if(schemaCheckBox->isChecked()) {
        QString schema = schemaComboBox->currentText().simplified();
        QString filter = QString("relTblAl_5.name = '%1'").arg(schema);
        if(basicFilter.isEmpty()) {
            basicFilter += filter;
        }
        else {
            basicFilter += " and " + filter;
        }
    }

    if(oldPurchasePriceCheckBox->isChecked()) {
        QString oldPurchasePrice = oldPurchasePriceLineEdit->text().simplified();
        if(!oldPurchasePrice.isEmpty()) {
            QString filter = QString("oldPurchasePrice = '%1'").arg(oldPurchasePrice);
            if(basicFilter.isEmpty()) {
                basicFilter += filter;
            }
            else {
                basicFilter +=  " and " + filter;
            }
        }
    }

    if(purchasePriceCheckBox->isChecked()) {
        QString purchasePrice = purchasePriceLineEdit->text().simplified();
        if(!purchasePrice.isEmpty()) {
            QString filter = QString("purchasePrice = '%1'").arg(purchasePrice);
            if(basicFilter.isEmpty()) {
                basicFilter += filter;
            }
            else {
                basicFilter +=  " and " + filter;
            }
        }
    }


    if(sellingPriceCheckBox->isChecked()) {
        QString sellingPrice = sellingPriceLineEdit->text().simplified();
        if(!sellingPrice.isEmpty()) {
            QString filter = QString("sellingPrice = '%1'").arg(sellingPrice);
            if(basicFilter.isEmpty()) {
                basicFilter += filter;
            }
            else {
                basicFilter +=  " and " + filter;
            }
        }
    }

    statisticModel->setFilter(basicFilter);
    statisticModel->select();
//    qDebug()<<statisticModel->filter();
//    qDebug()<<statisticModel->query().lastQuery();
    statisticView->resizeColumnsToContents();
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

        salesStatisticPlot->updatePlot(data, sArg);
    }
}

QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(Statistic_Invoicing, Statistic_Invoicing)
QT_END_NAMESPACE
