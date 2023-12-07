#pragma once

#include "updateproductdialog.h"

class SellUpdateProductDialog : public UpdateProductDialog
{
    Q_OBJECT
public:
    SellUpdateProductDialog(UserManagementInterface *userManagementInterface,
                            ProductManagementInterface *productManagementInterface,
                            QWidget *parent = 0);
    void hideForOrderInfo();

private slots:
    void updateOrderProductInfo();

public slots:
    void updateRecord(const QSqlRecord &record);

private:
    void init();
    bool checkInput();

};
