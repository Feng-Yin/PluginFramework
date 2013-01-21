SYSZUX = qsqlmysql
TEMPLATE = lib
CONFIG += qt plugin build_all
QT  = core sql
DEFINES += QT_NO_CAST_TO_ASCII QT_NO_CAST_FROM_ASCII

HEADERS += src/qsql_mysql.h
SOURCES += src/qsql_mysql.cpp \
    src/main.cpp

unix {
    isEmpty(QT_LFLAGS_MYSQL) {
        !contains(LIBS, .*mysqlclient.*):!contains(LIBS, .*mysqld.*) {
            use_libmysqlclient_r:LIBS += -lmysqlclient_r
            else:LIBS += -lmysqlclient
        }
    } else {
        LIBS *= $$QT_LFLAGS_MYSQL
        QMAKE_CXXFLAGS *= $$QT_CFLAGS_MYSQL
    }
} else {
    !contains(LIBS, .*mysql.*):!contains(LIBS, .*mysqld.*):LIBS += -L $$PWD/mysql -llibmysql
}

win32 {
    COPY = copy /y
    SYSZUX_SRC = $$PWD\\mysql\\libmysql.dll
    SYSZUX_DEST = $$[QT_INSTALL_BINS]
    SYSZUX_SRC=$$replace(SYSZUX_SRC,/,\\)
    SYSZUX_DEST=$$replace(SYSZUX_DEST,/,\\)
    system($$COPY $$SYSZUX_SRC $$SYSZUX_DEST)
    message("hello,gemfield@civilnet.cn")
}

win32|mac:!wince*:!win32-msvc:!macx-xcode:CONFIG += debug_and_release

wince*:LIBS += $$QMAKE_LIBS_GUI

symbian: {
    TARGET.EPOCALLOWDLLDATA=1
    TARGET.CAPABILITY = All -Tcb
    TARGET = $${TARGET}$${QT_LIBINFIX}
    load(armcc_warnings)
}

isEmpty(QT_MAJOR_VERSION) {
   VERSION=4.7.3
} else {
   VERSION=$${QT_MAJOR_VERSION}.$${QT_MINOR_VERSION}.$${QT_PATCH_VERSION}
}

contains(QT_CONFIG, reduce_exports):CONFIG += hide_symbols
CONFIG(release, debug|release) {
 TARGET = $$qtLibraryTarget($$SYSZUX)
 #DESTDIR=$$[QT_INSTALL_PLUGINS]/sqldrivers
DLLDESTDIR = ../../Main/release/sqldrivers/
}
CONFIG(debug, debug|release) {
 TARGET = $$qtLibraryTarget($$SYSZUX)
 #DESTDIR=$$[QT_INSTALL_PLUGINS]/sqldrivers
DLLDESTDIR = ../../Main/debug/sqldrivers/
}

QMAKE_TARGET_COMPANY = SYSZUX
QMAKE_TARGET_PRODUCT = SYSZUXmysql
QMAKE_TARGET_DESCRIPTION = http://civilnet.cn/syszux
QMAKE_TARGET_COPYRIGHT = Copyright (C) 2011 gemfield.


