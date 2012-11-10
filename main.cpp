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
#include <cstdlib>
#include <ctime>
#include "mainwindow.h"
#include "paths.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setOrganizationName("qtypist");

    Paths::setAppPath(app.applicationDirPath());

    // Setup translation.
    QString locale = QLocale::system().name();
    QTranslator translator;
    QString translation_basename =
            QDir(Paths::translationPath()).absoluteFilePath("qtypist_");
    qDebug() << "Translation file: " + translation_basename + locale + ".qm";
    translator.load(translation_basename + locale);
    app.installTranslator(&translator);

    srand(time(0));

    MainWindow w;
    w.show();
    return app.exec();
}
