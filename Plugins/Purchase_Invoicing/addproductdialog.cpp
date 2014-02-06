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
                productTypeComboBox->findText("����"));

    brandNameComboBox->setCurrentIndex(
                brandNameComboBox->findText("����"));

    productModelComboBox->setCurrentIndex(
                productModelComboBox->findText("����"));

    productColorComboBox->setCurrentIndex(
                productColorComboBox->findText("����"));

    productVendorComboBox->setCurrentIndex(
                productVendorComboBox->findText("����"));

    replacementInfoComboBox->setCurrentIndex(
                replacementInfoComboBox->findText("��"));

    schemaComboBox->setCurrentIndex(
                schemaComboBox->findText("δָ��"));

    sellerComboBox->setCurrentIndex(
                sellerComboBox->findText("δָ��"));

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
    sellerID = userManagementInterface->getUserIDByUserName("δָ��");
    statusID = productManagementInterface->getStatusIDByStatusName("¼��");
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
                                                   sellingPrice, userID, userID, sellerID, "δָ��", statusID,
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

