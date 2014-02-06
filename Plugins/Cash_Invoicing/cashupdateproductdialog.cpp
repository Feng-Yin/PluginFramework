#include "cashupdateproductdialog.h"
#include <QtGui>

CashUpdateProductDialog::CashUpdateProductDialog(UserManagementInterface *userManagementInterface,
                                                 ProductManagementInterface *productManagementInterface,
                                                 QWidget *parent)
    :UpdateProductDialog(userManagementInterface, productManagementInterface, parent),
      printBillButton(NULL),
      updateProductButtonBox(NULL),
      printer(NULL)
{
    init();
}

void CashUpdateProductDialog::init()
{
    serialNumberLabel->setEnabled(false);
    serialNumberLineEdit->setReadOnly(true);
    productTypeLabel->setEnabled(false);
    productTypeComboBox->setEnabled(false);
    brandNameLabel->setEnabled(false);
    brandNameComboBox->setEnabled(false);
    productModelLabel->setEnabled(false);
    productModelComboBox->setEnabled(false);
    productColorLabel->setEnabled(false);
    productColorComboBox->setEnabled(false);
    productVendorLabel->setEnabled(false);
    productVendorComboBox->setEnabled(false);
    replacementInfoLabel->setEnabled(false);
    replacementInfoComboBox->setEnabled(false);
    quantityLabel->setEnabled(false);
    quantitySpinBox->setEnabled(false);
    unitLabel->setEnabled(false);
    unitLineEdit->setEnabled(false);
    schemaLabel->setEnabled(false);
    schemaComboBox->setEnabled(false);
    oldPurchasePriceLabel->setEnabled(false);
    oldPurchasePriceLineEdit->setEnabled(false);
    purchasePriceLabel->setEnabled(false);
    purchasePriceLineEdit->setEnabled(false);
    sellingPriceLabel->setText(tr("Selling Price")+"<b style=\"COLOR: #ff0000\">*</b>: ");
    sellingPriceLineEdit->setReadOnly(true);
    updateProductButton->setText(tr("Paid"));
    updateProductButton->setDefault(true);
    updateProductButton->disconnect();
    connect(updateProductButton, SIGNAL(clicked()), this, SLOT(updateOrderProductInfo()));
    leftButton->setHidden(false);
    leftButton->setText(tr("Print Bill"));
    connect(leftButton, SIGNAL(clicked()), this, SLOT(printBill()));
    setWindowTitle(tr("Check Out"));
    setWindowIcon(QIcon(":/Icon/money_icon.png"));
}

bool CashUpdateProductDialog::checkInput()
{
    if(!UpdateProductDialog::checkInput()) {
        return false;
    }

    statusID = productManagementInterface->getStatusIDByStatusName("已销售");

    return true;
}

void CashUpdateProductDialog::updateOrderProductInfo()
{
    if(!checkInput()) {
        return;
    }

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

void CashUpdateProductDialog::updateRecord(const QSqlRecord &record)
{
    UpdateProductDialog::updateRecord(record);
    sellerComboBox->setCurrentIndex(sellerComboBox->findText(record.value(SellerID).toString()));
    updateProductButton->setEnabled(true);
    sellingPriceLabel->setEnabled(true);
    sellingPriceLineEdit->setEnabled(true);
}

void CashUpdateProductDialog::printBill()
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

QString CashUpdateProductDialog::getPrintContent()
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
