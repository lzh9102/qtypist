#include <QDir>
#include <QStringList>
#include <QSettings>
#include <QDebug>
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

    QDir dir(Paths::dataFileName("lists"));
    dir.setFilter(QDir::Files);
    QStringList list = dir.entryList();
    for (int i=0; i<list.size(); i++) {
        QString file = list.at(i);
        if (file.endsWith(".txt")) {
            qDebug() << "found file: " << file;
            file.chop(sizeof(".txt")-1);
            ui->listFiles->addItem(file);
            if (prev_list == file)
                prev_index = i;
        }
    }

    ui->listFiles->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->listFiles->setCurrentRow(prev_index);

    connect(this, SIGNAL(accepted()), this, SLOT(slotAccepted()));
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

void FileDialog::slotAccepted()
{
    QSettings settings;
    if (!ui->listFiles->selectedItems().isEmpty())
        settings.setValue("file_prev", ui->listFiles->selectedItems().at(0)->text());
}
