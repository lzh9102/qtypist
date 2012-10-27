#-------------------------------------------------
#
# Project created by QtCreator 2012-10-27T03:37:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = typist
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    queuedisplay.cpp \
    stringmatching.cpp \
    filedialog.cpp \
    paths.cpp \
    datasource.cpp \
    chartdisplay.cpp

HEADERS  += mainwindow.h \
    queuedisplay.h \
    stringmatching.h \
    filedialog.h \
    paths.h \
    datasource.h \
    chartdisplay.h

FORMS    += mainwindow.ui \
    filedialog.ui

RESOURCES += \
    resource.qrc

TRANSLATIONS += \
    translations/qtypist_zh_TW.ts
