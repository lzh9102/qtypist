#include <QDir>
#include <QStringList>
#include <QDebug>
#include "filedialog.h"
#include "ui_filedialog.h"
#include "paths.h"

FileDialog::FileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileDialog)
{
    ui->setupUi(this);

    QDir dir(Paths::dataFileName("lists"));
    dir.setFilter(QDir::Files);
    QStringList list = dir.entryList();
    foreach (QString file, list) {
        if (file.endsWith(".txt")) {
            qDebug() << "found file: " << file;
            file.chop(sizeof(".txt")-1);
            ui->listFiles->addItem(file);
        }
    }

    ui->listFiles->setSelectionMode(QAbstractItemView::SingleSelection);
}

FileDialog::~FileDialog()
{
    delete ui;
}

QString FileDialog::selectedFile() const
{
    QString filename;
    if (ui->listFiles->selectedItems().isEmpty())
        return "";
    else
        filename = ui->listFiles->selectedItems().at(0)->text().append(".txt");
    filename = Paths::dataFileName("lists/" + filename);
    return filename;
}
