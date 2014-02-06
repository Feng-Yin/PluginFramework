#-------------------------------------------------
#
# Project created by QtCreator 2012-03-09T20:15:59
#
#-------------------------------------------------

QT       += sql

TARGET = Purchase_Invoicing
TEMPLATE = lib
CONFIG += plugin copy_dir_files
CONFIG += qaxcontainer
INCLUDEPATH += ../../main
INCLUDEPATH += ../UpdateProductDialog
DEFINES += PURCHASE_INVOICING_LIBRARY
TRANSLATIONS = ./Translations/$${TARGET}_zh_CN.ts
CONFIG(debug, debug|release) {
    LIBS += ../../Main/debug/updateproductdialog.dll
    DLLDESTDIR = ../../Main/debug/plugins/Invoicing/
}
CONFIG(release, debug|release) {
    LIBS += ../../Main/release/updateproductdialog.dll
    DLLDESTDIR = ../../Main/release/plugins/Invoicing/
}

SOURCES += purchase_invoicing.cpp \
    addproductdialog.cpp

HEADERS += purchase_invoicing.h\
        Purchase_Invoicing_global.h \
    addproductdialog.h
symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xECF2370F
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = Purchase_Invoicing.dll
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

RESOURCES += \
    icon.qrc

OTHER_FILES += \
    Icon/purchase_icon.png \
    Icon/print_icon.png \
    Icon/print_icon1.png \
    Icon/import_icon.png \
    Icon/empty_icon.png \
    Icon/empty_icon1.png \
    Icon/delete_icon.png \
    Icon/add_icon.png \
    Icon/commit_icon.png \
    Icon/commitall_icon.png \
    Translations/$${TARGET}_zh_CN.qm \
    Translations/$${TARGET}_zh_CN.ts
