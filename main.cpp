/* qtypist - a typing training program
 *
 * Copyright (C) 2012 Timothy Lin <lzh9102@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 or 3 of the License.
 *
 * project page: http://code.google.com/p/qtypist
 */

#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QDir>
#include <QDebug>
#include <QSettings>
#include <cstdlib>
#include <ctime>
#include "mainwindow.h"
#include "paths.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Register QSettings information.
    app.setOrganizationName("qtypist");
#ifdef PORTABLE_APP // Portable App: Save settings in <exepath>/qtypist.ini.
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope
                       , app.applicationDirPath());
    qDebug() << "Setting path: " + app.applicationDirPath();
#endif

    Paths::setAppPath(app.applicationDirPath());

    // Setup translation.
    QString locale = QLocale::system().name();
    QTranslator translator;
    QString translation_basename =
            QDir(Paths::translationPath()).absoluteFilePath("qtypist_");
    qDebug() << "Translation file: " + translation_basename + locale + ".qm";
    translator.load(translation_basename + locale);
    app.installTranslator(&translator);

    // Load translation for Qt library
    QTranslator translator_qt;
    translator_qt.load("qt_" + QLocale::system().name(), Paths::qtTranslationPath());
    app.installTranslator(&translator_qt);

    srand(time(0));

    MainWindow w;
    w.show();
    return app.exec();
}
