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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QElapsedTimer>

namespace Ui {
class MainWindow;
}

class HistoryDisplay;
class ChartDisplay;
class DataSource;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void slotNext();
    void slotHandleInput();
    void slotTextEdited();
    void slotUnderline(bool);
    void slotHideParen(bool);
    void slotWindowLoaded();
    void slotOpenFile();
    void slotAbout();

private:
    Ui::MainWindow *ui;
    HistoryDisplay *m_display;
    ChartDisplay *m_chart;
    DataSource *m_dataSource;
    QElapsedTimer m_elapsedTimer;
    double m_maxSpeed;
    int m_totalChars;
    double m_totalTime;
    void loadSettings();
    void saveSettings();
    void setupEvents();
    void setupToolbar();
    void highlightError();
    void selectFirstError();
    bool judgeInput(QString string);
    void updateStatus(bool correct);
    void openFileDialog();
    void updateChart(int count, int ms);
};

#endif // MAINWINDOW_H
