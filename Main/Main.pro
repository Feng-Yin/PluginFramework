TARGET = Invoicing
TRANSLATIONS = ./Translations/Invoicing_zh_CN.ts

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h \
    plugin_interface.h \
    usermanagement_interface.h \
    productmanagement_interface.h

OTHER_FILES += \
    ideas.txt \
    Icon/plugin_icon.png \
    Icon/quit_icon.png \
    Icon/about_icon.png \
    Translations/Invoicing_zh_CN.ts \
    Translations/Invoicing_zh_CN.qm \
    Icon/update_icon.png \
    invoices.ico \
    logo.rc

RESOURCES += \
    icon.qrc

RC_FILE = logo.rc
