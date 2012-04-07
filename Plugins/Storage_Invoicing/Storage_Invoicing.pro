#-------------------------------------------------
#
# Project created by QtCreator 2012-03-26T16:41:30
#
#-------------------------------------------------

QT       += sql

TARGET = Storage_Invoicing
TEMPLATE = lib
CONFIG += plugin copy_dir_files
INCLUDEPATH += ../../main
DEFINES += STORAGE_INVOICING_LIBRARY

CONFIG(debug, debug|release) {
    DLLDESTDIR = ../../Main/debug/plugins/Invoicing/
}
CONFIG(release, debug|release) {
    DLLDESTDIR = ../../Main/release/plugins/Invoicing/
}

SOURCES += storage_invoicing.cpp \
    updateproductdialog.cpp

HEADERS += storage_invoicing.h\
        Storage_Invoicing_global.h \
    updateproductdialog.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xEE3CB746
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = Storage_Invoicing.dll
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
    Icon/storage_icon.png \
    Icon/up_icon.png \
    Icon/down_icon.png \
    Icon/update_icon.png

RESOURCES += \
    icon.qrc
