#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QDir>
#include <QDebug>
#include <cstdlib>
#include "mainwindow.h"
#include "paths.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    srand(time(0));

    MainWindow w;

    Paths::setAppPath(app.applicationDirPath());

    // Setup translation.
    QString locale = QLocale::system().name();
    QTranslator translator;
    QString translation_basename =
            QDir(Paths::translationPath()).absoluteFilePath("typist_");
    qDebug() << "Translation file: " + translation_basename + locale + ".qm";
    translator.load(translation_basename + locale);
    app.installTranslator(&translator);

    w.show();
    return app.exec();
}
