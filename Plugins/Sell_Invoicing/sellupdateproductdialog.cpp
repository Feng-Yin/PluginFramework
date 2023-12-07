#include "sellupdateproductdialog.h"

#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QInputDialog>
#include <QtWidgets>
#include <QHBoxLayout>

SellUpdateProductDialog::SellUpdateProductDialog(UserManagementInterface *userManagementInterface,
                                                 ProductManagementInterface *productManagementInterface,
                                                 QWidget *parent)
:UpdateProductDialog(userManagementInterface, productManagementInterface, parent)
{
    init();
}

void SellUpdateProductDialog::init()
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
    sellerLabel->setEnabled(true);
    sellerComboBox->setEnabled(true);
    unitLabel->setEnabled(false);
    unitLineEdit->setEnabled(false);
    sellingPriceLabel->setText(tr("Selling Price")+ "<b style=\"COLOR: #ff0000\">*</b>: ");
    updateProductButton->setText(tr("Submit Order"));
    updateProductButton->disconnect();
    connect(updateProductButton, SIGNAL(clicked()), this, SLOT(updateOrderProductInfo()));
    setWindowTitle(tr("Submit Order"));
    setWindowIcon(QIcon(":/Icon/submitorder_icon.png"));
}


void SellUpdateProductDialog::hideForOrderInfo()
{
    quantityLabel->setEnabled(false);
    quantitySpinBox->setEnabled(false);
    schemaLabel->setEnabled(false);
    schemaComboBox->setEnabled(false);
    barInfoLabel->setEnabled(false);
    barInfoLineEdit->setEnabled(false);
    sellerLabel->setEnabled(false);
    sellerComboBox->setEnabled(false);
    updateProductButton->hide();
    oldPurchasePriceLabel->setEnabled(false);
    oldPurchasePriceLineEdit->setReadOnly(true);
    purchasePriceLabel->setEnabled(false);
    purchasePriceLineEdit->setReadOnly(true);
    sellingPriceLabel->setEnabled(false);
    sellingPriceLineEdit->setReadOnly(true);

    commentsLabel->setEnabled(false);
    commentsTextEdit->setReadOnly(true);
}

void SellUpdateProductDialog::updateRecord(const QSqlRecord &record)
{
    UpdateProductDialog::updateRecord(record);
    quantitySpinBox->setValue(1);
    quantitySpinBox->setMaximum(record.value(Quantity).toInt());
    sellerComboBox->setCurrentIndex(sellerComboBox->findText(record.value(SellerID).toString()));
}

bool SellUpdateProductDialog::checkInput()
{
    if(!UpdateProductDialog::checkInput()) {
        return false;
    }

    sellerID = userManagementInterface->getUserIDByUserName(sellerComboBox->currentText());
    if(sellerID == userManagementInterface->getUserIDByUserName("未指定")) {
        QMessageBox::critical(this, tr("Error"), tr("Please select the correct seller !"));
        sellerComboBox->setFocus();
        return false;
    }
    if(sellingPrice.isEmpty()) {
        QMessageBox::critical(this, tr("Error"), tr("Selling Price can't be empty !"));
        sellingPriceLineEdit->setFocus();
        return false;
    }
    bool ok = false;
    sellingPrice.toDouble(&ok);
    if(!ok) {
        QMessageBox::critical(this, tr("Error"), tr("Selling Price error !"));
        sellingPriceLineEdit->setFocus();
        return false;
    }
    statusID = productManagementInterface->getStatusIDByStatusName("已下单");

    return true;
}

void SellUpdateProductDialog::updateOrderProductInfo()
{
    if(!checkInput()) {
        return;
    }

    productManagementInterface->updateProductDetailByProductID(productID, serialNumber, productTypeID, brandNameID,
                                                               productModelID, productColorID, productVendorID,
                                                               schemaID, quantity, unit, oldPurchasePrice, purchasePrice,
                                                               sellingPrice, userID, userID, sellerID, barInfo, statusID,
                                                               replacementInfoID, timeStamp, comments);
    int total = record.value(Quantity).toInt();
    if(quantity < total) {
        productManagementInterface->addProductByDetail(serialNumber, productTypeID, brandNameID,
                                                       productModelID, productColorID, productVendorID,
                                                       userManagementInterface->getSchemaIDBySchemaName(record.value(SchemaNameID).toString()),
                                                       total-quantity, unit, record.value(OldPurchasePrice).toString(),
                                                       record.value(PurchasePrice).toString(), record.value(SellingPrice).toString(),
                                                       userManagementInterface->getUserIDByUserName(record.value(OperatorUserID).toString()),
                                                       userManagementInterface->getUserIDByUserName(record.value(ResponserUserID).toString()),
                                                       userManagementInterface->getUserIDByUserName(record.value(SellerID).toString()),
                                                       record.value(BarInfo).toString(),
                                                       productManagementInterface->getStatusIDByStatusName("已入库"),
                                                       productManagementInterface->getReplacementStatusIDByReplacementStatusName(record.value(ReplacementStatusID).toString()),
                                                       record.value(TimeStamp).toString(),
                                                       record.value(Comments).toString());
    }

    emit(productUpdated());
    updateDBTableModel();
    //accept();
}
