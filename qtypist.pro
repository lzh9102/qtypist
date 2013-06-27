#-------------------------------------------------
#
# Project created by QtCreator 2012-10-27T03:37:15
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtypist
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    stringmatching.cpp \
    filedialog.cpp \
    paths.cpp \
    datasource.cpp \
    chartdisplay.cpp \
    historydisplay.cpp \
    workingset.cpp \
    httpdownloader.cpp \
    audioplayer.cpp

HEADERS  += mainwindow.h \
    stringmatching.h \
    filedialog.h \
    paths.h \
    datasource.h \
    chartdisplay.h \
    historydisplay.h \
    workingset.h \
    httpdownloader.h \
    audioplayer.h

FORMS    += mainwindow.ui \
    filedialog.ui

RESOURCES += \
    resource.qrc

TRANSLATIONS += \
    translations/qtypist_zh_TW.ts

win32 {
    RC_FILE = appicon.rc
}
