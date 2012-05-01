#-------------------------------------------------
#
# Project created by QtCreator 2012-03-23T19:53:53
#
#-------------------------------------------------

QT       += sql

QT       -= gui

TARGET = UserManagementIF
TEMPLATE = lib
CONFIG += plugin copy_dir_files
INCLUDEPATH += ../../main

DEFINES += USERMANAGEMENTIF_LIBRARY
CONFIG(debug, debug|release) {
    DLLDESTDIR = ../../Main/debug/plugins/
}
CONFIG(release, debug|release) {
    DLLDESTDIR = ../../Main/release/plugins/
}

SOURCES += usermanagementif.cpp

HEADERS += usermanagementif.h\
        UserManagementIF_global.h

#TRANSLATIONS = Translations/UserManagementIF_zh_CN.ts

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE4EAF9F2
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = UserManagementIF.dll
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
