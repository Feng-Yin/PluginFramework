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
QT_END_NAMESPACE

class UserManagementInterface;
class ProductManagementInterface;
class BarChart;

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
    virtual void update() const { updateDBTableModel(); }

    virtual void userChanged() const;

private:
    void initMainWidget();
    void createStatisticPanel();
    void createFilterPanel();

private slots:
    void updateDBTableModel() const;
    void setProductModelFilter();
    void populateSchemaComboBox() const;
    void onBasicFilter();
    void updateSalesStatisticPlot();

private:
    MainWindow *parentWindow;
    UserManagementInterface *userManagementInterface;
    ProductManagementInterface *productManagementInterface;

    QWidget *mainWidget;

    QWidget *statisticPanel;
    QSqlRelationalTableModel *statisticModel;
    QTableView *statisticView;

    QTabWidget *filterPanel;
    QWidget *basicFilterPanel;
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

    QLabel *serialNumberLabel;
    QLineEdit *serialNumberLineEdit;
    QCheckBox *serialNumberCheckBox;

    QLabel *productTypeLabel;
    QComboBox *productTypeComboBox;
    QSqlRelationalTableModel *productTypeModel;
    QCheckBox *productTypeCheckBox;

    QLabel *brandNameLabel;
    QComboBox *brandNameComboBox;
    QSqlRelationalTableModel *brandNameModel;
    QCheckBox *brandNameCheckBox;

    QLabel *productModelLabel;
    QComboBox *productModelComboBox;
    QSqlRelationalTableModel *productModelModel;
    QCheckBox *productModelCheckBox;

    QLabel *quantityLabel;
    QSpinBox *quantitySpinBox;
    QCheckBox *quantityCheckBox;

    QLabel *unitLabel;
    QLineEdit *unitLineEdit;
    QCheckBox *unitCheckBox;

    QLabel *schemaLabel;
    QComboBox *schemaComboBox;
    QSqlRelationalTableModel *userSchemaModel;
    QCheckBox *schemaCheckBox;

    QLabel *oldPurchasePriceLabel;
    QLineEdit *oldPurchasePriceLineEdit;
    QCheckBox *oldPurchasePriceCheckBox;

    QLabel *purchasePriceLabel;
    QLineEdit *purchasePriceLineEdit;
    QCheckBox *purchasePriceCheckBox;

    QLabel *sellingPriceLabel;
    QLineEdit *sellingPriceLineEdit;
    QCheckBox *sellingPriceCheckBox;

    QPushButton *basicFilterButton;
};

#endif // STATISTIC_INVOICING_H
