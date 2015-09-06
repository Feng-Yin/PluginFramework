#include <QtGui>
#include <QtSql>
#include <usermanagement_interface.h>
#include "productmanagement_interface.h"
#include "addproductdialog.h"

AddProductDialog::AddProductDialog(UserManagementInterface *userManagementInterface,
                                   ProductManagementInterface *productManagementInterface,
                                   QWidget *parent):
    UpdateProductDialog(userManagementInterface, productManagementInterface, parent)
{
    init();
}

void AddProductDialog::init()
{
    productTypeComboBox->setCurrentIndex(
                productTypeComboBox->findText("其它"));

    brandNameComboBox->setCurrentIndex(
                brandNameComboBox->findText("其它"));

    productModelComboBox->setCurrentIndex(
                productModelComboBox->findText("其它"));

    productColorComboBox->setCurrentIndex(
                productColorComboBox->findText("其它"));

    productVendorComboBox->setCurrentIndex(
                productVendorComboBox->findText("其它"));

    replacementInfoComboBox->setCurrentIndex(
                replacementInfoComboBox->findText("否"));

    schemaComboBox->setCurrentIndex(
                schemaComboBox->findText("未指定"));

    sellerComboBox->setCurrentIndex(
                sellerComboBox->findText("未指定"));

    updateProductButton->disconnect();
    updateProductButton->setText(tr("Add Product"));
    connect(updateProductButton, SIGNAL(clicked()), this, SLOT(addProduct()));
    rightButton->setText(tr("Clear Info"));
    rightButton->setHidden(false);
    connect(rightButton, SIGNAL(clicked()), this, SLOT(clearInfo()));
    setWindowTitle(tr("Add Product"));
    setWindowIcon(QIcon(":/Icon/add_icon.png"));
}

bool AddProductDialog::checkInput()
{
    if(!UpdateProductDialog::checkInput()) {
        return false;
    }

    barInfo = "";
    sellerID = userManagementInterface->getUserIDByUserName("未指定");
    statusID = productManagementInterface->getStatusIDByStatusName("录入");
    return true;
}

void AddProductDialog::addProduct()
{
    if(!checkInput()) {
        return;
    }

    productManagementInterface->addProductByDetail(serialNumber, productTypeID, brandNameID,
                                                   productModelID, productColorID, productVendorID,
                                                   schemaID, quantity, unit, oldPurchasePrice, purchasePrice,
                                                   sellingPrice, userID, userID, sellerID, "未指定", statusID,
                                                   replacementInfoID, timeStamp, comments);
    emit(productAdded());
    updateDBTableModel();
    serialNumberLineEdit->clear();
    serialNumberLineEdit->setFocus();
}

void AddProductDialog::clearInfo()
{
    serialNumberLineEdit->clear();
    productTypeComboBox->setCurrentIndex(0);
    brandNameComboBox->setCurrentIndex(0);
    productTypeComboBox->setCurrentIndex(0);
    productColorComboBox->setCurrentIndex(0);
    productVendorComboBox->setCurrentIndex(0);
    replacementInfoComboBox->setCurrentIndex(0);
    quantitySpinBox->setValue(1);
    unitLineEdit->clear();
    oldPurchasePriceLineEdit->clear();
    purchasePriceLineEdit->clear();
    sellingPriceLineEdit->clear();
    commentsTextEdit->clear();
}

