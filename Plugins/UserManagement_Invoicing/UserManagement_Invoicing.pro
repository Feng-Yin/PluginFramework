#-------------------------------------------------
#
# Project created by QtCreator 2012-02-02T12:14:30
#
#-------------------------------------------------

QT       += sql network widgets

TARGET = UserManagement_Invoicing
TEMPLATE = lib
CONFIG += plugin copy_dir_files
INCLUDEPATH += ../../main
DEFINES += USERMANAGEMENT_INVOICING_LIBRARY
TRANSLATIONS = Translations/$${TARGET}_zh_CN.ts
CONFIG(debug, debug|release) {
    DLLDESTDIR = ../../Main/debug/plugins/Invoicing/
}
CONFIG(release, debug|release) {
    DLLDESTDIR = ../../Main/release/plugins/Invoicing/
}

SOURCES += usermanagement_invoicing.cpp \
    logindialog.cpp \
    usermanagementdialog.cpp

HEADERS += usermanagement_invoicing.h\
        UserManagement_Invoicing_global.h \
    logindialog.h \
    usermanagementdialog.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE0EACA55
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = UserManagement_Invoicing.dll
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
    Icon/usermanagement_icon.png \
    Icon/login_icon.png \
    Icon/delete_icon.png \
    Icon/add_icon.png \
    Icon/changepassword_icon.png \
    Icon/logout_icon.png \
    Icon/switchuser_icon.png \
    Translations/$${TARGET}_zh_CN.qm \
    Translations/$${TARGET}_zh_CN.ts \
    Icon/cleanlogininfo_icon.png
