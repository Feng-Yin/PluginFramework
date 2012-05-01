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
    sellingPriceLabel->setEnabled(false);
    sellingPriceLineEdit = new QLineEdit(this);
    sellingPriceLineEdit->setEnabled(false);

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
    mainLayout->addWidget(updateProductButtonBox, 40, 2, 1, 2, Qt::AlignHCenter);
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);

    setWindowTitle(tr("Check Out"));
    setWindowIcon(QIcon(":/Icon/money_icon.png"));
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
    int operatorUserID = userManagementInterface->getUserIDByUserName(userManagementInterface->getCurrentUserName());
    int statusID = productManagementInterface->getStatusIDByStatusName("已销售");
    QString comments = commentsTextEdit->document()->toPlainText().simplified();
    int productID = record.value(ProductID).toInt();
    int responserUserID = productManagementInterface->getResponserUserIDByProductID(productID);
    productManagementInterface->updateProductDetailByProductID(productID, serialNumber, productTypeID, brandNameID,
                                                        productModelID, schemaID, quantity, unit,
                                                        oldPurchasePrice, purchasePrice, sellingPrice,
                                                        operatorUserID, responserUserID, statusID, comments);

//    int deleteID = productManagementInterface->getStatusIDByStatusName("已删除");
//    productManagementInterface->updateStatusIDByProductID(productID, deleteID);
    productManagementInterface->deleteProductByProductID(productID);

    emit(productUpdated());
    accept();
//    serialNumberLineEdit->clear();
//    serialNumberLineEdit->setFocus();
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
                        <th align=\"right\">产品数量: </th> \
                        <td align=\"right\"> %5</td> \
                        <th align=\"right\">单位: </th> \
                        <td align=\"right\"> %6</td> \
                        <th align=\"right\">销售仓库: </th> \
                        <td align=\"right\"> %7</td> \
                    </tr> \
                    <tr> \
                        <th align=\"right\">销售价格: </th> \
                        <td align=\"right\" colspan=\"5\"> %8</td> \
                    </tr> \
                    <tr> \
                        <th align=\"right\">备注: </th> \
                        <td align=\"left\" colspan=\"5\"> %9</br> \
                                    <h4 align=\"right\"> %10</h4></br> \
                                    <h4 align=\"right\"> %11</h4></br> \
                                    <h4 align=\"right\">操作员: %12</h4></td> \
                    </tr> \
                    </table> \
                    </body> \
                    </html>").arg(serialNumberLineEdit->text())
                            .arg(productTypeComboBox->currentText())
                            .arg(brandNameComboBox->currentText())
                            .arg(productModelComboBox->currentText())
                            .arg(QString("%1").arg(quantitySpinBox->value()).rightJustified(5, '0'))
                            .arg(unitLineEdit->text())
                            .arg(schemaComboBox->currentText())
                            .arg(sellingPriceLineEdit->text())
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
