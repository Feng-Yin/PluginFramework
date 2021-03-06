#-------------------------------------------------
#
# Project created by QtCreator 2012-03-26T19:42:37
#
#-------------------------------------------------

QT       += sql widgets

TARGET = Sell_Invoicing
TEMPLATE = lib
CONFIG += plugin copy_dir_files
INCLUDEPATH += ../../main
INCLUDEPATH += ../UpdateProductDialog
DEFINES += SELL_INVOICING_LIBRARY
TRANSLATIONS = Translations/$${TARGET}_zh_CN.ts
CONFIG(debug, debug|release) {
    LIBS += ../../Main/debug/updateproductdialog.dll
    DLLDESTDIR = ../../Main/debug/plugins/Invoicing/
}
CONFIG(release, debug|release) {
    LIBS += ../../Main/release/updateproductdialog.dll
    DLLDESTDIR = ../../Main/release/plugins/Invoicing/
}

SOURCES += sell_invoicing.cpp \
    sellupdateproductdialog.cpp

HEADERS += sell_invoicing.h\
        Sell_Invoicing_global.h \
    sellupdateproductdialog.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE5968762
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = Sell_Invoicing.dll
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
    Icon/sell_icon.png \
    Icon/up_icon.png \
    Icon/submitorder_icon.png \
    Icon/down_icon.png \
    Translations/$${TARGET}_zh_CN.ts \
    Translations/$${TARGET}_zh_CN.qm

RESOURCES += \
    icon.qrc
