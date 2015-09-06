#include "storageupdateproductdialog.h"
#if QT_VERSION >= 0x050000
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
#else
#include <QtGui>
#endif

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
