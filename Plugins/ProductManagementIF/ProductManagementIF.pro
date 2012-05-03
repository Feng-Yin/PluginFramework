#-------------------------------------------------
#
# Project created by QtCreator 2012-03-23T19:56:54
#
#-------------------------------------------------

QT       += sql

TARGET = ProductManagementIF
TEMPLATE = lib
CONFIG += plugin copy_dir_files
INCLUDEPATH += ../../main
DEFINES += PRODUCTMANAGEMENTIF_LIBRARY
CONFIG(debug, debug|release) {
    DLLDESTDIR = ../../Main/debug/plugins/
}
CONFIG(release, debug|release) {
    DLLDESTDIR = ../../Main/release/plugins/
}

SOURCES += productmanagementif.cpp

HEADERS += productmanagementif.h\
        ProductManagementIF_global.h

TRANSLATIONS = Translations/ProductManagementIF_zh_CN.ts

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xEC5895ED
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = ProductManagementIF.dll
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
    Translations/ProductManagementIF_zh_CN.ts \
    Translations/ProductManagementIF_zh_CN.qm

RESOURCES += \
    rc.qrc
