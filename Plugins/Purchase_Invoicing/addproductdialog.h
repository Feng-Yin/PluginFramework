#pragma once

#include "updateproductdialog.h"

class AddProductDialog : public UpdateProductDialog
{
    Q_OBJECT
public:
    AddProductDialog(UserManagementInterface *userManagementInterface,
                     ProductManagementInterface *productManagementInterface,
                     QWidget *parent = 0);
private slots:
    void addProduct();
    void clearInfo();

signals:
    void productAdded();

private:
    void init();
    bool checkInput();
};
