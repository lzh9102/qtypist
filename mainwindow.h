#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class QueueDisplay;
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
    DataSource *m_dataSource;
    void loadSettings();
    void saveSettings();
    void setupEvents();
    bool judgeInput(QString string);
    void updateStatus(bool correct);
    void openFileDialog();
    void refillQueue();
};

#endif // MAINWINDOW_H
