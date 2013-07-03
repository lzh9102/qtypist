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
#include <QFile>
#include <QTextStream>
#include <algorithm>
#include "ui_mainwindow.h"
#include "historydisplay.h"
#include "chartdisplay.h"
#include "filedialog.h"
#include "datasource.h"
#include "stringmatching.h"
#include "workingset.h"
#include "audiomanager.h"
#include "paths.h"

#define DEFAULT_FONT_SIZE 25
#define MIN_LINE_COUNT 50

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_display(new HistoryDisplay(this)),
    m_chart(new ChartDisplay(this)),
    m_dataSource(new DataSource(this)),
    m_workingSet(new WorkingSet(*m_dataSource, this)),
    m_audio(new AudioManager(this)),
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
    ui->lblInput->setText("");
    ui->actionHideParen->setVisible(false);

    setupToolbar();
    setupEvents();
    loadSettings();
    loadAudioUrls();
}

MainWindow::~MainWindow()
{
    saveSettings();
    delete ui;
}

// proceed to next word
void MainWindow::slotNext()
{
    const QString s = m_workingSet->currentPhrase();
    int average_time = m_elapsedTimer.elapsed();
    if (!s.isEmpty())
        average_time /= s.size();
    updateChart(/* 1 character */ 1, /* milliseconds */ average_time);
    m_display->push(s);
    m_workingSet->updatePriority(average_time);
    m_workingSet->next();
    QString phrase = m_workingSet->currentPhrase();
    QString comment = m_workingSet->currentComment();
    setInputHint(phrase, comment);
    ui->txtInput->clear();
    m_elapsedTimer.restart();
    slotSpeak();
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
            slotSpeak();
        }
        updateStatus(correct);
    } else {
        slotSpeak();
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

void MainWindow::slotHideParen(bool /*checked*/)
{
    // TODO: remove this slot
    //m_display->setHideParen(checked);
}

void MainWindow::slotMaskPhrase(bool checked)
{
    if (checked) {
        setInputHint(m_workingSet->currentPhrase(),
                     m_workingSet->currentComment());
    }
}

void MainWindow::slotHidePhrase(bool checked)
{
    if (checked) {
        setInputHint("", m_workingSet->currentComment());
    }
}

void MainWindow::slotShowSpeedChart(bool checked)
{
    m_chart->setVisible(checked);
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

void MainWindow::slotSpeak()
{
    if (!ui->actionMute->isChecked())
        m_audio->playPhrase(m_workingSet->currentPhrase(), m_dataSource->language());
    else // download but not play
        m_audio->downloadPhrase(m_workingSet->currentPhrase(), m_dataSource->language());
}

void MainWindow::loadAudioUrls()
{
    QString filename = Paths::dataFileName("audio-sources.txt");
    QFile source_list(filename);
    if (!source_list.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this
                              , tr("error")
                              , tr("file not found: %1").arg(filename));
    }
    QTextStream instr(&source_list);
    while (!instr.atEnd()) {
        QString line = instr.readLine();
        if (line.isEmpty())
            continue;
        QStringList fields = line.split(" ", QString::SkipEmptyParts);
        if (fields.size() == 2) {
            QString lang = fields[0];
            QString url = fields[1];
            m_audio->addUrlPattern(url, lang);
        }
    }
}

void MainWindow::loadSettings()
{
    QSettings settings;
#define LOAD_ACTION(aname, name, default_value) ui->action ## aname \
                    ->setChecked(settings.value(name, default_value).toBool())

    const int font_size = settings.value("fontsize", DEFAULT_FONT_SIZE).toInt();
    QFont font = ui->txtInput->font();
    font.setPointSize(font_size);
    ui->txtInput->setFont(font);
    font.setBold(true);
    ui->lblInput->setFont(font);
    m_display->setFontSize(font_size);

    const bool underline = settings.value("underline", true).toBool();
    slotUnderline(underline); // guarantee that m_display is updated

    LOAD_ACTION(Underline, "underline", false);
    LOAD_ACTION(AutoCommit, "autocommit", false);
    LOAD_ACTION(HideParen, "hideparen", false);
    LOAD_ACTION(MaskPhrase, "mask_phrase", false);
    LOAD_ACTION(Mute, "mute", false);
    LOAD_ACTION(CommentOnly, "hide_phrase", false);
    LOAD_ACTION(ShowSpeedChart, "show_speed_chart", true);

    slotShowSpeedChart(ui->actionShowSpeedChart->isChecked());

    restoreGeometry(settings.value("window_geometry").toByteArray());
    restoreState(settings.value("window_state").toByteArray());
}

void MainWindow::saveSettings()
{
#define SAVE_ACTION(aname, name) settings.setValue(name, ui->action ## aname->isChecked());
    QSettings settings;
    SAVE_ACTION(Underline, "underline");
    SAVE_ACTION(AutoCommit, "autocommit");
    SAVE_ACTION(HideParen, "hideparen");
    SAVE_ACTION(Mute, "mute");
    SAVE_ACTION(MaskPhrase, "mask_phrase");
    SAVE_ACTION(CommentOnly, "hide_phrase");
    SAVE_ACTION(ShowSpeedChart, "show_speed_chart");
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
    connect(ui->actionMaskPhrase, SIGNAL(triggered(bool))
            , this, SLOT(slotMaskPhrase(bool)));
    connect(ui->actionCommentOnly, SIGNAL(triggered(bool))
            , this, SLOT(slotHidePhrase(bool)));
    connect(ui->actionShowSpeedChart, SIGNAL(triggered(bool))
            , this, SLOT(slotShowSpeedChart(bool)));

    // call slotWindowReady() when program enters the event loop
    QTimer::singleShot(0, this, SLOT(slotWindowLoaded()));
}

void MainWindow::setupToolbar()
{
    ui->toolBar->addAction(ui->actionOpen);
    ui->toolBar->addAction(ui->actionSkip);
    ui->toolBar->addAction(ui->actionAutoCommit);
    ui->toolBar->addAction(ui->actionMute);
}

// highlight mistyped characters
void MainWindow::highlightError()
{
    QString expect = m_workingSet->currentPhrase().trimmed();
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
    if (!ui->actionMaskPhrase->isChecked())
        setInputHint(markup.join(""), m_workingSet->currentComment());
}

void MainWindow::selectFirstError()
{
    QString input = ui->txtInput->text().trimmed();
    if (!input.isEmpty()) {
        QString expect = m_workingSet->currentPhrase().trimmed();
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
    QString expect = m_workingSet->currentPhrase();
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
        m_workingSet->reset();
        if (!m_dataSource->loadFile(dialog.selectedFile())) {
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

void MainWindow::setInputHint(QString phrase, QString comment)
{
    if (ui->actionMaskPhrase->isChecked())
        phrase = maskPhrase(phrase);
    if (ui->actionCommentOnly->isChecked())
        phrase = "";
    QString hint = QString("%1").arg(phrase);
    if (!comment.isEmpty())
        hint.append(QString(" (%1)").arg(comment));
    ui->lblInput->setText(hint);
}

QString MainWindow::maskPhrase(QString phrase)
{
    if (!phrase.isEmpty()) {
        QString s = QString("%1%2")
                .arg(phrase[0])
                .arg(QString("_").repeated(phrase.size()-1));
        return s;
    } else {
        return "";
    }
}
