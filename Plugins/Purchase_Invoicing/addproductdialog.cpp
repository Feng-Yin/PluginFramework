#include <QtGui>
#include <QtSql>
#include <usermanagement_interface.h>
#include "productmanagement_interface.h"
#include "addproductdialog.h"

AddProductDialog::AddProductDialog(UserManagementInterface *userManagementInterface,
                                   ProductManagementInterface *productManagementInterface, QWidget *parent):
    QDialog(parent),
    userManagementInterface(userManagementInterface),
    productManagementInterface(productManagementInterface),
    serialNumberLabel(NULL),
    serialNumberLineEdit(NULL),
    productTypeLabel(NULL),
    productTypeComboBox(NULL),
    productTypeModel(NULL),
    brandNameLabel(NULL),
    brandNameComboBox(NULL),
    brandNameModel(NULL),
    productModelLabel(NULL),
    productModelComboBox(NULL),
    productModelModel(NULL),
    quantityLabel(NULL),
    quantitySpinBox(NULL),
    unitLabel(NULL),
    unitLineEdit(NULL),
    schemaLabel(NULL),
    schemaComboBox(NULL),
    userSchemaModel(NULL),
    oldPurchasePriceLabel(NULL),
    oldPurchasePriceLineEdit(NULL),
    purchasePriceLabel(NULL),
    purchasePriceLineEdit(NULL),
    sellingPriceLabel(NULL),
    sellingPriceLineEdit(NULL),
    commentsLabel(NULL),
    commentsTextEdit(NULL),
    addProductButton(NULL),
    clearInfoButton(NULL)
{
    init();
}

AddProductDialog::~AddProductDialog()
{
    userManagementInterface->deregisteObserver(this);
}

void AddProductDialog::init()
{
    serialNumberLabel = new QLabel(tr("Serial Number")+"<b style=\"COLOR: #ff0000\">*</b>: ", this);
    serialNumberLineEdit = new QLineEdit(this);

    productTypeLabel = new QLabel(tr("Product Type")+"<b style=\"COLOR: #ff0000\">*</b>: ", this);
    productTypeComboBox = new QComboBox(this);
    productTypeComboBox->setEditable(true);
    QSqlDatabase db(userManagementInterface->getDatabase());
    productTypeModel = new QSqlRelationalTableModel(this, db);
    productTypeModel->setTable("producttype");
    productTypeModel->select();
    productTypeComboBox->setModel(productTypeModel);
    productTypeComboBox->setModelColumn(1);
    connect(productTypeComboBox, SIGNAL(editTextChanged(const QString &))
            , this, SLOT(setProductModelFilter()));

    brandNameLabel = new QLabel(tr("Brand Name")+"<b style=\"COLOR: #ff0000\">*</b>: ", this);
    brandNameComboBox = new QComboBox(this);
    brandNameComboBox->setEditable(true);
    brandNameModel = new QSqlRelationalTableModel(this, db);
    brandNameModel->setTable("brandname");
    brandNameModel->select();
    brandNameComboBox->setModel(brandNameModel);
    brandNameComboBox->setModelColumn(1);
    connect(brandNameComboBox, SIGNAL(editTextChanged(const QString &))
            , this, SLOT(setProductModelFilter()));

    productModelLabel = new QLabel(tr("Product Model")+"<b style=\"COLOR: #ff0000\">*</b>: ", this);
    productModelComboBox = new QComboBox(this);
    productModelComboBox->setEditable(true);
    productModelModel = new QSqlRelationalTableModel(this, db);
    productModelModel->setTable("productmodel");
    setProductModelFilter();
    productModelComboBox->setModel(productModelModel);
    productModelComboBox->setModelColumn(3);

    quantityLabel = new QLabel(tr("Quantity")+"<b style=\"COLOR: #ff0000\">*</b>: ", this);
    quantitySpinBox = new QSpinBox(this);
    quantitySpinBox->setMinimum(1);
    quantitySpinBox->setValue(1);

    unitLabel = new QLabel(tr("Unit")+"<b style=\"COLOR: #ff0000\">*</b>: ", this);
    unitLineEdit = new QLineEdit(this);

    schemaLabel = new QLabel(tr("Schema: "), this);
    schemaComboBox = new QComboBox(this);
    schemaComboBox->setEditable(false);
    populateSchemaComboBox();

    oldPurchasePriceLabel = new QLabel(tr("Old Purchase Price: "), this);
    oldPurchasePriceLineEdit = new QLineEdit(this);

    purchasePriceLabel = new QLabel(tr("Purchase Price: "), this);
    purchasePriceLineEdit = new QLineEdit(this);

    sellingPriceLabel = new QLabel(tr("Selling Price: "), this);
    sellingPriceLineEdit = new QLineEdit(this);

    commentsLabel = new QLabel(tr("Comments: "), this);
    commentsTextEdit = new QTextEdit(this);

    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->addWidget(serialNumberLabel, 0, 0, Qt::AlignRight);
    mainLayout->addWidget(serialNumberLineEdit, 0, 1);

    QFrame *line1 = new QFrame();
    line1->setAttribute(Qt::WA_MouseNoMask);
    line1->setFrameStyle(QFrame::Sunken);
    line1->setFrameShape(QFrame::HLine);
    mainLayout->addWidget(line1, 5, 0, 1, 6);

    mainLayout->addWidget(productTypeLabel, 10, 0, Qt::AlignRight);
    mainLayout->addWidget(productTypeComboBox, 10, 1);
    mainLayout->addWidget(brandNameLabel, 10, 2, Qt::AlignRight);
    mainLayout->addWidget(brandNameComboBox, 10, 3);
    mainLayout->addWidget(productModelLabel, 10, 4, Qt::AlignRight);
    mainLayout->addWidget(productModelComboBox, 10, 5);

    mainLayout->addWidget(quantityLabel, 20, 0, Qt::AlignRight);
    mainLayout->addWidget(quantitySpinBox, 20, 1);
    mainLayout->addWidget(unitLabel, 20, 2, Qt::AlignRight);
    mainLayout->addWidget(unitLineEdit, 20, 3);
    mainLayout->addWidget(schemaLabel, 20, 4, Qt::AlignRight);
    mainLayout->addWidget(schemaComboBox, 20, 5);

    QFrame *line2 = new QFrame();
    line2->setAttribute(Qt::WA_MouseNoMask);
    line2->setFrameStyle(QFrame::Sunken);
    line2->setFrameShape(QFrame::HLine);
    mainLayout->addWidget(line2, 25, 0, 1, 6);

    mainLayout->addWidget(oldPurchasePriceLabel, 30, 0, Qt::AlignRight);
    mainLayout->addWidget(oldPurchasePriceLineEdit, 30, 1);
    mainLayout->addWidget(purchasePriceLabel, 30, 2, Qt::AlignRight);
    mainLayout->addWidget(purchasePriceLineEdit, 30, 3);
    mainLayout->addWidget(sellingPriceLabel, 30, 4, Qt::AlignRight);
    mainLayout->addWidget(sellingPriceLineEdit, 30, 5);

    QFrame *line3 = new QFrame();
    line3->setAttribute(Qt::WA_MouseNoMask);
    line3->setFrameStyle(QFrame::Sunken);
    line3->setFrameShape(QFrame::HLine);
    mainLayout->addWidget(line3, 35, 0, 1, 6);

    mainLayout->addWidget(commentsLabel, 36, 0, Qt::AlignLeft);
    mainLayout->addWidget(commentsTextEdit, 37, 0, 1, 6);

    addProductButton = new QPushButton(tr("Add Product"), this);
    connect(addProductButton, SIGNAL(clicked()), this, SLOT(addProduct()));
    clearInfoButton = new QPushButton(tr("Clear Info"), this);
    connect(clearInfoButton, SIGNAL(clicked()), this, SLOT(clearInfo()));
    QDialogButtonBox *addProductButtonBox = new QDialogButtonBox(Qt::Horizontal, this);
    addProductButtonBox->addButton(addProductButton, QDialogButtonBox::ActionRole);
    addProductButtonBox->addButton(clearInfoButton, QDialogButtonBox::ActionRole);
    mainLayout->addWidget(addProductButtonBox, 40, 2, 1, 2, Qt::AlignHCenter);
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);

    setWindowTitle(tr("Add Product"));
    setWindowIcon(QIcon(":/Icon/add_icon.png"));
    setLayout(mainLayout);

    userManagementInterface->registeObserver(this);

//    QTimer *timer = new QTimer(this);
//    connect(timer, SIGNAL(timeout()), this, SLOT(updateDBTableModel()));
//    timer->setInterval(1000);
//    timer->start(1000);
}

void AddProductDialog::addProduct()
{
    QString serialNumber = serialNumberLineEdit->text();
    serialNumber.simplified();
    if(serialNumber.isEmpty()) {
        QMessageBox::critical(this, tr("Add Product Error"), tr("Serial Number can't be empty !"));
        serialNumberLineEdit->setFocus();
        return;
    }
    if(productManagementInterface->checkSerialNumber(serialNumber)) {
        QMessageBox::critical(this, tr("Add Product Error"), tr("Serial Number has exist !"));
        serialNumberLineEdit->setFocus();
        return;
    }

    QString unit = unitLineEdit->text();
    unit.simplified();
    if(unit.isEmpty()) {
        QMessageBox::critical(this, tr("Add Product Error"), tr("Unit can't be empty !"));
        unitLineEdit->setFocus();
        return;
    }
    QString productType = productTypeComboBox->currentText();
    productType.simplified();
    if(productType.isEmpty()) {
        QMessageBox::critical(this, tr("Add Product Error"), tr("Product Type can't be empty !"));
        productTypeComboBox->setFocus();
        return;
    }

    int productTypeID = productManagementInterface->getTypeIDByTypeName(productType);
    if(!productTypeID) {
        productManagementInterface->addProductType(productType);
        productTypeModel->select();
        productTypeComboBox->setCurrentIndex(
                    productTypeComboBox->findText(productType));
    }
    productTypeID = productManagementInterface->getTypeIDByTypeName(productType);

    QString brandName = brandNameComboBox->currentText();
    brandName.simplified();
    if(brandName.isEmpty()) {
        QMessageBox::critical(this, tr("Add Product Error"), tr("Brand Name can't be empty !"));
        brandNameComboBox->setFocus();
        return;
    }
    int brandNameID = productManagementInterface->getBrandIDByBrandName(brandName);
    if(!brandNameID) {
        productManagementInterface->addBrandName(brandName);
        brandNameModel->select();
        brandNameComboBox->setCurrentIndex(
                    brandNameComboBox->findText(brandName));
    }
    brandNameID = productManagementInterface->getBrandIDByBrandName(brandName);

    QString productModel = productModelComboBox->currentText();
    productModel.simplified();
    if(productModel.isEmpty()) {
        QMessageBox::critical(this, tr("Add Product Error"), tr("Product Model can't be empty !"));
        productModelComboBox->setFocus();
        return;
    }
    int productModelID = productManagementInterface->getModelIDByTypeIDBrandIDModelName(productTypeID, brandNameID, productModel);
    if(!productModelID) {
        productManagementInterface->addProductModel(productTypeID, brandNameID, productModel);
        productModelModel->select();
        productModelComboBox->setCurrentIndex(
                    productModelComboBox->findText(productModel));
    }
    productModelID = productManagementInterface->getModelIDByTypeIDBrandIDModelName(productTypeID, brandNameID, productModel);

    QString oldPurchasePrice = oldPurchasePriceLineEdit->text().simplified();
    QString purchasePrice = purchasePriceLineEdit->text().simplified();
    QString sellingPrice = sellingPriceLineEdit->text().simplified();

    int schemaID = userManagementInterface->getSchemaIDBySchemaName(schemaComboBox->currentText());
    int quantity = quantitySpinBox->value();
    int userID = userManagementInterface->getUserIDByUserName(userManagementInterface->getCurrentUserName());
    int statusID = productManagementInterface->getStatusIDByStatusName("Â¼Èë");
    QString comments = commentsTextEdit->document()->toPlainText().simplified();
    productManagementInterface->addProductByDetail(serialNumber, productTypeID, brandNameID,
                                                   productModelID, schemaID, quantity, unit,
                                                   oldPurchasePrice, purchasePrice, sellingPrice,
                                                   userID, userID, statusID, comments);
    emit(productAdded());
    serialNumberLineEdit->clear();
    serialNumberLineEdit->setFocus();
}

void AddProductDialog::clearInfo()
{
    serialNumberLineEdit->clear();
    productTypeComboBox->setCurrentIndex(0);
    brandNameComboBox->setCurrentIndex(0);
    productTypeComboBox->setCurrentIndex(0);
    quantitySpinBox->setValue(1);
    unitLineEdit->clear();
    oldPurchasePriceLineEdit->clear();
    purchasePriceLineEdit->clear();
    sellingPriceLineEdit->clear();
    commentsTextEdit->clear();
}

void AddProductDialog::setProductModelFilter()
{
    QString productType = productTypeComboBox->currentText();
    QString brandName = brandNameComboBox->currentText();
    int productTypeID = productManagementInterface->getTypeIDByTypeName(productType);
    if(!productTypeID) {
        productTypeID = 1;
    }
    int brandNameID = productManagementInterface->getBrandIDByBrandName(brandName);
    if(!brandNameID) {
        brandNameID = 1;
    }

    productModelModel->setFilter(QString("productTypeID = %1 and brandNameID = %2")
                                 .arg(productTypeID).arg(brandNameID));
    productModelModel->select();
}

void AddProductDialog::populateSchemaComboBox() const
{
    QSet<QString> schemaNameSet;
    int userID = userManagementInterface->getUserIDByUserName(userManagementInterface->getCurrentUserName());
    if(userManagementInterface->isAdmin(userManagementInterface->getCurrentUserName())) {
        schemaNameSet = userManagementInterface->getAllSchemaName();
    }
    else {
        QSet<int> schemaIDSet = userManagementInterface->getSchemaIDSetByUserID(userID);
        foreach(int schemaID, schemaIDSet) {
            schemaNameSet<<userManagementInterface->getSchemaNameBySchemaID(schemaID);
        }
    }

    schemaComboBox->clear();
    schemaComboBox->addItems(schemaNameSet.toList());
//    QString schema = schemaComboBox->currentText();
//    QStringList list = schemaNameSet.toList();
//    schemaComboBox->addItems(list);
//    if(list.indexOf(schema)>-1) {
//        schemaComboBox->setCurrentIndex(list.indexOf(schema));
//    }
//    else {
//        schemaComboBox->setEditText(schema);
//    }
}

void AddProductDialog::updateDBTableModel()
{
//    QString tmp = productTypeComboBox->currentText();
//    QModelIndex index = productTypeComboBox->view()->currentIndex();
//    productTypeModel->select();
//    qDebug()<<tmp;
//    if(productTypeComboBox->findText(tmp)==-1) {
//        productTypeComboBox->setEditText(tmp);
//    }
//    else if(index.isValid()) {
//        productTypeComboBox->view()->setCurrentIndex(index);
//    }
//    else {
//        productTypeComboBox->setEditText(tmp);
//    }
//    QString tmp = productTypeComboBox->currentText();
//    productTypeModel->select();
//    productTypeComboBox->setEditText(tmp);


//    tmp = brandNameComboBox->currentText();
//    brandNameModel->select();
//    brandNameComboBox->setEditText(tmp);

//    tmp = productModelComboBox->currentText();
//    productModelModel->select();
//    productModelComboBox->setEditText(tmp);

    productTypeModel->select();
    brandNameModel->select();
    productModelModel->select();
    populateSchemaComboBox();
}

void AddProductDialog::userChanged() const
{
    populateSchemaComboBox();
}
