#pragma once

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
#include <QSqlRecord>

#include "usermanagement_interface.h"
#include "productmanagement_interface.h"

QT_BEGIN_NAMESPACE
class QLineEdit;
class QComboBox;
class QLabel;
class QSpinBox;
class QPushButton;
class QSqlRelationalTableModel;
class QString;
class QTextEdit;
class QDialogButtonBox;
class QPrinter;
QT_END_NAMESPACE

class UserManagementInterface;
class ProductManagementInterface;

class UpdateProductDialog : public QDialog, public UserChangeNotifyInterface
{
    Q_OBJECT
    Q_INTERFACES(UserChangeNotifyInterface)
public:
    explicit UpdateProductDialog(UserManagementInterface *userManagementInterface,
                                 ProductManagementInterface *productManagementInterface,
                                 QWidget *parent = 0);
    ~UpdateProductDialog();
    virtual void userChanged();
    void hidePurchasePrice(bool hide);
    bool checkInput();

signals:
    void productUpdated();

public slots:
    void updateDBTableModel();
    void updateRecord(const QSqlRecord &record);

private slots:
    void updateProductInfo();
    void setProductModelFilter();
    void populateBrandNameComboBox();
    void populateSchemaComboBox() const;
    void populateSellerNameComboBox() const;

private:
    void init();

public:
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

    QPushButton *updateProductButton;
    QPushButton *leftButton;
    QPushButton *rightButton;

    QSqlRecord record;

    QString serialNumber;
    QString unit;
    int productTypeID;
    int brandNameID;
    int productModelID;
    int productColorID;
    int productVendorID;
    int replacementInfoID;
    int sellerID;
    QString barInfo;
    QString oldPurchasePrice;
    QString purchasePrice;
    QString sellingPrice;
    int schemaID;
    int quantity;
    int userID;
    int statusID;
    QString comments;
    QString timeStamp;
    int productID;
};
