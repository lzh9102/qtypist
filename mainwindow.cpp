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
#include "ui_mainwindow.h"
#include "queuedisplay.h"
#include "chartdisplay.h"
#include "filedialog.h"
#include "datasource.h"

#define DEFAULT_FONT_SIZE 25
#define MIN_LINE_COUNT 50

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_display(new QueueDisplay(this)),
    m_chart(new ChartDisplay(this)),
    m_dataSource(new DataSource(this)),
    m_maxSpeed(0),
    m_totalChars(0),
    m_totalTime(0)
{
    ui->setupUi(this);
    ui->displayLayout->addWidget(m_display);
    ui->chartLayout->addWidget(m_chart);
    ui->chartLayout->setSizeConstraint(QLayout::SetMaximumSize);
    ui->lblAvgSpeed->setText("");
    ui->lblMaxSpeed->setText("");

    setupToolbar();
    setupEvents();
    loadSettings();
}

MainWindow::~MainWindow()
{
    saveSettings();
    delete ui;
}

// Handle Return key in the input box
void MainWindow::slotHandleInput()
{
    QString string = ui->txtInput->text().trimmed();
    if (!string.isEmpty() && !m_display->isEmpty()) {
        const bool correct = judgeInput(string);
        if (correct) { // correct input
            ui->txtInput->clear();
            m_display->pop();
            updateChart(string.size(), m_elapsedTimer.elapsed());
            m_elapsedTimer.restart();
        } else { // incorrect input
            ui->txtInput->selectAll();
        }
        updateStatus(correct);
        refillQueue();
    }
}

void MainWindow::slotTextEdited()
{
    if (!m_display->isEmpty() && ui->actionAutoCommit->isChecked()) {
        if (judgeInput(ui->txtInput->text()))
            slotHandleInput();
    }
}

void MainWindow::slotSkip()
{
    if (!m_display->isEmpty()) {
        m_display->pop();
    }
}

void MainWindow::slotUnderline(bool checked)
{
    m_display->setUnderlineFront(checked);
}

void MainWindow::slotHideParen(bool checked)
{
    m_display->setHideParen(checked);
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
                     "under the terms of the GNU General Public License version 2 or 3."));
}

void MainWindow::loadSettings()
{
    QSettings settings;

    const int font_size = settings.value("fontsize", DEFAULT_FONT_SIZE).toInt();
    QFont font = ui->txtInput->font();
    font.setPointSize(font_size);
    ui->txtInput->setFont(font);
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
            , this, SLOT(slotSkip()));
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

bool MainWindow::judgeInput(QString string)
{
    // remove text quoted by parenthesis
    QString expect = m_display->front();
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
        if (!m_dataSource->addFile(dialog.selectedFile())) {
            QMessageBox::critical(this, tr("Error")
                                  , tr("Failed to read file."));
        } else { // file opened successfully
            m_display->clear();
            refillQueue();
            setWindowTitle(QString("%1 - %2").arg(dialog.selectedName())
                           .arg(tr("Typing Drill")));
            m_elapsedTimer.restart();
        }
    }
}

void MainWindow::refillQueue()
{
    if (!m_dataSource->isEmpty()) {
        while (m_display->count() < MIN_LINE_COUNT)
            m_display->push(m_dataSource->next());
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
