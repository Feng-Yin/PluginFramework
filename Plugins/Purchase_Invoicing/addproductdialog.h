#ifndef ADDPRODUCTDIALOG_H
#define ADDPRODUCTDIALOG_H

#include <QDialog>
#include "usermanagement_interface.h"

QT_BEGIN_NAMESPACE
class QLineEdit;
class QComboBox;
class QLabel;
class QSpinBox;
class QPushButton;
class QSqlRelationalTableModel;
class QString;
class QTextEdit;
QT_END_NAMESPACE

class UserManagementInterface;
class ProductManagementInterface;

class AddProductDialog : public QDialog, public UserChangeNotifyInterface
{
    Q_OBJECT
    Q_INTERFACES(UserChangeNotifyInterface)
public:
    explicit AddProductDialog(UserManagementInterface *userManagementInterface,
                              ProductManagementInterface *productManagementInterface,
                              QWidget *parent = 0);
    ~AddProductDialog();
    virtual void userChanged();
    void hidePurchasePrice(bool hide);
signals:
    void productAdded();
    
public slots:
    void updateDBTableModel();

private slots:
    void addProduct();
    void clearInfo();
    void setProductModelFilter();
    void populateBrandNameComboBox();
    void populateSchemaComboBox() const;
    void populateSellerNameComboBox() const;

private:
    void init();

private:
    UserManagementInterface * userManagementInterface;
    ProductManagementInterface *productManagementInterface;

    QLabel *serialNumberLabel;
    QLineEdit *serialNumberLineEdit;

    QLabel *productTypeLabel;
    QComboBox *productTypeComboBox;
    QSqlRelationalTableModel *productTypeModel;

    QLabel *brandNameLabel;
    QComboBox *brandNameComboBox;
    QSqlRelationalTableModel *brandNameModel;

    QLabel *productModelLabel;
    QComboBox *productModelComboBox;
    QSqlRelationalTableModel *productModelModel;

    QLabel *productColorLabel;
    QComboBox *productColorComboBox;
    QSqlRelationalTableModel *productColorModel;

    QLabel *productVendorLabel;
    QComboBox *productVendorComboBox;
    QSqlRelationalTableModel *productVendorModel;

    QLabel *replacementInfoLabel;
    QComboBox *replacementInfoComboBox;
    QSqlRelationalTableModel *replacementInfoModel;

    QLabel *barInfoLabel;
    QLineEdit *barInfoLineEdit;

    QLabel *sellerLabel;
    QComboBox *sellerComboBox;

    QLabel *quantityLabel;
    QSpinBox *quantitySpinBox;

    QLabel *unitLabel;
    QLineEdit *unitLineEdit;

    QLabel *schemaLabel;
    QComboBox *schemaComboBox;
    QSqlRelationalTableModel *userSchemaModel;

    QLabel *oldPurchasePriceLabel;
    QLineEdit *oldPurchasePriceLineEdit;

    QLabel *purchasePriceLabel;
    QLineEdit *purchasePriceLineEdit;

    QLabel *sellingPriceLabel;
    QLineEdit *sellingPriceLineEdit;

    QLabel *commentsLabel;
    QTextEdit *commentsTextEdit;

    QPushButton *addProductButton;
    QPushButton *clearInfoButton;

    
};

#endif // ADDPRODUCTDIALOG_H
