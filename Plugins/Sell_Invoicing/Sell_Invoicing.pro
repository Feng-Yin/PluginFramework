#-------------------------------------------------
#
# Project created by QtCreator 2012-03-26T19:42:37
#
#-------------------------------------------------

QT       += sql

TARGET = Sell_Invoicing
TEMPLATE = lib
CONFIG += plugin copy_dir_files
INCLUDEPATH += ../../main
DEFINES += SELL_INVOICING_LIBRARY

CONFIG(debug, debug|release) {
    DLLDESTDIR = ../../Main/debug/plugins/Invoicing/
}
CONFIG(release, debug|release) {
    DLLDESTDIR = ../../Main/release/plugins/Invoicing/
}

SOURCES += sell_invoicing.cpp \
    updateproductdialog.cpp

HEADERS += sell_invoicing.h\
        Sell_Invoicing_global.h \
    updateproductdialog.h

TRANSLATIONS = Translations/Sell_Invoicing_zh_CN.ts

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
    Translations/Sell_Invoicing_zh_CN.ts \
    Translations/Sell_Invoicing_zh_CN.qm

RESOURCES += \
    icon.qrc
