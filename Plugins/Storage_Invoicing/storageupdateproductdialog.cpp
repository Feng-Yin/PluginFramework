#include "storageupdateproductdialog.h"
#include <QtGui>

StorageUpdateProductDialog::StorageUpdateProductDialog(UserManagementInterface *userManagementInterface,
                                                       ProductManagementInterface *productManagementInterface,
                                                       QWidget *parent)
    :UpdateProductDialog(userManagementInterface, productManagementInterface, parent)
{
    init();
}

void StorageUpdateProductDialog::init()
{
    serialNumberLabel->setEnabled(false);
    serialNumberLineEdit->setReadOnly(true);
    barInfoLabel->setEnabled(true);
    barInfoLineEdit->setEnabled(true);
}

void StorageUpdateProductDialog::serialNumberEditable(bool edit)
{
    serialNumberLineEdit->setReadOnly(!edit);
}
