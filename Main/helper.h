#pragma once

#define INSTALL_TRANSLATION \
do { \
    foreach (QString fileName, QDir(":/Translations").entryList(QDir::Files)) { \
        QTranslator *qtTranslator = new QTranslator(); \
        if(qtTranslator->load(QFileInfo(fileName).baseName(), ":/Translations")){ \
            QApplication::instance()->installTranslator(qtTranslator); \
        } \
    } \
} while(0)
