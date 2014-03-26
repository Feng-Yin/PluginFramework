TEMPLATE = subdirs

lessThan(QT_MAJOR_VERSION, 5) {
SUBDIRS += \
    Main \
    Plugins/UserManagementIF \
    Plugins/ProductManagementIF \
    Plugins/UserManagement_Invoicing \
    Plugins/UpdateProductDialog \
    Plugins/Purchase_Invoicing \
    Plugins/Storage_Invoicing \
    Plugins/Sell_Invoicing \
    Plugins/Cash_Invoicing \
    Plugins/Statistic_Invoicing \
    MySQLDriver/SYSZUXmysql \
    RegisterTool
}
else{
SUBDIRS += \
    Main \
    Plugins/UserManagementIF \
    Plugins/ProductManagementIF \
    Plugins/UserManagement_Invoicing \
    Plugins/UpdateProductDialog \
    Plugins/Purchase_Invoicing \
    Plugins/Storage_Invoicing \
    Plugins/Sell_Invoicing \
    Plugins/Cash_Invoicing \
    Plugins/Statistic_Invoicing \
    RegisterTool
}
