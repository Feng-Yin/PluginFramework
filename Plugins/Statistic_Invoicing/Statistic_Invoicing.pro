#-------------------------------------------------
#
# Project created by QtCreator 2012-03-26T20:43:28
#
#-------------------------------------------------

QT       += sql widgets printsupport

TARGET = Statistic_Invoicing
TEMPLATE = lib
CONFIG += plugin copy_dir_files
CONFIG += qwt
INCLUDEPATH += ../../main
INCLUDEPATH += ../UpdateProductDialog
DEFINES += STATISTIC_INVOICING_LIBRARY
TRANSLATIONS = Translations/$${TARGET}_zh_CN.ts
CONFIG(debug, debug|release) {
    LIBS += ../../Main/debug/updateproductdialog.dll
    DLLDESTDIR = ../../Main/debug/plugins/Invoicing/
}
CONFIG(release, debug|release) {
    LIBS += ../../Main/release/updateproductdialog.dll
    DLLDESTDIR = ../../Main/release/plugins/Invoicing/
}

SOURCES += statistic_invoicing.cpp \
        barchart.cpp \
    statisticupdateproductdialog.cpp \
    exportexcelobject.cpp

HEADERS += statistic_invoicing.h\
        Statistic_Invoicing_global.h\
        barchart.h \
    statisticupdateproductdialog.h \
    exportexcelobject.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xED5A9B43
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = Statistic_Invoicing.dll
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
    Icon/statistic_icon.png \
    Translations/$${TARGET}_zh_CN.ts \
    Translations/$${TARGET}_zh_CN.qm

RESOURCES += \
    icon.qrc
