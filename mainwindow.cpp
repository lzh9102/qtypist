#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "queuedisplay.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_display(new QueueDisplay(this))
{
    ui->setupUi(this);
    ui->displayLayout->addWidget(m_display);
    connect(ui->txtInput, SIGNAL(returnPressed())
            , this, SLOT(slotHandleInput()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

/* Handle Return key in the input box
 * Clear the input box and pop the queue
 */
void MainWindow::slotHandleInput()
{
    QString string = ui->txtInput->text().trimmed();
    if (!string.isEmpty() && !m_display->isEmpty()) {
        const bool correct = judgeInput(string);
        if (correct) { // correct input
            ui->txtInput->clear();
            m_display->pop();
        } else { // incorrect input
            ui->txtInput->selectAll();
        }
        updateStatus(correct);
    }
}

bool MainWindow::judgeInput(QString string)
{
    return string == m_display->front();
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
