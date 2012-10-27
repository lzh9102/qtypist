#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "queuedisplay.h"
#include <QSettings>

#define DEFAULT_FONT_SIZE 25

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_display(new QueueDisplay(this))
{
    ui->setupUi(this);
    ui->displayLayout->addWidget(m_display);

    setupEvents();
    loadSettings();

    m_display->push("hello");
    m_display->push("world");
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
        } else { // incorrect input
            ui->txtInput->selectAll();
        }
        updateStatus(correct);
    }
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

    restoreGeometry(settings.value("window_geometry").toByteArray());
}

void MainWindow::saveSettings()
{
    QSettings settings;
    settings.setValue("underline", ui->actionUnderline->isChecked());
    settings.setValue("window_geometry", saveGeometry());
}

void MainWindow::setupEvents()
{
    connect(ui->txtInput, SIGNAL(returnPressed())
            , this, SLOT(slotHandleInput()));
    connect(ui->actionExit, SIGNAL(triggered())
            , this, SLOT(close()));
    connect(ui->actionUnderline, SIGNAL(triggered(bool))
            , this, SLOT(slotUnderline(bool)));
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

void MainWindow::slotUnderline(bool checked)
{
    m_display->setUnderlineFront(checked);
}
