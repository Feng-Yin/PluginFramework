#ifndef CASHUPDATEPRODUCTDIALOG_H
#define CASHUPDATEPRODUCTDIALOG_H

#include "updateproductdialog.h"

QT_BEGIN_NAMESPACE
class QPushButton;
class QDialogButtonBox;
class QPrinter;
QT_END_NAMESPACE

class CashUpdateProductDialog : public UpdateProductDialog
{
    Q_OBJECT
public:
    CashUpdateProductDialog(UserManagementInterface *userManagementInterface,
                            ProductManagementInterface *productManagementInterface,
                            QWidget *parent = 0);
private:
    void init();
    QString getPrintContent();
    bool checkInput();

public slots:
    void updateRecord(const QSqlRecord &record);

private slots:
    void updateOrderProductInfo();
    void printBill();

public:
    QPushButton *printBillButton;
    QDialogButtonBox *updateProductButtonBox;
    QPrinter *printer;
};

#endif // CASHUPDATEPRODUCTDIALOG_H
