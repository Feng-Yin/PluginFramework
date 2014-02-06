#ifndef ADDPRODUCTDIALOG_H
#define ADDPRODUCTDIALOG_H

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

#endif // ADDPRODUCTDIALOG_H
