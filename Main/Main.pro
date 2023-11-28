QT       += network sql widgets core
TARGET = Invoicing
TRANSLATIONS = ./Translations/Invoicing_zh_CN.ts

include(../singleapplication/singleapplication.pri)
DEFINES += QAPPLICATION_CLASS=QApplication

SOURCES += \
    main.cpp \
    mainwindow.cpp \

HEADERS += \
    helper.h \
    mainwindow.h \
    plugin_interface.h \
    usermanagement_interface.h \
    productmanagement_interface.h \

OTHER_FILES += \
    ideas.txt \
    Icon/plugin_icon.png \
    Icon/quit_icon.png \
    Icon/about_icon.png \
    Translations/Invoicing_zh_CN.ts \
    Translations/Invoicing_zh_CN.qm \
    Icon/update_icon.png \
    invoices.ico \
    logo.rc

RESOURCES += \
    icon.qrc

RC_FILE = logo.rc
