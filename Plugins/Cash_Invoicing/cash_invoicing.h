﻿#ifndef CASH_INVOICING_H
#define CASH_INVOICING_H

#include <QObject>
#include <plugin_interface.h>
#include <usermanagement_interface.h>
#include "Cash_Invoicing_global.h"

QT_BEGIN_NAMESPACE
class QSqlRelationalTableModel;
class QTableView;
class QLineEdit;
class QPushButton;
QT_END_NAMESPACE

class CashUpdateProductDialog;
class UserManagementInterface;
class ProductManagementInterface;

class CASH_INVOICINGSHARED_EXPORT Cash_Invoicing :
        public QObject, public PluginInterface, public UserChangeNotifyInterface
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "Cash_Invoicing")
#endif
    Q_INTERFACES(PluginInterface UserChangeNotifyInterface)
public:
    Cash_Invoicing();

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
    void createCashPanel();
    void updateCashFilter();
    void hidePurchasePrice();

private slots:
    void updateDBTableModel();
    void onFilter();
    void updateProductinfo();
    void productUpdated();

private:
    MainWindow *parentWindow;
    UserManagementInterface *userManagementInterface;
    ProductManagementInterface *productManagementInterface;

    QWidget *mainWidget;

    QWidget *cashPanel;
    QSqlRelationalTableModel *cashModel;
    QTableView *cashView;

    CashUpdateProductDialog *cashUpdateProductDialog;

    QLineEdit *serialNumberLineEdit;
    QPushButton *filterPushButton;

};

#endif // CASH_INVOICING_H
