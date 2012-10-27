#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QElapsedTimer>

namespace Ui {
class MainWindow;
}

class QueueDisplay;
class ChartDisplay;
class DataSource;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void slotHandleInput();
    void slotTextEdited();
    void slotSkip();
    void slotUnderline(bool);
    void slotHideParen(bool);
    void slotWindowLoaded();
    void slotOpenFile();

private:
    Ui::MainWindow *ui;
    QueueDisplay *m_display;
    ChartDisplay *m_chart;
    DataSource *m_dataSource;
    QElapsedTimer m_elapsedTimer;
    void loadSettings();
    void saveSettings();
    void setupEvents();
    bool judgeInput(QString string);
    void updateStatus(bool correct);
    void openFileDialog();
    void refillQueue();
    void updateChart(int count, int ms);
};

#endif // MAINWINDOW_H
