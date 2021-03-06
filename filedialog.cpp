/* qtypist - a typing training program
 *
 * Copyright (C) 2012-2015 Che-Huai Lin <lzh9102@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 or 3 of the License.
 *
 * project page: https://github.com/lzh9102/qtypist
 */

#include <QDir>
#include <QStringList>
#include <QSettings>
#include <QDebug>
#include <QDesktopServices>
#include <QUrl>
#include "filedialog.h"
#include "ui_filedialog.h"
#include "paths.h"

FileDialog::FileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileDialog)
{
    ui->setupUi(this);

    QSettings settings;
    QString prev_list = settings.value("file_prev", "").toString();
    int prev_index = 0;
    qDebug() << "prev_list: " << prev_list;

    QDir dir(Paths::dataFileName("lists"));
    dir.setFilter(QDir::Files);
    QStringList list = dir.entryList(QStringList() << "*.txt");
    for (int i=0; i<list.size(); i++) {
        QString file = list.at(i);
        Q_ASSERT(file.endsWith(".txt"));
        qDebug() << "found file: " << file;
        file.chop(sizeof(".txt")-1);
        qDebug() << "file: " << file;
        ui->listFiles->addItem(file);
        if (prev_list == file)
            prev_index = i;
    }
    qDebug() << "index: " << prev_index;

    ui->listFiles->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->listFiles->setCurrentRow(prev_index, QItemSelectionModel::SelectCurrent);

    connect(this, SIGNAL(accepted()), this, SLOT(slotAccepted()));
    connect(ui->listFiles, SIGNAL(doubleClicked(QModelIndex))
            , this, SLOT(accept()));
    connect(ui->btnOpenListDir, SIGNAL(clicked())
            , this, SLOT(slotOpenListDir()));
}

FileDialog::~FileDialog()
{
    delete ui;
}

QString FileDialog::selectedFile() const
{
    return Paths::dataFileName("lists/" + selectedName() + ".txt");
}

QString FileDialog::selectedName() const
{
    if (ui->listFiles->selectedItems().isEmpty())
        return "";
    else
        return ui->listFiles->selectedItems().at(0)->text();
}

void FileDialog::slotAccepted()
{
    QSettings settings;
    if (!ui->listFiles->selectedItems().isEmpty())
        settings.setValue("file_prev", ui->listFiles->selectedItems().at(0)->text());
}

void FileDialog::slotOpenListDir()
{
    QString dir_path = Paths::dataFileName("lists");
    QDesktopServices::openUrl(QUrl::fromLocalFile(dir_path));
}
