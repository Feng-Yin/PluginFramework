#ifndef STORAGEUPDATEPRODUCTDIALOG_H
#define STORAGEUPDATEPRODUCTDIALOG_H

#include "updateproductdialog.h"

class StorageUpdateProductDialog : public UpdateProductDialog
{
    Q_OBJECT
public:
    StorageUpdateProductDialog(UserManagementInterface *userManagementInterface,
                               ProductManagementInterface *productManagementInterface,
                               QWidget *parent = 0);

private:
    void init();
};

#endif // STORAGEUPDATEPRODUCTDIALOG_H
