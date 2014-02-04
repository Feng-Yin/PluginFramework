#ifndef STATISTIC_INVOICING_H
#define STATISTIC_INVOICING_H

#include <QObject>
#include <plugin_interface.h>
#include <usermanagement_interface.h>
#include "Statistic_Invoicing_global.h"


QT_BEGIN_NAMESPACE
class QSqlRelationalTableModel;
class QTableView;
class QLabel;
class QLineEdit;
class QComboBox;
class QSpinBox;
class QCheckBox;
class QTabWidget;
class QPushButton;
class QRadioButton;
class QDateEdit;
class QDateTimeEdit;
QT_END_NAMESPACE

class UserManagementInterface;
class ProductManagementInterface;
class BarChart;
class UpdateProductDialog;

class STATISTIC_INVOICINGSHARED_EXPORT Statistic_Invoicing :
        public QObject, public PluginInterface, public UserChangeNotifyInterface
{
    Q_OBJECT
    Q_INTERFACES(PluginInterface UserChangeNotifyInterface)
public:
    Statistic_Invoicing();
    virtual QWidget* getMainWidget() const;
    virtual QAction* getAction() const;
    virtual QMenu* getMenu() const;
    virtual QToolBar* getToolBar() const;
    virtual bool init(MainWindow *parent);
    virtual bool deInit();
    virtual QString moduleName() const;
    virtual QString moduleDescription() const;
    virtual QSet<QString> getAccessRoleNameSet() const;
    virtual QSet<QString> getDependencySet() const;
    virtual void update() { updateDBTableModel(); }

    virtual void userChanged();

private:
    void initMainWidget();
    void createFilterPanel();
    void createAllProductsPanel();
    void createUnsellProductsPanel();
    void createBarChartPanel();
    void createAllProductsFilterPanel();
    void createUnsellProductsFilterPanel();
    void hidePurchasePrice();


private slots:
    void updateDBTableModel();
    void setProductModelFilter();
    void populateSchemaComboBox(QComboBox *schemaComboBox) const;
    void populateSellerNameComboBox(QComboBox *sellerNameComboBox) const;
    void onAllProductsFilter();
    void onUnsellProductsFilter();
    void updateSalesStatisticPlot();
    void updateUnsellProductinfo();
    void updateAllProductinfo();

private:
    MainWindow *parentWindow;
    UserManagementInterface *userManagementInterface;
    ProductManagementInterface *productManagementInterface;

    QWidget *mainWidget;

    QWidget *allProductsPanel;
    QSqlRelationalTableModel *allProductsModel;
    QTableView *allProductsView;

    QWidget *unsellProductsPanel;
    QSqlRelationalTableModel *unsellProductsModel;
    QTableView *unsellProductsView;

    QTabWidget *filterPanel;
    QWidget *unsellProductsFilterPanel;
    QWidget *allProductsFilterPanel;
    QWidget *salesStatisticContainer;
    BarChart *salesStatisticPlot;
    QRadioButton *sortByVolumeAscRadioButton;
    QRadioButton *sortByVolumeDesRadioButton;
    QRadioButton *sortByQuantityAscRadioButton;
    QRadioButton *sortByQuantityDesRadioButton;
    QDateEdit *time;
    QSpinBox *timeRange;
    QSpinBox *peopleRange;
    QPushButton *updateStatisticPushButton;
    QPushButton *printStatisticPushButton;

    QLabel *serialNumberAllProductsLabel;
    QLineEdit *serialNumberAllProductsLineEdit;
    QCheckBox *serialNumberAllProductsCheckBox;

    QLabel *productTypeAllProductsLabel;
    QComboBox *productTypeAllProductsComboBox;
    QSqlRelationalTableModel *productTypeAllProductsModel;
    QCheckBox *productTypeAllProductsCheckBox;

    QLabel *brandNameAllProductsLabel;
    QComboBox *brandNameAllProductsComboBox;
    QSqlRelationalTableModel *brandNameAllProductsModel;
    QCheckBox *brandNameAllProductsCheckBox;

    QLabel *productModelAllProductsLabel;
    QComboBox *productModelAllProductsComboBox;
    QSqlRelationalTableModel *productModelAllProductsModel;
    QCheckBox *productModelAllProductsCheckBox;

    QLabel *productColorAllProductsLabel;
    QComboBox *productColorAllProductsComboBox;
    QSqlRelationalTableModel *productColorAllProductsModel;
    QCheckBox *productColorAllProductsCheckBox;

    QLabel *productVendorAllProductsLabel;
    QComboBox *productVendorAllProductsComboBox;
    QSqlRelationalTableModel *productVendorAllProductsModel;
    QCheckBox *productVendorAllProductsCheckBox;

    QLabel *replacementInfoAllProductsLabel;
    QComboBox *replacementInfoAllProductsComboBox;
    QSqlRelationalTableModel *replacementInfoAllProductsModel;
    QCheckBox *replacementInfoAllProductsCheckBox;

    QLabel *barInfoAllProductsLabel;
    QLineEdit *barInfoAllProductsLineEdit;
    QCheckBox *barInfoAllProductsCheckBox;

    QLabel *sellerAllProductsLabel;
    QComboBox *sellerAllProductsComboBox;
    QCheckBox *sellerAllProductsCheckBox;

    QLabel *productStatusAllProductsLabel;
    QComboBox *productStatusAllProductsComboBox;
    QSqlRelationalTableModel *productStatusAllProductsModel;
    QCheckBox *productStatusAllProductsCheckBox;

    QLabel *quantityAllProductsLabel;
    QSpinBox *quantityAllProductsSpinBox;
    QCheckBox *quantityAllProductsCheckBox;

    QLabel *unitAllProductsLabel;
    QLineEdit *unitAllProductsLineEdit;
    QCheckBox *unitAllProductsCheckBox;

    QLabel *schemaAllProductsLabel;
    QComboBox *schemaAllProductsComboBox;
    QSqlRelationalTableModel *userSchemaAllProductsModel;
    QCheckBox *schemaAllProductsCheckBox;

    QLabel *oldPurchasePriceAllProductsLabel;
    QLineEdit *oldPurchasePriceAllProductsLineEdit;
    QCheckBox *oldPurchasePriceAllProductsCheckBox;

    QLabel *purchasePriceAllProductsLabel;
    QLineEdit *purchasePriceAllProductsLineEdit;
    QCheckBox *purchasePriceAllProductsCheckBox;

    QLabel *sellingPriceAllProductsLabel;
    QLineEdit *sellingPriceAllProductsLineEdit;
    QCheckBox *sellingPriceAllProductsCheckBox;

    QLabel *commentsAllProductsLabel;
    QLineEdit *commentsAllProductsLineEdit;
    QCheckBox *commentsAllProductsCheckBox;

    QLabel *startTimeAllProductsLabel;
    QDateTimeEdit *startTimeAllProductsDateTimeEdit;
    QCheckBox *startTimeAllProductsCheckBox;

    QLabel *endTimeAllProductsLabel;
    QDateTimeEdit *endTimeAllProductsDateTimeEdit;
    QCheckBox *endTimeAllProductsCheckBox;

    QPushButton *allProductsFilterButton;
    QLabel *allProductsSummaryLabel;
    QLineEdit *allProductsSummaryLineEdit;

    QLabel *serialNumberUnsellProductsLabel;
    QLineEdit *serialNumberUnsellProductsLineEdit;
    QCheckBox *serialNumberUnsellProductsCheckBox;

    QLabel *productTypeUnsellProductsLabel;
    QComboBox *productTypeUnsellProductsComboBox;
    QSqlRelationalTableModel *productTypeUnsellProductsModel;
    QCheckBox *productTypeUnsellProductsCheckBox;

    QLabel *brandNameUnsellProductsLabel;
    QComboBox *brandNameUnsellProductsComboBox;
    QSqlRelationalTableModel *brandNameUnsellProductsModel;
    QCheckBox *brandNameUnsellProductsCheckBox;

    QLabel *productModelUnsellProductsLabel;
    QComboBox *productModelUnsellProductsComboBox;
    QSqlRelationalTableModel *productModelUnsellProductsModel;
    QCheckBox *productModelUnsellProductsCheckBox;

    QLabel *productColorUnsellProductsLabel;
    QComboBox *productColorUnsellProductsComboBox;
    QSqlRelationalTableModel *productColorUnsellProductsModel;
    QCheckBox *productColorUnsellProductsCheckBox;

    QLabel *productVendorUnsellProductsLabel;
    QComboBox *productVendorUnsellProductsComboBox;
    QSqlRelationalTableModel *productVendorUnsellProductsModel;
    QCheckBox *productVendorUnsellProductsCheckBox;

    QLabel *replacementInfoUnsellProductsLabel;
    QComboBox *replacementInfoUnsellProductsComboBox;
    QSqlRelationalTableModel *replacementInfoUnsellProductsModel;
    QCheckBox *replacementInfoUnsellProductsCheckBox;

    QLabel *barInfoUnsellProductsLabel;
    QLineEdit *barInfoUnsellProductsLineEdit;
    QCheckBox *barInfoUnsellProductsCheckBox;

    QLabel *sellerUnsellProductsLabel;
    QComboBox *sellerUnsellProductsComboBox;
    QCheckBox *sellerUnsellProductsCheckBox;

    QLabel *productStatusUnsellProductsLabel;
    QComboBox *productStatusUnsellProductsComboBox;
    QSqlRelationalTableModel *productStatusUnsellProductsModel;
    QCheckBox *productStatusUnsellProductsCheckBox;

    QLabel *quantityUnsellProductsLabel;
    QSpinBox *quantityUnsellProductsSpinBox;
    QCheckBox *quantityUnsellProductsCheckBox;

    QLabel *unitUnsellProductsLabel;
    QLineEdit *unitUnsellProductsLineEdit;
    QCheckBox *unitUnsellProductsCheckBox;

    QLabel *schemaUnsellProductsLabel;
    QComboBox *schemaUnsellProductsComboBox;
    QSqlRelationalTableModel *userSchemaUnsellProductsModel;
    QCheckBox *schemaUnsellProductsCheckBox;

    QLabel *oldPurchasePriceUnsellProductsLabel;
    QLineEdit *oldPurchasePriceUnsellProductsLineEdit;
    QCheckBox *oldPurchasePriceUnsellProductsCheckBox;

    QLabel *purchasePriceUnsellProductsLabel;
    QLineEdit *purchasePriceUnsellProductsLineEdit;
    QCheckBox *purchasePriceUnsellProductsCheckBox;

    QLabel *sellingPriceUnsellProductsLabel;
    QLineEdit *sellingPriceUnsellProductsLineEdit;
    QCheckBox *sellingPriceUnsellProductsCheckBox;

    QLabel *commentsUnsellProductsLabel;
    QLineEdit *commentsUnsellProductsLineEdit;
    QCheckBox *commentsUnsellProductsCheckBox;

    QLabel *startTimeUnsellProductsLabel;
    QDateTimeEdit *startTimeUnsellProductsDateTimeEdit;
    QCheckBox *startTimeUnsellProductsCheckBox;

    QLabel *endTimeUnsellProductsLabel;
    QDateTimeEdit *endTimeUnsellProductsDateTimeEdit;
    QCheckBox *endTimeUnsellProductsCheckBox;

    QPushButton *unsellProductsFilterButton;
    QLabel *unsellProductsSummaryLabel;
    QLineEdit *unsellProductsSummaryLineEdit;

    UpdateProductDialog *updateAllProductDialog;
    UpdateProductDialog *updateUnsellProductDialog;
};

#endif // STATISTIC_INVOICING_H
