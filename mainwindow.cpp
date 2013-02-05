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

#include "mainwindow.h"
#include <QSettings>
#include <QMessageBox>
#include <QTimer>
#include <QElapsedTimer>
#include <QDebug>
#include <algorithm>
#include "ui_mainwindow.h"
#include "historydisplay.h"
#include "chartdisplay.h"
#include "filedialog.h"
#include "datasource.h"
#include "stringmatching.h"
#include "workingset.h"

#define DEFAULT_FONT_SIZE 25
#define MIN_LINE_COUNT 50

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_display(new HistoryDisplay(this)),
    //m_chart(new ChartDisplay(this)),
    m_dataSource(new DataSource(this)),
    m_workingSet(new WorkingSet(*m_dataSource, this)),
    m_maxSpeed(0),
    m_totalChars(0),
    m_totalTime(0)
{
    ui->setupUi(this);
    ui->displayLayout->addWidget(m_display);
    //ui->chartLayout->addWidget(m_chart);
    ui->chartLayout->setSizeConstraint(QLayout::SetMaximumSize);
    ui->lblAvgSpeed->setText("");
    ui->lblMaxSpeed->setText("");
    ui->lblInput->setText("");
    ui->actionHideParen->setVisible(false);

    setupToolbar();
    setupEvents();
    loadSettings();
}

MainWindow::~MainWindow()
{
    saveSettings();
    delete ui;
}

// proceed to next word
void MainWindow::slotNext()
{
    const QString s = m_workingSet->current();
    int average_time = m_elapsedTimer.elapsed();
    if (!s.isEmpty())
        average_time /= s.size();
    m_display->push(s);
    ui->lblInput->setText(m_workingSet->next(average_time));
    ui->txtInput->clear();
    m_elapsedTimer.restart();
}

// Handle Return key in the input box
void MainWindow::slotHandleInput()
{
    QString string = ui->txtInput->text().trimmed();
    if (!string.isEmpty()) {
        const bool correct = judgeInput(string);
        if (correct) { // correct input
            slotNext();
        } else { // incorrect input
            selectFirstError();
        }
        updateStatus(correct);
    }
}

void MainWindow::slotTextEdited()
{
    if (!ui->lblInput->text().isEmpty()) {
        highlightError();
        if (ui->actionAutoCommit->isChecked()
                && judgeInput(ui->txtInput->text())) {
            slotHandleInput();
        }
    }
}

void MainWindow::slotUnderline(bool checked)
{
    QFont font = ui->lblInput->font();
    font.setUnderline(checked);
    ui->lblInput->setFont(font);
}

void MainWindow::slotHideParen(bool checked)
{
    // TODO: remove this slot
    //m_display->setHideParen(checked);
}

void MainWindow::slotWindowLoaded()
{
    openFileDialog();
}

void MainWindow::slotOpenFile()
{
    openFileDialog();
}

void MainWindow::slotAbout()
{
    QMessageBox::information(
                this,
                tr("About this program"),
                "qtypist (C) 2012 Timothy Lin\n\n"
                + tr("This program is free software; you can redistribute it and/or modify it "
                     "under the terms of the GNU General Public License version 2 or 3.")
                + "\n\n" + tr("Project Homepage: ") + "http://code.google.com/p/qtypist");
}

void MainWindow::loadSettings()
{
    QSettings settings;

    const int font_size = settings.value("fontsize", DEFAULT_FONT_SIZE).toInt();
    QFont font = ui->txtInput->font();
    font.setPointSize(font_size);
    ui->txtInput->setFont(font);
    font.setBold(true);
    ui->lblInput->setFont(font);
    m_display->setFontSize(font_size);

    const bool underline = settings.value("underline", true).toBool();
    ui->actionUnderline->setChecked(underline);
    slotUnderline(underline); // guarantee that m_display is updated

    ui->actionAutoCommit->setChecked(settings.value("autocommit", true).toBool());

    const bool hideparen = settings.value("hideparen", false).toBool();
    ui->actionHideParen->setChecked(hideparen);
    slotHideParen(hideparen);

    restoreGeometry(settings.value("window_geometry").toByteArray());
    restoreState(settings.value("window_state").toByteArray());
}

void MainWindow::saveSettings()
{
    QSettings settings;
    settings.setValue("underline", ui->actionUnderline->isChecked());
    settings.setValue("autocommit", ui->actionAutoCommit->isChecked());
    settings.setValue("hideparen", ui->actionHideParen->isChecked());
    settings.setValue("window_geometry", saveGeometry());
    settings.setValue("window_state", saveState());
}

void MainWindow::setupEvents()
{
    connect(ui->txtInput, SIGNAL(returnPressed())
            , this, SLOT(slotHandleInput()));
    connect(ui->actionExit, SIGNAL(triggered())
            , this, SLOT(close()));
    connect(ui->actionUnderline, SIGNAL(triggered(bool))
            , this, SLOT(slotUnderline(bool)));
    connect(ui->actionOpen, SIGNAL(triggered())
            , this, SLOT(slotOpenFile()));
    connect(ui->txtInput, SIGNAL(textChanged(QString))
            , this, SLOT(slotTextEdited()));
    connect(ui->actionSkip, SIGNAL(triggered())
            , this, SLOT(slotNext()));
    connect(ui->actionHideParen, SIGNAL(triggered(bool))
            , this, SLOT(slotHideParen(bool)));
    connect(ui->actionAbout, SIGNAL(triggered())
            , this, SLOT(slotAbout()));

    // call slotWindowReady() when program enters the event loop
    QTimer::singleShot(0, this, SLOT(slotWindowLoaded()));
}

void MainWindow::setupToolbar()
{
    ui->toolBar->addAction(ui->actionOpen);
    ui->toolBar->addAction(ui->actionSkip);
    ui->toolBar->addAction(ui->actionAutoCommit);
}

// highlight mistyped characters
void MainWindow::highlightError()
{
    QString expect = m_workingSet->current().trimmed();
    QString input = ui->txtInput->text().trimmed();
    QStringList markup;
    QString tag_begin, tag_end;
    //ui->lblInput->
    //const int length = std::min(expect.size(), input.size());
    for (int i=0; i<expect.size(); i++) {
        if (i < input.size()) {
            if (input.at(i) == expect.at(i)) { // correct
                tag_begin = "<font color='green'>";
                tag_end = "</font>";
            } else { // incorrect
                tag_begin = "<font color='red'>";
                tag_end = "</font>";
            }
        } else { // not typed
            tag_begin = "";
            tag_end = "";
        }
        markup << tag_begin << expect.at(i) << tag_end;
    }
    ui->lblInput->setText(markup.join(""));
}

void MainWindow::selectFirstError()
{
    QString input = ui->txtInput->text().trimmed();
    if (!input.isEmpty()) {
        QString expect = m_workingSet->current().trimmed();
        int offset = ui->txtInput->text().indexOf(input.at(0));
        const int length = std::min(input.size(), expect.size());
        int index_begin = -1, index_end = length-1;
        for (int i=0; i<length; i++) {
            if (input.at(i) == expect.at(i)) {
                if (index_begin >= 0) {
                    index_end = i-1;
                    break;
                }
            } else { // mismatch
                if (index_begin < 0)
                    index_begin = i;
            }
        }
        if (index_begin < 0 && input.size() > expect.size()) {
            index_begin = expect.size();
            index_end = input.size() - 1;
        }
        if (index_begin >= 0) {
            ui->txtInput->setSelection(index_begin + offset, index_end - index_begin + 1);
        }
    }
}

bool MainWindow::judgeInput(QString string)
{
    // remove text quoted by parenthesis
    QString expect = m_workingSet->current();
    expect.remove(QRegExp("\\([^)]*\\)"));
    return string.trimmed() == expect.trimmed();
}

void MainWindow::updateStatus(bool correct)
{
    const char *image_path;
    QColor text_color;

    if (correct) {
        image_path = ":/icon/images/correct.png";
        text_color = this->palette().color(QPalette::Text);
    } else {
        image_path = ":/icon/images/incorrect.png";
        text_color = Qt::red;
    }

    // update correct/incorrect icon
    ui->lblStatus->setPixmap(QPixmap(image_path));

    // update inputbox text color
    QPalette palette = ui->txtInput->palette();
    palette.setColor(QPalette::Text, text_color);
    ui->txtInput->setPalette(palette);
}

void MainWindow::openFileDialog()
{
    FileDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        // remove existing phrases and load the new list
        m_dataSource->clear();
        m_workingSet->reset();
        if (!m_dataSource->addFile(dialog.selectedFile())) {
            QMessageBox::critical(this, tr("Error")
                                  , tr("Failed to read file."));
        } else { // file opened successfully
            m_display->clear();
            slotNext();
            setWindowTitle(QString("%1 - %2").arg(dialog.selectedName())
                           .arg(tr("Typing Drill")));
            m_elapsedTimer.restart();
        }
    }
}

void MainWindow::updateChart(int count, int ms)
{
    double speed = (double)count * 1000 * 60 / (ms + 1);
    m_chart->pushData(speed);
    if (speed > m_maxSpeed)
        m_maxSpeed = speed;
    m_totalChars += count;
    m_totalTime += ms;
    double avg_speed = m_totalChars * 1000 * 60 / (m_totalTime+1);
    ui->lblMaxSpeed->setText(tr("Max Speed: %1").arg(m_maxSpeed));
    ui->lblAvgSpeed->setText(tr("Average Speed: %1").arg(avg_speed));
}
