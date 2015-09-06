#-------------------------------------------------
#
# Project created by QtCreator 2014-02-04T20:32:39
#
#-------------------------------------------------

QT       += sql widgets
TARGET = UpdateProductDialog
INCLUDEPATH += ../../main
CONFIG += copy_dir_files
TEMPLATE = lib
TRANSLATIONS = Translations/$${TARGET}_zh_CN.ts

DEFINES += UPDATEPRODUCTDIALOG_LIBRARY

CONFIG(debug, debug|release) {
    DLLDESTDIR = ../../Main/debug/
}
CONFIG(release, debug|release) {
    DLLDESTDIR = ../../Main/release/
}

SOURCES += updateproductdialog.cpp

HEADERS += updateproductdialog.h\
        updateproductdialog_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

OTHER_FILES += \
    Translations/$${TARGET}_zh_CN.ts \
    Translations/$${TARGET}_zh_CN.qm
