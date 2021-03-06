﻿#include <QtGui>
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
    productColorLabel(NULL),
    productColorComboBox(NULL),
    productColorModel(NULL),
    productVendorLabel(NULL),
    productVendorComboBox(NULL),
    productVendorModel(NULL),
    replacementInfoLabel(NULL),
    replacementInfoComboBox(NULL),
    replacementInfoModel(NULL),
    barInfoLabel(NULL),
    barInfoLineEdit(NULL),
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
    leftButton(NULL),
    rightButton(NULL)
{
    QDir qmdir(":/Translations");
    foreach (QString fileName, qmdir.entryList(QDir::Files)) {
        //qDebug()<<QFileInfo(fileName).baseName();
        QTranslator *qtTranslator = new QTranslator(this);
        qtTranslator->load(QFileInfo(fileName).baseName(), ":/Translations");
        QApplication::instance()->installTranslator(qtTranslator);
    }
    init();
}

UpdateProductDialog::~UpdateProductDialog()
{
    userManagementInterface->deregisteObserver(this);
}

void UpdateProductDialog::init()
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
            , this, SLOT(populateBrandNameComboBox()));

    brandNameLabel = new QLabel(tr("Brand Name")+"<b style=\"COLOR: #ff0000\">*</b>: ", this);
    brandNameComboBox = new QComboBox(this);
    brandNameComboBox->setEditable(true);
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

    productColorLabel = new QLabel(tr("Product Color")+"<b style=\"COLOR: #ff0000\">*</b>: ", this);
    productColorComboBox = new QComboBox(this);
    productColorComboBox->setEditable(true);
    productColorModel = new QSqlRelationalTableModel(this, db);
    productColorModel->setTable("colorinfo");
    productColorModel->select();
    productColorComboBox->setModel(productColorModel);
    productColorComboBox->setModelColumn(1);

    productVendorLabel = new QLabel(tr("Vendor")+"<b style=\"COLOR: #ff0000\">*</b>: ", this);
    productVendorComboBox = new QComboBox(this);
    productVendorComboBox->setEditable(true);
    productVendorModel = new QSqlRelationalTableModel(this, db);
    productVendorModel->setTable("vendorinfo");
    productVendorModel->select();
    productVendorComboBox->setModel(productVendorModel);
    productVendorComboBox->setModelColumn(1);
    \
    replacementInfoLabel = new QLabel(tr("Replacement Info")+"<b style=\"COLOR: #ff0000\">*</b>: ", this);
    replacementInfoComboBox = new QComboBox(this);
    replacementInfoComboBox->setEditable(false);
    replacementInfoModel = new QSqlRelationalTableModel(this, db);
    replacementInfoModel->setTable("replacementstatus");
    replacementInfoModel->select();
    replacementInfoComboBox->setModel(replacementInfoModel);
    replacementInfoComboBox->setModelColumn(1);

    barInfoLabel = new QLabel(tr("Bar Info")+"<b style=\"COLOR: #ff0000\">*</b>: ", this);
    barInfoLineEdit = new QLineEdit(this);
    barInfoLabel->setEnabled(false);
    barInfoLineEdit->setEnabled(false);

    sellerLabel = new QLabel(tr("Seller")+"<b style=\"COLOR: #ff0000\">*</b>: ", this);
    sellerComboBox = new QComboBox(this);
    sellerComboBox->setEditable(false);
    populateSellerNameComboBox();
    sellerLabel->setEnabled(false);
    sellerComboBox->setEnabled(false);

    quantityLabel = new QLabel(tr("Quantity")+"<b style=\"COLOR: #ff0000\">*</b>: ", this);
    quantitySpinBox = new QSpinBox(this);
    quantitySpinBox->setMinimum(1);
    quantitySpinBox->setValue(1);

    unitLabel = new QLabel(tr("Unit")+"<b style=\"COLOR: #ff0000\">*</b>: ", this);
    unitLineEdit = new QLineEdit(this);

    schemaLabel = new QLabel(tr("Schema")+"<b style=\"COLOR: #ff0000\">*</b>: ", this);
    schemaComboBox = new QComboBox(this);
    schemaComboBox->setEditable(false);
    populateSchemaComboBox();

    oldPurchasePriceLabel = new QLabel(tr("Old Purchase Price: "), this);
    oldPurchasePriceLineEdit = new QLineEdit(this);
    oldPurchasePriceLineEdit->setValidator(new QRegExpValidator(QRegExp("(0|([1-9][0-9]*\\.?[0-9]*))"), this));

    purchasePriceLabel = new QLabel(tr("Purchase Price: "), this);
    purchasePriceLineEdit = new QLineEdit(this);
    purchasePriceLineEdit->setValidator(new QRegExpValidator(QRegExp("(0|([1-9][0-9]*\\.?[0-9]*))"), this));

    sellingPriceLabel = new QLabel(tr("Selling Price: "), this);
    sellingPriceLineEdit = new QLineEdit(this);
    sellingPriceLineEdit->setValidator(new QRegExpValidator(QRegExp("(0|([1-9][0-9]*\\.?[0-9]*))"), this));

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

    mainLayout->addWidget(productColorLabel, 15, 0, Qt::AlignRight);
    mainLayout->addWidget(productColorComboBox, 15, 1);
    mainLayout->addWidget(productVendorLabel, 15, 2, Qt::AlignRight);
    mainLayout->addWidget(productVendorComboBox, 15, 3);
    mainLayout->addWidget(replacementInfoLabel, 15, 4, Qt::AlignRight);
    mainLayout->addWidget(replacementInfoComboBox, 15, 5);

    mainLayout->addWidget(quantityLabel, 20, 0, Qt::AlignRight);
    mainLayout->addWidget(quantitySpinBox, 20, 1);
    mainLayout->addWidget(unitLabel, 20, 2, Qt::AlignRight);
    mainLayout->addWidget(unitLineEdit, 20, 3);
    mainLayout->addWidget(schemaLabel, 20, 4, Qt::AlignRight);
    mainLayout->addWidget(schemaComboBox, 20, 5);

    mainLayout->addWidget(barInfoLabel, 23, 0, Qt::AlignRight);
    mainLayout->addWidget(barInfoLineEdit, 23, 1);
    mainLayout->addWidget(sellerLabel, 23, 2, Qt::AlignRight);
    mainLayout->addWidget(sellerComboBox, 23, 3);

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

    updateProductButton = new QPushButton(tr("Update Product"), this);
    connect(updateProductButton, SIGNAL(clicked()), this, SLOT(updateProductInfo()));
    leftButton = new QPushButton(tr("Left Button"), this);
    leftButton->hide();
    rightButton = new QPushButton(tr("Right Button"), this);
    rightButton->hide();

    QHBoxLayout *buttonsHLayout = new QHBoxLayout();
    buttonsHLayout->addWidget(leftButton);
    buttonsHLayout->addWidget(updateProductButton);
    buttonsHLayout->addWidget(rightButton);

    mainLayout->addLayout(buttonsHLayout, 40, 2, 1, 2, Qt::AlignHCenter);
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);

    setWindowTitle(tr("Update Product"));
    setWindowIcon(QIcon(":/Icon/update_icon.png"));
    setLayout(mainLayout);

    userManagementInterface->registeObserver(this);

}

void UpdateProductDialog::updateProductInfo()
{
    if(!checkInput()) {
        return;
    }

    productManagementInterface->updateProductDetailByProductID(productID, serialNumber, productTypeID, brandNameID,
                                                               productModelID, productColorID, productVendorID,
                                                               schemaID, quantity, unit, oldPurchasePrice, purchasePrice,
                                                               sellingPrice, userID, userID, sellerID, barInfo, statusID,
                                                               replacementInfoID, timeStamp, comments);

    emit(productUpdated());
    updateDBTableModel();
    //accept();
}

void UpdateProductDialog::populateBrandNameComboBox()
{
    QString productType = productTypeComboBox->currentText();
    int productTypeID = productManagementInterface->getTypeIDByTypeName(productType);
    if(!productTypeID) {
        productTypeID = 1;
    }

    QSet<int> IDs = productManagementInterface->getBrandNameIDSetbyProductTypeID(productTypeID);
    //qDebug()<<IDs;
    brandNameComboBox->clear();
    foreach(int id, IDs) {
        QString brandName = productManagementInterface->getBrandNameByBrandID(id);
        brandNameComboBox->addItem(brandName);
    }
    setProductModelFilter();
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
}

void UpdateProductDialog::populateSellerNameComboBox() const
{
    int sellerRoleID = userManagementInterface->getRoleIDByRoleName("销售");
    int adminRoleID = userManagementInterface->getRoleIDByRoleName("管理员");
    QSet<int> sellers = userManagementInterface->getUserIDSetByRoleID(sellerRoleID);
    QSet<int> admins = userManagementInterface->getUserIDSetByRoleID(adminRoleID);
    sellerComboBox->clear();
    //sellerComboBox->addItem(userManagementInterface->getUserNameByUserID(1));
    sellerComboBox->addItem("未指定");
    foreach(int admin, admins) {
        sellerComboBox->addItem(userManagementInterface->getUserNameByUserID(admin));
    }
    foreach(int seller, sellers) {
        sellerComboBox->addItem(userManagementInterface->getUserNameByUserID(seller));
    }
}

void UpdateProductDialog::updateDBTableModel()
{
    QString productType = productTypeComboBox->currentText();
    QString brandName = brandNameComboBox->currentText();
    QString productModel = productModelComboBox->currentText();
    QString productColor = productColorComboBox->currentText();
    QString productVendor = productVendorComboBox->currentText();
    QString replacementInfo = replacementInfoComboBox->currentText();
    QString schemaInfo = schemaComboBox->currentText();
    QString sellerName = sellerComboBox->currentText();

    productTypeModel->select();
    //brandNameModel->select();
    populateBrandNameComboBox();
    productModelModel->select();
    productColorModel->select();
    productVendorModel->select();
    replacementInfoModel->select();
    populateSchemaComboBox();
    populateSellerNameComboBox();

    if(!productType.simplified().isEmpty()) {
        productTypeComboBox->setCurrentIndex(
                    productTypeComboBox->findText(productType));
    }
    if(!brandName.simplified().isEmpty()) {
        brandNameComboBox->setCurrentIndex(
                    brandNameComboBox->findText(brandName));
    }
    if(!productModel.simplified().isEmpty()) {
        productModelComboBox->setCurrentIndex(
                    productModelComboBox->findText(productModel));
    }
    if(!productColor.simplified().isEmpty()) {
        productColorComboBox->setCurrentIndex(
                    productColorComboBox->findText(productColor));
    }
    if(!productVendor.simplified().isEmpty()) {
        productVendorComboBox->setCurrentIndex(
                    productVendorComboBox->findText(productVendor));
    }
    if(!replacementInfo.simplified().isEmpty()) {
        replacementInfoComboBox->setCurrentIndex(
                    replacementInfoComboBox->findText(replacementInfo));
    }
    if(!schemaInfo.simplified().isEmpty()) {
        schemaComboBox->setCurrentIndex(
                    schemaComboBox->findText(schemaInfo));
    }
    if(!sellerName.simplified().isEmpty()) {
        sellerComboBox->setCurrentIndex(
                    sellerComboBox->findText(sellerName));
    }
}

void UpdateProductDialog::updateRecord(const QSqlRecord &record)
{
    serialNumberLineEdit->setText(record.value(SerialNumber).toString());
    productTypeComboBox->setEditText(record.value(ProductTypeID).toString());
    brandNameComboBox->setEditText(record.value(BrandNameID).toString());
    productModelComboBox->setEditText(record.value(ProductModelID).toString());
    productColorComboBox->setEditText(record.value(ColorID).toString());
    productVendorComboBox->setEditText(record.value(VendorID).toString());
    replacementInfoComboBox->setCurrentIndex(replacementInfoComboBox->findText(record.value(ReplacementStatusID).toString()));
    quantitySpinBox->setValue(record.value(Quantity).toInt());
    unitLineEdit->setText(record.value(Unit).toString());
    schemaComboBox->setCurrentIndex(schemaComboBox->findText(record.value(SchemaNameID).toString()));
    barInfoLineEdit->setText(record.value(BarInfo).toString());
    sellerComboBox->setEditText(record.value(SellerID).toString());
    oldPurchasePriceLineEdit->setText(record.value(OldPurchasePrice).toString());
    purchasePriceLineEdit->setText(record.value(PurchasePrice).toString());
    sellingPriceLineEdit->setText(record.value(SellingPrice).toString());
    commentsTextEdit->setText(record.value(Comments).toString());
    this->record = record;
}

void UpdateProductDialog::userChanged()
{
    populateSchemaComboBox();
    populateSellerNameComboBox();
}

void UpdateProductDialog::hidePurchasePrice(bool hide)
{
    oldPurchasePriceLabel->setHidden(hide);
    oldPurchasePriceLineEdit->setHidden(hide);
    purchasePriceLabel->setHidden(hide);
    purchasePriceLineEdit->setHidden(hide);
}

bool UpdateProductDialog::checkInput()
{
    serialNumber = serialNumberLineEdit->text();
    serialNumber.simplified();
    if(serialNumber.isEmpty()) {
        QMessageBox::critical(this, tr("Error"), tr("Serial Number can't be empty !"));
        serialNumberLineEdit->setFocus();
        return false;
    }

    unit = unitLineEdit->text();
    unit.simplified();
    if(unit.isEmpty()) {
        QMessageBox::critical(this, tr("Error"), tr("Unit can't be empty !"));
        unitLineEdit->setFocus();
        return false;
    }

    QString productType = productTypeComboBox->currentText();
    QString brandName = brandNameComboBox->currentText();
    QString productModel = productModelComboBox->currentText();

    productType.simplified();
    if(productType.isEmpty()) {
        QMessageBox::critical(this, tr("Error"), tr("Product Type can't be empty !"));
        productTypeComboBox->setFocus();
        return false;
    }
    productTypeID = productManagementInterface->getTypeIDByTypeName(productType);
    if(!productTypeID) {
        productManagementInterface->addProductType(productType);
    }

    brandName.simplified();
    if(brandName.isEmpty()) {
        QMessageBox::critical(this, tr("Error"), tr("Brand Name can't be empty !"));
        brandNameComboBox->setFocus();
        return false;
    }
    brandNameID = productManagementInterface->getBrandIDByBrandName(brandName);
    if(!brandNameID) {
        productManagementInterface->addBrandName(brandName);
    }

    productModel.simplified();
    if(productModel.isEmpty()) {
        QMessageBox::critical(this, tr("Error"), tr("Product Model can't be empty !"));
        productModelComboBox->setFocus();
        return false;
    }
    productModelID = productManagementInterface->getModelIDByTypeIDBrandIDModelName(productTypeID, brandNameID, productModel);
    if(!productModelID) {
        productManagementInterface->addProductModel(productTypeID, brandNameID, productModel);
    }

    QString productColor = productColorComboBox->currentText();
    productColor.simplified();
    if(productColor.isEmpty()) {
        QMessageBox::critical(this, tr("Error"), tr("Product Color can't be empty !"));
        productColorComboBox->setFocus();
        return false;
    }
    productColorID = productManagementInterface->getColorIDByColorName(productColor);
    if(!productColorID) {
        productManagementInterface->addColorName(productColor);
    }

    QString productVendor = productVendorComboBox->currentText();
    productVendor.simplified();
    if(productVendor.isEmpty()) {
        QMessageBox::critical(this, tr("Error"), tr("Product Vendor can't be empty !"));
        productVendorComboBox->setFocus();
        return false;
    }
    productVendorID = productManagementInterface->getVendorIDByVendorName(productVendor);
    if(!productVendorID) {
        productManagementInterface->addVendorName(productVendor);
    }

    QString replacementInfo = replacementInfoComboBox->currentText();
    replacementInfo.simplified();
    if(replacementInfo.isEmpty()) {
        QMessageBox::critical(this, tr("Error"), tr("Replacement Info can't be empty !"));
        replacementInfoComboBox->setFocus();
        return false;
    }

    serialNumber = serialNumberLineEdit->text().simplified();
    unit = unitLineEdit->text().simplified();
    productTypeID = productManagementInterface->getTypeIDByTypeName(productTypeComboBox->currentText().simplified());
    brandNameID = productManagementInterface->getBrandIDByBrandName(brandNameComboBox->currentText().simplified());
    productModelID = productManagementInterface->getModelIDByTypeIDBrandIDModelName(productTypeID, brandNameID,
                                                                                        productModelComboBox->currentText().simplified());
    productColorID = productManagementInterface->getColorIDByColorName(productColorComboBox->currentText().simplified());
    productVendorID = productManagementInterface->getVendorIDByVendorName(productVendorComboBox->currentText().simplified());
    replacementInfoID = productManagementInterface->getReplacementStatusIDByReplacementStatusName(
                replacementInfoComboBox->currentText().simplified());
    sellerID = userManagementInterface->getUserIDByUserName(record.value(SellerID).toString());
    barInfo = barInfoLineEdit->text();
    oldPurchasePrice = oldPurchasePriceLineEdit->text().simplified();
    purchasePrice = purchasePriceLineEdit->text().simplified();
    sellingPrice = sellingPriceLineEdit->text().simplified();
    schemaID = userManagementInterface->getSchemaIDBySchemaName(schemaComboBox->currentText());
    quantity = quantitySpinBox->value();
    userID = userManagementInterface->getUserIDByUserName(userManagementInterface->getCurrentUserName());
    statusID = productManagementInterface->getStatusIDByStatusName(record.value(ProductStatusID).toString());
    comments = commentsTextEdit->document()->toPlainText().simplified();
    timeStamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    productID = record.value(ProductID).toInt();

    return true;
}
