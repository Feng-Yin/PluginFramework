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
    Translations/zh_CN.qm \
    Translations/zh_CN.ts \
    Translations/UserManagement_Invoicing_zh_CN.ts \
    Translations/Main_zh_CN.ts \
    Translations/Main_zh_CN.qm \
    Translations/UserManagement_Invoicing_zh_CN.ts

RESOURCES += \
    icon.qrc

TRANSLATIONS = Translations/Main_zh_CN.ts
