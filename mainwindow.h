#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class QueueDisplay;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void slotHandleInput();
    void slotUnderline(bool);
    
private:
    Ui::MainWindow *ui;
    QueueDisplay *m_display;
    void loadSettings();
    void saveSettings();
    void setupEvents();
    bool judgeInput(QString string);
    void updateStatus(bool correct);
};

#endif // MAINWINDOW_H
