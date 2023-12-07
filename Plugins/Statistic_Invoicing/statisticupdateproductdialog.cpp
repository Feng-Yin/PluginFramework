#include "statisticupdateproductdialog.h"

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

StatisticUpdateProductDialog::StatisticUpdateProductDialog(UserManagementInterface *userManagementInterface,
                                                           ProductManagementInterface *productManagementInterface,
                                                           QWidget *parent)
    :UpdateProductDialog(userManagementInterface, productManagementInterface, parent)
{
    init();
}

void StatisticUpdateProductDialog::init()
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
    sellingPriceLabel->setEnabled(false);
    sellingPriceLineEdit->setReadOnly(true);
    commentsLabel->setEnabled(false);
    commentsTextEdit->setReadOnly(true);
    updateProductButton->hide();
    setWindowTitle(tr("Product Info"));
    setWindowIcon(QIcon(":/Icon/statistic_icon.png"));
}

void StatisticUpdateProductDialog::updateRecord(const QSqlRecord &record)
{
    UpdateProductDialog::updateRecord(record);
    sellerComboBox->setCurrentIndex(sellerComboBox->findText(record.value(SellerID).toString()));
    sellingPriceLineEdit->setEnabled(true);
}
