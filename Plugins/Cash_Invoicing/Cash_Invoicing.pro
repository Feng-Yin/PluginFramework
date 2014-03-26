#-------------------------------------------------
#
# Project created by QtCreator 2012-03-26T19:54:01
#
#-------------------------------------------------

QT       += sql widgets printsupport

TARGET = Cash_Invoicing
TEMPLATE = lib
CONFIG += plugin  copy_dir_files
INCLUDEPATH += ../../main
INCLUDEPATH += ../UpdateProductDialog
DEFINES += CASH_INVOICING_LIBRARY
TRANSLATIONS = ./Translations/$${TARGET}_zh_CN.ts
CONFIG(debug, debug|release) {
    LIBS += ../../Main/debug/updateproductdialog.dll
    DLLDESTDIR = ../../Main/debug/plugins/Invoicing/
}
CONFIG(release, debug|release) {
    LIBS += ../../Main/release/updateproductdialog.dll
    DLLDESTDIR = ../../Main/release/plugins/Invoicing/
}

SOURCES += cash_invoicing.cpp \
    cashupdateproductdialog.cpp

HEADERS += cash_invoicing.h\
        Cash_Invoicing_global.h \
    cashupdateproductdialog.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE60B2F0C
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = Cash_Invoicing.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

OTHER_FILES += \
    Icon/cash_icon.png \
    Icon/money_icon.png \
    Translations/$${TARGET}_zh_CN.qm \
    Translations/$${TARGET}_zh_CN.ts

RESOURCES += \
    icon.qrc
