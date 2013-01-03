TARGET = Invoicing

TRANSLATIONS = ./Translations/$${TARGET}_zh_CN.ts

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
    Translations/$${TARGET}_zh_CN.ts \
    Translations/$${TARGET}_zh_CN.qm

RESOURCES += \
    icon.qrc
