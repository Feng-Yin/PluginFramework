#ifndef STATISTICUPDATEPRODUCTDIALOG_H
#define STATISTICUPDATEPRODUCTDIALOG_H

#include "updateproductdialog.h"

class StatisticUpdateProductDialog : public UpdateProductDialog
{
    Q_OBJECT
public:
    StatisticUpdateProductDialog(UserManagementInterface *userManagementInterface,
                                 ProductManagementInterface *productManagementInterface,
                                 QWidget *parent = 0);
public slots:
    void updateRecord(const QSqlRecord &record);

private:
    void init();
};

#endif // STATISTICUPDATEPRODUCTDIALOG_H
