#include <QtGui>
#include <QtSql>
#include <usermanagement_interface.h>
#include "productmanagement_interface.h"
#include "updateproductdialog.h"

UpdateProductDialog::UpdateProductDialog(UserManagementInterface *userManagementInterface,
                                         ProductManagementInterface *productManagementInterface,
                                         QWidget *parent):
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
    updateProductButton(NULL),
    clearInfoButton(NULL)
{
    init();
}

UpdateProductDialog::~UpdateProductDialog()
{
    userManagementInterface->deregisteObserver(this);
}

void UpdateProductDialog::init()
{
    serialNumberLabel = new QLabel(tr("Serial Number")+"<b style=\"COLOR: #ff0000\">*</b>: ", this);
    serialNumberLabel->setEnabled(false);
    serialNumberLineEdit = new QLineEdit(this);
    serialNumberLineEdit->setEnabled(false);

    productTypeLabel = new QLabel(tr("Product Type")+"<b style=\"COLOR: #ff0000\">*</b>: ", this);
    productTypeLabel->setEnabled(false);
    productTypeComboBox = new QComboBox(this);
    productTypeComboBox->setEnabled(false);
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
    brandNameLabel->setEnabled(false);
    brandNameComboBox = new QComboBox(this);
    brandNameComboBox->setEnabled(false);
    brandNameComboBox->setEditable(true);
    brandNameModel = new QSqlRelationalTableModel(this, db);
    brandNameModel->setTable("brandname");
    brandNameModel->select();
    brandNameComboBox->setModel(brandNameModel);
    brandNameComboBox->setModelColumn(1);
    connect(brandNameComboBox, SIGNAL(editTextChanged(const QString &))
            , this, SLOT(setProductModelFilter()));

    productModelLabel = new QLabel(tr("Product Model")+"<b style=\"COLOR: #ff0000\">*</b>: ", this);
    productModelLabel->setEnabled(false);
    productModelComboBox = new QComboBox(this);
    productModelComboBox->setEnabled(false);
    productModelComboBox->setEditable(true);
    productModelModel = new QSqlRelationalTableModel(this, db);
    productModelModel->setTable("productmodel");
    setProductModelFilter();
    productModelComboBox->setModel(productModelModel);
    productModelComboBox->setModelColumn(3);

    quantityLabel = new QLabel(tr("Quantity")+"<b style=\"COLOR: #ff0000\">*</b>: ", this);
    quantityLabel->setEnabled(false);
    quantitySpinBox = new QSpinBox(this);
    quantitySpinBox->setEnabled(false);
    quantitySpinBox->setMinimum(1);
    quantitySpinBox->setValue(1);

    unitLabel = new QLabel(tr("Unit")+"<b style=\"COLOR: #ff0000\">*</b>: ", this);
    unitLabel->setEnabled(false);
    unitLineEdit = new QLineEdit(this);
    unitLineEdit->setEnabled(false);

    schemaLabel = new QLabel(tr("Schema: "), this);
    schemaLabel->setEnabled(false);
    schemaComboBox = new QComboBox(this);
    schemaComboBox->setEnabled(false);
    schemaComboBox->setEditable(true);
    populateSchemaComboBox();

    oldPurchasePriceLabel = new QLabel(tr("Old Purchase Price: "), this);
    oldPurchasePriceLabel->setEnabled(false);
    oldPurchasePriceLineEdit = new QLineEdit(this);
    oldPurchasePriceLineEdit->setEnabled(false);

    purchasePriceLabel = new QLabel(tr("Purchase Price: "), this);
    purchasePriceLabel->setEnabled(false);
    purchasePriceLineEdit = new QLineEdit(this);
    purchasePriceLineEdit->setEnabled(false);

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

    updateProductButton = new QPushButton(tr("Submit Order"), this);
    updateProductButton->setDefault(true);
    connect(updateProductButton, SIGNAL(clicked()), this, SLOT(updateProductInfo()));
    clearInfoButton = new QPushButton(tr("Clear Info"), this);
    clearInfoButton->hide();
    connect(clearInfoButton, SIGNAL(clicked()), this, SLOT(clearInfo()));
    QDialogButtonBox *addProductButtonBox = new QDialogButtonBox(Qt::Horizontal, this);
    addProductButtonBox->addButton(updateProductButton, QDialogButtonBox::ActionRole);
    //addProductButtonBox->addButton(clearInfoButton, QDialogButtonBox::ActionRole);
    mainLayout->addWidget(addProductButtonBox, 40, 2, 1, 2, Qt::AlignHCenter);
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);

    setWindowTitle(tr("Submit Order"));
    setWindowIcon(QIcon(":/Icon/submitorder_icon.png"));
    setLayout(mainLayout);

    userManagementInterface->registeObserver(this);

//    QTimer *timer = new QTimer(this);
//    connect(timer, SIGNAL(timeout()), this, SLOT(updateDBTableModel()));
//    timer->setInterval(1000);
//    timer->start(1000);
}

void UpdateProductDialog::updateProductInfo()
{
    QString serialNumber = serialNumberLineEdit->text();
    serialNumber.simplified();

    QString unit = unitLineEdit->text();
    unit.simplified();
    if(unit.isEmpty()) {
        QMessageBox::critical(this, tr("Submit Order Error"), tr("Unit can't be empty !"));
        unitLineEdit->setFocus();
        return;
    }
    QString productType = productTypeComboBox->currentText();
    productType.simplified();
    if(productType.isEmpty()) {
        QMessageBox::critical(this, tr("Submit Order Error"), tr("Product Type can't be empty !"));
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
        QMessageBox::critical(this, tr("Submit Order Error"), tr("Brand Name can't be empty !"));
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
        QMessageBox::critical(this, tr("Submit Order Error"), tr("Product Model can't be empty !"));
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
    if(sellingPrice.isEmpty()) {
        QMessageBox::critical(this, tr("Submit Order Error"), tr("Selling Price can't be empty !"));
        sellingPriceLineEdit->setFocus();
        return;
    }

    int schemaID = userManagementInterface->getSchemaIDBySchemaName(schemaComboBox->currentText());
    int quantity = quantitySpinBox->value();
    int userID = userManagementInterface->getUserIDByUserName(userManagementInterface->getCurrentUserName());
    int statusID = productManagementInterface->getStatusIDByStatusName("ÒÑÈë¿â");
    QString comments = commentsTextEdit->document()->toPlainText().simplified();
    int productID = record.value(ProductID).toInt();
    productManagementInterface->updateProductDetailByProductID(productID, serialNumber, productTypeID, brandNameID,
                                                        productModelID, schemaID, quantity, unit,
                                                        oldPurchasePrice, purchasePrice, sellingPrice,
                                                        userID, userID, statusID, comments);
    //emit(productUpdated());
    accept();
//    serialNumberLineEdit->clear();
//    serialNumberLineEdit->setFocus();
}

void UpdateProductDialog::clearInfo()
{
    //serialNumberLineEdit->clear();
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

void UpdateProductDialog::setProductModelFilter()
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

void UpdateProductDialog::populateSchemaComboBox() const
{
    QSet<QString> schemaNameSet = userManagementInterface->getAllSchemaName();
//    int userID = userManagementInterface->getUserIDByUserName(userManagementInterface->getCurrentUserName());
//    if(userManagementInterface->isAdmin(userManagementInterface->getCurrentUserName())) {
//        schemaNameSet = userManagementInterface->getAllSchemaName();
//    }
//    else {
//        QSet<int> schemaIDSet = userManagementInterface->getSchemaIDSetByUserID(userID);
//        foreach(int schemaID, schemaIDSet) {
//            schemaNameSet<<userManagementInterface->getSchemaNameBySchemaID(schemaID);
//        }
//    }

    schemaComboBox->clear();
    schemaComboBox->addItems(schemaNameSet.toList());
}

void UpdateProductDialog::updateDBTableModel()
{
    productTypeModel->select();
    brandNameModel->select();
    productModelModel->select();
    populateSchemaComboBox();
}

void UpdateProductDialog::updateRecord(const QSqlRecord &record)
{
    serialNumberLineEdit->setText(record.value(SerialNumber).toString());
    serialNumberLineEdit->setEnabled(false);
    productTypeComboBox->setEditText(record.value(ProductTypeID).toString());
    brandNameComboBox->setEditText(record.value(BrandNameID).toString());
    productModelComboBox->setEditText(record.value(ProductModelID).toString());
    quantitySpinBox->setValue(record.value(Quantity).toInt());
    unitLineEdit->setText(record.value(Unit).toString());
    schemaComboBox->setEditText(record.value(SchemaNameID).toString());
    oldPurchasePriceLineEdit->setText(record.value(OldPurchasePrice).toString());
    purchasePriceLineEdit->setText(record.value(PurchasePrice).toString());
    sellingPriceLineEdit->setText(record.value(SellingPrice).toString());
    commentsTextEdit->setText(record.value(Comments).toString());
    this->record = record;
}

void UpdateProductDialog::userChanged() const
{
    populateSchemaComboBox();
}
