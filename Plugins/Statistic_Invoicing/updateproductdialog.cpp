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
    printBillButton(NULL),
    updateProductButtonBox(NULL),
    printer(NULL)
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
    serialNumberLineEdit = new QLineEdit(this);
    serialNumberLabel->setEnabled(false);
    serialNumberLineEdit->setReadOnly(true);

    productTypeLabel = new QLabel(tr("Product Type")+"<b style=\"COLOR: #ff0000\">*</b>: ", this);
    productTypeComboBox = new QComboBox(this);
    productTypeComboBox->setEditable(true);
    productTypeLabel->setEnabled(false);
    productTypeComboBox->setEnabled(false);
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
    brandNameLabel->setEnabled(false);
    brandNameComboBox->setEnabled(false);
    connect(brandNameComboBox, SIGNAL(editTextChanged(const QString &))
            , this, SLOT(setProductModelFilter()));

    productModelLabel = new QLabel(tr("Product Model")+"<b style=\"COLOR: #ff0000\">*</b>: ", this);
    productModelComboBox = new QComboBox(this);
    productModelComboBox->setEditable(true);
    productModelLabel->setEnabled(false);
    productModelComboBox->setEnabled(false);
    productModelModel = new QSqlRelationalTableModel(this, db);
    productModelModel->setTable("productmodel");
    setProductModelFilter();
    productModelComboBox->setModel(productModelModel);
    productModelComboBox->setModelColumn(3);

    productColorLabel = new QLabel(tr("Product Color")+"<b style=\"COLOR: #ff0000\">*</b>: ", this);
    productColorComboBox = new QComboBox(this);
    productColorComboBox->setEditable(true);
    productColorLabel->setEnabled(false);
    productColorComboBox->setEnabled(false);
    productColorModel = new QSqlRelationalTableModel(this, db);
    productColorModel->setTable("colorinfo");
    productColorModel->select();
    productColorComboBox->setModel(productColorModel);
    productColorComboBox->setModelColumn(1);

    productVendorLabel = new QLabel(tr("Vendor")+"<b style=\"COLOR: #ff0000\">*</b>: ", this);
    productVendorComboBox = new QComboBox(this);
    productVendorComboBox->setEditable(true);
    productVendorLabel->setEnabled(false);
    productVendorComboBox->setEnabled(false);
    productVendorModel = new QSqlRelationalTableModel(this, db);
    productVendorModel->setTable("vendorinfo");
    productVendorModel->select();
    productVendorComboBox->setModel(productVendorModel);
    productVendorComboBox->setModelColumn(1);
    \
    replacementInfoLabel = new QLabel(tr("Replacement Info")+"<b style=\"COLOR: #ff0000\">*</b>: ", this);
    replacementInfoComboBox = new QComboBox(this);
    replacementInfoComboBox->setEditable(false);
    replacementInfoLabel->setEnabled(false);
    replacementInfoComboBox->setEnabled(false);
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
    quantityLabel->setEnabled(false);
    quantitySpinBox->setEnabled(false);
    quantitySpinBox->setMinimum(1);
    quantitySpinBox->setValue(1);

    unitLabel = new QLabel(tr("Unit")+"<b style=\"COLOR: #ff0000\">*</b>: ", this);
    unitLineEdit = new QLineEdit(this);
    unitLabel->setEnabled(false);
    unitLineEdit->setEnabled(false);

    schemaLabel = new QLabel(tr("Schema")+"<b style=\"COLOR: #ff0000\">*</b>: ", this);
    schemaComboBox = new QComboBox(this);
    schemaComboBox->setEditable(false);
    schemaLabel->setEnabled(false);
    schemaComboBox->setEnabled(false);
    populateSchemaComboBox();

    oldPurchasePriceLabel = new QLabel(tr("Old Purchase Price: "), this);
    oldPurchasePriceLineEdit = new QLineEdit(this);
    oldPurchasePriceLabel->setEnabled(false);
    oldPurchasePriceLineEdit->setEnabled(false);
    oldPurchasePriceLineEdit->setValidator(new QRegExpValidator(QRegExp("(0|([1-9][0-9]*\\.?[0-9]*))"), this));

    purchasePriceLabel = new QLabel(tr("Purchase Price: "), this);
    purchasePriceLineEdit = new QLineEdit(this);
    purchasePriceLabel->setEnabled(false);
    purchasePriceLineEdit->setEnabled(false);
    purchasePriceLineEdit->setValidator(new QRegExpValidator(QRegExp("(0|([1-9][0-9]*\\.?[0-9]*))"), this));

    sellingPriceLabel = new QLabel(tr("Selling Price")+"<b style=\"COLOR: #ff0000\">*</b>: ", this);
    sellingPriceLineEdit = new QLineEdit(this);
    sellingPriceLabel->setEnabled(false);
    sellingPriceLineEdit->setReadOnly(true);
    purchasePriceLineEdit->setValidator(new QRegExpValidator(QRegExp("(0|([1-9][0-9]*\\.?[0-9]*))"), this));

    commentsLabel = new QLabel(tr("Comments: "), this);
    commentsTextEdit = new QTextEdit(this);
    commentsLabel->setEnabled(false);
    commentsTextEdit->setReadOnly(true);

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

    updateProductButton = new QPushButton(tr("Paid"), this);
    updateProductButton->setDefault(true);
    connect(updateProductButton, SIGNAL(clicked()), this, SLOT(updateProductInfo()));
    printBillButton = new QPushButton(tr("Print Bill"), this);
    printBillButton->setDefault(false);
    printBillButton->setAutoDefault(false);
    connect(printBillButton, SIGNAL(clicked()), this, SLOT(printBill()));
    updateProductButtonBox = new QDialogButtonBox(Qt::Horizontal, this);
    updateProductButtonBox->addButton(printBillButton, QDialogButtonBox::ActionRole);
    updateProductButtonBox->addButton(updateProductButton, QDialogButtonBox::ActionRole);
    updateProductButtonBox->hide();
    //mainLayout->addWidget(updateProductButtonBox, 40, 2, 1, 2, Qt::AlignHCenter);
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);

    setWindowTitle(tr("Product Info"));
    setWindowIcon(QIcon(":/Icon/statistic_icon.png"));
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
    if(serialNumber.isEmpty()) {
        QMessageBox::critical(this, tr("Update Product Error"), tr("Serial Number can't be empty !"));
        serialNumberLineEdit->setFocus();
        return;
    }
//    if(productManagementInterface->checkSerialNumber(serialNumber)) {
//        QMessageBox::critical(this, tr("Update Product Error"), tr("Serial Number has exist !"));
//        serialNumberLineEdit->setFocus();
//        return;
//    }

    QString unit = unitLineEdit->text();
    unit.simplified();
    if(unit.isEmpty()) {
        QMessageBox::critical(this, tr("Update Product Error"), tr("Unit can't be empty !"));
        unitLineEdit->setFocus();
        return;
    }

    QString productType = productTypeComboBox->currentText();
    QString brandName = brandNameComboBox->currentText();
    QString productModel = productModelComboBox->currentText();

    productType.simplified();
    if(productType.isEmpty()) {
        QMessageBox::critical(this, tr("Update Product Error"), tr("Product Type can't be empty !"));
        productTypeComboBox->setFocus();
        return;
    }
    int productTypeID = productManagementInterface->getTypeIDByTypeName(productType);
    if(!productTypeID) {
        productManagementInterface->addProductType(productType);
        productTypeID = productManagementInterface->getTypeIDByTypeName(productType);
    }


    brandName.simplified();
    if(brandName.isEmpty()) {
        QMessageBox::critical(this, tr("Update Product Error"), tr("Brand Name can't be empty !"));
        brandNameComboBox->setFocus();
        return;
    }
    int brandNameID = productManagementInterface->getBrandIDByBrandName(brandName);
    if(!brandNameID) {
        productManagementInterface->addBrandName(brandName);
        brandNameID = productManagementInterface->getBrandIDByBrandName(brandName);
    }


    productModel.simplified();
    if(productModel.isEmpty()) {
        QMessageBox::critical(this, tr("Update Product Error"), tr("Product Model can't be empty !"));
        productModelComboBox->setFocus();
        return;
    }
    int productModelID = productManagementInterface->getModelIDByTypeIDBrandIDModelName(productTypeID, brandNameID, productModel);
    if(!productModelID) {
        productManagementInterface->addProductModel(productTypeID, brandNameID, productModel);
        productModelID = productManagementInterface->getModelIDByTypeIDBrandIDModelName(productTypeID, brandNameID, productModel);
    }

    QString productColor = productColorComboBox->currentText();
    productColor.simplified();
    if(productColor.isEmpty()) {
        QMessageBox::critical(this, tr("Update Product Error"), tr("Product Color can't be empty !"));
        productColorComboBox->setFocus();
        return;
    }
    int productColorID = productManagementInterface->getColorIDByColorName(productColor);
    if(!productColorID) {
        productManagementInterface->addColorName(productColor);
        productColorID = productManagementInterface->getColorIDByColorName(productColor);
    }

    QString productVendor = productVendorComboBox->currentText();
    productVendor.simplified();
    if(productVendor.isEmpty()) {
        QMessageBox::critical(this, tr("Update Product Error"), tr("Product Vendor can't be empty !"));
        productVendorComboBox->setFocus();
        return;
    }
    int productVendorID = productManagementInterface->getVendorIDByVendorName(productVendor);
    if(!productVendorID) {
        productManagementInterface->addVendorName(productVendor);
        productVendorID = productManagementInterface->getVendorIDByVendorName(productVendor);
    }

    QString replacementInfo = replacementInfoComboBox->currentText();
    replacementInfo.simplified();
    if(replacementInfo.isEmpty()) {
        QMessageBox::critical(this, tr("Update Product Error"), tr("Replacement Info can't be empty !"));
        replacementInfoComboBox->setFocus();
        return;
    }
    int replacementInfoID = productManagementInterface->getReplacementStatusIDByReplacementStatusName(replacementInfo);

    int sellerID = userManagementInterface->getUserIDByUserName(record.value(SellerID).toString());
    QString barInfo = barInfoLineEdit->text();

    QString oldPurchasePrice = oldPurchasePriceLineEdit->text().simplified();
    QString purchasePrice = purchasePriceLineEdit->text().simplified();
    QString sellingPrice = sellingPriceLineEdit->text().simplified();

    int schemaID = userManagementInterface->getSchemaIDBySchemaName(schemaComboBox->currentText());
    int quantity = quantitySpinBox->value();
    int userID = userManagementInterface->getUserIDByUserName(userManagementInterface->getCurrentUserName());
    int statusID = productManagementInterface->getStatusIDByStatusName("已销售");
    QString comments = commentsTextEdit->document()->toPlainText().simplified();
    //qDebug()<<comments;
    QDateTime tmpTime = QDateTime::currentDateTime();
    QString timeStamp = tmpTime.toString("yyyy-MM-dd hh:mm:ss");

    int productID = record.value(ProductID).toInt();
    productManagementInterface->updateProductDetailByProductID(productID, serialNumber, productTypeID, brandNameID,
                                                               productModelID, productColorID, productVendorID,
                                                               schemaID, quantity, unit, oldPurchasePrice, purchasePrice,
                                                               sellingPrice, userID, userID, sellerID, barInfo, statusID,
                                                               replacementInfoID, timeStamp, comments);

    productManagementInterface->deleteProductByProductID(productID);

    emit(productUpdated());
    updateDBTableModel();
    updateProductButton->setEnabled(false);
    sellingPriceLabel->setEnabled(false);
    sellingPriceLineEdit->setEnabled(false);
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
    sellerComboBox->setCurrentIndex(sellerComboBox->findText(record.value(SellerID).toString()));
    oldPurchasePriceLineEdit->setText(record.value(OldPurchasePrice).toString());
    purchasePriceLineEdit->setText(record.value(PurchasePrice).toString());
    sellingPriceLineEdit->setText(record.value(SellingPrice).toString());
    commentsTextEdit->setText(record.value(Comments).toString());
    this->record = record;
//    updateProductButton->setEnabled(true);
//    sellingPriceLabel->setEnabled(true);
    sellingPriceLineEdit->setEnabled(true);
}

void UpdateProductDialog::userChanged()
{
    populateSchemaComboBox();
    populateSellerNameComboBox();
}

void UpdateProductDialog::printBill()
{
    if(!printer) {
        printer = new QPrinter(QPrinter::HighResolution);
    }

    QPrintDialog *dialog = new QPrintDialog(printer, this);
    dialog->setWindowTitle(tr("Print Bill"));
    if (dialog->exec() == QDialog::Accepted) {
        QString html = getPrintContent();
        QTextDocument textDocument;
        textDocument.setHtml(html);
        textDocument.print(printer);

//        QPainter painter;
//        painter.begin(printer);

//        updateProductButtonBox->hide();
//        QString originalText = commentsTextEdit->toPlainText();
//        QString declearText = "\n\n\n=========================\n此单据为销售及保修凭证，盖章后生效\n=========================";
//        commentsTextEdit->setPlainText(originalText + declearText);
//        double xscale = printer->pageRect().width()/double(this->width());
//        double yscale = printer->pageRect().height()/double(this->height());
//        double scale = qMin(xscale, yscale);
//        painter.translate(printer->paperRect().x() + printer->pageRect().width()/2,
//                          printer->paperRect().y() + printer->pageRect().height()/2);
//        painter.scale(scale, scale);
//        painter.translate(-width()/2, -height()/2);

//        this->render(&painter);
//        commentsTextEdit->setPlainText(originalText);
//        updateProductButtonBox->show();

//        qreal x = 0;
//        qreal y = 0;

//        QFont font;
//        font = QFont(font, painter.device());
//        QFontMetricsF fontMetrics(font);
//        painter.setFont(font);

//        QStringList outputs = getPrintContent();
//        foreach(QString output, outputs) {
//            output = "<br><hr><br>";
//            QRectF rect = fontMetrics.boundingRect(output);
//            y += rect.height();
//            painter.drawText(QPointF(x, y), output);
//        }
//        y += rect.height();
//        painter.drawText(QPointF(x, y), "此单据为销售及保修凭证，盖章后生效\n");

//        painter.end();
    }
}

QString UpdateProductDialog::getPrintContent()
{
    QString declearText = "此单据为销售及保修凭证，盖章后生效";
    QString datetime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString table = QString("\
                    <html> \
                    <body> \
                    <table align=\"center\" border=\"5\" cellpadding=\"20\"> \
                    <caption align=\"center\"><h1>销 售 凭 证</h1></caption><br/> \
                    <tr> \
                        <th align=\"right\">产品序列号: </th> \
                        <td align=\"right\" colspan=\"5\"> %1</td> \
                    </tr> \
                    <tr> \
                        <th align=\"right\">产品类型: </th> \
                        <td align=\"right\"> %2</td> \
                        <th align=\"right\">产品品牌: </th> \
                        <td align=\"right\"> %3</td> \
                        <th align=\"right\">产品型号: </th> \
                        <td align=\"right\"> %4</td> \
                    </tr> \
                    <tr> \
                        <th align=\"right\">产品颜色: </th> \
                        <td align=\"right\"> %5</td> \
                        <th align=\"right\">供应商: </th> \
                        <td align=\"right\"> %6</td> \
                        <th align=\"right\">柜台信息: </th> \
                        <td align=\"right\"> %7</td> \
                    </tr> \
                    <tr> \
                        <th align=\"right\">产品数量: </th> \
                        <td align=\"right\"> %8</td> \
                        <th align=\"right\">单位: </th> \
                        <td align=\"right\"> %9</td> \
                        <th align=\"right\">销售仓库: </th> \
                        <td align=\"right\"> %10</td> \
                    </tr> \
                    <tr> \
                        <th align=\"right\">销售价格: </th> \
                        <td align=\"center\" colspan=\"3\"> %11</td> \
                        <th align=\"right\">销售人员: </th> \
                        <td align=\"right\"> %12</td> \
                    </tr> \
                    <tr> \
                        <th align=\"right\">备注: </th> \
                        <td align=\"left\" colspan=\"5\"> %13</br> \
                                    <h4 align=\"right\"> %14</h4></br> \
                                    <h4 align=\"right\"> %15</h4></br> \
                                    <h4 align=\"right\">操作员: %16</h4></td> \
                    </tr> \
                    </table> \
                    </body> \
                    </html>").arg(serialNumberLineEdit->text())
                            .arg(productTypeComboBox->currentText())
                            .arg(brandNameComboBox->currentText())
                            .arg(productModelComboBox->currentText())
                            .arg(productColorComboBox->currentText())
                            .arg(productVendorComboBox->currentText())
                            .arg(barInfoLineEdit->text())
                            .arg(QString("%1").arg(quantitySpinBox->value()).rightJustified(5, '0'))
                            .arg(unitLineEdit->text())
                            .arg(schemaComboBox->currentText())
                            .arg(sellingPriceLineEdit->text())
                            .arg(sellerComboBox->currentText())
                            .arg(commentsTextEdit->toPlainText())
                            .arg(declearText)
                            .arg(datetime)
                            .arg(userManagementInterface->getCurrentUserName());
    return table;

//    QStringList ret;
//    ret.append(QString(tr("Serial Number: %1")).arg(serialNumberLineEdit->text()));
//    ret.append(QString(tr("Product Type: %1")).arg(productTypeComboBox->currentText()));
//    ret.append(QString(tr("Brand Name: %1")).arg(brandNameComboBox->currentText()));
//    ret.append(QString(tr("Product Model: %1")).arg(productModelComboBox->currentText()));
//    ret.append(QString(tr("Quantity: %1")).arg(quantitySpinBox->value()));
//    ret.append(QString(tr("Unit: %1")).arg(unitLineEdit->text()));
//    ret.append(QString(tr("Schema: %1")).arg(schemaComboBox->currentText()));
//    ret.append(QString(tr("Price: %1")).arg(sellingPriceLineEdit->text()));
//    ret.append(QString(tr("Comments: %1")).arg(commentsTextEdit->toPlainText()));

//    return ret;
}

void UpdateProductDialog::hidePurchasePrice(bool hide)
{
    oldPurchasePriceLabel->setHidden(hide);
    oldPurchasePriceLineEdit->setHidden(hide);
    purchasePriceLabel->setHidden(hide);
    purchasePriceLineEdit->setHidden(hide);
}
